//
// Created by xdbeef on 19.05.18.
//

#ifndef SPELUNKYDS_MITT_H
#define SPELUNKYDS_MITT_H


#include "../_common/Orientation.hpp"
#include "../items/_BaseItem.h"
#include "../_interfaces/ShoppingObject.h"

//http://spelunky.wikia.com/wiki/Pitcher%27s_Mitt
class Mitt : public BaseItem, public ShoppingObject {

    static constexpr u8 mitt_sprite_width = 16;
    static constexpr u8 mitt_sprite_height = 16;
    static constexpr u16 mitt_physical_width = 14;
    static constexpr u16 mitt_physical_height = 14;
    static constexpr u16 mitt_cost = 4000;
    static constexpr const char *mitt_name = "MITT";
    static constexpr SpritesheetType mitt_spritesheet_type = SpritesheetType::SALEABLE;

public:

    Mitt(int x, int y) : BaseItem(
            x,
            y,
            mitt_sprite_width,
            mitt_sprite_height,
            mitt_spritesheet_type,
            mitt_physical_width,
            mitt_physical_height,
            0, 0, 0
    ), ShoppingObject(mitt_cost, mitt_name) {
        init_anim_icon();
        update_anim_icon(x, y, _physical_width);
        init_sprites();
    }

    // Base item overrides

    void update_item_specific() override;

    void print_classname() override { printf("MITT\n"); };

    // IRenderable overrides

    void init_sprites() override;

    // Other, creature specific

    double pos_inc_timer{};
    bool collected{};

    void equip();

    void match_animation();
};


#endif //SPELUNKYDS_MITT_H
