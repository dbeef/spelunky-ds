//
// Created by xdbeef on 16.05.18.
//

#include <maxmod9.h>
#include "../../../build/gfx_spike_collectibles_flame.h"
#include "chest.h"
#include "../../collisions/collisions.h"
#include "../../globals_declarations.h"
#include "../../../build/soundbank.h"
#include "crate.h"
#include "shotgun.h"
#include "pistol.h"
#include "spring_shoes.h"
#include "mitt.h"
#include "glove.h"
#include "compass.h"
#include "../animations/got_collectible.h"
#include "../items/spike_shoes.h"

#define CRATE_POS_INC_DELTA 15

void Crate::draw() {

    if (ready_to_dispose)
        return;

    if(!dropped_loot) {
        check_if_can_be_pickuped();
        set_pickuped_position(8, 2);
    }

    if (check_if_can_be_opened()) {
        global::hud->draw();
        mmEffect(SFX_XPICKUP);
        if (hold_by_main_dude) {
            hold_by_main_dude = false;
            global::main_dude->holding_item = false;
        }
    }

    if (activated_by_main_dude) {

        if (hold_by_main_dude) {
            hold_by_main_dude = false;
            global::main_dude->holding_item = false;
        }

        if (!dropped_loot)
            drop_loot();

        play_collectible_animation();

    } else {
        mainSpriteInfo->entry->isHidden = false;
        subSpriteInfo->entry->isHidden = false;
    }


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
    MapTile *tiles[9];
    Collisions::getNeighboringTiles(global::level_generator->map_tiles, x_current_pos_in_tiles, y_current_pos_in_tiles,
                                    tiles);

    bottomCollision = Collisions::checkBottomCollision(tiles, &x, &y, &ySpeed, physical_width, physical_height, true, BOUNCING_FACTOR_Y);
    leftCollision = Collisions::checkLeftCollision(tiles, &x, &y, &xSpeed, physical_width, physical_height, true, BOUNCING_FACTOR_X);
    rightCollision = Collisions::checkRightCollision(tiles, &x, &y, &xSpeed, physical_width, physical_height, true, BOUNCING_FACTOR_X);
    upperCollision = Collisions::checkUpperCollision(tiles, &x, &y, &ySpeed, physical_width, true, BOUNCING_FACTOR_Y);

}

void Crate::initSprite() {


    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_spike_collectibles_flamePal, gfx_spike_collectibles_flamePalLen,
                                                        nullptr, sprite_width * sprite_height, sprite_width,
                                                        spriteType, true, false, LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_spike_collectibles_flamePal, gfx_spike_collectibles_flamePalLen,
                                                          nullptr, sprite_width * sprite_height, sprite_width,
                                                          spriteType, true, false, LAYER_LEVEL::MIDDLE_TOP);
    if (activated_by_main_dude)
        frameGfx = nullptr;
    else
        frameGfx = (u8 *) gfx_spike_collectibles_flameTiles + (sprite_width * sprite_height * (4) / 2);

    subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
    mainSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);

}

void Crate::set_position() {

    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);

    mainSpriteInfo->entry->x = main_x;
    mainSpriteInfo->entry->y = main_y;

    subSpriteInfo->entry->x = sub_x;
    subSpriteInfo->entry->y = sub_y;

    mainSpriteInfo->entry->vFlip = false;
    mainSpriteInfo->entry->hFlip = false;

    subSpriteInfo->entry->vFlip = false;
    subSpriteInfo->entry->hFlip = false;

}

Crate::Crate() {
    physical_height = CRATE_PHYSICAL_HEIGHT;
    physical_width = CRATE_PHYSICAL_WIDTH;
    sprite_height = CRATE_SPRITE_HEIGHT;
    sprite_width = CRATE_SPRITE_WIDTH;
    spriteType = SpritesheetType::SPIKES_COLLECTIBLES;
}

//TODO lower chances for good items
void Crate::drop_loot() {

    int r = rand() % 9;

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

        g->init();
        global::sprites.push_back(g);
        global::hud->draw();
    } else {

        //drop an item

        MovingObject *m = nullptr;

        switch (r) {
            case 2:
                m = new Pistol();
                break;
            case 3:
                m = new SpringShoes();
                break;
            case 4:
                m = new Mitt();
                break;
            case 5:
                m = new Compass();
                break;
            case 6:
                m = new Glove();
                break;
            case 7:
                m = new Shotgun();
                break;
            case 8:
                m = new SpikeShoes();
                break;

            default:
                break;
        }

        m->x = this->x;
        m->y = this->y;
        m->init();
        global::sprites.push_back(m);
    }

    dropped_loot = true;

}

void Crate::play_collectible_animation() {
    frameGfx = (u8 *) gfx_spike_collectibles_flameTiles + (sprite_width * sprite_height * (5 + animFrame) / 2);
    subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
    mainSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);

    animFrameTimer += *global::timer;

    if (animFrameTimer > 75) {
        animFrame++;
        animFrameTimer = 0;
    }

    if (animFrame >= 6) {

        ready_to_dispose = true;
        mainSpriteInfo->entry->isHidden = true;
        subSpriteInfo->entry->isHidden = true;
    }
}

