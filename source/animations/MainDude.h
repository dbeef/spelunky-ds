//
// Created by xdbeef on 24.02.18.
//

#ifndef SPELUNKYDS_MAINDUDE_H
#define SPELUNKYDS_MAINDUDE_H
#define MAX_X_SPEED 1.5
#define MAX_X_SPEED_CRAWLING 0.25

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
#define STUN_TIME 2000
#define STUN_JUMPING_TIME 1500
#define PUSHING_TIME 500

#include <nds.h>
#include "SpriteState.h"
#include "../level_layout/MapTile.h"
#include "../level_layout/LevelGenerator.h"
#include "Camera.h"
#include "../level_layout/SpriteInfo.h"
#include "Bomb.h"

struct MainDude {

    //wrócić do starego sposobu, przy czym nadal sprawdzać kolizję co piksel robiąc warunek:
    //while(xspeed > 0) xspeed--; xpos--;

public:

    SpriteInfo *main_whip_left_spriteInfo;
    SpriteInfo *main_whip_right_spriteInfo;
    SpriteInfo *main_pre_whip_left_spriteInfo;
    SpriteInfo *main_pre_whip_right_spriteInfo;

    SpriteInfo *sub_whip_left_spriteInfo;
    SpriteInfo *sub_whip_right_spriteInfo;
    SpriteInfo *sub_pre_whip_left_spriteInfo;
    SpriteInfo *sub_pre_whip_right_spriteInfo;

    SpriteInfo *main_spriteInfo;
    SpriteInfo *sub_spriteInfo;

    Bomb *bomb;
    bool key_left;
    bool key_right;

    int animationFrameTimer;

    int speedIncTimer;
    int posIncTimer;

    int frictionTimer = 0;

    //How much time was spent on hanging, since last flag hangingOnTileLeft/right occured.
    int hangingTimer;

    bool stunned;
    int stunnedTimer;
    int jumpingTimer;
    int pushingTimer;
    int whip_timer;

    bool hangingOnTileLeft;
    bool hangingOnTileRight;
    bool bottomCollision;
    bool upperCollision;
    bool leftCollision;
    bool rightCollision;
    bool crawling;
    bool pushing_left;
    bool pushing_right;
    bool whip;

    SpriteState state;

    double xSpeed;
    double ySpeed;
    int x;
    int y;
    int animFrame;
    u8 *frameGfx;

    double *timer;

    void checkCollisionWithMap(MapTile *mapTiles[32][32], int xx, int yy);

    void updateSpeed(MapTile *mapTiles[32][32]);

    void animate(Camera * camera);

    void handleKeyInput(int keys_held, int keys_up);

    void init(double *timer, Bomb *bomb);

    void updateTimers(int timeElapsed);

    void canHangOnTile(MapTile *neighboringTiles[9]);

    void applyFriction();

    void update(Camera *camera, int keys_held, int keys_up, LevelGenerator *l);

    void clearTilesOnRight(MapTile *mapTile[32][32]);

    void hideWhipLeftMain();
    void hideWhipLeftSub();
    void hideWhipRightMain();
    void hideWhipRightSub();
    
    void hidePreWhipLeftMain();
    void hidePreWhipLeftSub();
    void hidePreWhipRightMain();
    void hidePreWhipRightSub();
    
    void showWhipLeftMain();
    void showWhipLeftSub();
    void showWhipRightMain();
    void showWhipRightSub();

    void showPreWhipLeftMain();
    void showPreWhipLeftSub();
    void showPreWhipRightMain();
    void showPreWhipRightSub();
};


#endif //SPELUNKYDS_MAINDUDE_H
