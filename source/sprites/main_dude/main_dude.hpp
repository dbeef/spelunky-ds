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
#define MAIN_DUDE_SPRITE_SIZE MAIN_DUDE_SPRITE_WIDTH * MAIN_DUDE_SPRITE_HEIGHT

#define MAIN_DUDE_MAX_X_SPEED 2
#define MAIN_DUDE_MAX_X_SPEED_CRAWLING 1.5f
#define MAIN_DUDE_MAX_X_SPEED_RUNNING 3
#define MAIN_DUDE_MAX_Y_SPEED 3.3f
#define MAIN_DUDE_MAX_Y_SPEED_USING_CAPE 1.7f
#define MAIN_DUDE_JUMP_SPEED 2.55f
#define MAIN_DUDE_MIN_HANGING_TIME 100
#define MAIN_DUDE_STUN_TIME 2000
#define MAIN_DUDE_STUN_FALLING_TIME 900
#define MAIN_DUDE_PUSHING_TIME 500
#define MAIN_DUDE_DAMAGE_PROTECTION_TIME 1500
#define MAIN_DUDE_FRAMES_PER_ANIMATION 6
#define MAIN_DUDE_HITPOINTS 4
#define MAIN_DUDE_SPRITESHEET_ROW_WIDTH 6
#define MAIN_DUDE_X_SPEED_DELTA_TIME_MS 2
#define MAIN_DUDE_X_SPEED_DELTA_VALUE 2
#define MAIN_DUDE_FRICTION_DELTA_SPEED 1

#include <nds.h>
#include <vector>
#include "../sprite_state.hpp"
#include "../../tiles/map_tile.hpp"
#include "../../tiles/level.hpp"
#include "../../camera/camera.hpp"
#include "../sprite_info.h"
#include "../items/bomb.hpp"
#include "../moving_object.h"
#include "../../input/input_handler.hpp"
#include "whip.hpp"

//http://spelunky.wikia.com/wiki/Spelunky_Guy
class MainDude : public MovingObject {

public:

    MainDude();

    void introduce_yourself() override { printf("MAIN_DUDE\n"); };

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

    double animation_frame_timer{};
    double speed_inc_timer{};
    double pos_inc_timer{};
    //How much time_utils was spent on hanging, since last flag hanging_on_tile_left/right occured.
    double hanging_timer{};

    double stunned_timer{};
    double jumping_timer{};
    double pushing_timer{};
    double time_since_last_jump{};
    double time_since_last_damage = MAIN_DUDE_DAMAGE_PROTECTION_TIME + 1;

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

    double climbing_timer{};
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

    void set_sprite_crawling();

    void set_sprite_hanging_on_tile();

    void set_sprite_whiping();

    void set_sprite_pushing();

    void set_sprite_stunned();

    void set_sprite_climbing();

    void set_sprite_dead();

    void set_sprite_walking_when_in_air();

    void set_sprite_falling();

    void set_sprite_exiting_level();

    void apply_blinking_on_damage();

    void reset_values_checked_every_frame();

    void throw_item();

    void take_out_bomb();

    void throw_rope();

    void spawn_carried_items();

    void match_animation();
};


#endif //SPELUNKYDS_MAINDUDE_H
