//
// Created by xdbeef on 23.04.18.
//

#ifndef SPELUNKYDS_JAR_H
#define SPELUNKYDS_JAR_H

#include "../sprites/MovingObject.h"

class Jar : public MovingObject {

public:

    void updateOther() override {};

    void init() override;

    void draw() override;

    void kill() override {};

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

    int frame;
    int frameTimer;

};



#endif //SPELUNKYDS_JAR_H
