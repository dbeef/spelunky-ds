//
// Created by xdbeef on 26.04.18.
//

#ifndef SPELUNKYDS_MONIEZ_H
#define SPELUNKYDS_MONIEZ_H



#include "../sprites/MovingObject.h"

class Moniez: public MovingObject {

public:

    void updateOther() override {};

    void init() override;

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

    int sprite_width;
    int sprite_height;

    int physics_width;
    int physics_height;

    int value;

    bool collected;
    SpriteType  spriteType;
};



#endif //SPELUNKYDS_MONIEZ_H
