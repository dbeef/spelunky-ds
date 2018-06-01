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

#define ROPE_PHYSICAL_HEIGHT 8
#define ROPE_PHYSICAL_WIDTH 8
#define ROPE_SPRITE_HEIGHT 8
#define ROPE_SPRITE_WIDTH 8


class Rope : public MovingObject {

public:

    Rope();

    void updateOther() override {};

    void init() override;

    void apply_dmg(int dmg_to_apply) override {};

    void draw() override;

    void initSprite() override;

    void updateTimers() override {};

    void updateSpeed() override;

    void updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) override;

    void updateCollisionsOtherMoving() override {};

    void onCollisionWithMainCharacter() override {};


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
