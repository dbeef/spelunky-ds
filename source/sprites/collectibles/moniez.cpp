//
// Created by xdbeef on 23.04.18.
//

#include <maxmod9.h>
#include "../items/jar.h"
#include "../moving_object.h"
#include "moniez.h"
#include "../../globals_declarations.h"
#include "../../../build/gfx_triple_goldbar.h"
#include "../../../build/gfx_rubies_big.h"
#include "../items/rock.h"
#include "../../../build/soundbank.h"
#include "../sprite_type.h"
#include "../../collisions/collisions.h"
#include "../../tiles/map_utils.h"


void Moniez::draw() {

    if (ready_to_dispose)
        return;

    if (!collected && Collisions::checkCollisionWithMainDudeWidthBoundary(x, y, physics_width, physics_height, 8)) {
        global::hud->draw();

        if (spriteType == SpritesheetType::MONIEZ_RUBY_BIG) {
            mmEffect(SFX_XGEM);
        } else if (spriteType == SpritesheetType::MONIEZ_TRIPLE_GOLD_BARS) {
            mmEffect(SFX_XCOIN);
        }

        global::hud->collectedMoniez(value);
        collected = true;
        subSpriteInfo->entry->isHidden = true;
        mainSpriteInfo->entry->isHidden = true;

        if(spriteType == SpritesheetType::MONIEZ_RUBY_BIG) {
            if (ruby_type == 0) {
                global::collected_loot.push_back(SpriteType ::S_MONIEZ_RUBY_BIG_RED);
            } else if (ruby_type == 1) {
                global::collected_loot.push_back(SpriteType ::S_MONIEZ_RUBY_BIG_GREEN);
            } else if (ruby_type == 2) {
                global::collected_loot.push_back(SpriteType ::S_MONIEZ_RUBY_BIG_BLUE);
            }
        }
        else
        {
            global::collected_loot.push_back(SpriteType ::S_MONIEZ_TRIPLE_GOLD_BARS);
        }



        subSpriteInfo->entry = nullptr;
        mainSpriteInfo->entry = nullptr;
        ready_to_dispose = true;
    }


    set_position();
}


void Moniez::init() {
    ruby_type = rand() % 3;

    initSprite();

    activated_by_main_dude = true;
    collected = false;

}

void Moniez::updateSpeed() {

    if (xSpeed > MAX_X_SPEED_ROCK)
        xSpeed = MAX_X_SPEED_ROCK;
    if (xSpeed < -MAX_X_SPEED_ROCK)
        xSpeed = -MAX_X_SPEED_ROCK;

    if (ySpeed > MAX_Y_SPEED_ROCK)
        ySpeed = MAX_Y_SPEED_ROCK;
    if (ySpeed < -MAX_Y_SPEED_ROCK)
        ySpeed = -MAX_Y_SPEED_ROCK;

    pos_inc_timer += *global::timer;

    bool change_pos = (pos_inc_timer > 15) && !hold_by_main_dude;

    if (change_pos) {
        updatePosition();
    }

}

void Moniez::updatePosition() {

    if (bottomCollision && xSpeed > 0) {
        xSpeed -= 0.055;
        if (xSpeed < 0)
            xSpeed = 0;
    }
    if (bottomCollision && xSpeed < 0) {
        xSpeed += 0.055;
        if (xSpeed > 0)
            xSpeed = 0;
    }

    double tempXspeed = abs(xSpeed);
    double tempYspeed = abs(ySpeed);

    int old_xx = -1;
    int old_yy = -1;
    int xx;
    int yy;

    while (tempXspeed > 0 || tempYspeed > 0) {
        if (tempXspeed > 0) {
            if (xSpeed > 0) {
                x += 1;
            } else if (xSpeed < 0) {
                x -= 1;
            }
        }
        if (tempYspeed > 0) {
            if (ySpeed > 0)
                y += 1;
            else if (ySpeed < 0)
                y -= 1;
        }

//            Collisions::getCenterTile(this->x, this->y, MAIN_DUDE_HEIGHT, MAIN_DUDE_WIDTH, xx, yy);
//fixme

        xx = floor_div(this->x + 0.5 * BOMB_SIZE, 16);
        yy = floor_div(this->y + 0.5 * BOMB_SIZE, 16);

        if (old_xx != xx || old_yy != yy) {
            if (xx < 31 && yy < 31)
                updateCollisionsMap(xx, yy);
        }

        old_xx = xx;
        old_yy = yy;

        tempXspeed--;
        tempYspeed--;
    }


    if (!bottomCollision)
        ySpeed += GRAVITY_DELTA_SPEED;

    pos_inc_timer = 0;
}

