//
// Created by xdbeef on 04.04.18.
//

#ifndef SPELUNKYDS_ROPE_ELEMENT_H
#define SPELUNKYDS_ROPE_ELEMENT_H

#include <nds/arm9/sprite.h>
#include <vector>
#include "../level_layout/SpriteInfo.h"
#include "../level_layout/MapTile.h"
#include "Camera.h"
#include "../level_layout/LevelGenerator.h"
#include "../sprites/MovingObject.h"

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


    SpriteInfo *mainSpriteInfo = nullptr;
    SpriteInfo *subSpriteInfo = nullptr;

    SpriteInfo *mainSpriteInfo_2 = nullptr;
    SpriteInfo *subSpriteInfo_2 = nullptr;

    u8 * frameGfx;

};


#endif //SPELUNKYDS_ROPE_ELEMENT_H
