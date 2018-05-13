//
// Created by xdbeef on 04.04.18.
//

#ifndef SPELUNKYDS_ROPE_ELEMENT_H
#define SPELUNKYDS_ROPE_ELEMENT_H

#include <nds/arm9/sprite.h>
#include <vector>
#include "../../camera/camera.h"
#include "../moving_object.h"

#define ROPE_ELEMENT_SIZE 8

class RopeElement : public MovingObject {

public:

    void updateOther() override {};

    void kill() override {};

    void init() override;

    void draw() override;

    void updateTimers() override {};

    void initSprite() override;

    void updatePosition() override ;

    void updateSpeed() override;

    void updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) override;

    void updateCollisionsOtherMoving() override {};

    void onCollisionWithMainCharacter() override {};


    SpriteInfo *mainSpriteInfo {};
    SpriteInfo *subSpriteInfo {};

    SpriteInfo *mainSpriteInfo_2 {};
    SpriteInfo *subSpriteInfo_2 {};

    u8 * frameGfx{};

};


#endif //SPELUNKYDS_ROPE_ELEMENT_H
