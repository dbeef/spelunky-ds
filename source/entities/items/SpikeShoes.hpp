//
// Created by xdbeef on 02.06.18.
//

#ifndef SPELUNKYDS_SPIKE_SHOES_H
#define SPELUNKYDS_SPIKE_SHOES_H


#include "../_common/Orientation.hpp"
#include "../items/_BaseItem.h"
#include "../_interfaces/ShoppingObject.h"

//http://spelunky.wikia.com/wiki/Spike_Shoes
class SpikeShoes : public BaseItem, public ShoppingObject {

    static constexpr u8 spike_shoes_sprite_width = 16;
    static constexpr u8 spike_shoes_sprite_height = 16;
    static constexpr u16 spike_shoes_physical_width = 14;
    static constexpr u16 spike_shoes_physical_height = 11;
    static constexpr u16 spike_shoes_cost = 4000;
    static constexpr const char *spike_shoes_name = "SPIKE SHOES";
    static constexpr SpritesheetType spike_shoes_spritesheet_type = SpritesheetType::SALEABLE;

public:

    SpikeShoes(int x, int y) : BaseItem(
            x,
            y,
            spike_shoes_sprite_width,
            spike_shoes_sprite_height,
            spike_shoes_spritesheet_type,
            spike_shoes_physical_width,
            spike_shoes_physical_height,
            4, 4, -4
    ), ShoppingObject(spike_shoes_cost, spike_shoes_name) {
        init_anim_icon();
        update_anim_icon(x, y, _physical_width);
        init_sprites();
    }

    // Base item overrides

    void update_item_specific() override;

    void print_classname() const override;

    // IRenderable overrides

    void init_sprites() override;

    // Other, creature specific

    void equip();

};


#endif //SPELUNKYDS_SPIKE_SHOES_H
