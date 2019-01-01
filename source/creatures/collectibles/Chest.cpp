//
// Created by xdbeef on 16.05.18.
//

#include <maxmod9.h>
#include <cstdlib>
#include "../../../build/gfx_spike_collectibles_flame.h"
#include "Chest.hpp"
#include "../../collisions/Collisions.hpp"
#include "../../GlobalsDeclarations.hpp"
#include "../../../build/soundbank.h"
#include "../SpriteUtils.hpp"
#include "../treasures/RubySmall.h"
#include "../treasures/RubyBig.h"

#define SPEED_OF_THROWING_CHEST_X 1
#define SPEED_OF_THROWING_CHEST_Y 1
#define CHEST_PICKUP_OFFSET_X 8
#define CHECK_PICKUP_OFFSET_Y 2
#define CHEST_POS_INC_DELTA 15
#define CHEST_FRICTION 0.2

void Chest::update_creature_specific() {

    if (_ready_to_dispose) return;

    sprite_utils::set_visibility(true, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);

    check_if_can_be_pickuped();
    set_pickuped_position(CHEST_PICKUP_OFFSET_X, CHECK_PICKUP_OFFSET_Y);

    if (check_if_can_be_opened()) {
        match_animation();
        mmEffect(SFX_XCHESTOPEN);
        spawn_treasure();
    }

    kill_mobs_if_thrown(1);
    update_sprites_position();
}

void Chest::init_sprites() {

    delete_sprites();

    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_spike_collectibles_flamePal,
                                                        gfx_spike_collectibles_flamePalLen,
                                                        nullptr, _sprite_size, _sprite_width,
                                                        _spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_spike_collectibles_flamePal,
                                                          gfx_spike_collectibles_flamePalLen,
                                                          nullptr, _sprite_size, _sprite_width,
                                                          _spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);

    sprite_utils::set_visibility(true, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);
    update_sprites_position();
    match_animation();
}

void Chest::update_sprites_position() {
    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);
    sprite_utils::set_entry_xy(mainSpriteInfo, main_x, main_y);
    sprite_utils::set_entry_xy(subSpriteInfo, sub_x, sub_y);
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
            global::treasures_to_add.push_back(ruby_small);

        } else {
            
            auto *ruby_big = new RubyBig(_x + _physical_width * 0.5, _y + _physical_width * 0.5);
            ruby_big->_y_speed = -1.7;
            ruby_big->_collectible_timer = 0;
            if (rand() % 2 == 0)
                ruby_big->_x_speed = -0.8;
            else
                ruby_big->_x_speed = 0.8;
            global::treasures_to_add.push_back(ruby_big);
        }
    }
}

void Chest::match_animation() {

    if (activated)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spike_collectibles_flameTiles, _sprite_size, 3);
    else
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spike_collectibles_flameTiles, _sprite_size, 2);

    sprite_utils::update_frame(frameGfx, _sprite_size, mainSpriteInfo, subSpriteInfo);
}

void Chest::delete_sprites() {
    delete mainSpriteInfo;
    delete subSpriteInfo;
    mainSpriteInfo = nullptr;
    subSpriteInfo = nullptr;
}

