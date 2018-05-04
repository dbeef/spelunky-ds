//
// Created by xdbeef on 29.04.18.
//

#ifndef SPELUNKYDS_COPYRIGHTS_H
#define SPELUNKYDS_COPYRIGHTS_H


#include "../sprites/MovingObject.h"

class Copyrights : public MovingObject {

public:

    void initSprite() override;

    void updateOther() override {};

    void kill() override {};

    void init() override;

    void draw() override;

    void updateTimers() override {};

    void updatePosition() override ;

    void updateSpeed() override;

    void updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) override;

    void updateCollisionsOtherMoving() override {};

    void onCollisionWithMainCharacter() override {};

    SpriteInfo *mainSpriteInfo_1 = nullptr;
    SpriteInfo *mainSpriteInfo_2 = nullptr;
    SpriteInfo *mainSpriteInfo_3 = nullptr;
    SpriteInfo *mainSpriteInfo_4 = nullptr;
    SpriteInfo *mainSpriteInfo_5 = nullptr;

    SpriteInfo *subSpriteInfo_1 = nullptr;
    SpriteInfo *subSpriteInfo_2 = nullptr;
    SpriteInfo *subSpriteInfo_3 = nullptr;
    SpriteInfo *subSpriteInfo_4 = nullptr;
    SpriteInfo *subSpriteInfo_5 = nullptr;

    OamType oamType;
};




#endif //SPELUNKYDS_COPYRIGHTS_H
