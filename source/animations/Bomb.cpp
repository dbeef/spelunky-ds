//
// Created by xdbeef on 04.04.18.
//

#include "Bomb.h"
#include "../Globals.h"
#include "../level_layout/MapUtils.h"
#include "Collisions.h"
#include "MainDude.h"
#include "../../build/explosion.h"
#include <math.h>       /* floor */

extern u16 map[4096];

void Bomb::draw() {


    if (hold_by_main_dude) {

        y = global::main_dude->y + 6;

        if (global::main_dude->state == 1) {
            x = global::main_dude->x - 2;
        } else
            x = global::main_dude->x + 10;
    }


    int main_x = x - global::camera->x;
    int main_y = y - global::camera->y;
    int sub_x = x - global::camera->x;
    int sub_y = y - global::camera->y - 192;

    if(explosionFrame > 0) {
        main_x -= 32;
        main_y -= 32;

        sub_x += 32;
        sub_y -= 32;

    } else{
        main_x -= 29;
        main_y -= 29;

        sub_x += 29;
        sub_y -= 29;
    }

    if (this->y < 320 + 64 && this->y > 320 - 64) {
        if (explosionFrame > 0)
            sub_x -= 64;
        else
            sub_x -= 56;
    } else if (this->y > 320) {
        main_x = -128;
        main_y = -128;
        sub_x -= 56;
    } else if (this->y < 320) {
        sub_x = -128;
        sub_y = -128;
    }


    if (sub_y < 0 || sub_x < 0) {
        sub_x = -128;
        sub_y = -128;
    }
    if (main_y < 0 || main_x < 0) {
        main_x = -128;
        main_y = -128;
    }


    if (activated_by_main_dude && !armed) {
        arm();
        armed = true;
        armedTimer = 0;
//        std::cout<< main_x << " " << main_y << " " << x << " "<<  y << '\n';
    }

    if (armed) {
        armedTimer += *timer;
        if (armedTimer < ARMED_TIME_BLINK_SLOW) {
            if ((armedTimer) % 250 < 125) {
                disarm();
            } else {
                arm();
            }
        } else if (armedTimer < ARMED_TIME_BLINK_FAST) {
            if ((armedTimer) % 120 < 60) {
                disarm();
            } else {
                arm();
            }
        } else {


            explosionTimer += *timer;
            if (explosionTimer > 50 && explosionFrame < 10) {

                if (explosionFrame == 0) {

                    int xx = floor_div(this->x + 0.5 * BOMB_SIZE, 16);
                    int yy = floor_div(this->y + 0.5 * BOMB_SIZE, 16);

                    Collisions::bombNeighboringTiles(global::level_generator->mapTiles, xx, yy);
                    global::bombed = true;
//                    std::cout<< main_x << " " << main_y << " " << x << " "<<  y << '\n';

                }

                explosionTimer = 0;
                explosionFrame++;

                if (explosionFrame >= 10) {

                    subSpriteInfo->updateFrame(nullptr, 64 * 64);
                    mainSpriteInfo->updateFrame(nullptr, 64 * 64);

                    //explosion! todo dispose object
                } else {
                    frameGfx = (u8 *) explosionTiles + (2 + explosionFrame) * 64 * 64 / 2;
                    subSpriteInfo->updateFrame(frameGfx, 64 * 64);
                    mainSpriteInfo->updateFrame(frameGfx, 64 * 64);
                }
            }
        }

    }

    mainSpriteInfo->entry->x = main_x;
    mainSpriteInfo->entry->y = main_y;

    subSpriteInfo->entry->x = sub_x;
    subSpriteInfo->entry->y = sub_y;

}


void Bomb::init() {
    subSpriteInfo = global::sub_oam_manager->initSprite(explosionPal, explosionPalLen,
                                                        nullptr, 64 * 64, 64, BOMB, true, false);
    mainSpriteInfo = global::main_oam_manager->initSprite(explosionPal, explosionPalLen,
                                                          nullptr, 64 * 64, 64, BOMB, true, false);
    disarm();
    explosionFrame = 0;

//    frameGfx = (u8 *) explosionTiles + 3* 64 * 64 / 2;
//    subSpriteInfo->updateFrame(frameGfx, 64 * 64);
//    mainSpriteInfo->updateFrame(frameGfx, 64 * 64);



}

void Bomb::arm() {
    frameGfx = (u8 *) explosionTiles + 1 * 64 * 64 / 2;
    subSpriteInfo->updateFrame(frameGfx, 64 * 64);
    mainSpriteInfo->updateFrame(frameGfx, 64 * 64);
}

void Bomb::disarm() {
    frameGfx = (u8 *) explosionTiles; //+ 1 * 8 * 8  / 2; dla drugiej klatki
    subSpriteInfo->updateFrame(frameGfx, 64 * 64);
    mainSpriteInfo->updateFrame(frameGfx, 64 * 64);
}

void Bomb::updateSpeed() {

    if (xSpeed > MAX_X_SPEED_BOMB)
        xSpeed = MAX_X_SPEED_BOMB;
    if (xSpeed < -MAX_X_SPEED_BOMB)
        xSpeed = -MAX_X_SPEED_BOMB;

    if (ySpeed > MAX_Y_SPEED_BOMB)
        ySpeed = MAX_Y_SPEED_BOMB;
    if (ySpeed < -MAX_Y_SPEED_BOMB)
        ySpeed = -MAX_Y_SPEED_BOMB;

    pos_inc_timer += *timer;

    bool change_pos = (pos_inc_timer > 15) && !hold_by_main_dude && explosionFrame == 0;

    if (change_pos) {
        updatePosition();
    }

}

void Bomb::updatePosition() {

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

void Bomb::updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) {

    MapTile *tiles[9];
    Collisions::getNeighboringTiles(global::level_generator->mapTiles, x_current_pos_in_tiles, y_current_pos_in_tiles,
                                    tiles);

    bottomCollision = Collisions::checkBottomCollision(tiles, &x, &y, &ySpeed, 8, 8, true);
    leftCollision = Collisions::checkLeftCollision(tiles, &x, &y, &xSpeed, 8, 8, true);
    rightCollision = Collisions::checkRightCollision(tiles, &x, &y, &xSpeed, 8, 8, true);
    upperCollision = Collisions::checkUpperCollision(tiles, &x, &y, &ySpeed, 8, true);

    if (bottomCollision) {

    }

}



