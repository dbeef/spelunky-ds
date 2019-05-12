//
// Created by dbeef on 5/4/19.
//

#include "../../tiles/MapTileType.hpp"
#include "MainDude.hpp"
#include "../../tiles/Level.hpp"
#include "../../camera/Camera.hpp"
#include "../../tiles/TileOrientation.hpp"
#include "../items/Pistol.hpp"
#include "../items/Shotgun.hpp"
#include "../items/Mitt.hpp"
#include "../items/Cape.hpp"
#include "../items/Glove.hpp"
#include "../items/Compass.hpp"
#include "../items/SpikeShoes.hpp"
#include "../items/SpringShoes.hpp"
#include "../items/Jetpack.hpp"
#include "../items/Rope.hpp"
#include "../items/Bomb.hpp"
#include "../../math/Math.hpp"
#include "../../graphics/SpriteUtils.hpp"
#include "../../sound/Sound.hpp"

// FIXME: This could be moved to Base interface, up in hierarchy, since it's probably used by shopkeeper.
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

// FIXME: This could be moved to Base interface, up in hierarchy, since it's used by caveman/damsel.
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

// FIXME: This will be moved to DudeEquipmentState handler
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

// FIXME: This could be moved out to DudeActionState handler
void MainDude::take_out_bomb() {
    Hud::instance().bombs--;
    Hud::instance().draw_level_hud();
    Bomb *bomb = new Bomb(_x, _y);
    bomb->_hold_by_main_dude = true;
    GameState::instance().items.push_back(bomb);
    holding_item = true;
}

// FIXME: This could be moved out to DudeActionState handler
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

// FIXME: This could be moved out to DudeActionState handler
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

// FIXME: This could be moved out to DudeActionState handler
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
