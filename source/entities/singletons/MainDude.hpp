//
// Created by xdbeef on 24.02.18.
//
//https://www.quora.com/Are-primitives-default-initialized-in-C++11

#ifndef SPELUNKYDS_MAINDUDE_H
#define SPELUNKYDS_MAINDUDE_H

#include <nds.h>

#include "../../preprocessor/Debug.h"
#include "../creatures/_BaseCreature.h"
#include "../_common/Orientation.hpp"
#include "Whip.hpp"

class _BaseItem;
class IPickupable;

//http://spelunky.wikia.com/wiki/Spelunky_Guy
class MainDude : public BaseCreature {

public:

    static constexpr u8 main_dude_sprite_width = 16;
    static constexpr u8 main_dude_sprite_height = 16;
    static constexpr u16 main_dude_physical_width = 16;
    static constexpr u16 main_dude_physical_height = 16;
    static constexpr SpritesheetType main_dude_spritesheet_type = SpritesheetType::MAIN_DUDE;

    static constexpr u16 main_dude_pos_update_delta_walking_running = 15;
    static constexpr u16 main_dude_pos_update_delta_crawling = 30;
    static constexpr float main_dude_max_x_speed_walking = 1.50f;
    static constexpr float main_dude_max_x_speed_running = 3.0f;
    static constexpr float main_dude_max_x_crawling = 0.55f;

    static void init();

    static void dispose();

    inline static MainDude& instance() {
        SPELUNKYDS_BREAKING_ASSERT(_instance);
        return *_instance;
    }

    MainDude(int x, int y);

    ~MainDude() override;

    // Base creature overrides
    void update_creature_specific() override;

    void introduce_yourself() override { printf("MAIN_DUDE\n"); };

    void apply_dmg(int dmg_to_apply) override;

    bool can_update_collidable() const override { return true; }

    // IRenderable overrides

    void init_sprites() override;

    void delete_sprites() override;

    void update_sprites_position() override;

    // ICollidable overrides

    bool can_apply_friction() const override { return true; }

    bool can_apply_gravity() const override;

    // Other, creature specific

    // Updates sprites' graphics according to current animation frame index.
    void match_animation();

    // Sets dead as true and effecively launches game summary.
    void set_dead();

    void boost_going_through_map_holes(MapTile **const t);

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

    void set_position_to(MapTileType t);

    void reset_state();

    BaseItem *_currently_held_item{};
    BaseCreature *_currently_held_creature{};
    IPickupable *_currently_held_pickupable{};


    SpriteInfo *main_sprite_info{};
    SpriteInfo *sub_sprite_info{};

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
    double time_since_last_damage{};

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

private:

    static MainDude* _instance;
};


#endif //SPELUNKYDS_MAINDUDE_H
