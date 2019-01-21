//
// Created by xdbeef on 23.04.18.
//

#ifndef SPELUNKYDS_ROCK_H
#define SPELUNKYDS_ROCK_H

#include <nds/arm9/sprite.h>

#include "../items/_BaseItem.h"
#include "../../memory/SpriteInfo.h"

//http://spelunky.wikia.com/wiki/Rock
class Rock : public BaseItem {

public:
    
    static constexpr int rock_x_pickuped_offset_left = 0;
    static constexpr int rock_x_pickuped_offset_right = 10;
    static constexpr int rock_y_pickuped_offset = 6;
    static constexpr u8 rock_sprite_width = 8;
    static constexpr u8 rock_sprite_height = 8;
    static constexpr u16 rock_physical_width = 7;
    static constexpr u16 rock_physical_height = 7;
    static constexpr SpritesheetType rock_spritesheet_type = SpritesheetType::BLOOD_ROCK_ROPE_POOF;

    Rock(int x, int y) : BaseItem(
            x,
            y,
            rock_sprite_width,
            rock_sprite_height,
            rock_spritesheet_type,
            rock_physical_width,
            rock_physical_height,
            rock_x_pickuped_offset_left,
            rock_x_pickuped_offset_right,
            rock_y_pickuped_offset
    ) {
        _activated = true;
        _max_x_speed = 3.5f;
        _friction = ICollidable::default_friction * 12;
        init_sprites();
    }

    // BaseItem overrides

    void update_item_specific() override;

    // IRenderable overrides

    void init_sprites() override;

    // Other, creature specific

};


#endif //SPELUNKYDS_ROCK_H
