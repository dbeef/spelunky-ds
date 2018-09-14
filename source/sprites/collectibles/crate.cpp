//
// Created by xdbeef on 16.05.18.
//

#include <maxmod9.h>
#include <cstdlib>
#include <cstdio>
#include "../../../build/gfx_spike_collectibles_flame.h"
#include "chest.h"
#include "../../collisions/collisions.h"
#include "../../globals_declarations.h"
#include "../../../build/soundbank.h"
#include "crate.h"
#include "shotgun.h"
#include "spring_shoes.h"
#include "mitt.h"
#include "glove.h"
#include "compass.h"
#include "../animations/got_collectible.h"
#include "../items/spike_shoes.h"
#include "../items/cape.h"
#include "../items/jetpack.h"
#include "collectibles_utils.h"
#include "../sprite_utils.h"

#define CRATE_POS_INC_DELTA 15

void Crate::draw() {

    if (ready_to_dispose)
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
    set_position();

}


void Crate::init() {
    initSprite();
}

void Crate::updateSpeed() {

    limit_speed(MAX_X_SPEED_CRATE, MAX_Y_SPEED_CRATE);

    pos_inc_timer += *global::timer;

    bool change_pos = (pos_inc_timer > CRATE_POS_INC_DELTA) && !hold_by_main_dude;

    if (change_pos) {
        update_position();
        apply_friction(0.055);
        apply_gravity(GRAVITY_DELTA_SPEED);
        pos_inc_timer = 0;
    }

}

void Crate::updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) {
    MapTile *t[9];
    Collisions::getNeighboringTiles(global::level_generator->map_tiles,
                                    x_current_pos_in_tiles, y_current_pos_in_tiles, t);

    upperCollision = Collisions::checkUpperCollision(t, &x, &y, &ySpeed, physical_width, true, 0.35);
    bottomCollision = Collisions::checkBottomCollision(t, &x, &y, &ySpeed, physical_width, physical_height, true, 0.35);
    leftCollision = Collisions::checkLeftCollision(t, &x, &y, &xSpeed, physical_width, physical_height, true, 0.15);
    rightCollision = Collisions::checkRightCollision(t, &x, &y, &xSpeed, physical_width, physical_height, true, 0.15);
}

void Crate::initSprite() {

    delete mainSpriteInfo;
    delete subSpriteInfo;

    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_spike_collectibles_flamePal,
                                                        gfx_spike_collectibles_flamePalLen,
                                                        nullptr, CRATE_SPRITE_SIZE, sprite_width,
                                                        spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_spike_collectibles_flamePal,
                                                          gfx_spike_collectibles_flamePalLen,
                                                          nullptr, CRATE_SPRITE_SIZE, sprite_width,
                                                          spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
    if (!activated) {
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spike_collectibles_flameTiles, CRATE_SPRITE_SIZE, 4);
        sprite_utils::update_frame(frameGfx, CRATE_SPRITE_SIZE, mainSpriteInfo, subSpriteInfo);
    } else
        play_collectible_animation();

    sprite_utils::set_visibility(true, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);
    set_position();

}

void Crate::set_position() {
    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);
    sprite_utils::set_entry_xy(mainSpriteInfo, main_x, main_y);
    sprite_utils::set_entry_xy(subSpriteInfo, sub_x, sub_y);
}

Crate::Crate() {
    physical_height = CRATE_PHYSICAL_HEIGHT;
    physical_width = CRATE_PHYSICAL_WIDTH;
    sprite_height = CRATE_SPRITE_HEIGHT;
    sprite_width = CRATE_SPRITE_WIDTH;
    spritesheet_type = SpritesheetType::SPIKES_COLLECTIBLES;
}

//TODO lower chances for good items
void Crate::drop_loot() {

    int r = rand() % 3;
    //drop rope or bomb
    if (r == 0 || r == 1) {
        auto *g = new GotCollectible();
        g->x = x - 12;
        g->y = y - 20;

        if (r == 0) {
            global::hud->ropes += 4;
            g->collectible_type = 2;
        } else {
            global::hud->bombs += 4;
            g->collectible_type = 1;
        }

        global::hud->draw_level_hud();

        g->init();
        global::sprites_to_add.push_back(g);
    } else {
        collectibles_utils::spawn_random_item(this->x, this->y);
    }

    dropped_loot = true;
}

void Crate::play_collectible_animation() {

    if (animFrameTimer > 75) {
        animFrame++;
        animFrameTimer = 0;
    }

    if (animFrame >= 6) {
        ready_to_dispose = true;
        sprite_utils::set_visibility(false, mainSpriteInfo, subSpriteInfo);
    } else {
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spike_collectibles_flameTiles, CRATE_SPRITE_SIZE, 5 + animFrame);
        sprite_utils::update_frame(frameGfx, CRATE_SPRITE_SIZE, mainSpriteInfo, subSpriteInfo);
    }

}

