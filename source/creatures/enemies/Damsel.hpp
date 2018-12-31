//
// Created by xdbeef on 09.06.18.
// http://spelunky.wikia.com/wiki/Damsel
//

#ifndef SPELUNKYDS_DAMSEL_H
#define SPELUNKYDS_DAMSEL_H

#define DAMSEL_STUN_TIME 7000
#define DAMSEL_HITPOINTS 3

#include "../_BaseCreature.h"
#include "../SpriteState.hpp"
#include "../SpriteInfo.h"

//http://spelunky.wikia.com/wiki/Damsel
class Damsel : public BaseCreature {

public:

    static constexpr u8 damsel_sprite_width = 16;
    static constexpr u8 damsel_sprite_height = 16;
    static constexpr u16 damsel_physical_width = 16;
    static constexpr u16 damsel_physical_height = 16;
    static constexpr SpritesheetType damsel_spritesheet_type = SpritesheetType::CAVEMAN_DAMSEL;

    Damsel(int x, int y) : BaseCreature(
            x,
            y,
            damsel_sprite_width,
            damsel_sprite_height,
            damsel_spritesheet_type,
            damsel_physical_width,
            damsel_physical_height,
            CreatureType::DAMSEL
    ) {
        _max_x_speed = 1.5f;
        hitpoints = DAMSEL_HITPOINTS;
        call_for_help = true;
        sprite_state = SpriteState::W_LEFT;
        activated = true;
        randomizeMovement();
        _bouncing_factor_x = 0;
        _bouncing_factor_y = 0;
        init_sprites();
        _friction = 0.5f;
        _pos_update_delta = 16;
//        _gravity = 0.75f * ICollidable::default_gravity;
    }

    // Base creature overrides

    void update_creature_specific() override;

    void introduce_yourself() override { printf("WHIP\n"); };

    void apply_dmg(int dmg_to_apply) override;

    bool can_update_collidable() const override { return !_ready_to_dispose && !hold_by_main_dude; }

    // IRenderable overrides

    void init_sprites() override;

    void delete_sprites() override;

    void update_sprites_position() override;

    // ICollidable overrides

    bool can_apply_friction() const override { return !hold_by_main_dude && _bottom_collision /*&& (stunned || killed)*/; }

    bool can_apply_gravity() const override { return !hold_by_main_dude; }

    // Other, creature specific
    
    void randomizeMovement();

    void apply_walking_sprites();

    void apply_stunned_sprites();

    void make_some_movement();

    void apply_stunned_carried_sprites();

    void apply_dead_carried_sprites();

    void apply_dead_sprites();

    void apply_exiting_level_sprites();

    void apply_yelling_sprites();

    void match_animation();

    void apply_smooching_sprites();

    double pos_inc_timer{};

    SpriteInfo *mainSpriteInfo{};
    SpriteInfo *subSpriteInfo{};

    SpriteInfo *yell_mainSpriteInfo{};
    SpriteInfo *yell_subSpriteInfo{};

    u8 *frameGfx{};

    double waitTimer{};
    double goTimer{};

    int animFrame{};
    double animFrameTimer{};

    double invert_speed_timer{};
    double yell_timer{};
    bool yelling{};
    double blood_spawn_timer{};

    bool landlocked{};
    bool triggered{};
    bool stunned{};
    bool exiting_level{};
    bool rescued{};
    bool call_for_help{};

    double stunned_timer{};

};


#endif //SPELUNKYDS_DAMSEL_H
