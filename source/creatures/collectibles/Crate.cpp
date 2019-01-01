//
// Created by xdbeef on 16.05.18.
//

#include <maxmod9.h>
#include <cstdlib>
#include <cstdio>
#include "../../../build/gfx_spike_collectibles_flame.h"
#include "Chest.hpp"
#include "../../collisions/Collisions.hpp"
#include "../../GlobalsDeclarations.hpp"
#include "../../../build/soundbank.h"
#include "Crate.hpp"
#include "Shotgun.hpp"
#include "SpringShoes.hpp"
#include "Mitt.hpp"
#include "Glove.hpp"
#include "Compass.hpp"
#include "../../decorations/GotCollectible.hpp"
#include "../items/SpikeShoes.hpp"
#include "../items/Cape.hpp"
#include "../items/Jetpack.hpp"
#include "_CollectiblesUtils.hpp"
#include "../SpriteUtils.hpp"

#define CRATE_POS_INC_DELTA 15

void Crate::update_creature_specific() {

    if (_ready_to_dispose)
        return;

    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);

    if (!dropped_loot) {
        check_if_can_be_pickuped();
        set_pickuped_position(8, 2);
    }

    if (check_if_can_be_opened()) {
        mmEffect(SFX_XPICKUP);
        if (hold_by_main_dude) {
            hold_by_main_dude = false;
            global::main_dude->holding_item = false;
        }
    }

    if (activated) {

        if (hold_by_main_dude) {
            hold_by_main_dude = false;
            global::main_dude->holding_item = false;
        }

        if (!dropped_loot)
            drop_loot();

        animFrameTimer += *global::timer;
        play_collectible_animation();

    } else
        sprite_utils::set_visibility(true, mainSpriteInfo, subSpriteInfo);

    kill_mobs_if_thrown(1);
    update_sprites_position();

}

void Crate::init_sprites() {

    delete_sprites();

    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_spike_collectibles_flamePal,
                                                        gfx_spike_collectibles_flamePalLen,
                                                        nullptr, _sprite_size, ObjSize::OBJSIZE_16,
                                                        _spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_spike_collectibles_flamePal,
                                                          gfx_spike_collectibles_flamePalLen,
                                                          nullptr, _sprite_size, ObjSize::OBJSIZE_16,
                                                          _spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
    if (!activated) {
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spike_collectibles_flameTiles, _sprite_size, 4);
        sprite_utils::update_frame(frameGfx, _sprite_size, mainSpriteInfo, subSpriteInfo);
    } else
        play_collectible_animation();

    sprite_utils::set_visibility(true, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);
    update_sprites_position();

}

void Crate::update_sprites_position() {
    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);
    sprite_utils::set_entry_xy(mainSpriteInfo, main_x, main_y);
    sprite_utils::set_entry_xy(subSpriteInfo, sub_x, sub_y);
}

//TODO lower chances for good items
void Crate::drop_loot() {

    int r = rand() % 3;
    //drop rope or bomb
    if (r == 0 || r == 1) {

        GotCollectible *g;

        if (r == 0) {
            global::hud->ropes += 4;
            g = new GotCollectible(_x - 12, _y - 20, GotCollectible::Type::ROPE);
        } else {
            global::hud->bombs += 4;
            g = new GotCollectible(_x - 12, _y - 20, GotCollectible::Type::BOMB);
        }

        global::hud->draw_level_hud();

        global::decorations_to_add.push_back(g);
    } else {
        collectibles_utils::spawn_random_item(this->_x, this->_y);
    }

    dropped_loot = true;
}

void Crate::play_collectible_animation() {

    if (animFrameTimer > 75) {
        animFrame++;
        animFrameTimer = 0;
    }

    if (animFrame >= 6) {
        _ready_to_dispose = true;
        sprite_utils::set_visibility(false, mainSpriteInfo, subSpriteInfo);
    } else {
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spike_collectibles_flameTiles, _sprite_size, 5 + animFrame);
        sprite_utils::update_frame(frameGfx, _sprite_size, mainSpriteInfo, subSpriteInfo);
    }

}

void Crate::delete_sprites() {
    delete mainSpriteInfo;
    delete subSpriteInfo;
    mainSpriteInfo = nullptr;
    subSpriteInfo = nullptr;
}

