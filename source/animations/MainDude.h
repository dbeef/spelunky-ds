//
// Created by xdbeef on 24.02.18.
//

#ifndef SPELUNKYDS_MAINDUDE_H
#define SPELUNKYDS_MAINDUDE_H
#define MAX_X_SPEED 1.5
#define MAX_Y_SPEED 2.7
#define MIN_HANGING_TIME 100
#define FRICTION_DELTA_TIME_MS 30
#define FRICTION_DELTA_SPEED 0.25
#define Y_SPEED_DELTA_TIME_MS 2
#define GRAVITY_DELTA_SPEED 0.2
#define X_SPEED_DELTA_TIME_MS 2
#define X_SPEED_DELTA 1
#define JUMP_SPEED MAX_Y_SPEED*0.75

#include <nds.h>
#include "SpriteState.h"
#include "../level_layout/MapTile.h"
#include "../level_layout/LevelGenerator.h"

struct MainDude {

public:

    int animationFrameTimer;

    int speedIncTimerX;
    int speedIncTimerY;
    int posIncTimer;

    int frictionTimer = 0;

    //How much time was spent on hanging, since last flag hangingOnTileLeft/right occured.
    int hangingTimer;

    bool hangingOnTileLeft;
    bool hangingOnTileRight;
    bool bottomCollision;
    bool upperCollision;
    bool leftCollision;
    bool rightCollision;

    SpriteState state;

    double xSpeed;
    double ySpeed;
    int x;
    int y;
    int animFrame;
    u16 *spriteGfxMemMain;
    u16 *spriteGfxMemSub;
    u8 *frameGfx;

    void checkCollisionWithMap(MapTile *mapTiles[32][32]);

    void animate(int camera_x, int camera_y);

    void handleKeyInput(int keys_held, int keys_up);

    void init();

    void updateTimers(int timeElapsed);

    void canHangOnTile(MapTile *mapTiles[32][32]);

    void applyFriction();

    void update(int camera_x, int camera_y, int keys_held, int keys_up, LevelGenerator *l);
};


#endif //SPELUNKYDS_MAINDUDE_H
