//
// Created by xdbeef on 30.04.18.
//

#ifndef SPELUNKYDS_TITLEMENUSIGN_H
#define SPELUNKYDS_TITLEMENUSIGN_H


#include "../sprites/MovingObject.h"
#include "MenuSignType.h"

class TitleMenuSign : public MovingObject {

public:

    void initSprite() override;

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

    SpriteInfo *mainSpriteInfo_1 = nullptr;
    SpriteInfo *mainSpriteInfo_2 = nullptr;

    SpriteInfo *subSpriteInfo_1 = nullptr;
    SpriteInfo *subSpriteInfo_2 = nullptr;

    OamType oamType;
    MenuSignType  menuSignType;
};



#endif //SPELUNKYDS_TITLEMENUSIGN_H
