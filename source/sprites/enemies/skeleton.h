//
// Created by xdbeef on 22.07.18.
// http://spelunky.wikia.com/wiki/Skeleton
//
#ifndef SPELUNKYDS_SKELETON_H
#define SPELUNKYDS_SKELETON_H

#define SKELETON_PHYSICAL_HEIGHT 16
#define SKELETON_PHYSICAL_WIDTH 16

#define SKELETON_SPRITE_HEIGHT 16
#define SKELETON_SPRITE_WIDTH 16
#define SKELETON_SPRITE_SIZE SKELETON_SPRITE_HEIGHT * SKELETON_SPRITE_WIDTH

#define MAX_X_SPEED_SKELETON 4
#define MAX_Y_SPEED_SKELETON 4

#include "../moving_object.h"
#include "../sprite_state.hpp"
#include "../sprite_info.h"

//http://spelunky.wikia.com/wiki/Skeleton
class Skeleton : public MovingObject {

public:

    void introduce_yourself() override { printf("SKELETON\n"); };

    Skeleton();
    Skeleton(int x, int y);

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

    double change_turn_timer{};
    double summoning_delay_timer{};
    int anim_frame_counter{};
    double anim_frame_timer{};
    bool set_up{};
    bool summoned{};
    bool can_be_summoned{};
    SpriteState main_dude_orientation_at_summoning_moment{};

    void set_position();

    void set_sprite_walking();

    void set_sprite_summoning();

    void check_if_can_be_summoned();

    void set_sprite_pile_of_bones();

    void match_animation();

    void spawn_remnants();
};


#endif //SPELUNKYDS_SKELETON_H
