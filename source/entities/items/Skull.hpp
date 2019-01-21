//
// Created by xdbeef on 29.07.18.
//

#ifndef SPELUNKYDS_SKULL_H
#define SPELUNKYDS_SKULL_H

#include "../items/_BaseItem.h"

class Skull : public BaseItem {

public:

    static constexpr u8 skull_sprite_width = 16;
    static constexpr u8 skull_sprite_height = 16;
    static constexpr u16 skull_physical_width = 8;
    static constexpr u16 skull_physical_height = 8;
    static constexpr SpritesheetType skull_spritesheet_type = SpritesheetType::SKELETON_SPIDER;

    Skull(int x, int y) : BaseItem(
            x,
            y,
            skull_sprite_width,
            skull_sprite_height,
            skull_spritesheet_type,
            skull_physical_width,
            skull_physical_height,
            0, 0, 3
    ) {
        _activated = true;
        init_sprites();
    }

    // Base item overrides

    void update_item_specific() override;

    // IRenderable overrides

    void init_sprites() override;

    // Other, creature specific

private:

    void match_animation();

    bool _collided{};
    double _anim_frame_timer{};
    int _anim_frame{};
};


#endif //SPELUNKYDS_SKULL_H
