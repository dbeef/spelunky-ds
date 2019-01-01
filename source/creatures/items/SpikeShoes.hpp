//
// Created by xdbeef on 02.06.18.
//

#ifndef SPELUNKYDS_SPIKE_SHOES_H
#define SPELUNKYDS_SPIKE_SHOES_H

#include "../SpriteInfo.h"
#include "../Orientation.hpp"
#include "../_BaseCreature.h"
#include "../../interfaces/ShoppingObject.h"

//http://spelunky.wikia.com/wiki/Spike_Shoes
class SpikeShoes : public BaseCreature, public ShoppingObject {

    static constexpr u8 spike_shoes_sprite_width = 16;
    static constexpr u8 spike_shoes_sprite_height = 16;
    static constexpr u16 spike_shoes_physical_width = 14;
    static constexpr u16 spike_shoes_physical_height = 11;
    static constexpr u16 spike_shoes_cost = 4000;
    static constexpr const char *spike_shoes_name = "SPIKE SHOES";
    static constexpr SpritesheetType spike_shoes_spritesheet_type = SpritesheetType::SALEABLE;

public:

    SpikeShoes(int x, int y) : BaseCreature(
            x,
            y,
            spike_shoes_sprite_width,
            spike_shoes_sprite_height,
            spike_shoes_spritesheet_type,
            spike_shoes_physical_width,
            spike_shoes_physical_height,
            CreatureType::SPIKE_SHOES
    ), ShoppingObject(spike_shoes_cost, spike_shoes_name) {
        init_anim_icon();
        update_anim_icon(x, y, _physical_width);
        init_sprites();
    }

    // Base creature overrides

    void update_creature_specific() override;

    void introduce_yourself() override { printf("WHIP\n"); };

    void apply_dmg(int dmg_to_apply) override {};

    bool can_update_collidable() const override { return !_collected; }

    // IRenderable overrides

    void init_sprites() override;

    void delete_sprites() override;

    void update_sprites_position() override;

    // ICollidable overrides

    bool can_apply_friction() const override { return true; }

    bool can_apply_gravity() const override { return true; }

    // Other, creature specific

    void equip();

    // if collected, then drawn in HUD
    bool _collected{};
    SpriteInfo *_main_sprite_info{};
    SpriteInfo *_sub_sprite_info{};

};


#endif //SPELUNKYDS_SPIKE_SHOES_H
