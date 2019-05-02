//
// Created by xdbeef on 23.04.18.
//

#ifndef SPELUNKYDS_ARROW_H
#define SPELUNKYDS_ARROW_H

#include <nds/arm9/sprite.h>

#include "../items/_BaseItem.h"

#include "../_common/Orientation.hpp"

//http://spelunky.wikia.com/wiki/Arrow
class Arrow : public BaseItem {
public:

    static constexpr int arrow_x_pickup_offset_left = 1;
    static constexpr int arrow_x_pickup_offset_right = 10;
    static constexpr int arrow_y_pickup_offset = 6;
    static constexpr u8 arrow_sprite_width = 8;
    static constexpr u8 arrow_sprite_height = 8;
    static constexpr u16 arrow_physical_width = 8;
    static constexpr u16 arrow_physical_height = 8;
    static constexpr SpritesheetType arrow_spritesheet_type = SpritesheetType::ARROW;

    Arrow(int x, int y, Orientation orientation) : BaseItem(
            x,
            y,
            arrow_sprite_width,
            arrow_sprite_height,
            arrow_spritesheet_type,
            arrow_physical_width,
            arrow_physical_height,
            arrow_x_pickup_offset_left,
            arrow_x_pickup_offset_right,
            arrow_y_pickup_offset
    ), _orientation(orientation) {
        init_sprites();
        _thrown = true;
        _activated = true;
        _angle = 90;
        _max_x_speed = 5.0f;
        _friction = 0.085f;
    }

    // BaseItem overrides

    void update_item_specific() override;

    void print_classname() override { printf("ARROW\n"); };

    // IRenderable overrides

    void init_sprites() override;

    // Other, item specific

    void update_frame(int frame_num);

    Orientation _orientation;
    double _angle{};
    bool _thrown{};
    double _armed_timer{};

};


#endif //SPELUNKYDS_ARROW_H
