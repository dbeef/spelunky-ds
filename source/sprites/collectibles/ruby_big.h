//
// Created by xdbeef on 16.09.18.
//

#ifndef SPELUNKYDS_RUBY_BIG_H
#define SPELUNKYDS_RUBY_BIG_H

#include "../moving_object.h"
#include "moniez.hpp"

#define RUBY_BIG_PHYSICAL_HEIGHT 8
#define RUBY_BIG_PHYSICAL_WIDTH 8

#define RUBY_BIG_SPRITE_HEIGHT 8
#define RUBY_BIG_SPRITE_WIDTH 8
#define RUBY_BIG_SPRITE_SIZE RUBY_BIG_SPRITE_WIDTH *RUBY_BIG_SPRITE_HEIGHT

class RubyBig : public MovingObject, public Moniez {

public:

    void introduce_yourself() override { printf("RUBY BIG\n"); };

    RubyBig();

    RubyBig(int x, int y, SpriteType sprite_type);

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


#endif //SPELUNKYDS_RUBY_BIG_H
