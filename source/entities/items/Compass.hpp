//
// Created by xdbeef on 19.05.18.
//

#ifndef SPELUNKYDS_COMPASS_H
#define SPELUNKYDS_COMPASS_H


#include "../_common/Orientation.hpp"
#include "../items/_BaseItem.h"
#include "../_interfaces/ShoppingObject.h"

//http://spelunky.wikia.com/wiki/Compass
class Compass : public BaseItem, public ShoppingObject {

public:

    static constexpr u8 compas_sprite_width = 16;
    static constexpr u8 compas_sprite_height = 16;
    static constexpr u16 compas_physical_width = 12;
    static constexpr u16 compas_physical_height = 12;
    static constexpr u16 compas_cost = 3000;
    static constexpr const char *compas_name = "COMPASS";
    static constexpr SpritesheetType compas_spritesheet_type = SpritesheetType::SALEABLE;

public:

    Compass(int x, int y) : BaseItem(
            x,
            y,
            compas_sprite_width,
            compas_sprite_height,
            compas_spritesheet_type,
            compas_physical_width,
            compas_physical_height,
            0, 0, 0
    ), ShoppingObject(compas_cost, compas_name) {
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

    void draw_arrow_to_exit();

    void equip();

    double pos_inc_timer{};
    bool collected{};

};


#endif //SPELUNKYDS_COMPASS_H
