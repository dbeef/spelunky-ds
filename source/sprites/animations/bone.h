//
// Created by xdbeef on 29.07.18.
//

#ifndef SPELUNKYDS_BONE_H
#define SPELUNKYDS_BONE_H

#include "../sprite_info.h"
#include "../sprite_state.hpp"
#include "../moving_object.h"

#define BONE_SPRITE_HEIGHT 16
#define BONE_SPRITE_WIDTH 16

#define BONE_PHYSICAL_HEIGHT 8
#define BONE_PHYSICAL_WIDTH 8

#define MAX_X_SPEED_BONE 4
#define MAX_Y_SPEED_BONE 4

class Bone : public MovingObject {

public:

    Bone();

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

};



#endif //SPELUNKYDS_BONE_H
