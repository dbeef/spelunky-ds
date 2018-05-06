//
// Created by xdbeef on 24.02.18.
//

#ifndef SPELUNKYDS_MAINDUDE_H
#define SPELUNKYDS_MAINDUDE_H

#define MAIN_DUDE_MAX_X_SPEED 2
#define MAIN_DUDE_MAX_X_SPEED_CRAWLING 1.5
#define MAIN_DUDE_MAX_Y_SPEED 2.5
#define MAIN_DUDE_JUMP_SPEED 3.3
#define MAIN_DUDE_HEIGHT 16
#define MAIN_DUDE_WIDTH 16

#define MIN_HANGING_TIME 100
#define STUN_TIME 2000
#define STUN_FALLING_TIME 1200
#define PUSHING_TIME 500
#define DAMAGE_PROTECTION_TIME 1500

#include <nds.h>
#include <vector>
#include "../sprite_state.h"
#include "../../tiles/map_tile.h"
#include "../../tiles/level_generator.h"
#include "../../camera/camera.h"
#include "../sprite_info.h"
#include "../items/bomb.h"
#include "../moving_object.h"
#include "../../input/input_handler.h"

class MainDude : public MovingObject {

public:

    void updateOther() override;

    void kill() override {};

    void draw() override;

    void initSprite() override;

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
    int time_since_last_damage;

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
    bool dead;

    SpriteState state;
    int animFrame;
    u8 *frameGfx = nullptr;

    void handleKeyInput();

    void canHangOnTile(MapTile *neighboringTiles[9]);


};


#endif //SPELUNKYDS_MAINDUDE_H
