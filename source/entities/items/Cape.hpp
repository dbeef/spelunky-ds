//
// Created by xdbeef on 02.06.18.
//

#ifndef SPELUNKYDS_CAPE_H
#define SPELUNKYDS_CAPE_H

#include "../items/_BaseItem.h"
#include "../_interfaces/ShoppingObject.h"

//http://spelunky.wikia.com/wiki/Cape
class Cape : public BaseItem , public ShoppingObject {
    
public:

    static constexpr u16 cape_anim_frame_delta = 60;
    static constexpr int cape_x_pickup_offset_left = 4;
    static constexpr int cape_x_pickup_offset_right = 4;
    static constexpr int cape_y_pickup_offset = -4;
    static constexpr u16 cape_cost = 13000;
    static constexpr const char *cape_name = "CAPE";
    static constexpr u8 cape_sprite_width = 16;
    static constexpr u8 cape_sprite_height = 16;
    static constexpr u16 cape_physical_width = 14;
    static constexpr u16 cape_physical_height = 15;
    static constexpr SpritesheetType cape_spritesheet_type = SpritesheetType::MONIEZ_GOLDBARS;

    Cape(int x, int y) : BaseItem(
            x,
            y,
            cape_sprite_width,
            cape_sprite_height,
            cape_spritesheet_type,
            cape_physical_width,
            cape_physical_height,
            cape_x_pickup_offset_left,
            cape_x_pickup_offset_right,
            cape_y_pickup_offset
    ), ShoppingObject(cape_cost, cape_name) {
        init_sprites();
        init_anim_icon();
        update_anim_icon(x, y, _physical_width);
    }

    // BaseItem overrides

    void update_item_specific() override;

    // IRenderable overrides

    void init_sprites() override;

    // Other, item specific

    void match_animation();

    void equip();

    bool _collected{};
    double _anim_frame_timer{};
    u16 _anim_frame_index{};

};




#endif //SPELUNKYDS_CAPE_H
