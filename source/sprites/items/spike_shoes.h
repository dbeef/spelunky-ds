//
// Created by xdbeef on 02.06.18.
//

#ifndef SPELUNKYDS_SPIKE_SHOES_H
#define SPELUNKYDS_SPIKE_SHOES_H

#include "../sprite_info.h"
#include "../sprite_state.h"
#include "../moving_object.h"

#define SPIKE_SHOES_PHYSICAL_HEIGHT 11
#define SPIKE_SHOES_PHYSICAL_WIDTH 14
#define SPIKE_SHOES_SPRITE_HEIGHT 16
#define SPIKE_SHOES_SPRITE_WIDTH 16

#define MAX_X_SPEED_SPIKE_SHOES 4
#define MAX_Y_SPEED_SPIKE_SHOES 4

class SpikeShoes : public MovingObject {

public:

    SpikeShoes();

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

    double pos_inc_timer{};

    bool collected{};

    SpriteInfo *mainSpriteInfo{};
    SpriteInfo *subSpriteInfo{};

    u8 *frameGfx{};

    SpritesheetType spriteType{};

    void set_position();
};


#endif //SPELUNKYDS_SPIKE_SHOES_H
