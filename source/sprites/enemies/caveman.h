//
// Created by xdbeef on 31.05.18.
// http://spelunky.wikia.com/wiki/Caveman
//

#ifndef SPELUNKYDS_CAVEMAN_H
#define SPELUNKYDS_CAVEMAN_H

#define CAVEMAN_STUN_TIME 7000
#define CAVEMAN_HITPOINTS 3

#define CAVEMAN_PHYSICAL_HEIGHT 16
#define CAVEMAN_PHYSICAL_WIDTH 16

#define CAVEMAN_SPRITE_HEIGHT 16
#define CAVEMAN_SPRITE_WIDTH 16
#define CAVEMAN_SPRITE_SIZE CAVEMAN_SPRITE_WIDTH * CAVEMAN_SPRITE_HEIGHT

#define MAX_X_SPEED_CAVEMAN 4
#define MAX_Y_SPEED_CAVEMAN 4

#include "../moving_object.h"
#include "../sprite_state.hpp"
#include "../sprite_info.h"

class Caveman : public MovingObject {

public:

    void introduce_yourself() override { printf("CAVEMAN\n"); };

    Caveman();

    Caveman(int x, int y);

    void updateOther() override {};

    void init() override;

    void draw() override;

    void apply_dmg(int dmg_to_apply) override;

    void updateTimers() override {};

    void updateSpeed() override;

    void updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) override;

    void updateCollisionsOtherMoving() override {};

    void onCollisionWithMainCharacter() override {};

    void initSprite() override;

    double pos_inc_timer{};

    SpriteInfo *mainSpriteInfo{};
    SpriteInfo *subSpriteInfo{};
    u8 *frameGfx{};

    double waitTimer{};
    double goTimer{};

    int animFrame{};
    double animFrameTimer{};

    double invert_speed_timer{};

    double blood_spawn_timer{};

    bool landlocked{};
    bool triggered{};
    bool stunned{};

    double stunned_timer{};

    void randomizeMovement();

    void set_position();

    void apply_walking_sprites();

    void apply_stunned_sprites();

    void make_some_movement();

    void check_if_can_be_triggered();

    void apply_stunned_carried_sprites();

    void apply_dead_carried_sprites();

    void apply_dead_sprites();

    void match_animation();
};


#endif //SPELUNKYDS_CAVEMAN_H
