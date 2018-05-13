//
// Created by xdbeef on 26.04.18.
//

#ifndef SPELUNKYDS_MONIEZ_H
#define SPELUNKYDS_MONIEZ_H

#define RUBY_PHYSICAL_HEIGHT 8
#define RUBY_PHYSICAL_WIDTH 8
#define RUBY_SPRITE_HEIGHT 8
#define RUBY_SPRITE_WIDTH 8

#define TRIPLE_GOLDBAR_PHYSICAL_HEIGHT 12
#define TRIPLE_GOLDBAR_PHYSICAL_WIDTH 16
#define TRIPLE_GOLDBAR_SPRITE_HEIGHT 16
#define TRIPLE_GOLDBAR_SPRITE_WIDTH 16

#include "../moving_object.h"
#include "../sprite_info.h"

class Moniez: public MovingObject {

public:

    Moniez();
    
    void updateOther() override {};

    void init() override;

    void draw() override;

    void initSprite() override;

    void kill() override {};

    void updateTimers() override {};

    void updateSpeed() override;

    void updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) override;

    void updateCollisionsOtherMoving() override {};

    void onCollisionWithMainCharacter() override {};


    double pos_inc_timer{};

    SpriteInfo *mainSpriteInfo {};
    SpriteInfo *subSpriteInfo {};
    u8 * frameGfx{};

    int value{};
    int ruby_type{};

    bool collected{};
    SpritesheetType  spriteType{};
    void set_position();
};



#endif //SPELUNKYDS_MONIEZ_H
