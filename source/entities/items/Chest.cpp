//
// Created by xdbeef on 16.05.18.
//


#include <cstdlib>
#include "Chest.hpp"
#include "../../../build/gfx_spike_collectibles_flame.h"
#include "../../../build/soundbank.h"
#include "../../collisions/Collisions.hpp"
#include "../../GameState.hpp"

#include "../treasures/RubySmall.h"
#include "../treasures/RubyBig.h"
#include "../../graphics/SpriteUtils.hpp"
#include "../../sound/Sound.hpp"

void Chest::update_item_specific() {

    if (_ready_to_dispose) return;

    if (_activated && !_dropped_loot) {

        if (GameState::instance().input_handler->up_key_held) {
            sound::open_chest();
            spawn_treasure();
            _dropped_loot = true;
            match_animation();
        } else {
            // FIXME For now, throwing a chest requires pressing throwing key (Y) twice.
            // Launch throwing code manually?
            // write function that would work i.e like: throwItem(this);
        }

    }

    kill_creatures_if_have_speed(1);
    update_sprites_position();

    // TODO Let's make a function pointer for this, so it wouldn't be updated every frame,
    //  but rather every pickup/leaving item.
    if (_hold_by_main_dude)
        sprite_utils::set_priority(OBJPRIORITY_0, _main_sprite_info, _sub_sprite_info);
    else
        sprite_utils::set_priority(OBJPRIORITY_1, _main_sprite_info, _sub_sprite_info);
}

void Chest::init_sprites() {

    delete_sprites();

    _sub_sprite_info = GameState::instance().sub_oam_manager->initSprite(gfx_spike_collectibles_flamePal,
                                                           gfx_spike_collectibles_flamePalLen,
                                                           nullptr, _sprite_size, ObjSize::OBJSIZE_16,
                                                           _spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
    _main_sprite_info = GameState::instance().main_oam_manager->initSprite(gfx_spike_collectibles_flamePal,
                                                             gfx_spike_collectibles_flamePalLen,
                                                             nullptr, _sprite_size, ObjSize::OBJSIZE_16,
                                                             _spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);

    sprite_utils::set_visibility(true, _main_sprite_info, _sub_sprite_info);
    sprite_utils::set_vertical_flip(false, _main_sprite_info, _sub_sprite_info);
    sprite_utils::set_horizontal_flip(false, _main_sprite_info, _sub_sprite_info);
    update_sprites_position();
    match_animation();

    if (_hold_by_main_dude)
        sprite_utils::set_priority(OBJPRIORITY_0, _main_sprite_info, _sub_sprite_info);
    else
        sprite_utils::set_priority(OBJPRIORITY_1, _main_sprite_info, _sub_sprite_info);

}

void Chest::spawn_treasure() {

    for (int a = 0; a < 4; a++) {

        int ruby_type = rand() % 2;

        if (ruby_type == 0) {

            auto *ruby_small = new RubySmall(_x + _physical_width * 0.5, _y + _physical_height * 0.5);
            ruby_small->_y_speed = -1.7;
            ruby_small->_collectible_timer = 0;
            if (rand() % 2 == 0)
                ruby_small->_x_speed = -0.8;
            else
                ruby_small->_x_speed = 0.8;
            GameState::instance().treasures.push_back(ruby_small);

        } else {

            auto *ruby_big = new RubyBig(_x + _physical_width * 0.5, _y + _physical_width * 0.5);
            ruby_big->_y_speed = -1.7;
            ruby_big->_collectible_timer = 0;
            if (rand() % 2 == 0)
                ruby_big->_x_speed = -0.8;
            else
                ruby_big->_x_speed = 0.8;
            GameState::instance().treasures.push_back(ruby_big);
        }
    }
}

void Chest::match_animation() {

    u8 *frame_gfx;

    if (_dropped_loot)
        frame_gfx = sprite_utils::get_frame((u8 *) gfx_spike_collectibles_flameTiles, _sprite_size, 3);
    else
        frame_gfx = sprite_utils::get_frame((u8 *) gfx_spike_collectibles_flameTiles, _sprite_size, 2);

    sprite_utils::update_frame(frame_gfx, _sprite_size, _main_sprite_info, _sub_sprite_info);
}

