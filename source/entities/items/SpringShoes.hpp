//
// Created by xdbeef on 18.05.18.
//

#ifndef SPELUNKYDS_SPRING_SHOES_H
#define SPELUNKYDS_SPRING_SHOES_H


#include "../../memory/SpriteInfo.h"
#include "../_common/Orientation.hpp"
#include "../items/_BaseItem.h"
#include "../_interfaces/ShoppingObject.h"

//http://spelunky.wikia.com/wiki/Spring_Shoes
class SpringShoes : public BaseItem, public ShoppingObject {

    static constexpr u8 spring_shoes_sprite_width = 16;
    static constexpr u8 spring_shoes_sprite_height = 16;
    static constexpr u16 spring_shoes_physical_width = 14;
    static constexpr u16 spring_shoes_physical_height = 12;
    static constexpr u16 spring_shoes_cost = 5000;
    static constexpr const char *spring_shoes_name = "SPRING SHOES";
    static constexpr SpritesheetType spring_shoes_spritesheet_type = SpritesheetType::SALEABLE;

public:

    SpringShoes(int x, int y) : BaseItem(
            x,
            y,
            spring_shoes_sprite_width,
            spring_shoes_sprite_height,
            spring_shoes_spritesheet_type,
            spring_shoes_physical_width,
            spring_shoes_physical_height,
            0, 0, 0
    ), ShoppingObject(spring_shoes_cost, spring_shoes_name) {
        init_anim_icon();
        update_anim_icon(x, y, _physical_width);
        init_sprites();
    }

    // Base item overrides

    void update_item_specific() override;

    // IRenderable overrides

    void init_sprites() override;

    // Other, creature specific

    void equip();

    double pos_inc_timer{};
    bool collected{};

};


#endif //SPELUNKYDS_SPRING_SHOES_H
