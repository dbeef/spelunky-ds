//
// Created by xdbeef on 02.05.18.
//

#ifndef SPELUNKYDS_BAT_H
#define SPELUNKYDS_BAT_H

#include "../sprites/MovingObject.h"
#include "SpriteState.h"

class Bat : public MovingObject {

public:

    void updateOther() override {};

    void init() override;

    void draw() override;

    void kill() override ;

    void updateTimers() override {};

    void updatePosition() override;

    void updateSpeed() override;

    void updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) override;

    void updateCollisionsOtherMoving() override {};

    void onCollisionWithMainCharacter() override {};

    double *timer = nullptr;
    double pos_inc_timer;

    SpriteInfo *mainSpriteInfo = nullptr;
    SpriteInfo *subSpriteInfo = nullptr;
    u8 *frameGfx;

    SpriteState spriteState;

    int animFrame;
    int animFrameTimer;

    bool hanging;
    bool hunting;
};



#endif //SPELUNKYDS_BAT_H
