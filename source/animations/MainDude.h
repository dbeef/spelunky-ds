//
// Created by xdbeef on 24.02.18.
//

#ifndef SPELUNKYDS_MAINDUDE_H
#define SPELUNKYDS_MAINDUDE_H
#define MAX_X_SPEED 1.5

#define MAX_Y_SPEED 2
#define MIN_HANGING_TIME 100
#define FRICTION_DELTA_TIME_MS 30
#define FRICTION_DELTA_SPEED 1
#define Y_SPEED_DELTA_TIME_MS 2
#define GRAVITY_DELTA_SPEED 0.19
#define X_SPEED_DELTA_TIME_MS 2
#define X_SPEED_DELTA 2
#define JUMP_SPEED 2.3
#define MAIN_DUDE_HEIGHT 16
#define MAIN_DUDE_WIDTH 16

#include <nds.h>
#include "SpriteState.h"
#include "../level_layout/MapTile.h"
#include "../level_layout/LevelGenerator.h"
#include "Camera.h"
#include "../level_layout/SpriteInfo.h"

struct MainDude {

    //wrócić do starego sposobu, przy czym nadal sprawdzać kolizję co piksel robiąc warunek:
    //while(xspeed > 0) xspeed--; xpos--;

public:

    SpriteInfo *main_spriteInfo;
    SpriteInfo *sub_spriteInfo;

    int animationFrameTimer;

    int speedIncTimer;
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
    u8 *frameGfx;

    void checkCollisionWithMap(MapTile *mapTiles[32][32], int xx, int yy);

    void updateSpeed(MapTile *mapTiles[32][32]);

    void animate(Camera * camera);

    void handleKeyInput(int keys_held, int keys_up);

    void init();

    void updateTimers(int timeElapsed);

    void canHangOnTile(MapTile *neighboringTiles[9]);

    void applyFriction();

    void update(Camera *camera, int keys_held, int keys_up, LevelGenerator *l);

    void clearTilesOnRight(MapTile *mapTile[32][32]);
};


#endif //SPELUNKYDS_MAINDUDE_H
