//
// Created by xdbeef on 23.04.18.
//

#include "Jar.h"
#include "../sprites/MovingObject.h"
#include "Collisions.h"
#include "Moniez.h"
#include "../Globals.h"
#include "../level_layout/MapUtils.h"
#include "../../build/triple_goldbar.h"
#include "../../build/rubies_big.h"
#include "Rock.h"

extern u16 map[4096];

void Moniez::draw() {

    if (!collected && Collisions::checkCollisionWithMainDudeWidthBoundary(x, y, physics_width, physics_height, 8)) {
        global::hud->draw();
        global::hud->collectedMoniez(value);
        collected = true;
        subSpriteInfo->entry->isHidden = true;
        mainSpriteInfo->entry->isHidden = true;
    }

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


    if (activated_by_main_dude) {
        //nothing
    }

    mainSpriteInfo->entry->x = main_x;
    mainSpriteInfo->entry->y = main_y;

    subSpriteInfo->entry->x = sub_x;
    subSpriteInfo->entry->y = sub_y;

}


void Moniez::init() {
    if (spriteType == SpriteType::MONIEZ_TRIPLE_GOLD_BARS) {
        subSpriteInfo = global::sub_oam_manager->initSprite(triple_goldbarPal, triple_goldbarPalLen,
                                                            nullptr, sprite_width * sprite_height, sprite_width,
                                                            spriteType, true, true);
        mainSpriteInfo = global::main_oam_manager->initSprite(triple_goldbarPal, triple_goldbarPalLen,
                                                              nullptr, sprite_width * sprite_height, sprite_width,
                                                              spriteType, true, true);
        frameGfx = (u8 *) triple_goldbarTiles;
    } else if (spriteType == SpriteType::MONIEZ_RUBY_BIG) {
        subSpriteInfo = global::sub_oam_manager->initSprite(rubies_bigPal, rubies_bigPalLen,
                                                            nullptr, sprite_width * sprite_height, sprite_width,
                                                            spriteType, true, true);
        mainSpriteInfo = global::main_oam_manager->initSprite(rubies_bigPal, rubies_bigPalLen,
                                                              nullptr, sprite_width * sprite_height, sprite_width,
                                                              spriteType, true, true);

        int a = rand() % 3;

        if (a == 0)
            frameGfx = (u8 *) rubies_bigTiles;
        else if (a == 1)
            frameGfx = (u8 *) rubies_bigTiles + 8 * 8 * 1 / 2;
        else if (a == 2)
            frameGfx = (u8 *) rubies_bigTiles + 8 * 8 * 2 / 2;


    }

    activated_by_main_dude = true;
    collected = false;

    subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
    mainSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
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

    pos_inc_timer += *timer;

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

