//
// Created by xdbeef on 24.02.18.
//

#include <nds.h>
#include <nds/arm9/sprite.h>
#include <iostream>
#include "MainDude.h"
#include "../Consts.h"
#include "../../build/spelunker.h"
#include "Collisions.h"

void MainDude::handleKeyInput(int keys_held, int keys_up) {

    if (keys_held) {

        if (keys_held & KEY_UP) {
            if (bottomCollision) {
                ySpeed = -MAX_Y_SPEED;
            }
            if ((hangingOnTileLeft || hangingOnTileRight) && hangingTimer > MIN_HANGING_TIME) {
                ySpeed = -MAX_Y_SPEED;
                hangingOnTileLeft = false;
                hangingOnTileRight = false;
            }
        }

        if (keys_held & KEY_LEFT) {
            hangingOnTileLeft = false;
            if (xSpeed > -MAX_X_SPEED && !(hangingOnTileRight || hangingOnTileLeft))
                if (speedIncTimerX > X_SPEED_DELTA_TIME_MS) {
                    xSpeed -= X_SPEED_DELTA;
                    speedIncTimerX = 0;
                }
            state = W_LEFT;
        }

        if (keys_held & KEY_RIGHT) {
            state = W_RIGHT;
            hangingOnTileRight = false;
            if (xSpeed < MAX_X_SPEED && !(hangingOnTileRight || hangingOnTileLeft)) {
                if (speedIncTimerX > X_SPEED_DELTA_TIME_MS) {
                    xSpeed += X_SPEED_DELTA;
                    speedIncTimerX = 0;
                }
            }
        }

        if (keys_held & KEY_DOWN) {
            hangingOnTileLeft = false;
            hangingOnTileRight = false;
        }
    }
}


void MainDude::updateTimers(int timeElapsed) {
    speedIncTimerX += timeElapsed;
    speedIncTimerY += timeElapsed;
    posIncTimer += timeElapsed;
    frictionTimer += timeElapsed;
    hangingTimer += timeElapsed;

    if (xSpeed != 0)
        animationFrameTimer += timeElapsed;
}


void MainDude::checkCollisionWithMap(MapTile *mapTiles[32][32]) {


    if (xSpeed > MAX_X_SPEED)
        xSpeed = MAX_X_SPEED;
    if (xSpeed < -MAX_X_SPEED)
        xSpeed = -MAX_X_SPEED;

    if (ySpeed > MAX_Y_SPEED)
        ySpeed = MAX_Y_SPEED;
    if (ySpeed < -MAX_Y_SPEED)
        ySpeed = -MAX_Y_SPEED;


    if (posIncTimer > 10) {
        x += xSpeed;
        y += ySpeed;
        posIncTimer = 0;
    }

    bottomCollision = Collisions::checkBottomCollision(mapTiles, &this->x, &this->y, &ySpeed, 16, 16);
    leftCollision = Collisions::checkLeftCollision(mapTiles, &this->x, &this->y, &xSpeed, 16, 16);
    rightCollision = Collisions::checkRightCollision(mapTiles, &this->x, &this->y, &xSpeed, 16, 16);
    upperCollision = Collisions::checkUpperCollision(mapTiles, &this->x, &this->y, &ySpeed, 16);

    if (!bottomCollision) {
        if (speedIncTimerY > Y_SPEED_DELTA_TIME_MS && !(hangingOnTileLeft || hangingOnTileRight)) {
            ySpeed += GRAVITY_DELTA_SPEED;
            speedIncTimerY = 0;
        }
    }

    canHangOnTile(mapTiles);

    if (upperCollision || bottomCollision) {
        hangingOnTileLeft = false;
        hangingOnTileRight = false;
    }

}

void MainDude::init() {
    spriteGfxMemMain = oamAllocateGfx(&oamMain, SpriteSize_16x16, SpriteColorFormat_256Color);
    spriteGfxMemSub = oamAllocateGfx(&oamSub, SpriteSize_16x16, SpriteColorFormat_256Color);
    frameGfx = (u8 *) spelunkerTiles;
    dmaCopy(spelunkerPal, SPRITE_PALETTE, 512);
    dmaCopy(spelunkerPal, SPRITE_PALETTE_SUB, 512);
}


