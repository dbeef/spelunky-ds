//
// Created by xdbeef on 23.04.18.
//

#ifndef SPELUNKYDS_SNAKE_H
#define SPELUNKYDS_SNAKE_H


#include "../sprites/MovingObject.h"
#include "SpriteState.h"

class Snake : public MovingObject {

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

    void initSprite() override;

    double *timer = nullptr;
    double pos_inc_timer;

    SpriteInfo *mainSpriteInfo = nullptr;
    SpriteInfo *subSpriteInfo = nullptr;
    u8 *frameGfx;
    int sameDirectionInRow;


    //Snake goes for random amount of time on random direction, then waits random time and the cycle goes again
    SpriteState spriteState;
    int waitTimer;
    int goTimer;

    int animFrame;
    int animFrameTimer;

    void randomizeMovement();

    void set_position();

    void set_sprite_left();

    void set_sprite_right();
};


#endif //SPELUNKYDS_SNAKE_H
