//
// Created by xdbeef on 18.03.18.
//

#include <nds/arm9/sprite.h>
#include "Snake.h"
#include "../../build/snake.h"
#include "Collisions.h"
#include "../Consts.h"

#define SNAKE_FRAMES_PER_ANIMATION 4
#define MAX_X_SPEED 1.5
#define MAX_Y_SPEED 2.3
#define MIN_HANGING_TIME 100
#define FRICTION_DELTA_TIME_MS 30
#define FRICTION_DELTA_SPEED 0.25
#define Y_SPEED_DELTA_TIME_MS 5
#define GRAVITY_DELTA_SPEED 0.2
#define X_SPEED_DELTA_TIME_MS 50
#define X_SPEED_DELTA 1

void Snake::update(int camera_x, int camera_y, int keys_held, int keys_up, LevelGenerator *l) {
    this->applyFriction();
    this->updateTimers(timerElapsed(0) / TICKS_PER_SECOND);
    this->animate(camera_x, camera_y);
    this->checkCollisionWithMap(l->mapTiles);


}

void Snake::init() {
    spriteGfxMemMain = oamAllocateGfx(&oamMain, SpriteSize_16x16, SpriteColorFormat_256Color);
    spriteGfxMemSub = oamAllocateGfx(&oamSub, SpriteSize_16x16, SpriteColorFormat_256Color);
    frameGfx = (u8 *) snakeTiles;
    dmaCopy(snakePal, SPRITE_PALETTE, 512);
    dmaCopy(snakePal, SPRITE_PALETTE_SUB, 512);
}


void Snake::updateTimers(int timeElapsed) {
    speedIncTimerX += timeElapsed;
    speedIncTimerY += timeElapsed;
    posIncTimer += timeElapsed;
    frictionTimer += timeElapsed;
    if (xSpeed != 0)
        animationFrameTimer += timeElapsed;
}


void Snake::checkCollisionWithMap(MapTile *mapTiles[32][32]) {


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
    Collisions::isStandingOnEdge(mapTiles, &this->x, &this->y, &ySpeed, 16, 16);


    if (!bottomCollision) {
        if (speedIncTimerY > Y_SPEED_DELTA_TIME_MS) {
            ySpeed += GRAVITY_DELTA_SPEED;
            speedIncTimerY = 0;
        }
    }
}


void Snake::animate(int camera_x, int camera_y) {


    if (animationFrameTimer > 70) {
        animationFrameTimer = 0;
        animFrame++;
    }

    if (animFrame >= SNAKE_FRAMES_PER_ANIMATION) animFrame = 0;

    int state;
    if (xSpeed < 0)
        state = 0;
    else
        state = 1;

    int frame = animFrame + state * SNAKE_FRAMES_PER_ANIMATION;

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

