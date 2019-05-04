//
// Created by xdbeef on 29.12.18.
//

#include <cmath>
#include "../../math/Math.hpp"
#include "../../collisions/Collisions.hpp"
#include "ICollidable.h"
#include "../../../build/soundbank.h"
#include "../../preprocessor/Debug.h"
#include "../../GameState.hpp"
#include "../../time/Timer.h"
#include "../../sound/Sound.hpp"
#include "../singletons/states/_DudeStateHandler.hpp"

void ICollidable::update_collisions_with_map(int x_current_pos_in_tiles, int y_current_pos_in_tiles) {
    Collisions::getNeighboringTiles(Level::instance().map_tiles, x_current_pos_in_tiles,
                                    y_current_pos_in_tiles, _neighboring_tiles);

    _bottom_collision = Collisions::checkBottomCollision(_neighboring_tiles, &_x, &_y, &_y_speed, _physical_width,
                                                         _physical_height, _bouncing_factor_y);
    _left_collision = Collisions::checkLeftCollision(_neighboring_tiles, &_x, &_y, &_x_speed, _physical_width,
                                                     _physical_height, _bouncing_factor_x);
    _right_collision = Collisions::checkRightCollision(_neighboring_tiles, &_x, &_y, &_x_speed, _physical_width,
                                                       _physical_height, _bouncing_factor_x);
    _upper_collision = Collisions::checkUpperCollision(_neighboring_tiles, &_x, &_y, &_y_speed, _physical_width,
                                                       _bouncing_factor_y);

    _map_collisions_checked = true;
    _current_x_in_tiles = x_current_pos_in_tiles;
    _current_y_in_tiles = y_current_pos_in_tiles;
}

void ICollidable::update_position() {

    double temp_x_speed = fabs(_x_speed);
    double temp_y_speed = fabs(_y_speed);

    int old_xx = -1;
    int old_yy = -1;
    int xx;
    int yy;

    while (temp_x_speed > 0 || temp_y_speed > 0) {
        if (temp_x_speed > 0) {
            if (_x_speed > 0) {
                _x += 1;
            } else if (_x_speed < 0) {
                _x -= 1;
            }
        }
        if (temp_y_speed > 0) {
            if (_y_speed > 0)
                _y += 1;
            else if (_y_speed < 0)
                _y -= 1;
        }

        xx = math::floor_div(_x + 0.5 * _physical_width, TILE_WIDTH);
        yy = math::floor_div(_y + 0.5 * _physical_height, TILE_HEIGHT);

        if (old_xx != xx || old_yy != yy || _physical_width <= 8 || _physical_height <= 8) {
            SPELUNKYDS_BREAKING_ASSERT(xx < 31 && yy < 31 && xx >= 0 && yy >= 0);
            update_collisions_with_map(xx, yy);
        }

        old_xx = xx;
        old_yy = yy;

        temp_x_speed--;
        temp_y_speed--;

    }
}

void ICollidable::apply_friction() {
    if (_x_speed > 0) {
        _x_speed -= _friction;
        if (_x_speed < 0)
            _x_speed = 0;
    }
    if (_x_speed < 0) {
        _x_speed += _friction;
        if (_x_speed > 0)
            _x_speed = 0;
    }
}

void ICollidable::update_collidable() {

    limit_speed();

    _pos_inc_timer += Timer::getDeltaTime();

    if (_pos_inc_timer > _pos_update_delta) {
        update_position();
        if (can_apply_friction())
            apply_friction();
        if (can_apply_gravity())
            apply_gravity();
        _pos_inc_timer = 0;
    }

}

void ICollidable::apply_gravity() {
    if (!_bottom_collision)
        _y_speed += _gravity;
}

void ICollidable::limit_speed() {
    if (_y_speed > _max_y_speed)
        _y_speed = _max_y_speed;
    else if (_y_speed < -_max_y_speed)
        _y_speed = -_max_y_speed;

    if (_x_speed > _max_x_speed)
        _x_speed = _max_x_speed;
    else if (_x_speed < -_max_x_speed)
        _x_speed = -_max_x_speed;
}

