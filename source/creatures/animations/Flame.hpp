//
// Created by xdbeef on 02.06.18.
//

#ifndef SPELUNKYDS_FLAME_H
#define SPELUNKYDS_FLAME_H

#define MAX_X_SPEED_FLAME 1.5
#define MAX_Y_SPEED_FLAME 1.5

#define FLAME_CHANGE_POS_DELTA 16
#define FLAME_ANIM_FRAME_DELTA 90

#include <vector>
#include <cstdlib>

#define flame_sprite_width 16
#define flame_sprite_height 16
#define flame_physical_width 6
#define flame_physical_height 6
#define flame_spritesheet_type SpritesheetType::SPIKES_COLLECTIBLES

#include "../_BaseCreature.h"
#include "../SpriteInfo.h"
#include "FlameElement.hpp"

//almost same as blood animation
class Flame : public BaseCreature {

public:
    
    Flame(int x, int y) : BaseCreature(
            x,
            y,
            flame_sprite_width,
            flame_sprite_height,
            flame_spritesheet_type,
            flame_physical_width,
            flame_physical_height
    ) {
        _pos_update_delta = 30;
        change_pos_delta_offset = FLAME_CHANGE_POS_DELTA + (rand() % 5);
        init_sprites();
    }

    // Base creature overrides

    void update_creature_specific() override;

    void introduce_yourself() override { printf("WHIP\n"); }

    void apply_dmg(int dmg_to_apply) override {};

    bool can_update_collidable() const override { return !finished && !_ready_to_dispose; }

    // IRenderable overrides

    void init_sprites() override;

    void delete_sprites() override;

    void update_sprites_position() override;

    // ICollidable overrides

    bool can_apply_friction() const override { return true; }

    bool can_apply_gravity() const override { return true; }

    // Other, creature specific

    void spawn_flame();

    void match_animation();

    double *timer {};
    SpriteInfo *mainSpriteInfo {};
    SpriteInfo *subSpriteInfo {};
    u8 *frameGfx{};
    bool finished{};
    int currentFrame{};
    double animFrameTimer{};
    std::vector<FlameElement *> flame_trail;
    double living_timer{};
    double time_since_last_spawn{};
    int change_pos_delta_offset{};

};

#endif //SPELUNKYDS_FLAME_H
