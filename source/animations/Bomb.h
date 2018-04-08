//
// Created by xdbeef on 04.04.18.
//

#ifndef SPELUNKYDS_BOMB_H
#define SPELUNKYDS_BOMB_H

#define BOUNCE_VELOCITY_LOSS 0.65
#define BOMB_SIZE 8
#define ARMED_TIME_BLINK_SLOW 2000
#define ARMED_TIME_BLINK_FAST 2000

#define MAX_X_SPEED_BOMB 1
#define MAX_Y_SPEED_BOMB 1.5

#include <nds/arm9/sprite.h>
#include "../level_layout/SpriteInfo.h"
#include "../level_layout/MapTile.h"
#include "Camera.h"
#include "../level_layout/LevelGenerator.h"
#include "../sprites/MovingObject.h"

class Bomb : public MovingObject {

public:

    void updateOther() override {};

    void init(OAMManager *mainOam, OAMManager *subOam) override;

    void draw() override;

    void updateTimers() override {};

    void updatePosition() override ;

    void updateSpeed() override;

    void updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) override;

    void updateCollisionsOtherMoving() override {};

    void onCollisionWithMainCharacter() override {};


    double *timer = nullptr;
    double pos_inc_timer;

    SpriteInfo *mainSpriteInfo = nullptr;
    SpriteInfo *subSpriteInfo = nullptr;

    bool carried;
    bool armed;

    int armedTimer;

};


#endif //SPELUNKYDS_BOMB_H
