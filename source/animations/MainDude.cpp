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
#include "../level_layout/MapUtils.h"

void MainDude::clearTilesOnRight(MapTile *mapTiles[32][32]) {

    int xx = floor_div(this->x, 16);
    int yy = floor_div(this->y, 16);

    MapTile *left_middle = mapTiles[xx - 1][yy];
    MapTile *right_middle = mapTiles[xx + 1][yy];
    MapTile *up_middle = mapTiles[xx][yy - 1];
    MapTile *down_middle = mapTiles[xx][yy + 1];
    MapTile *center = mapTiles[xx][yy];
    MapTile *left_up = mapTiles[xx - 1][yy - 1];
    MapTile *right_up = mapTiles[xx + 1][yy - 1];
    MapTile *left_down = mapTiles[xx - 1][yy + 1];
    MapTile *right_down = mapTiles[xx + 1][yy + 1];

    left_middle->values[0] = 16;
    left_middle->values[1] = 16;
    left_middle->values[2] = 16;
    left_middle->values[3] = 16;

    left_down->values[0] = 0;
    left_down->values[1] = 0;
    left_down->values[2] = 0;
    left_down->values[3] = 0;

    left_up->values[0] = 0;
    left_up->values[1] = 0;
    left_up->values[2] = 0;
    left_up->values[3] = 0;
    right_middle->values[0] = 16;
    right_middle->values[1] = 16;
    right_middle->values[2] = 16;
    right_middle->values[3] = 16;

    right_down->values[0] = 0;
    right_down->values[1] = 0;
    right_down->values[2] = 0;
    right_down->values[3] = 0;

    right_up->values[0] = 0;
    right_up->values[1] = 0;
    right_up->values[2] = 0;
    right_up->values[3] = 0;

}

void MainDude::handleKeyInput(int keys_held, int keys_up) {

    if (keys_held) {

        if (keys_held & KEY_UP) {
            if (bottomCollision) {
                ySpeed = -JUMP_SPEED;
            }
            if ((hangingOnTileLeft || hangingOnTileRight) && hangingTimer > MIN_HANGING_TIME) {
                ySpeed = -JUMP_SPEED;
                hangingOnTileLeft = false;
                hangingOnTileRight = false;
            }
        }

        if (keys_held & KEY_LEFT) {
            state = W_LEFT;
            hangingOnTileLeft = false;
            if (xSpeed > -MAX_X_SPEED && !(hangingOnTileRight || hangingOnTileLeft))
                if (speedIncTimerX > X_SPEED_DELTA_TIME_MS) {
                    xSpeed -= X_SPEED_DELTA;
                    speedIncTimerX = 0;
                }
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
    posIncTimerX += timeElapsed;
    posIncTimerY += timeElapsed;
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

    bool pixelPerfectCollision = xSpeed < 10 || ySpeed < 10;

    if (pixelPerfectCollision) {
        if (posIncTimerX > 20 / xSpeed * 3) {
            if (xSpeed > 0) {
                if (pixelPerfectCollision)
                    x += 1;
                else
                    x += 2;
            } else if (xSpeed < 0)
                if (pixelPerfectCollision)
                    x -= 1;
                else
                    x -= 2;
            posIncTimerX = 0;
//        x += xSpeed;
        }

        if (posIncTimerY > 20 / ySpeed * 3) {
            if (ySpeed > 0)
                if (pixelPerfectCollision)
                    y += 1;
                else
                    y += 2;
            else if (ySpeed < 0)
                if (pixelPerfectCollision)
                    y -= 1;
                else
                    y -= 2;
//        y += ySpeed;
            posIncTimerY = 0;
        }
    }

    int xx = floor_div(this->x + 0.5 * MAIN_DUDE_WIDTH, 16);
    int yy = floor_div(this->y + 0.5 * MAIN_DUDE_HEIGHT, 16);

    MapTile *left_middle = mapTiles[xx - 1][yy];
    MapTile *right_middle = mapTiles[xx + 1][yy];
    MapTile *up_middle = mapTiles[xx][yy - 1];
    MapTile *down_middle = mapTiles[xx][yy + 1];
    MapTile *center = mapTiles[xx][yy];
    MapTile *left_up = mapTiles[xx - 1][yy - 1];
    MapTile *right_up = mapTiles[xx + 1][yy - 1];
    MapTile *left_down = mapTiles[xx - 1][yy + 1];
    MapTile *right_down = mapTiles[xx + 1][yy + 1];
    MapTile *tiles[9] = {
            left_middle, //0
            right_middle, //1
            up_middle, //2
            down_middle, //3
            center, //4
            left_up, //5
            right_up, //6
            left_down, //7
            right_down //8
    };

    bottomCollision = Collisions::checkBottomCollision(tiles, &this->x, &this->y, &ySpeed, 16, 16);
    leftCollision = Collisions::checkLeftCollision(tiles, &this->x, &this->y, &xSpeed, 16, 16, !bottomCollision);
    rightCollision = Collisions::checkRightCollision(tiles, &this->x, &this->y, &xSpeed, 16, 16, !bottomCollision);
    upperCollision = Collisions::checkUpperCollision(tiles, &this->x, &this->y, &ySpeed, 16);
    Collisions::isStandingOnEdge(tiles, &this->x, &this->y, &ySpeed, 16, 16);


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

    if (!bottomCollision) {
        if (/*up_middle == 0 && down_middle == 0 && */(right_middle == 0 && (right_up != 0 && right_down != 0))) {
            if (xSpeed > 0)
                x += 1;
        }
        if (/*up_middle == 0 && down_middle == 0 && */(left_middle == 0 && (left_up != 0 && left_down != 0))) {
            if (xSpeed < 0)
                x -= 1;
        }
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

                        fprintf(stdout, "HANGING" + '\n');

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

