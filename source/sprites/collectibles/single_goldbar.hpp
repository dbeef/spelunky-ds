//
// Created by xdbeef on 16.09.18.
//

#ifndef SPELUNKYDS_ONE_GOLDBAR_H
#define SPELUNKYDS_ONE_GOLDBAR_H

#include "../moving_object.h"

class SingleGoldbar : public MovingObject {

    void introduce_yourself() override { printf("SINGLE GOLDBAR\n"); };

    SingleGoldbar();

    SingleGoldbar(int x, int y, SpriteType sprite_type);

    void updateOther() override {};

    void init() override;

    void draw() override;

    void initSprite() override;

    void apply_dmg(int dmg_to_apply) override {};

    void updateTimers() override {};

    void updateSpeed() override;

    void updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) override;

    void updateCollisionsOtherMoving() override {};

    void onCollisionWithMainCharacter() override {};

    //rename to vectors_update_timer?
    double pos_inc_timer{};

    SpriteInfo *mainSpriteInfo{};
    SpriteInfo *subSpriteInfo{};
    u8 *frameGfx{};

    void set_position();

};

#endif //SPELUNKYDS_ONE_GOLDBAR_H
