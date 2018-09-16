//
// Created by xdbeef on 15.05.18.
//

#ifndef SPELUNKYDS_SPIKES_H
#define SPELUNKYDS_SPIKES_H

#include "../moving_object.h"

#define SPIKES_PHYSICAL_HEIGHT 13
#define SPIKES_PHYSICAL_WIDTH 16

#define SPIKES_SPRITE_HEIGHT 16
#define SPIKES_SPRITE_WIDTH 16
#define SPIKES_SPRITE_SIZE SPIKES_SPRITE_WIDTH * SPIKES_SPRITE_HEIGHT

//http://spelunky.wikia.com/wiki/Spikes
class Spikes: public MovingObject {

public:

    void introduce_yourself() override { printf("SPIKES\n"); };

    Spikes();

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


    double pos_inc_timer{};

    SpriteInfo *mainSpriteInfo {};
    SpriteInfo *subSpriteInfo {};
    u8 * frameGfx{};

    bool blood{};
    SpritesheetType  spriteType{};
    void set_position();

    void kill_main_dude();
};


#endif //SPELUNKYDS_SPIKES_H
