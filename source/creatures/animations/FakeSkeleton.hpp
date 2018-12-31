//
// Created by xdbeef on 29.07.18.
//

#ifndef SPELUNKYDS_FAKE_SKELETON_H
#define SPELUNKYDS_FAKE_SKELETON_H

#include "../SpriteInfo.h"
#include "../SpriteState.hpp"
#include "../_BaseCreature.h"

#define FAKE_SKELETON_PHYSICAL_HEIGHT_WOUT_SKULL 4
#define FAKE_SKELETON_PHYSICAL_HEIGHT_WITH_SKULL 7
#define FAKE_SKELETON_PHYSICAL_WIDTH 8

class FakeSkeleton : public BaseCreature {

public:

    static constexpr u8 fake_skeleton_sprite_width = 16;
    static constexpr u8 fake_skeleton_sprite_height = 16;
    static constexpr u16 fake_skeleton_physical_width = 8;
    static constexpr u16 fake_skeleton_physical_height = 7;
    static constexpr SpritesheetType fake_skeleton_spritesheet_type = SpritesheetType::SKELETON_SPIDER;

    FakeSkeleton(int x, int y) : BaseCreature(
            x,
            y,
            fake_skeleton_sprite_width,
            fake_skeleton_sprite_height,
            fake_skeleton_spritesheet_type,
            fake_skeleton_physical_width,
            fake_skeleton_physical_height
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

    bool can_update_collidable() override { return true; }
    
    bool can_apply_friction() override { return true; }

    bool can_apply_gravity() override { return true; }

    // Other, creature specific

    void spawn_skull();

    void match_animation();
    
    bool tried_to_pickup{};
    int animFrameTimer{};
    int animFrame{};
    SpriteInfo *mainSpriteInfo{};
    SpriteInfo *subSpriteInfo{};
    u8 *frameGfx{};
};


#endif //SPELUNKYDS_FAKE_SKELETON_H
