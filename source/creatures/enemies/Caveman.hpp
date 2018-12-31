//
// Created by xdbeef on 31.05.18.
//

#ifndef SPELUNKYDS_CAVEMAN_H
#define SPELUNKYDS_CAVEMAN_H

#define CAVEMAN_STUN_TIME 7000
#define CAVEMAN_HITPOINTS 3

#include "../_BaseCreature.h"
#include "../SpriteState.hpp"
#include "../SpriteInfo.h"

//http://spelunky.wikia.com/wiki/Caveman
class Caveman : public BaseCreature {

public:

    static constexpr u8 caveman_sprite_width = 16;
    static constexpr u8 caveman_sprite_height = 16;
    static constexpr u16 caveman_physical_width = 16;
    static constexpr u16 caveman_physical_height = 16;
    static constexpr SpritesheetType caveman_spritesheet_type = SpritesheetType::CAVEMAN_DAMSEL;

    Caveman(int x, int y) : BaseCreature(
            x,
            y,
            caveman_sprite_width,
            caveman_sprite_height,
            caveman_spritesheet_type,
            caveman_physical_width,
            caveman_physical_height,
            CreatureType::CAVEMAN
    ) {
        _max_x_speed = 1.5f;
        _pos_update_delta = 15;
        hitpoints = CAVEMAN_HITPOINTS;
        activated = true;
        randomizeMovement();
        _bouncing_factor_x = 0;
        _bouncing_factor_y = 0;
        init_sprites();
        _friction = ICollidable::default_friction * 5;
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

    bool can_apply_friction() const override { return !hold_by_main_dude && _bottom_collision && (stunned || killed); }

    bool can_apply_gravity() const override { return !hold_by_main_dude; }

    // Other, creature specific

    void randomizeMovement();

    void apply_walking_sprites();

    void apply_stunned_sprites();

    void make_some_movement();

    void check_if_can_be_triggered();

    void apply_stunned_carried_sprites();

    void apply_dead_carried_sprites();

    void apply_dead_sprites();

    void match_animation();

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
};


#endif //SPELUNKYDS_CAVEMAN_H