void Moniez::updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) {

    MapTile *tiles[9];
    Collisions::getNeighboringTiles(global::level_generator->mapTiles, x_current_pos_in_tiles, y_current_pos_in_tiles,
                                    tiles);

    bottomCollision = Collisions::checkBottomCollision(tiles, &x, &y, &ySpeed, sprite_width, sprite_width, true);
    leftCollision = Collisions::checkLeftCollision(tiles, &x, &y, &xSpeed, sprite_width, sprite_width, true);
    rightCollision = Collisions::checkRightCollision(tiles, &x, &y, &xSpeed, sprite_width, sprite_width, true);
    upperCollision = Collisions::checkUpperCollision(tiles, &x, &y, &ySpeed, sprite_width, true);

    if (bottomCollision) {
        //nothing
    }

}

void Moniez::initSprite() {

    if (spriteType == SpritesheetType::MONIEZ_TRIPLE_GOLD_BARS) {
        subSpriteInfo = global::sub_oam_manager->initSprite(gfx_triple_goldbarPal, gfx_triple_goldbarPalLen,
                                                            nullptr, sprite_width * sprite_height, sprite_width,
                                                            spriteType, true, true);
        mainSpriteInfo = global::main_oam_manager->initSprite(gfx_triple_goldbarPal, gfx_triple_goldbarPalLen,
                                                              nullptr, sprite_width * sprite_height, sprite_width,
                                                              spriteType, true, true);
        frameGfx = (u8 *) gfx_triple_goldbarTiles;
    } else if (spriteType == SpritesheetType::MONIEZ_RUBY_BIG) {
        subSpriteInfo = global::sub_oam_manager->initSprite(gfx_rubies_bigPal, gfx_rubies_bigPalLen,
                                                            nullptr, sprite_width * sprite_height, sprite_width,
                                                            spriteType, true, true);
        mainSpriteInfo = global::main_oam_manager->initSprite(gfx_rubies_bigPal, gfx_rubies_bigPalLen,
                                                              nullptr, sprite_width * sprite_height, sprite_width,
                                                              spriteType, true, true);


        //r g b
        if (ruby_type == 0)
            frameGfx = (u8 *) gfx_rubies_bigTiles;
        else if (ruby_type == 1)
            frameGfx = (u8 *) gfx_rubies_bigTiles + 8 * 8 * 1 / 2;
        else if (ruby_type == 2)
            frameGfx = (u8 *) gfx_rubies_bigTiles + 8 * 8 * 2 / 2;


    }


    subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
    mainSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
}

void Moniez::set_position() {

    int main_x = x - global::camera->x;
    int main_y = y - global::camera->y;
    int sub_x = x - global::camera->x;
    int sub_y = y - global::camera->y - 192;

    if (global::camera->y + 192 > this->y + sprite_height || global::camera->y + 192 + 192 < this->y - sprite_height) {
        sub_x = -128;
        sub_y = -128;
    }
    if (global::camera->y > this->y + sprite_height || global::camera->y + 192 < this->y - sprite_height) {
        main_x = -128;
        main_y = -128;

    }

    if (sub_y + sprite_height < 0 || sub_x + sprite_width < 0) {
        sub_x = -128;
        sub_y = -128;
    }

    if (main_y + sprite_height < 0 || main_x + sprite_width < 0) {
        main_x = -128;
        main_y = -128;
    }


    mainSpriteInfo->entry->x = main_x;
    mainSpriteInfo->entry->y = main_y;

    subSpriteInfo->entry->x = sub_x;
    subSpriteInfo->entry->y = sub_y;

    mainSpriteInfo->entry->vFlip = false;
    mainSpriteInfo->entry->hFlip = false;

    subSpriteInfo->entry->vFlip = false;
    subSpriteInfo->entry->hFlip = false;

}

