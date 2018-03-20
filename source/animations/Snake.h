//
// Created by xdbeef on 18.03.18.
//
//TODO Abstract class and interfaces for both MainDude and Snake to extend

#ifndef SPELUNKYDS_SNAKE_H
#define SPELUNKYDS_SNAKE_H


#include <nds.h>
#include "SpriteState.h"
#include "../level_layout/LevelGenerator.h"

class Snake {

public:

    int nextRandomizedWalkingTimestamp;

    int animationFrameTimer;

    int speedIncTimerX;
    int speedIncTimerY;
    int posIncTimer;

    int frictionTimer = 0;

    bool bottomCollision;
    bool upperCollision;
    bool leftCollision;
    bool rightCollision;

    double xSpeed;
    double ySpeed;
    int x;
    int y;
    int animFrame;
    u16 *spriteGfxMemMain;
    u16 *spriteGfxMemSub;
    u8 *frameGfx;

    void animate(int camera_x, int camera_y);

    void init();

    void updateTimers(int timeElapsed);

    void applyFriction();

    void checkCollisionWithMap(MapTile *mapTiles[32][32]);

    void update(int camera_x, int camera_y, int keys_held, int keys_up, LevelGenerator *l);

};


#endif //SPELUNKYDS_SNAKE_H
