//
// Created by xdbeef on 09.06.18.
// http://spelunky.wikia.com/wiki/Damsel
//

#ifndef SPELUNKYDS_DAMSEL_H
#define SPELUNKYDS_DAMSEL_H

#define DAMSEL_STUN_TIME 7000
#define DAMSEL_HITPOINTS 3

#define DAMSEL_PHYSICAL_HEIGHT 16
#define DAMSEL_PHYSICAL_WIDTH 16
#define DAMSEL_SPRITE_HEIGHT 16
#define DAMSEL_SPRITE_WIDTH 16

#define MAX_X_SPEED_DAMSEL 5
#define MAX_Y_SPEED_DAMSEL 4

#include "../moving_object.h"
#include "../sprite_state.hpp"
#include "../sprite_info.h"

class Damsel : public MovingObject {

public:

    Damsel();
    Damsel(int x, int y);

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

    SpriteInfo *yell_mainSpriteInfo{};
    SpriteInfo *yell_subSpriteInfo{};

    u8 *frameGfx{};

    int waitTimer{};
    int goTimer{};

    int animFrame{};
    int animFrameTimer{};

    int invert_speed_timer{};
    int yell_timer{};
    bool yelling{};
    int blood_spawn_timer{};

    bool landlocked{};
    bool triggered{};
    bool stunned{};
    bool exiting_level{};
    bool rescued{};
    bool call_for_help{};

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

    void apply_exiting_level_sprites();

    void apply_yelling_sprites();

    void update_animation();

    void apply_smooching_sprites();
};


#endif //SPELUNKYDS_DAMSEL_H