bool ICollidable::check_collision(ICollidable const &other) const {
    return Collisions::checkCollisionBodies(_x, _y, _physical_width, _physical_height,
                                            other._x, other._y, other._physical_width, other._physical_height);
}

bool ICollidable::check_collision(ICollidable const *other) const {
    return Collisions::checkCollisionBodies(_x, _y, _physical_width, _physical_height,
                                            other->_x, other->_y, other->_physical_width, other->_physical_height);
}

static bool is_killable_creature(BaseCreature *creature) {
    CreatureType type = creature->_creature_type;
    return
            type == CreatureType::SNAKE ||
            type == CreatureType::BAT ||
            type == CreatureType::SHOPKEEPER ||
            type == CreatureType::CAVEMAN ||
            type == CreatureType::DAMSEL ||
            type == CreatureType::SKELETON ||
            type == CreatureType::SPIDER;
}

bool ICollidable::kill_creatures_if_have_speed(u8 dmg_to_apply) const {

    bool killed = false;

    if (abs(_x_speed) > 0 || abs(_y_speed) > 0) {
        for (unsigned long a = 0; a < GameState::instance().creatures.size(); a++) {
            if (is_killable_creature(GameState::instance().creatures.at(a))
                && !GameState::instance().creatures.at(a)->killed) {
                if (Collisions::checkCollisionBodies(_x, _y, 16, 16, GameState::instance().creatures.at(a)->_x,
                                                     GameState::instance().creatures.at(a)->_y, _physical_width,
                                                     _physical_height)) {

                    GameState::instance().creatures.at(a)->apply_dmg(dmg_to_apply);
                    killed = true;

                }
            }
        }
    }

    return killed;
}

//when applied, item kills mobs and destroys items (like jars), if it both travels and collides them
bool ICollidable::kill_creatures_jars_if_have_speed_recoil(u8 dmg_to_apply) const {

    bool killed = false;

    if (abs(_x_speed) > 0 || abs(_y_speed) > 0) {
        for (unsigned long a = 0; a < GameState::instance().creatures.size(); a++) {
            if ((is_killable_creature(GameState::instance().creatures.at(a)) ||
                 GameState::instance().creatures.at(a)->_creature_type == CreatureType::JAR)
                && !GameState::instance().creatures.at(a)->_ready_to_dispose) {

                if (Collisions::checkCollisionBodies(_x, _y, _physical_width, _physical_height,
                                                     GameState::instance().creatures.at(a)->_x,
                                                     GameState::instance().creatures.at(a)->_y,
                                                     GameState::instance().creatures.at(a)->_physical_width,
                                                     GameState::instance().creatures.at(a)->_physical_height)) {

                    GameState::instance().creatures.at(a)->_x_speed += this->_x_speed * 0.3f;
                    GameState::instance().creatures.at(a)->apply_dmg(dmg_to_apply);
                    killed = true;

                }
            }
        }
    }
    return killed;
}

bool ICollidable::kill_main_dude_if_have_speed(u8 dmg_to_apply) const {

    if (abs(_x_speed) > 0 || abs(_y_speed) > 0) {

        if (Collisions::checkCollisionBodies(_x, _y, _physical_width, _physical_height, MainDude::instance()._x,
                                             MainDude::instance()._y,
                                             MainDude::instance()._physical_width,
                                             MainDude::instance()._physical_height)) {
            MainDude::instance()._x_speed += this->_x_speed * 0.3f;
            MainDude::instance().apply_dmg(dmg_to_apply);
            return true;
        }

    }
    return false;
}

void ICollidable::deal_damage_main_dude_on_collision(int dmg_to_apply) const {
    if (MainDude::instance()._current_state->_state != _MainDudeState::DEAD &&
        MainDude::instance()._current_state->_state != _MainDudeState::STUNNED &&
        Collisions::checkCollisionWithMainDude(_x, _y, 16, 16) &&
        MainDude::instance().time_since_last_damage > 1000) {

        MainDude::instance().time_since_last_damage = 0;
        Hud::instance().hearts -= dmg_to_apply;
        Hud::instance().draw_level_hud();

        if (Hud::instance().hearts <= 0) {
            MainDude::instance().set_dead();
        } else
            sound::hit();
    }
}