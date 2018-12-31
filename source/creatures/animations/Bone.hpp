//
// Created by xdbeef on 29.07.18.
//

#ifndef SPELUNKYDS_BONE_H
#define SPELUNKYDS_BONE_H

#include "../SpriteInfo.h"
#include "../SpriteState.hpp"
#include "../_BaseCreature.h"

class Bone : public BaseCreature {

public:

    static constexpr u8 bone_sprite_width = 16;
    static constexpr u8 bone_sprite_height = 16;
    static constexpr u16 bone_physical_width = 8;
    static constexpr u16 bone_physical_height = 8;
    static constexpr SpritesheetType bone_spritesheet_type = SpritesheetType::SKELETON_SPIDER;

    Bone(int x, int y) : BaseCreature(
            x,
            y,
            bone_sprite_width,
            bone_sprite_height,
            bone_spritesheet_type,
            bone_physical_width,
            bone_physical_height
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

    void match_animation();

    bool collided{};
    double animFrameTimer{};
    int animFrame{};
    double pos_inc_timer{};
    SpriteInfo *mainSpriteInfo{};
    SpriteInfo *subSpriteInfo{};
    u8 *frameGfx{};
};


#endif //SPELUNKYDS_BONE_H
