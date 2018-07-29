//
// Created by xdbeef on 29.07.18.
//

#ifndef SPELUNKYDS_SKULL_H
#define SPELUNKYDS_SKULL_H

#include "../sprite_info.h"
#include "../sprite_state.hpp"
#include "../moving_object.h"

#define SKULL_SPRITE_HEIGHT 16
#define SKULL_SPRITE_WIDTH 16

#define SKULL_PHYSICAL_HEIGHT 8
#define SKULL_PHYSICAL_WIDTH 8

#define MAX_X_SPEED_SKULL 4
#define MAX_Y_SPEED_SKULL 4

class Skull : public MovingObject {

public:

    Skull();

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

    bool collided{};
    int animFrameTimer{};
    int animFrame{};
    int pos_inc_timer{};

    SpriteInfo *mainSpriteInfo{};
    SpriteInfo *subSpriteInfo{};

    u8 *frameGfx{};

    void set_position();

    void set_orientation();
};




#endif //SPELUNKYDS_SKULL_H
