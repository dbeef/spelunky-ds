//
// Created by xdbeef on 19.05.18.
//

#ifndef SPELUNKYDS_GLOVE_H
#define SPELUNKYDS_GLOVE_H



#include "../creatures/_BaseCreature.h"
#include "../_interfaces/ShoppingObject.h"

//http://spelunky.wikia.com/wiki/Climbing_Gloves
class Glove : public BaseItem, public ShoppingObject {


    static constexpr u8 glove_pickup_offset_x_left = 4;
    static constexpr u8 glove_pickup_offset_x_right = 4;
    static constexpr u8 glove_pickup_offset_y = 4;
    static constexpr u8 glove_sprite_width = 16;
    static constexpr u8 glove_sprite_height = 16;
    static constexpr u16 glove_physical_width = 16;
    static constexpr u16 glove_physical_height = 15;
    static constexpr u16 glove_cost = 9000;
    static constexpr const char *glove_name = "GLOVE";
    static constexpr SpritesheetType glove_spritesheet_type = SpritesheetType::SALEABLE;

public:

    Glove(int x, int y) : BaseItem(
            x,
            y,
            glove_sprite_width,
            glove_sprite_height,
            glove_spritesheet_type,
            glove_physical_width,
            glove_physical_height,
            glove_pickup_offset_x_left,
            glove_pickup_offset_x_right,
            glove_pickup_offset_y
    ), ShoppingObject(glove_cost, glove_name) {
        init_anim_icon();
        update_anim_icon(x, y, _physical_width);
        init_sprites();
    }

    // BaseItem overrides

    void update_item_specific() override;

    void print_classname() override { printf("GLOVE\n"); };

    // IRenderable overrides

    void init_sprites() override;

    // Other, creature specific

    void equip();

    void match_animation();

};


#endif //SPELUNKYDS_GLOVE_H
