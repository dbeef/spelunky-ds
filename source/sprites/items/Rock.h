//
// Created by xdbeef on 23.04.18.
//

#ifndef SPELUNKYDS_ROCK_H
#define SPELUNKYDS_ROCK_H

#define MAX_X_SPEED_ROCK 4
#define MAX_Y_SPEED_ROCK 4

#include <nds/arm9/sprite.h>

class Rock : public MovingObject {

public:

    void updateOther() override {};

    void initSprite() override;

    void init() override;

    void kill() override {};

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
    u8 * frameGfx;

};


#endif //SPELUNKYDS_ROCK_H
