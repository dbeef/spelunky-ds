//
// Created by xdbeef on 16.09.18.
//

#ifndef SPELUNKYDS_TRIPLE_GOLDBAR_H
#define SPELUNKYDS_TRIPLE_GOLDBAR_H

#include "../_base_creature.h"
#include "../../interfaces/moniez.hpp"
#include "../sprite_info.h"


class TripleGoldbar : public BaseCreature, public Moniez {

public:

    static constexpr u8 triple_goldbar_sprite_width = 16;
    static constexpr u8 triple_goldbar_sprite_height = 16;
    static constexpr u16 triple_goldbar_physical_width = 16;
    static constexpr u16 triple_goldbar_physical_height = 10;
    static constexpr SpritesheetType triple_goldbar_spritesheet_type = SpritesheetType::MONIEZ_GOLDBARS;

    TripleGoldbar(int x, int y) : BaseCreature(
            x,
            y,
            triple_goldbar_sprite_width,
            triple_goldbar_sprite_height,
            triple_goldbar_spritesheet_type,
            triple_goldbar_physical_width,
            triple_goldbar_physical_height
    ), Moniez(VALUE_TRIPLE_GOLDBAR) {
        activated = true;
        _collected = false;
        _collectible_timer = 500;
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

    //rename to vectors_update_timer?
    double pos_inc_timer{};
    SpriteInfo *mainSpriteInfo{};
    SpriteInfo *subSpriteInfo{};
    u8 *frameGfx{};
};

#endif //SPELUNKYDS_TRIPLE_GOLDBAR_H
