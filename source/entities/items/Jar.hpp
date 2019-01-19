//
// Created by xdbeef on 23.04.18.
//

#ifndef SPELUNKYDS_JAR_H
#define SPELUNKYDS_JAR_H

#include "_BaseItem.h"
#include "../../memory/SpriteInfo.h"
#include "../creatures/CreatureType.hpp"

//http://spelunky.wikia.com/wiki/Pot
class Jar : public BaseItem {

public:

    static constexpr int jar_x_pickuped_offset_left = 2;
    static constexpr int jar_x_pickuped_offset_right = 6;
    static constexpr int jar_y_pickuped_offset = 8;
    static constexpr u8 jar_sprite_width = 16;
    static constexpr u8 jar_sprite_height = 16;
    static constexpr u16 jar_physical_width = 10;
    static constexpr u16 jar_physical_height = 9;
    static constexpr SpritesheetType jar_spritesheet_type = SpritesheetType::JAR;

    Jar(int x, int y) : BaseItem(
            x,
            y,
            jar_sprite_width,
            jar_sprite_height,
            jar_spritesheet_type,
            jar_physical_width,
            jar_physical_height,
            jar_x_pickuped_offset_left,
            jar_x_pickuped_offset_right,
            jar_y_pickuped_offset
    ) {
        init_sprites();
        _activated = false;
    }

    // BaseItem overrides

    void update_item_specific() override;

    // IRenderable overrides

    void init_sprites() override;

    // Other, creature specific

    void match_animation();

    void destroy();

    bool _destroyed{};
    u16 _anim_frame_index{};
    double _anim_frame_timer{};
};


#endif //SPELUNKYDS_JAR_H
