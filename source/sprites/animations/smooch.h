//
// Created by xdbeef on 10.06.18.
//

#ifndef SPELUNKYDS_SMOOCH_H
#define SPELUNKYDS_SMOOCH_H

#include "../sprite_info.h"
#include "../sprite_state.h"
#include "../moving_object.h"

#define SMOOCH_SPRITE_HEIGHT 16
#define SMOOCH_SPRITE_WIDTH 16

class Smooch : public MovingObject {

public:

    Smooch();

    void updateOther() override {};

    void init() override;

    void draw() override;

    void initSprite() override;

    void apply_dmg(int dmg_to_apply) override {};

    void updateTimers() override {};

    void updateSpeed() override {};

    void updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) override {};

    void updateCollisionsOtherMoving() override {};

    void onCollisionWithMainCharacter() override {};

    int animFrameTimer{};
    int animFrame{};
    int cycle_counter{};

    SpriteInfo *mainSpriteInfo{};
    SpriteInfo *subSpriteInfo{};

    u8 *frameGfx{};

    void set_position();

};



#endif //SPELUNKYDS_SMOOCH_H
