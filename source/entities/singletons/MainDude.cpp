//
// Created by xdbeef on 24.02.18.
//

#include <nds.h>
#include <nds/arm9/sprite.h>
#include "../../math/Math.hpp"
#include <cstdlib>
#include <cstring>

#include <cstdlib>
#include <cmath>
#include <cstdio>
#include "../../GameState.hpp"
#include "../items/Shotgun.hpp"
#include "../items/Mitt.hpp"
#include "../items/Glove.hpp"
#include "../items/Compass.hpp"
#include "../items/SpringShoes.hpp"
#include "../items/Rope.hpp"
#include "../../collisions/Collisions.hpp"
#include "../../../build/soundbank.h"
#include "../../tiles/TileOrientation.hpp"
#include "../items/SpikeShoes.hpp"
#include "../items/Cape.hpp"
#include "../items/Jetpack.hpp"

#include "../../../build/gfx_spelunker.h"
#include "../items/Pistol.hpp"
#include "MainDude.hpp"
#include "../../sound/Sound.hpp"
#include "../../tiles/MapTile.hpp"
#include "../../tiles/Level.hpp"
#include "../../camera/Camera.hpp"

#include "../items/Bomb.hpp"
#include "../../input/InputHandler.hpp"
#include "../../GameState.hpp"
#include "../../graphics/SpriteUtils.hpp"
#include "../../math/Math.hpp"

#include "../../graphics/SpriteUtils.hpp"
#include "../../GameState.hpp"
#include "MainDude.hpp"
#include "../../GameState.hpp"
#include "../../collisions/Collisions.hpp"
#include "../../../build/gfx_spelunker.h"
#include "../../../build/soundbank.h"
#include "../../tiles/TileOrientation.hpp"
#include "../animations/FallPoof.hpp"
#include "../../sound/Sound.hpp"

#include "../../tiles/MapTile.hpp"
#include "../../tiles/Level.hpp"
#include "../../camera/Camera.hpp"

#include "../items/Bomb.hpp"
#include "../../input/InputHandler.hpp"
#include "../items/_BaseItem.h"
#include "../_interfaces/IPickupable.h"
#include "../../time/Timer.h"

#include "states/_DudeStateHandler.hpp"
#include "states/Climbing.hpp"
#include "states/Dead.hpp"
#include "states/ExitingLevel.hpp"
#include "states/HangingOnEdge.hpp"
#include "states/Stunned.hpp"
#include "states/Walking.hpp"

MainDude *MainDude::_instance = nullptr;

void MainDude::init() {
    SPELUNKYDS_BREAKING_ASSERT(!_instance);
    _instance = new MainDude(MAIN_DUDE_MENU_START_POSITION_X, MAIN_DUDE_MENU_START_POSITION_Y);
    SPELUNKYDS_BREAKING_ASSERT(_instance);
}

void MainDude::dispose() {
    SPELUNKYDS_BREAKING_ASSERT(_instance);
    delete _instance;
}

void MainDude::reset_state() {
    reset_carried_items();
    _current_state = _walking;
}

// Sets main dude's position to the first tile of given type that occurs during iteration through the map.
void MainDude::set_position_to(MapTileType t) {

    MapTile *entrance;
    Level::instance().get_first_tile_of_given_type(t, entrance);

    if (entrance != nullptr && entrance->exists) {
        _x = entrance->x * 16;
        _y = entrance->y * 16;
    } else {
        _x = 0;
        _y = 144;
    }

    Camera::instance().follow_main_dude();
    Camera::instance().instant_focus();
}

void MainDude::boost_going_through_map_holes(MapTile **const t) {
    if (!_bottom_collision) {
        if ((t[static_cast<uint16>(TileOrientation::RIGHT_MIDDLE)] == nullptr ||
             !t[static_cast<uint16>(TileOrientation::RIGHT_MIDDLE)]->collidable) &&
            (t[static_cast<uint16>(TileOrientation::RIGHT_UP)] != nullptr &&
             t[static_cast<uint16>(TileOrientation::RIGHT_DOWN)] != nullptr)) {
            //if there's no collidable tile on right-mid, but there are on right-up and right-down,
            //add extra x-pos to ease going through a hole
            if (_x_speed > 0)
                _x += 2;
        }

        if ((t[static_cast<uint16>(TileOrientation::LEFT_MIDDLE)] == nullptr ||
             !t[static_cast<uint16>(TileOrientation::LEFT_MIDDLE)]->collidable) &&
            (t[static_cast<uint16>(TileOrientation::LEFT_UP)] != nullptr &&
             t[static_cast<uint16>(TileOrientation::LEFT_DOWN)] != nullptr)) {
            //same but for left side
            if (_x_speed < 0)
                _x -= 2;
        }
    }
}

