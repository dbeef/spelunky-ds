//
// Created by xdbeef on 24.02.18.
//

#ifndef SPELUNKYDS_MAINDUDE_H
#define SPELUNKYDS_MAINDUDE_H

#define MAIN_DUDE_MAX_X_SPEED 1.5
#define MAIN_DUDE_MAX_X_SPEED_CRAWLING 0.25
#define MAIN_DUDE_MAX_Y_SPEED 2
#define MAIN_DUDE_JUMP_SPEED 2.3
#define MAIN_DUDE_HEIGHT 16
#define MAIN_DUDE_WIDTH 16

#define MIN_HANGING_TIME 100
#define STUN_TIME 2000
#define STUN_FALLING_TIME 1500
#define PUSHING_TIME 500

#include <nds.h>
#include "SpriteState.h"
#include "../level_layout/MapTile.h"
#include "../level_layout/LevelGenerator.h"
#include "Camera.h"
#include "../level_layout/SpriteInfo.h"
#include "Bomb.h"
#include "../sprites/MovingObject.h"

class MainDude : public MovingObject {

public:

    void updateOther() override;

    void draw() override;

    void updateTimers() override;

    void updatePosition() override;

    void updateSpeed() override;

    void updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) override;

    void updateCollisionsOtherMoving() override {};

    void onCollisionWithMainCharacter() override {};

    void init() override;

    SpriteInfo *main_whip_left_spriteInfo;
//    SpriteInfo *main_whip_right_spriteInfo;
    SpriteInfo *main_pre_whip_left_spriteInfo;
//    SpriteInfo *main_pre_whip_right_spriteInfo;

    SpriteInfo *sub_whip_left_spriteInfo;
//    SpriteInfo *sub_whip_right_spriteInfo;
    SpriteInfo *sub_pre_whip_left_spriteInfo;
//    SpriteInfo *sub_pre_whip_right_spriteInfo;

    SpriteInfo *main_spriteInfo;
    SpriteInfo *sub_spriteInfo;

    Bomb *bomb;

    int animationFrameTimer;
    int speedIncTimer;
    int posIncTimer;
    int frictionTimer;
    //How much time was spent on hanging, since last flag hangingOnTileLeft/right occured.
    int hangingTimer;


    int stunnedTimer;
    int jumpingTimer;
    int pushingTimer;
    int whip_timer;

    //todo przenieść do osobnej klasy od inputu
    bool left_key_held;
    bool right_key_held;

    bool hangingOnTileLeft;
    bool hangingOnTileRight;
    bool stunned;
    bool crawling;
    bool pushing_left;
    bool pushing_right;
    bool whip;

    SpriteState state;

    int animFrame;
    u8 *frameGfx;

    double *timer;

    Camera *camera;

    void handleKeyInput(int keys_held, int keys_up);

    void canHangOnTile(MapTile *neighboringTiles[9]);


};


#endif //SPELUNKYDS_MAINDUDE_H
