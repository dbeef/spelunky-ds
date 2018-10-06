//
// Created by xdbeef on 16.09.18.
//

#ifndef SPELUNKYDS_TRIPLE_GOLDBAR_H
#define SPELUNKYDS_TRIPLE_GOLDBAR_H

#include "../moving_object.h"
#include "moniez.hpp"

#define TRIPLE_GOLDBAR_PHYSICAL_HEIGHT 10
#define TRIPLE_GOLDBAR_PHYSICAL_WIDTH 16

#define TRIPLE_GOLDBAR_SPRITE_HEIGHT 16
#define TRIPLE_GOLDBAR_SPRITE_WIDTH 16
#define TRIPLE_GOLDBAR_SPRITE_SIZE TRIPLE_GOLDBAR_SPRITE_WIDTH * TRIPLE_GOLDBAR_SPRITE_HEIGHT

class TripleGoldbar : public MovingObject, public Moniez {

public:

    void introduce_yourself() override { printf("TRIPLE GOLDBAR\n"); };

    TripleGoldbar();

    TripleGoldbar(int x, int y, SpriteType sprite_type);

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

};

#endif //SPELUNKYDS_TRIPLE_GOLDBAR_H