void MainDude::update_creature_specific() {
    SPELUNKYDS_BREAKING_ASSERT(_current_state);
    _current_state->handle_input();
    _current_state->update();
}

void MainDude::init_sprites() {

    delete_sprites();

    main_sprite_info = OAMManager::main().initSprite(gfx_spelunkerPal, gfx_spelunkerPalLen, nullptr,
                                                     _sprite_size, ObjSize::OBJSIZE_16, _spritesheet_type, true,
                                                     false,
                                                     LAYER_LEVEL::MIDDLE_TOP);


    sub_sprite_info = OAMManager::sub().initSprite(gfx_spelunkerPal, gfx_spelunkerPalLen, nullptr,
                                                   _sprite_size, ObjSize::OBJSIZE_16, _spritesheet_type, true,
                                                   false,
                                                   LAYER_LEVEL::MIDDLE_TOP);

    sprite_utils::set_vertical_flip(false, main_sprite_info, sub_sprite_info);
    sprite_utils::set_horizontal_flip(false, main_sprite_info, sub_sprite_info);

    update_sprites_position();
    match_animation();

}


void MainDude::reset_values_checked_every_frame() {
//    can_climb_rope = false;
//    can_climb_ladder = false;
}

// FIXME: This is not called.
void MainDude::throw_item() {

    //throw holding item
    //if holding bomb, arm it only

    if (_currently_held_item || _currently_held_creature || _currently_held_pickupable) {

        bool activated;
        ICollidable *held;
        if (_currently_held_item) {
            held = dynamic_cast<ICollidable *>(_currently_held_item);
            activated = _currently_held_item->_activated;
        } else if (_currently_held_creature) {
            held = dynamic_cast<ICollidable *>(_currently_held_creature);
            activated = _currently_held_creature->activated;
        } else {
            held = _currently_held_pickupable;
            activated = _currently_held_pickupable->_activated;
        }

        if (activated) {

            if (!InputHandler::instance().keys.down_key_held) {

                if (carried_items.mitt) {
                    if (sprite_state == Orientation::LEFT)
                        held->_x_speed = -6 - abs(_x_speed);
                    else
                        held->_x_speed = 6 + abs(_x_speed);

                } else {
                    if (sprite_state == Orientation::LEFT)
                        held->_x_speed = -4 - abs(_x_speed);
                    else
                        held->_x_speed = 4 + abs(_x_speed);
                }

            } else {

                if (sprite_state == Orientation::LEFT)
                    held->_x_speed = -0.04f;
                else
                    held->_x_speed = 0.04f;

            }


            if (_neighboring_tiles[static_cast<uint16>(TileOrientation::UP_MIDDLE)]->exists &&
                _neighboring_tiles[static_cast<uint16>(TileOrientation::UP_MIDDLE)]->collidable) {
                held->_y_speed = 0.0f;
                held->_x_speed *= 2;
            } else {

                if (InputHandler::instance().keys.up_key_held)
                    held->_y_speed = -2.55 - abs(_y_speed);
                else {
                    held->_y_speed = -1;
                }

            }

            // it's not held anymore, so update flags
            if (_currently_held_item) {
                _currently_held_item->_hold_by_main_dude = false;
                _currently_held_item = nullptr;
            } else if (_currently_held_creature) {
                _currently_held_creature->hold_by_main_dude = false;
                _currently_held_creature = nullptr;
            } else {
                _currently_held_pickupable->_hold_by_main_dude = false;
                _currently_held_pickupable = nullptr;
            }


            holding_item = false;

            Hud::instance().disable_all_prompts();
            Hud::instance().draw_level_hud();

            sound::throwing();

        } else {

            // activate item because it's not activated yet
            if (_currently_held_item)
                _currently_held_item->_activated = true;
            else if (_currently_held_creature)
                _currently_held_creature->activated = true;
            else
                _currently_held_pickupable->_activated = true;
        }

    }
}

void MainDude::take_out_bomb() {
    Hud::instance().bombs--;
    Hud::instance().draw_level_hud();
    Bomb *bomb = new Bomb(_x, _y);
    bomb->_hold_by_main_dude = true;
    GameState::instance().items.push_back(bomb);
    holding_item = true;
}

