//
// Created by xdbeef on 03.05.18.
//

#ifndef SPELUNKYDS_SPIDER_H
#define SPELUNKYDS_SPIDER_H

#include <nds/jtypes.h>
#include "../level_layout/SpriteInfo.h"
#include "../sprites/MovingObject.h"
#include "SpriteState.h"

class Spider : public MovingObject {

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

    double random_speed;

    bool hanging;
    bool hunting;
    bool jumping;

};



#endif //SPELUNKYDS_SPIDER_H
