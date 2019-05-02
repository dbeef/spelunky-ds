//
// Created by xdbeef on 29.07.18.
//

#ifndef SPELUNKYDS_BONE_H
#define SPELUNKYDS_BONE_H


#include "../_common/Orientation.hpp"
#include "../creatures/_BaseCreature.h"

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
            bone_physical_height,
            CreatureType::BONE
    ) {
        init_sprites();
    }

    // Base creature overrides

    void update_creature_specific() override;

    void introduce_yourself() override { printf("WHIP\n"); };

    void apply_dmg(int dmg_to_apply) override {};

    bool can_update_collidable() const override { return true; }

    // IRenderable overrides

    void init_sprites() override;

    void delete_sprites() override;

    void update_sprites_position() override;

    // ICollidable overrides

    bool can_apply_friction() const override { return true; }

    bool can_apply_gravity() const override { return true; }

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
