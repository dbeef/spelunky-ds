//
// Created by xdbeef on 29.04.18.
//

#ifndef SPELUNKYDS_COPYRIGHTS_H
#define SPELUNKYDS_COPYRIGHTS_H


#include "../moving_object.h"

class Copyrights : public MovingObject {

public:

    void initSprite() override;

    void updateOther() override {};

    void kill() override {};

    void init() override;

    void draw() override;

    void updateTimers() override {};

    void update_position() override;

    void updateSpeed() override;

    void updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) override;

    void updateCollisionsOtherMoving() override {};

    void onCollisionWithMainCharacter() override {};

    SpriteInfo *mainSpriteInfo_1{};
    SpriteInfo *mainSpriteInfo_2{};
    SpriteInfo *mainSpriteInfo_3{};
    SpriteInfo *mainSpriteInfo_4{};
    SpriteInfo *mainSpriteInfo_5{};

    SpriteInfo *subSpriteInfo_1{};
    SpriteInfo *subSpriteInfo_2{};
    SpriteInfo *subSpriteInfo_3{};
    SpriteInfo *subSpriteInfo_4{};
    SpriteInfo *subSpriteInfo_5{};

    OamType oamType{};
};


#endif //SPELUNKYDS_COPYRIGHTS_H
