//
// Created by xdbeef on 26.04.18.
//

#ifndef SPELUNKYDS_MONIEZ_H
#define SPELUNKYDS_MONIEZ_H

#define RUBY_BIG_PHYSICAL_HEIGHT 8
#define RUBY_BIG_PHYSICAL_WIDTH 8
#define RUBY_BIG_SPRITE_HEIGHT 8
#define RUBY_BIG_SPRITE_WIDTH 8

#define RUBY_SMALL_PHYSICAL_HEIGHT 4
#define RUBY_SMALL_PHYSICAL_WIDTH 4
#define RUBY_SMALL_SPRITE_HEIGHT 8
#define RUBY_SMALL_SPRITE_WIDTH 8

#define TRIPLE_GOLDBAR_PHYSICAL_HEIGHT 10
#define TRIPLE_GOLDBAR_PHYSICAL_WIDTH 16
#define TRIPLE_GOLDBAR_SPRITE_HEIGHT 16
#define TRIPLE_GOLDBAR_SPRITE_WIDTH 16

#define MAX_X_SPEED_MONIEZ 4
#define MAX_Y_SPEED_MONIEZ 4

#include "../moving_object.h"
#include "../sprite_info.h"

class Moniez : public MovingObject {

public:

    Moniez();

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

    int value{};
    int ruby_type{};

    int collectible_timer{};

    bool collected{};
    SpritesheetType spriteType{};

    void set_position();
};


#endif //SPELUNKYDS_MONIEZ_H