void MainDude::animate(int camera_x, int camera_y) {


    if (animationFrameTimer > 70) {
        animationFrameTimer = 0;
        animFrame++;
    }

    if (animFrame >= FRAMES_PER_ANIMATION) animFrame = 0;

    if (hangingOnTileRight) {
        int frame = (2 * 6) + 1;
        u8 *offset = frameGfx + frame * 16 * 16;
        dmaCopy(offset, spriteGfxMemMain, 16 * 16);
        dmaCopy(offset, spriteGfxMemSub, 16 * 16);


        if (this->y <= 320 + 16) {
            oamSet(&oamMain, 0, x - camera_x, y - camera_y, 0, 0, SpriteSize_16x16, SpriteColorFormat_256Color,
                   spriteGfxMemMain, -1, false, false, false, false, false);
        } else
            oamSet(&oamMain, 0, -16, -16, 0, 0, SpriteSize_16x16, SpriteColorFormat_256Color,
                   0, -1, false, false, false, false, false);

        if (this->y >= 320) {
            oamSet(&oamSub, 0, x - camera_x, y - camera_y - 192, 0, 0, SpriteSize_16x16, SpriteColorFormat_256Color,
                   spriteGfxMemSub, -1, false, false, false, false, false);
        } else
            oamSet(&oamSub, 0, -16, -16, 0, 0, SpriteSize_16x16, SpriteColorFormat_256Color,
                   0, -1, false, false, false, false, false);

        return;
    }
    if (hangingOnTileLeft) {
        int frame = (2 * 6);
        u8 *offset = frameGfx + frame * 16 * 16;
        dmaCopy(offset, spriteGfxMemMain, 16 * 16);
        dmaCopy(offset, spriteGfxMemSub, 16 * 16);
        if (this->y <= 320) {
            oamSet(&oamMain, 0, x - camera_x, y - camera_y, 0, 0, SpriteSize_16x16, SpriteColorFormat_256Color,
                   spriteGfxMemMain, -1, false, false, false, false, false);
        } else
            oamSet(&oamMain, 0, -16, -16, 0, 0, SpriteSize_16x16, SpriteColorFormat_256Color,
                   0, -1, false, false, false, false, false);


        if (this->y >= 320) {
            oamSet(&oamSub, 0, x - camera_x, y - camera_y - 192, 0, 0, SpriteSize_16x16, SpriteColorFormat_256Color,
                   spriteGfxMemSub, -1, false, false, false, false, false);
        } else
            oamSet(&oamSub, 0, -16, -16, 0, 0, SpriteSize_16x16, SpriteColorFormat_256Color,
                   0, -1, false, false, false, false, false);

        return;
    }

    int frame = animFrame + state * FRAMES_PER_ANIMATION;
    u8 *offset = frameGfx + frame * 16 * 16;
    dmaCopy(offset, spriteGfxMemMain, 16 * 16);
    dmaCopy(offset, spriteGfxMemSub, 16 * 16);
    if (this->y <= 320) {
        oamSet(&oamMain, 0, x - camera_x, y - camera_y, 0, 0, SpriteSize_16x16, SpriteColorFormat_256Color,
               spriteGfxMemMain, -1, false, false, false, false, false);
    } else
        oamSet(&oamMain, 0, -16, -16, 0, 0, SpriteSize_16x16, SpriteColorFormat_256Color,
               0, -1, false, false, false, false, false);

    if (this->y >= 320) {
        oamSet(&oamSub, 0, x - camera_x, y - camera_y - 192, 0, 0, SpriteSize_16x16, SpriteColorFormat_256Color,
               spriteGfxMemSub, -1, false, false, false, false, false);
    } else
        oamSet(&oamSub, 0, -16, -16, 0, 0, SpriteSize_16x16, SpriteColorFormat_256Color,
               0, -1, false, false, false, false, false);

}

void MainDude::canHangOnTile(MapTile *mapTiles[32][32]) {

    for (int x = 0; x < 32; x++) {
        for (int y = 0; y < 32; y++) {

            if (mapTiles[x][y] == 0)
                continue;

            if (!hangingOnTileLeft && !hangingOnTileRight) {

                if (x == 0 || y == 0 || x == 31 || y == 31)
                    continue;

                if (leftCollision || rightCollision) {

                    bool upper_tile_exists = mapTiles[x][y - 1] != 0;
                    bool lower_tile_exists = mapTiles[x][y + 1] != 0;

                    if ((upper_tile_exists && lower_tile_exists) || (upper_tile_exists))
                        continue;

                    bool y_bound = (this->y > (y * 16) - 2) && (this->y < (y * 16) + 8);
                    bool x_bound = false;
                    if (rightCollision && state == W_LEFT) {

                        bool upper_right_tile_exists = mapTiles[x + 1][y - 1] != 0;
                        if (upper_right_tile_exists)
                            continue;

                        x_bound = (this->x <= (x * 16) + 16 && (this->x >= (x * 16) + 12));
                    } else if (leftCollision && state == W_RIGHT) {
                        bool upper_left_tile_exists = mapTiles[x - 1][y - 1] != 0;
                        if (upper_left_tile_exists)
                            continue;

                        x_bound = (this->x <= (x * 16) - 12 && (this->x >= (x * 16) - 16));
                    }

                    if (y_bound && x_bound) {
                        hangingTimer = 0;
                        this->y = (y * 16);
                        ySpeed = 0;

                        std::cout << "HANGING" << '\n';

                        if (rightCollision)
                            hangingOnTileRight = true;
                        if (leftCollision)
                            hangingOnTileLeft = true;
                    }
                }
            }
        }
    }
}

void MainDude::applyFriction() {

    if (bottomCollision) {
        if (frictionTimer > FRICTION_DELTA_TIME_MS) {
            frictionTimer = 0;
            if (xSpeed > 0) {
                xSpeed -= FRICTION_DELTA_SPEED;
                if (xSpeed < 0)
                    xSpeed = 0;
            }
            if (xSpeed < 0) {
                xSpeed += FRICTION_DELTA_SPEED;
                if (xSpeed > 0)
                    xSpeed = 0;
            }
        }
    }

}

void MainDude::update(int camera_x, int camera_y, int keys_held, int keys_up, LevelGenerator *l) {
    this->applyFriction();
    this->updateTimers(timerElapsed(0) / TICKS_PER_SECOND);
    this->animate(camera_x, camera_y);
    this->checkCollisionWithMap(l->mapTiles);
    this->handleKeyInput(keys_held, keys_up);
}



