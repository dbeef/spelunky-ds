//
// Created by xdbeef on 29.07.18.
//

#ifndef SPELUNKYDS_FAKE_SKELETON_H
#define SPELUNKYDS_FAKE_SKELETON_H

#include "../sprite_info.h"
#include "../sprite_state.hpp"
#include "../moving_object.h"

#define FAKE_SKELETON_SPRITE_HEIGHT 16
#define FAKE_SKELETON_SPRITE_WIDTH 16
#define FAKE_SKELETON_SPRITE_SIZE FAKE_SKELETON_SPRITE_WIDTH * FAKE_SKELETON_SPRITE_HEIGHT

#define FAKE_SKELETON_PHYSICAL_HEIGHT_WOUT_SKULL 4
#define FAKE_SKELETON_PHYSICAL_HEIGHT_WITH_SKULL 7
#define FAKE_SKELETON_PHYSICAL_WIDTH 8

#define MAX_X_SPEED_FAKE_SKELETON 4
#define MAX_Y_SPEED_FAKE_SKELETON 4

class FakeSkeleton : public MovingObject {

public:

    void introduce_yourself() override { printf("FAKE_SKELETON\n"); };

    FakeSkeleton();

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

    bool tried_to_pickup{};
    int animFrameTimer{};
    int animFrame{};
    double pos_inc_timer{};

    SpriteInfo *mainSpriteInfo{};
    SpriteInfo *subSpriteInfo{};

    u8 *frameGfx{};

    void set_position();

    void spawn_skull();

    void match_animation();
};


#endif //SPELUNKYDS_FAKE_SKELETON_H
