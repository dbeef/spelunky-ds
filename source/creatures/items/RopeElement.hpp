//
// Created by xdbeef on 04.04.18.
//

#ifndef SPELUNKYDS_ROPE_ELEMENT_H
#define SPELUNKYDS_ROPE_ELEMENT_H

#include <nds/arm9/sprite.h>
#include <vector>

#include "Rope.hpp"
#include "../../camera/Camera.hpp"
#include "../_BaseCreature.h"
#include "../SpriteInfo.h"

// Represents a single 16x16 element in rope chain. 
class RopeElement : public BaseCreature {
    
public:

    RopeElement(int x, int y) : BaseCreature(
            x,
            y,
            rope_sprite_width,
            rope_sprite_height,
            rope_spritesheet_type,
            rope_physical_width,
            rope_physical_height,
            CreatureType::ROPE_ELEMENT
    ) {
        init_sprites();
    }

    // Base creature overrides

    void update_creature_specific() override;

    void introduce_yourself() override { printf("WHIP\n"); };

    void apply_dmg(int dmg_to_apply) override {};

    bool can_update_collidable() const override { return false; }

    // IRenderable overrides

    void init_sprites() override;

    void delete_sprites() override;

    void update_sprites_position() override;

    // ICollidable overrides

    bool can_apply_friction() const override { return false; }

    bool can_apply_gravity() const override { return false; }

    // Other, creature specific

    void update_visibility();

    double _active_timer{};
    bool _active{};

    // To make creating rope chain smoother, instead of 16x16 rope chunks,
    // there are 8x8 chunks, two per RopeElement, created separately in delay.
    SpriteInfo *_main_sprite_info{};
    SpriteInfo *_sub_sprite_info{};
    SpriteInfo *_main_sprite_info_2{};
    SpriteInfo *_sub_sprite_info_2{};

};


#endif //SPELUNKYDS_ROPE_ELEMENT_H