void MainDude::throw_rope() {

    Hud::instance().ropes--;
    Hud::instance().draw_level_hud();

    u8 ROPE_PHYSICAL_WIDTH = 16;
    Rope *rope = new Rope(
            (math::floor_div(_x + (0.5 * _physical_width), TILE_WIDTH) * TILE_WIDTH) + (ROPE_PHYSICAL_WIDTH * 0.5),
            _y + 6);
    rope->_activated = true;
    rope->_y_speed = -4;
    GameState::instance().items.push_back(rope);

}

void MainDude::spawn_carried_items() {

    if (carried_items.spring_shoes) {
        auto *springShoes = new SpringShoes(HUD_ITEMS_ROW_X, Hud::instance().items_offset_y);
        springShoes->collected = true;
        springShoes->_bought = true;
        GameState::instance().items.push_back(springShoes);
        Hud::instance().increment_offset_on_grabbed_item();
    }

    if (carried_items.spike_shoes) {
        auto *spikeShoes = new SpikeShoes(HUD_ITEMS_ROW_X, Hud::instance().items_offset_y);
        spikeShoes->_render_in_hud = true;
        spikeShoes->_bought = true;
        GameState::instance().items.push_back(spikeShoes);
        Hud::instance().increment_offset_on_grabbed_item();
    }

    if (carried_items.compass) {
        auto *compass = new Compass(HUD_ITEMS_ROW_X, Hud::instance().items_offset_y);
        compass->collected = true;
        compass->_bought = true;
        GameState::instance().items.push_back(compass);
        Hud::instance().increment_offset_on_grabbed_item();
    }
    if (carried_items.glove) {
        auto *glove = new Glove(HUD_ITEMS_ROW_X, Hud::instance().items_offset_y);
        glove->_render_in_hud = true;
        glove->_bought = true;
        GameState::instance().items.push_back(glove);
        Hud::instance().increment_offset_on_grabbed_item();
    }
    if (carried_items.cape) {
        auto cape = new Cape(HUD_ITEMS_ROW_X, Hud::instance().items_offset_y);
        cape->_collected = true;
        cape->_bought = true;
        GameState::instance().items.push_back(cape);
        Hud::instance().increment_offset_on_grabbed_item();
    }
    if (carried_items.jetpack) {
        auto *jetpack = new Jetpack(HUD_ITEMS_ROW_X, Hud::instance().items_offset_y);
        jetpack->collected = true;
        jetpack->_bought = true;
        GameState::instance().items.push_back(jetpack);
        Hud::instance().increment_offset_on_grabbed_item();
    }
    if (carried_items.mitt) {
        auto mitt = new Mitt(HUD_ITEMS_ROW_X, Hud::instance().items_offset_y);
        mitt->collected = true;
        mitt->_bought = true;
        GameState::instance().items.push_back(mitt);
        Hud::instance().increment_offset_on_grabbed_item();
    }
    if (carried_items.shotgun) {
        holding_item = true;
        auto *shotgun = new Shotgun(_x, _y);
        shotgun->_bought = true;
        shotgun->_hold_by_main_dude = true;
        GameState::instance().items.push_back(shotgun);
    }

    if (carried_items.pistol) {
        holding_item = true;
        auto *pistol = new Pistol(_x, _y);
        pistol->_bought = true;
        pistol->_hold_by_main_dude = true;
        GameState::instance().items.push_back(pistol);
    }
}


void MainDude::apply_blinking_on_damage() {
    if (time_since_last_damage < MAIN_DUDE_DAMAGE_PROTECTION_TIME) {
        sprite_utils::set_visibility(((int) time_since_last_damage % 100) >= 50, main_sprite_info, sub_sprite_info);
    } else
        sprite_utils::set_visibility(true, main_sprite_info, sub_sprite_info);
}

