//
// Created by xdbeef on 24.02.18.
//

#ifndef SPELUNKYDS_MAINDUDE_H
#define SPELUNKYDS_MAINDUDE_H

#define MAIN_DUDE_MAX_X_SPEED 2
#define MAIN_DUDE_MAX_X_SPEED_CRAWLING 1.5
#define MAIN_DUDE_MAX_Y_SPEED 2.1
#define MAIN_DUDE_JUMP_SPEED 2.5
#define MAIN_DUDE_HEIGHT 16
#define MAIN_DUDE_WIDTH 16

#define MIN_HANGING_TIME 100
#define STUN_TIME 2000
#define STUN_FALLING_TIME 1200
#define PUSHING_TIME 500

#include <nds.h>
#include <vector>
#include "SpriteState.h"
#include "../level_layout/MapTile.h"
#include "../level_layout/LevelGenerator.h"
#include "Camera.h"
#include "../level_layout/SpriteInfo.h"
#include "Bomb.h"
#include "../sprites/MovingObject.h"
#include "../input/InputHandler.h"

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

    SpriteInfo *main_whip = nullptr;
    SpriteInfo *main_pre_whip = nullptr;
    SpriteInfo *sub_whip = nullptr;
    SpriteInfo *sub_pre_whip = nullptr;
    SpriteInfo *main_spelunker = nullptr;
    SpriteInfo *sub_spelunker = nullptr;

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
    int timeSinceLastJump;

    bool hangingOnTileLeft;
    bool hangingOnTileRight;
    bool stunned;
    bool crawling;
    bool pushing_left;
    bool pushing_right;
    bool whip;
    bool canClimbRope;
    bool canClimbLadder;
    bool climbing;
    bool onTopOfClimbingSpace;
    bool exitingLevel;
    bool holding_item;

    SpriteState state;
    int animFrame;
    u8 *frameGfx = nullptr;

    double *timer = nullptr;

    void handleKeyInput();

    void canHangOnTile(MapTile *neighboringTiles[9]);


};


#endif //SPELUNKYDS_MAINDUDE_H
