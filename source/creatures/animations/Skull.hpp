//
// Created by xdbeef on 29.07.18.
//

#ifndef SPELUNKYDS_SKULL_H
#define SPELUNKYDS_SKULL_H

#include "../SpriteInfo.h"
#include "../SpriteState.hpp"
#include "../_BaseCreature.h"

class Skull : public BaseCreature {

public:

    static constexpr u8 skull_sprite_width = 16;
    static constexpr u8 skull_sprite_height = 16;
    static constexpr u16 skull_physical_width = 8;
    static constexpr u16 skull_physical_height = 8;
    static constexpr SpritesheetType skull_spritesheet_type = SpritesheetType::SKELETON_SPIDER;

    Skull(int x, int y) : BaseCreature(
            x,
            y,
            skull_sprite_width,
            skull_sprite_height,
            skull_spritesheet_type,
            skull_physical_width,
            skull_physical_height
    ) {
        init_sprites();
    }

    // Base creature overrides

    void update_creature_specific() override;

    void introduce_yourself() override { printf("WHIP\n"); };

    void apply_dmg(int dmg_to_apply) override {};

    // IRenderable overrides

    void init_sprites() override;

    void delete_sprites() override;

    void update_sprites_position() override;

    // ICollidable overrides

    bool can_update_collidable() override { return !hold_by_main_dude && !collided; }

    bool can_apply_friction() override { return !collided; }

    bool can_apply_gravity() override { return !collided; }

    // Other, creature specific

    void match_animation();
    
    bool collided{};
    double animFrameTimer{};
    int animFrame{};
    double  pos_inc_timer{};
    SpriteInfo *mainSpriteInfo{};
    SpriteInfo *subSpriteInfo{};
    u8 *frameGfx{};
};




#endif //SPELUNKYDS_SKULL_H