bool MainDude::can_hang_on_tile(MapTile **neighboringTiles) {

    if (_bottom_collision || (!_left_collision && !_right_collision))
        return false;

    if ((neighboringTiles[static_cast<uint16>(TileOrientation::UP_MIDDLE)] != nullptr &&
         neighboringTiles[static_cast<uint16>(TileOrientation::UP_MIDDLE)]->collidable) ||
        (neighboringTiles[static_cast<uint16>(TileOrientation::DOWN_MIDDLE)] != nullptr &&
         neighboringTiles[static_cast<uint16>(TileOrientation::DOWN_MIDDLE)]->collidable))
        return false;

    bool y_bound = false;
    bool x_bound = false;

    if (_right_collision && sprite_state == Orientation::LEFT) {

        if (!carried_items.glove && (neighboringTiles[static_cast<uint16>(TileOrientation::LEFT_UP)] != nullptr &&
                                     neighboringTiles[static_cast<uint16>(TileOrientation::LEFT_UP)]->collidable))
            return false;

        x_bound = (this->_x <= (neighboringTiles[static_cast<uint16>(TileOrientation::LEFT_MIDDLE)]->x * 16) + 16 &&
                   (this->_x >= (neighboringTiles[static_cast<uint16>(TileOrientation::LEFT_MIDDLE)]->x * 16) + 12));
        y_bound = (this->_y > (neighboringTiles[static_cast<uint16>(TileOrientation::LEFT_MIDDLE)]->y * 16) - 2) &&
                  (this->_y < (neighboringTiles[static_cast<uint16>(TileOrientation::LEFT_MIDDLE)]->y * 16) + 8);
    } else if (_left_collision && sprite_state == Orientation::RIGHT) {

        if (!carried_items.glove && (neighboringTiles[static_cast<uint16>(TileOrientation::RIGHT_UP)] != nullptr &&
                                     neighboringTiles[static_cast<uint16>(TileOrientation::RIGHT_UP)]->collidable))
            return false;

        y_bound = (this->_y > (neighboringTiles[static_cast<uint16>(TileOrientation::RIGHT_MIDDLE)]->y * 16) - 2) &&
                  (this->_y < (neighboringTiles[static_cast<uint16>(TileOrientation::RIGHT_MIDDLE)]->y * 16) + 8);
        x_bound = (this->_x <= (neighboringTiles[static_cast<uint16>(TileOrientation::RIGHT_MIDDLE)]->x * 16) - 12 &&
                   (this->_x >= (neighboringTiles[static_cast<uint16>(TileOrientation::RIGHT_MIDDLE)]->x * 16) - 16));
    }

    if ((y_bound && x_bound) && hanging_timer > MAIN_DUDE_MIN_HANGING_TIME) {

        if (_right_collision && neighboringTiles[static_cast<uint16>(TileOrientation::LEFT_MIDDLE)]->collidable) {
            this->_y = (neighboringTiles[static_cast<uint16>(TileOrientation::LEFT_MIDDLE)]->y * 16);
            _orientation = Orientation::RIGHT;
            jumping_timer = 0;
            hanging_timer = 0;
            _y_speed = 0;
        }
        if (_left_collision && neighboringTiles[static_cast<uint16>(TileOrientation::RIGHT_MIDDLE)]->collidable) {
            jumping_timer = 0;
            _orientation = Orientation::LEFT;
            this->_y = (neighboringTiles[static_cast<uint16>(TileOrientation::RIGHT_MIDDLE)]->y * 16);
            hanging_timer = 0;
            _y_speed = 0;
        }
    }
    return true;
}

void MainDude::apply_dmg(int dmg_to_apply) {
    if (dmg_to_apply == 4) {
        //FIXME: An enum that would indicate 'instant death,
        // no matter for hp quantity' or a function kill_instantly.
        set_dead();
    }
}

void MainDude::update_sprites_position() {
    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);
    sprite_utils::set_entry_xy(main_sprite_info, static_cast<u16>(main_x), static_cast<u16>(main_y));
    sprite_utils::set_entry_xy(sub_sprite_info, static_cast<u16>(sub_x), static_cast<u16>(sub_y));
}

bool MainDude::can_apply_gravity() const {
    return !_bottom_collision &&
           _current_state->_state != _MainDudeState::CLIMBING &&
           _current_state->_state != _MainDudeState::HANGING_ON_EDGE;
}

void MainDude::delete_sprites() {
    delete main_sprite_info;
    delete sub_sprite_info;
    main_sprite_info = nullptr;
    sub_sprite_info = nullptr;
}

MainDude::~MainDude() {
    delete _climbing;
    delete _dead;
    delete _exiting_level;
    delete _hanging_on_edge;
    delete _stunned;
    delete _walking;
}

MainDude::MainDude(int x, int y) : BaseCreature(
        x,
        y,
        main_dude_sprite_width,
        main_dude_sprite_height,
        main_dude_spritesheet_type,
        main_dude_physical_width,
        main_dude_physical_height,
        CreatureType::MAIN_DUDE
) {

    // main dude specific characteristics
    time_since_last_damage = MAIN_DUDE_DAMAGE_PROTECTION_TIME + 1;
    _friction = ICollidable::default_friction * 5.0f;
    _bouncing_factor_y = 0;
    _bouncing_factor_x = 0;
    _gravity = ICollidable::default_gravity * 1.15f;

    // allocate possible states and start with walking
    _climbing = new Climbing();
    _dead = new Dead();
    _exiting_level = new ExitingLevel();
    _hanging_on_edge = new HangingOnEdge();
    _stunned = new Stunned();
    _walking = new Walking();

    _current_state = _walking;
}

