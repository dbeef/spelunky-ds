//
// Created by xdbeef on 29.12.18.
//

#include <cmath>
#include "ICollidable.h"
#include "../collisions/collisions.hpp"
#include "../tiles/level_rendering_utils.hpp"
#include "../globals_declarations.hpp"

void ICollidable::update_collisions_with_map(int x_current_pos_in_tiles, int y_current_pos_in_tiles) {
    Collisions::getNeighboringTiles(global::current_level->map_tiles, x_current_pos_in_tiles,
                                    y_current_pos_in_tiles, _neighboring_tiles);

    _bottom_collision = Collisions::checkBottomCollision(_neighboring_tiles, &_x, &_y, &_y_speed, _physical_width, _physical_height, _bouncing_factor_y);
    _left_collision = Collisions::checkLeftCollision(_neighboring_tiles, &_x, &_y, &_x_speed, _physical_width, _physical_height, _bouncing_factor_x);
    _right_collision = Collisions::checkRightCollision(_neighboring_tiles, &_x, &_y, &_x_speed, _physical_width, _physical_height, _bouncing_factor_x);
    _upper_collision = Collisions::checkUpperCollision(_neighboring_tiles, &_x, &_y, &_y_speed, _physical_width, _bouncing_factor_y);

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

        xx = floor_div(_x + 0.5 * _physical_width, TILE_W);
        yy = floor_div(_y + 0.5 * _physical_height, TILE_H);

        if (old_xx != xx || old_yy != yy || _physical_width <= 8 || _physical_height <= 8) {
            if (xx < 31 && yy < 31)
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

    _pos_inc_timer += *global::timer;

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

bool ICollidable::check_collision(ICollidable const &other) {
    return Collisions::checkCollisionBodies(_x, _y, _physical_width, _physical_height,
                                            other._x, other._y, other._physical_width, other._physical_height);
}
