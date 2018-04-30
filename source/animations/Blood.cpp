//
// Created by xdbeef on 28.04.18.
//

#include "Blood.h"
#include "Collisions.h"
#include "../Globals.h"
#include "../level_layout/MapUtils.h"
#include "../../build/blood.h"


void Blood::draw() {

    int main_x = x - global::camera->x;
    int main_y = y - global::camera->y;
    int sub_x = x - global::camera->x;
    int sub_y = y - global::camera->y - 192;

    for (int a = 0; a < bloodTrail.size(); a++) {
        bloodTrail.at(a)->draw();
        bloodTrail.at(a)->update();
    }


    if (global::camera->y + 192 > this->y + 8 || global::camera->y + 192 + 192 < this->y - 8) {
        sub_x = -128;
        sub_y = -128;
    }
    if (global::camera->y > this->y + 8 || global::camera->y + 192 < this->y - 8) {
        main_x = -128;
        main_y = -128;
    }


    if (!finished) {
        frameTimer += *global::timer;

        if (frameTimer > 150) {
            frameTimer = 0;
            currentFrame++;
            if (currentFrame >= 7) {
                currentFrame = 0;
                finished = true;
                mainSpriteInfo->entry->isHidden = true;
                subSpriteInfo->entry->isHidden = true;
            }
            else {
                frameGfx = (u8 *) bloodTiles + (currentFrame * 8 * 8 / 2);
                subSpriteInfo->updateFrame(frameGfx, 8 * 8);
                mainSpriteInfo->updateFrame(frameGfx, 8 * 8);
            }


            if(currentFrame % 2 == 0 && !finished) {

                BloodElement *element = new BloodElement();
                element->x = x;
                element->y = y - 4;
                element->xSpeed = xSpeed * 0.9;
                element->ySpeed = ySpeed * 0.9;
                element->currentFrame = currentFrame;
                element->init();
                element->draw();
                bloodTrail.push_back(element);
            }

        }

    }

    mainSpriteInfo->entry->x = main_x;
    mainSpriteInfo->entry->y = main_y;

    subSpriteInfo->entry->x = sub_x;
    subSpriteInfo->entry->y = sub_y;

}


void Blood::init() {
    subSpriteInfo = global::sub_oam_manager->initSprite(bloodPal, bloodPalLen,
                                                        nullptr, 8 * 8, 8, BLOOD, true, false);
    mainSpriteInfo = global::main_oam_manager->initSprite(bloodPal, bloodPalLen,
                                                          nullptr, 8 * 8, 8, BLOOD, true, false);
    frameGfx = (u8 *) bloodTiles + 0 * 8 * 8 / 2;
    subSpriteInfo->updateFrame(frameGfx, 8 * 8);
    mainSpriteInfo->updateFrame(frameGfx, 8 * 8);

    hold_by_main_dude = false;
    activated_by_main_dude = false;

}

void Blood::updateSpeed() {

    if (xSpeed > MAX_X_SPEED_BLOOD)
        xSpeed = MAX_X_SPEED_BLOOD;
    if (xSpeed < -MAX_X_SPEED_BLOOD)
        xSpeed = -MAX_X_SPEED_BLOOD;

    if (ySpeed > MAX_Y_SPEED_BLOOD)
        ySpeed = MAX_Y_SPEED_BLOOD;
    if (ySpeed < -MAX_Y_SPEED_BLOOD)
        ySpeed = -MAX_Y_SPEED_BLOOD;

    pos_inc_timer += *global::timer;

    bool change_pos = (pos_inc_timer > 25) && !finished;

    if (change_pos) {
        updatePosition();
    }

}

void Blood::updatePosition() {

    if (xSpeed > 0) {
        xSpeed -= 0.055;
        if (xSpeed < 0)
            xSpeed = 0;
    }
    if (xSpeed < 0) {
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

        xx = floor_div(this->x + 0.5 * BLOOD_SIZE, 16);
        yy = floor_div(this->y + 0.5 * BLOOD_SIZE, 16);

        if (old_xx != xx || old_yy != yy) {
            updateCollisionsMap(xx, yy);
        }

        old_xx = xx;
        old_yy = yy;

        tempXspeed--;
        tempYspeed--;
    }


    if (!bottomCollision)
        ySpeed += GRAVITY_DELTA_SPEED ;

    pos_inc_timer = 0;
}

void Blood::onCollisionWithMainCharacter() {}

void Blood::updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) {

    MapTile *tiles[9];
    Collisions::getNeighboringTiles(global::level_generator->mapTiles, x_current_pos_in_tiles, y_current_pos_in_tiles,
                                    tiles);

    bottomCollision = Collisions::checkBottomCollision(tiles, &x, &y, &ySpeed, 8, 8, true);
    leftCollision = Collisions::checkLeftCollision(tiles, &x, &y, &xSpeed, 8, 8, true);
    rightCollision = Collisions::checkRightCollision(tiles, &x, &y, &xSpeed, 8, 8, true);
    upperCollision = Collisions::checkUpperCollision(tiles, &x, &y, &ySpeed, 8, true);

}