void MainDude::print_classname() const {
    printf("MAIN_DUDE\n");
}

void MainDude::reset_carried_items() {
    carried_items = {};
}

void MainDude::set_stunned() {
    _current_state = _stunned;
    _current_state->enter();
}

void MainDude::set_exiting_level() {
    _current_state = _exiting_level;
    _current_state->enter();
}

void MainDude::set_walking() {
    _current_state = _hanging_on_edge;
    _current_state->enter();
}

void MainDude::set_climbing() {
    _current_state = _climbing;
    _current_state->enter();
}

void MainDude::set_hanging_on_edge() {
    _current_state = _hanging_on_edge;
    _current_state->enter();
};

void MainDude::set_dead() {
    _current_state = _dead;
    _current_state->enter();
}

void MainDude::set_sprite_crawling() {
    // FIXME: animFrame checking
    if (sprite_state == Orientation::LEFT)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, _sprite_size,
                                           animFrame + 24);
    else
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, _sprite_size,
                                           animFrame + 33);
}

void MainDude::set_sprite_hanging_on_tile() {
    // FIXME: animFrame checking
    if (_orientation == Orientation::RIGHT)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, _sprite_size, 13);
    else if (_orientation == Orientation::LEFT)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, _sprite_size, 12);
}

void MainDude::set_sprite_whiping() {
    // FIXME: animFrame checking
    if (sprite_state == Orientation::LEFT)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, _sprite_size, 56 + animFrame);
    else if (sprite_state == Orientation::RIGHT)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, _sprite_size, 62 + animFrame);
}

void MainDude::set_sprite_pushing() {
    // FIXME: animFrame checking
    if (_orientation == Orientation::LEFT)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, _sprite_size, 42 + animFrame);
    else
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, _sprite_size, 49 + animFrame);
}

void MainDude::set_sprite_stunned() {
    if (animFrame > 4) animFrame = 0;
    frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, _sprite_size, 18 + animFrame);
}

void MainDude::set_sprite_climbing_ladder() {
    if (animFrame >= 6) animFrame = 0;
    frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, _sprite_size, 96 + animFrame);
}

void MainDude::set_sprite_climbing_rope() {
    if (animFrame >= 12) animFrame = 0;
    frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, _sprite_size, 74 + animFrame);
}

void MainDude::set_sprite_dead() {
    if (_bottom_collision)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, _sprite_size, 17);
    else
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, _sprite_size, 16);
}

void MainDude::set_sprite_walking() {
    // FIXME: animFrame checking
    frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, _sprite_size, static_cast<u16>(sprite_state) * 6);
}

void MainDude::set_sprite_falling() {
    // FIXME: animFrame checking
    if (fabs(_x_speed) != 0)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, _sprite_size,
                                           animFrame +
                                           (static_cast<u16>(sprite_state) * MAIN_DUDE_FRAMES_PER_ANIMATION));

    else if (sprite_state == Orientation::LEFT)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, _sprite_size, 14);
    else if (sprite_state == Orientation::RIGHT)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, _sprite_size, 15);
}

void MainDude::set_sprite_exiting_level() {
    // FIXME: animFrame checking
    frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, _sprite_size, 80 + animFrame);
}

void MainDude::match_animation() {
    switch (_current_state->_state) {
        case _MainDudeState::WALKING: {
            if (using_whip)
                set_sprite_whiping();
            else
                set_sprite_walking();
            break;
        }
        case _MainDudeState::CLIMBING: {
            if (using_whip)
                set_sprite_whiping();
            else {
                switch(_climbing->_type){
                    case Climbing::LADDER:{
                        set_sprite_climbing_ladder();
                        break;
                    }
                    case Climbing::ROPE:{
                        set_sprite_climbing_rope();
                        break;
                    }
                }
            }
            break;
        }
        case _MainDudeState::HANGING_ON_EDGE: {
            if (using_whip)
                set_sprite_whiping();
            else
                set_sprite_hanging_on_tile();
            break;
        }
        case _MainDudeState::STUNNED: {
            set_sprite_stunned();
            break;
        }
        case _MainDudeState::DEAD: {
            set_sprite_dead();
            break;
        }
        case _MainDudeState::EXITING_LEVEL: {
            set_sprite_exiting_level();
            break;
        }
    }
}
