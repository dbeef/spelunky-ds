//
// Created by xdbeef on 16.09.18.
//

#ifndef SPELUNKYDS_ONE_GOLDBAR_H
#define SPELUNKYDS_ONE_GOLDBAR_H

#include "../../interfaces/Moniez.hpp"
#include "../_BaseCreature.h"
#include "../SpriteInfo.h"

class SingleGoldbar : public BaseCreature, public Moniez {

public:

    static constexpr u8 single_goldbar_sprite_width = 16;
    static constexpr u8 single_goldbar_sprite_height = 16;
    static constexpr u16 single_goldbar_physical_width = 8;
    static constexpr u16 single_goldbar_physical_height = 5;
    static constexpr SpritesheetType single_goldbar_spritesheet_type = SpritesheetType::MONIEZ_GOLDBARS;

    SingleGoldbar(int x, int y) : BaseCreature(
            x,
            y,
            single_goldbar_sprite_width,
            single_goldbar_sprite_height,
            single_goldbar_spritesheet_type,
            single_goldbar_physical_width,
            single_goldbar_physical_height,
            CreatureType ::SINGLE_GOLDBAR
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

    //rename to vectors_update_timer?
    double pos_inc_timer{};
    SpriteInfo *mainSpriteInfo{};
    SpriteInfo *subSpriteInfo{};
    u8 *frameGfx{};
};

#endif //SPELUNKYDS_ONE_GOLDBAR_H
