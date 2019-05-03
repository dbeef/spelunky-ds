//
// Created by xdbeef on 04.04.18.
//



#include "Rope.hpp"
#include "../../GameState.hpp"
#include "../../../build/gfx_blood_rock_rope_poof.h"
#include "../../../build/soundbank.h"
#include "../../collisions/Collisions.hpp"

#include "../../time/Timer.h"
#include "../../graphics/SpriteUtils.hpp"
#include "../../sound/Sound.hpp"
#include "../../math/Math.hpp"

void Rope::update_item_specific() {

    if (_ready_to_dispose) return;

    _expand_timer += Timer::getDeltaTime();

    for (int a = _rope_chain.size() - 1; a >= 0; a--) {

        _rope_chain.at(a)->update();

        if (_finished) {
            if (_expand_timer > 40) {
                if (!_rope_chain.at(a)->_active) {
                    _rope_chain.at(a)->_active = true;
                    _expand_timer = 0;
                }
            }
        }
    }

    if (_activated && !_thrown && !_finished) {
        sound::throwing();

        _thrown = true;
        _throwing_timer = 0;

        _x = math::floor_div(MainDude::instance()._x + 0.5 * MainDude::main_dude_physical_width, TILE_WIDTH) * TILE_WIDTH +
             _physical_width * 0.5;
        _y -= 16;
        _y_speed = -4;
    }

    if (_thrown && !_finished) {
        _throwing_timer += Timer::getDeltaTime();
        add_rope_if_needed();
    }

    //checking if main dude can climb over the rope

    bool onTopOfClimbingSpace = MainDude::instance()._y < _y + 6;
    MainDude::instance().on_top_of_climbing_space = onTopOfClimbingSpace;

    for (unsigned long a = 0; a < _rope_chain.size(); a++) {

        if (_rope_chain.at(a)->_x + 5 > MainDude::instance()._x &&
            _rope_chain.at(a)->_x + 5 < MainDude::instance()._x + MainDude::main_dude_physical_width &&
            _rope_chain.at(a)->_y + 5 > MainDude::instance()._y &&
            _rope_chain.at(a)->_y - 5 < MainDude::instance()._y + MainDude::main_dude_physical_height &&
            _rope_chain.at(a)->_active
            && !onTopOfClimbingSpace) {
            if (!InputHandler::instance().keys.r_bumper_down) {
                MainDude::instance().can_climb_rope = true;
                if (InputHandler::instance().keys.up_key_held) {
                    MainDude::instance()._x = _rope_chain.at(a)->_x - 4;
                }
            }

            break;
        }
    }

    if (!_finished) {
        kill_creatures_if_have_speed(1);
    }

    update_sprites_position();

    if (_map_collisions_checked) {

        // FIXME There's some bug that causes no collision flag set to be true,
        // but it sets _y_speed to zero anyway and actually acts as colliding with map.
        if (!_finished && _y_speed == 0) {

            set_sprite_throwing_finished();

            _finished = true;

            int temp_y = math::floor_div(this->_y + (0.5 * _physical_height), TILE_HEIGHT);

            auto element = new RopeElement(_x, temp_y * TILE_HEIGHT);
            element->update();
            _rope_chain.push_back(element);
        }

        _map_collisions_checked = false;
    }
}

void Rope::set_sprite_throwing() {
    u8 *frame_gfx = sprite_utils::get_frame((u8 *) gfx_blood_rock_rope_poofTiles, _sprite_size, 9);
    sprite_utils::update_frame(frame_gfx, _sprite_size, _main_sprite_info, _sub_sprite_info);
}

void Rope::set_sprite_throwing_finished() {
    u8 *frame_gfx = sprite_utils::get_frame((u8 *) gfx_blood_rock_rope_poofTiles, _sprite_size, 10);
    sprite_utils::update_frame(frame_gfx, _sprite_size, _main_sprite_info, _sub_sprite_info);
}

bool Rope::is_there_chain_for_this_tile(int rope_y) {
    for (auto &a : _rope_chain) {
        if (a->_y == rope_y)
            return true;
    }
    return false;
}

void Rope::init_sprites() {

    delete_sprites();

    _sub_sprite_info = OAMManager::sub().initSprite(gfx_blood_rock_rope_poofPal, gfx_blood_rock_rope_poofPalLen,
                                                           nullptr, _sprite_size, ObjSize::OBJSIZE_8,
                                                                         SpritesheetType::BLOOD_ROCK_ROPE_POOF, true,
                                                           false, LAYER_LEVEL::MIDDLE_TOP);
    _main_sprite_info = OAMManager::main().initSprite(gfx_blood_rock_rope_poofPal,
                                                             gfx_blood_rock_rope_poofPalLen,
                                                             nullptr, _sprite_size, ObjSize::OBJSIZE_8,
                                                                           SpritesheetType::BLOOD_ROCK_ROPE_POOF, true,
                                                             false, LAYER_LEVEL::MIDDLE_TOP);

    sprite_utils::set_visibility(true, _main_sprite_info, _sub_sprite_info);
    sprite_utils::set_horizontal_flip(false, _main_sprite_info, _sub_sprite_info);
    sprite_utils::set_vertical_flip(false, _main_sprite_info, _sub_sprite_info);

    if (!_finished)
        set_sprite_throwing();
    else
        set_sprite_throwing_finished();

    for (auto &a : _rope_chain)
        a->init_sprites();
}

void Rope::add_rope_if_needed() {

    int temp_y = math::floor_div(this->_y + (0.5 * _physical_height), TILE_HEIGHT);

    if (!is_there_chain_for_this_tile(temp_y * TILE_HEIGHT)) {

        if (temp_y * TILE_HEIGHT > _y) {

            auto *element = new RopeElement(_x, temp_y * TILE_HEIGHT);
            element->update();
            _rope_chain.push_back(element);

            if ((_extended_rope && _rope_chain.size() == max_rope_chain_size_extended) ||
                (!_extended_rope && _rope_chain.size() == max_rope_chain_size)) {
                set_sprite_throwing_finished();
                _finished = true;
                _y_speed = 0;
            }

        }
    }
}

void Rope::print_classname() const {
    printf("ROPE\n");
}
