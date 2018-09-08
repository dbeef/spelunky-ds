//
// Created by xdbeef on 02.06.18.
//

#ifndef SPELUNKYDS_FLAME_ELEMENT_H
#define SPELUNKYDS_FLAME_ELEMENT_H

#include "../moving_object.h"
#include "../sprite_info.h"

class FlameElement : public MovingObject {

public:

    FlameElement();

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


    SpriteInfo *mainSpriteInfo {};
    SpriteInfo *subSpriteInfo {};

    u8 * frameGfx{};

    int currentFrame{};
    double frameTimer{};
    double pos_inc_timer{};
    double inactive_delay{};
    int pos_inc_delta_offset{};
    bool finished{};

    void set_position();

    void match_animation();
};





#endif //SPELUNKYDS_FLAME_ELEMENT_H
