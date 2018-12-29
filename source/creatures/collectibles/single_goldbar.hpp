//
// Created by xdbeef on 16.09.18.
//

#ifndef SPELUNKYDS_ONE_GOLDBAR_H
#define SPELUNKYDS_ONE_GOLDBAR_H

#include "../_base_creature.h"
#include "moniez.hpp"

#define SINGLE_GOLDBAR_PHYSICAL_HEIGHT 5
#define SINGLE_GOLDBAR_PHYSICAL_WIDTH 8

#define SINGLE_GOLDBAR_SPRITE_HEIGHT 16
#define SINGLE_GOLDBAR_SPRITE_WIDTH 16
#define SINGLE_GOLDBAR_SPRITE_SIZE SINGLE_GOLDBAR_SPRITE_WIDTH * SINGLE_GOLDBAR_SPRITE_HEIGHT

class SingleGoldbar : public BaseCreature, public Moniez {

public:

    void introduce_yourself() override { printf("SINGLE GOLDBAR\n"); };

    void updateOther() override {};

    void init() override;

    void draw() override;

    void initSprite() override;

    void deleteSprite() override;

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

    SingleGoldbar();
};

#endif //SPELUNKYDS_ONE_GOLDBAR_H
