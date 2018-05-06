//
// Created by xdbeef on 04.04.18.
//

#ifndef SPELUNKYDS_BOMB_H
#define SPELUNKYDS_BOMB_H

#define BOUNCE_VELOCITY_LOSS 0.65
#define BOMB_SIZE 8
#define ARMED_TIME_BLINK_SLOW 2000
#define ARMED_TIME_BLINK_FAST 3500

#define MAX_X_SPEED_BOMB 4
#define MAX_Y_SPEED_BOMB 4

#include <nds/arm9/sprite.h>
#include "../../camera/camera.h"
#include "../moving_object.h"
#include "../sprite_info.h"

class Bomb : public MovingObject {

public:

    void updateOther() override {};

    void init() override;

    void kill() override {};

    void draw() override;

    void initSprite() override;

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
    u8 * frameGfx;

    //fixme cloning carried_by_main_dude
//    bool carried;
    bool armed;

    int armedTimer;
    int explosionTimer;

    int explosionFrame;

    void disarm();

    void arm();
};


#endif //SPELUNKYDS_BOMB_H
