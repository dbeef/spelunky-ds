//
// Created by xdbeef on 04.04.18.
//

#ifndef SPELUNKYDS_ROPE_H
#define SPELUNKYDS_ROPE_H

#include <nds/arm9/sprite.h>
#include <vector>
#include "rope_element.h"

#define MAX_Y_SPEED_ROPE 4
#define MAX_X_SPEED_ROPE 4
#define ROPE_SIZE 8

class Rope : public MovingObject {

public:

    void updateOther() override {};

    void init() override;

    void kill() override {};

    void draw() override;

    void initSprite() override;

    void updateTimers() override {};

    void updatePosition() override ;

    void updateSpeed() override;

    void updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) override;

    void updateCollisionsOtherMoving() override {};

    void onCollisionWithMainCharacter() override;


    double pos_inc_timer{};

    SpriteInfo *mainSpriteInfo {};
    SpriteInfo *subSpriteInfo {};

    u8 * frameGfx{};

    bool thrown{};
    bool finished{};

    int throwingTimer{};
    std::vector<RopeElement *> ropeChain{};

    void throwingFinished();
    void notThrown();


    bool isThereChainForThisTile(int rope_y_tiles);
};


#endif //SPELUNKYDS_ROPE_H
