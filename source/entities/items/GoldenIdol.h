//
// Created by dbeef on 2/17/19.
//

#ifndef SPELUNKYDS_GOLDENIDOL_H
#define SPELUNKYDS_GOLDENIDOL_H


#include <nds/arm9/sprite.h>

#include "../items/_BaseItem.h"
#include "../../memory/SpriteInfo.h"

//http://spelunky.wikia.com/wiki/GoldenIdol
class GoldenIdol : public BaseItem {

public:

    static constexpr int golden_idol_x_pickuped_offset_left = 0;
    static constexpr int golden_idol_x_pickuped_offset_right = 10;
    static constexpr int golden_idol_y_pickuped_offset = -3;
    static constexpr u8 golden_idol_sprite_width = 16;
    static constexpr u8 golden_idol_sprite_height = 16;
    static constexpr u16 golden_idol_physical_width = 12;
    static constexpr u16 golden_idol_physical_height = 15;
    static constexpr SpritesheetType golden_idol_spritesheet_type = SpritesheetType::MONIEZ_GOLDBARS;

    GoldenIdol(int x, int y) : BaseItem(
            x,
            y,
            golden_idol_sprite_width,
            golden_idol_sprite_height,
            golden_idol_spritesheet_type,
            golden_idol_physical_width,
            golden_idol_physical_height,
            golden_idol_x_pickuped_offset_left,
            golden_idol_x_pickuped_offset_right,
            golden_idol_y_pickuped_offset
    ) {
        _activated = true;
        _max_x_speed = 3.5f;
        _friction = ICollidable::default_friction * 12;
        init_sprites();
    }

    // BaseItem overrides

    void update_item_specific() override;

    void print_classname() override { printf("GOLDEN IDOL\n"); };

    // IRenderable overrides

    void init_sprites() override;

    // Other, creature specific

    bool never_pickuped = true;
    bool collected{};
    double animation_timer{};
    double up_timer{};

    void match_frame();

};


#endif //SPELUNKYDS_GOLDENIDOL_H
