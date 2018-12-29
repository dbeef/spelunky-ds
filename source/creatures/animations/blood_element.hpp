//
// Created by xdbeef on 28.04.18.
//

#ifndef SPELUNKYDS_BLOODELEMENT_H
#define SPELUNKYDS_BLOODELEMENT_H

#include "../_base_creature.h"
#include "../sprite_info.h"

class BloodElement : public BaseCreature {

public:

    void introduce_yourself() override { printf("BLOOD_ELEMENT\n"); };

    BloodElement();

    void updateOther() override {};

    void init() override;

    void apply_dmg(int dmg_to_apply) override {};


    void draw() override;

    void initSprite() override;

    void deleteSprite() override;

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
    bool finished{};

    void set_position();

    void match_animation();
};



#endif //SPELUNKYDS_BLOODELEMENT_H
