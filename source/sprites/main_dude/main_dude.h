//
// Created by xdbeef on 24.02.18.
//
//https://www.quora.com/Are-primitives-default-initialized-in-C++11

#ifndef SPELUNKYDS_MAINDUDE_H
#define SPELUNKYDS_MAINDUDE_H

#define MAIN_DUDE_PHYSICAL_WIDTH 16
#define MAIN_DUDE_PHYSICAL_HEIGHT 16
#define MAIN_DUDE_SPRITE_WIDTH 16
#define MAIN_DUDE_SPRITE_HEIGHT 16

#define MAIN_DUDE_MAX_X_SPEED 2
#define MAIN_DUDE_MAX_X_SPEED_CRAWLING 1.5
#define MAIN_DUDE_MAX_X_SPEED_RUNNING 3

#define MAIN_DUDE_MAX_Y_SPEED 3.3
#define MAIN_DUDE_MAX_Y_SPEED_USING_CAPE 1.7
#define MAIN_DUDE_JUMP_SPEED 2.55
#define MIN_HANGING_TIME 100
#define STUN_TIME 2000
#define STUN_FALLING_TIME 900
#define PUSHING_TIME 500
#define DAMAGE_PROTECTION_TIME 1500

#define MAIN_DUDE_HITPOINTS 4

#include <nds.h>
#include <vector>
#include "../sprite_state.hpp"
#include "../../tiles/map_tile.hpp"
#include "../../tiles/level_generator.h"
#include "../../camera/camera.h"
#include "../sprite_info.h"
#include "../items/bomb.h"
#include "../moving_object.h"
#include "../../input/input_handler.h"
#include "whip.h"

class MainDude : public MovingObject {

public:

    MainDude();

    void updateOther() override {};

    void apply_dmg(int dmg_to_apply) override;

    void draw() override;

    void initSprite() override;

    void updateTimers() override;

    void updateSpeed() override;

    void updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) override;

    void updateCollisionsOtherMoving() override {};

    void onCollisionWithMainCharacter() override {};

    void init() override;

    SpriteInfo *main_spelunker{};
    SpriteInfo *sub_spelunker{};

    int current_x_in_tiles{};
    int current_y_in_tiles{};

    int animation_frame_timer{};
    int speed_inc_timer{};
    int pos_inc_timer{};
    //How much time_utils was spent on hanging, since last flag hanging_on_tile_left/right occured.
    int hanging_timer{};

    int stunned_timer{};
    int jumping_timer{};
    int pushing_timer{};
    int time_since_last_jump{};
    int time_since_last_damage{};

    bool hanging_on_tile_left{};
    bool hanging_on_tile_right{};
    bool stunned{};
    bool crawling{};
    bool pushing_left{};
    bool pushing_right{};

    bool using_whip{};

    bool can_climb_rope{};
    bool can_climb_ladder{};
    bool climbing{};
    bool on_top_of_climbing_space{};
    bool exiting_level{};
    bool holding_item{};
    bool dead{};

    int animFrame{};
    u8 *frameGfx{};
    Whip *whip{};

    int climbing_timer{};
    int climbing_sound{};

    bool started_climbing_rope{};
    bool started_climbing_ladder{};

    bool carrying_spring_shoes{};
    bool carrying_spike_shoes{};
    bool carrying_mitt{};
    bool carrying_glove{};
    bool carrying_compass{};
    bool carrying_pistol{};
    bool carrying_shotgun{};

    bool carrying_cape{};
    bool using_cape{};
    bool carrying_jetpack{};
    bool using_jetpack{};
    int jetpack_fuel_counter{};

    bool carrying_damsel{};

    void handle_key_input();

    void can_hang_on_tile(MapTile **neighboringTiles);

    void apply_crawling_sprite();

    void apply_hanging_on_tile_sprite();

    void apply_whip_sprite();

    void apply_pushing_sprite();

    void apply_stunned_sprite();

    void apply_climbing_sprite();

    void apply_dead_sprite();

    void apply_walking_sprite();

    void apply_falling_sprite();

    void apply_exiting_level_sprite();

    void apply_blinking_on_damage();

    void reset_values_checked_every_frame();

    void throw_item();

    void take_out_bomb();

    void throw_rope();

    void spawn_carried_items();
};


#endif //SPELUNKYDS_MAINDUDE_H
