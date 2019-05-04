//
// Created by xdbeef on 24.02.18.
//

#ifndef SPELUNKYDS_MAINDUDE_H
#define SPELUNKYDS_MAINDUDE_H

#include <nds.h>

#include "../../preprocessor/Debug.h"
#include "../creatures/_BaseCreature.h"
#include "../_common/Orientation.hpp"
#include "Whip.hpp"
#include "states/_MainDudeState.hpp"

static const float MAIN_DUDE_MAX_X_SPEED = 2;
static const float MAIN_DUDE_MAX_X_SPEED_CRAWLING = 1.5f;
static const float MAIN_DUDE_MAX_X_SPEED_RUNNING = 3;
static const float MAIN_DUDE_MAX_Y_SPEED = 3.3f;
static const float MAIN_DUDE_MAX_Y_SPEED_USING_CAPE = 1.7f;
static const float MAIN_DUDE_JUMP_SPEED = 2.55f;

static const u16 MAIN_DUDE_MIN_HANGING_TIME = 100;
static const u16 MAIN_DUDE_STUN_TIME = 2000;
static const u16 MAIN_DUDE_STUN_FALLING_TIME = 900;
static const u16 MAIN_DUDE_PUSHING_TIME = 500;
static const u16 MAIN_DUDE_DAMAGE_PROTECTION_TIME = 1500;
static const u16 MAIN_DUDE_FRAMES_PER_ANIMATION = 6;
static const u16 MAIN_DUDE_HITPOINTS = 4;
static const u16 MAIN_DUDE_SPRITESHEET_ROW_WIDTH = 6;
static const u16 MAIN_DUDE_X_SPEED_DELTA_TIME_MS = 2;
static const u16 MAIN_DUDE_X_SPEED_DELTA_VALUE = 2;
static const u16 MAIN_DUDE_FRICTION_DELTA_SPEED = 1;

static const u16 MAIN_DUDE_MENU_START_POSITION_X = 224;
static const u16 MAIN_DUDE_MENU_START_POSITION_Y = 300;


class _BaseItem;

class IPickupable;

class _DudeStateHandler;

class Climbing;

class Dead;

class ExitingLevel;

class HangingOnEdge;

class Stunned;

class Walking;

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

    inline static MainDude &instance() {
        SPELUNKYDS_BREAKING_ASSERT(_instance);
        return *_instance;
    }

    MainDude(int x, int y);

    ~MainDude() override;

    // Base creature overrides

    void update_creature_specific() override;

    void print_classname() const override;

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

    void boost_going_through_map_holes(MapTile **const t);

    bool can_hang_on_tile(MapTile **neighboringTiles);

    void apply_blinking_on_damage();

    void reset_values_checked_every_frame();

    void throw_item();

    void take_out_bomb();

    void throw_rope();

    void spawn_carried_items();

    void set_position_to(MapTileType t);

    void reset_state();

    void reset_carried_items();

    BaseItem *_currently_held_item{};
    BaseCreature *_currently_held_creature{};
    IPickupable *_currently_held_pickupable{};

    SpriteInfo *main_sprite_info{};
    SpriteInfo *sub_sprite_info{};

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

    bool holding_item{};

    double climbing_timer{};
    int climbing_sound{};

    // FIXME: Should be in jetpack.
    int jetpack_fuel_counter{};

    int animFrame{};
    u8 *frameGfx{};

    struct {
        bool spring_shoes{};
        bool spike_shoes{};
        bool mitt{};
        bool glove{};
        bool compass{};
        bool pistol{};
        bool shotgun{};
        bool cape{};
        bool jetpack{};
        bool damsel{};
    } carried_items;

    bool using_whip{};
    bool using_cape{};
    bool using_jetpack{};

    Orientation _orientation{};
    _DudeStateHandler *_current_state{};

    // State transition

    void set_stunned();

    void set_dead();

    void set_hanging_on_edge();

    void set_exiting_level();

    void set_walking();

    void set_climbing();

    // Setting animation

    void match_animation();

private:

    static MainDude *_instance;

    Climbing *_climbing;
    Dead *_dead;
    ExitingLevel *_exiting_level;
    HangingOnEdge *_hanging_on_edge;
    Stunned *_stunned;
    Walking *_walking;

    void set_sprite_crawling();

    void set_sprite_pushing();

    void set_sprite_walking();

    void set_sprite_falling();

    void set_sprite_hanging_on_tile();

    void set_sprite_whiping();

    void set_sprite_stunned();

    void set_sprite_climbing_rope();

    void set_sprite_climbing_ladder();

    void set_sprite_dead();

    void set_sprite_exiting_level();

};


#endif //SPELUNKYDS_MAINDUDE_H
