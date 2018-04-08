//
// Created by xdbeef on 04.04.18.
//

#include "Bomb.h"
#include "../Globals.h"
#include "../level_layout/MapUtils.h"
#include "Collisions.h"
#include "MainDude.h"
#include "../../build/bomb_armed.h"
#include "../../build/bomb_unarmed.h"

void Bomb::draw() {

    int main_x = x - global::camera->x;
    int main_y = y - global::camera->y;
    int sub_x = x - global::camera->x;
    int sub_y = y - global::camera->y - 192;

    if (this->y > 320) {
        main_x = -16;
        main_y = -16;
    }

    if (this->y < 320) {
        sub_x = -16;
        sub_y = -16;
    }

    mainSpriteInfo->entry->x = main_x;
    mainSpriteInfo->entry->y = main_y;

    subSpriteInfo->entry->x = sub_x;
    subSpriteInfo->entry->y = sub_y;

}


void Bomb::init() {
    subSpriteInfo = global::sub_oam_manager->initSprite(bomb_unarmedPal, bomb_unarmedPalLen,
                                       bomb_unarmedTiles, bomb_unarmedTilesLen, 8);
    mainSpriteInfo = global::main_oam_manager->initSprite(bomb_unarmedPal, bomb_unarmedPalLen,
                                         bomb_unarmedTiles, bomb_unarmedTilesLen, 8);
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

    bool change_pos = (pos_inc_timer > 25) && !carried;

    if (change_pos) {
        updatePosition();
    }

}

void Bomb::updatePosition() {

    if (xSpeed > 0) {
        xSpeed -= 0.005;
        if (xSpeed < 0)
            xSpeed = 0;
    }
    if (xSpeed < 0) {
        xSpeed += 0.005;
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

    bottomCollision = Collisions::checkBottomCollision(tiles, &x, &y, &ySpeed, 8, 8);
    leftCollision = Collisions::checkLeftCollision(tiles, &x, &y, &xSpeed, 8, 8);
    rightCollision = Collisions::checkRightCollision(tiles, &x, &y, &xSpeed, 8, 8);
    upperCollision = Collisions::checkUpperCollision(tiles, &x, &y, &ySpeed, 8);

}



