//
// Created by xdbeef on 31.05.18.
//

#ifndef SPELUNKYDS_CAVEMAN_H
#define SPELUNKYDS_CAVEMAN_H

#define CAVEMAN_STUN_TIME 7000
#define CAVEMAN_HITPOINTS 3

#define CAVEMAN_PHYSICAL_HEIGHT 16
#define CAVEMAN_PHYSICAL_WIDTH 16
#define CAVEMAN_SPRITE_HEIGHT 16
#define CAVEMAN_SPRITE_WIDTH 16

#define MAX_X_SPEED_CAVEMAN 4
#define MAX_Y_SPEED_CAVEMAN 4

#include "../moving_object.h"
#include "../sprite_state.hpp"
#include "../sprite_info.h"

class Caveman : public MovingObject {

public:

    Caveman();

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

    int waitTimer{};
    int goTimer{};

    int animFrame{};
    int animFrameTimer{};

    int invert_speed_timer{};

    int blood_spawn_timer{};

    bool landlocked{};
    bool triggered{};
    bool stunned{};

    int stunned_timer{};

    void randomizeMovement();

    void set_position();

    void apply_walking_sprites();

    void apply_stunned_sprites();

    void make_some_movement();

    void check_if_can_be_triggered();

    void apply_stunned_carried_sprites();

    void apply_dead_carried_sprites();

    void apply_dead_sprites();
};


#endif //SPELUNKYDS_CAVEMAN_H
