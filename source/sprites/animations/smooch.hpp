//
// Created by xdbeef on 10.06.18.
//

#ifndef SPELUNKYDS_SMOOCH_H
#define SPELUNKYDS_SMOOCH_H

#include "../sprite_info.h"
#include "../sprite_state.hpp"
#include "../moving_object.h"

#define SMOOCH_SPRITE_HEIGHT 16
#define SMOOCH_SPRITE_WIDTH 16
#define SMOOCH_SPRITE_SIZE SMOOCH_SPRITE_WIDTH * SMOOCH_SPRITE_HEIGHT

class Smooch : public MovingObject {

public:

    void introduce_yourself() override { printf("SMOOCH\n"); };

    Smooch();

    void updateOther() override {};

    void init() override;

    void draw() override;

    void initSprite() override;

    void deleteSprite() override;

    void apply_dmg(int dmg_to_apply) override {};

    void updateTimers() override {};

    void updateSpeed() override {};

    void updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) override {};

    void updateCollisionsOtherMoving() override {};

    void onCollisionWithMainCharacter() override {};

    double animFrameTimer{};
    int animFrame{};
    int cycle_counter{};

    SpriteInfo *mainSpriteInfo{};
    SpriteInfo *subSpriteInfo{};

    u8 *frameGfx{};

    void set_position();

    void match_animation();
};



#endif //SPELUNKYDS_SMOOCH_H
