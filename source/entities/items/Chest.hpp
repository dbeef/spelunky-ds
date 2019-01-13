//
// Created by xdbeef on 16.05.18.
//

#ifndef SPELUNKYDS_CHEST_H
#define SPELUNKYDS_CHEST_H

#include "_BaseItem.h"

//http://spelunky.wikia.com/wiki/Chest
class Chest : public BaseItem {

public:

    static constexpr u8 chest_pickup_offset_x_left = 4;
    static constexpr u8 chest_pickup_offset_x_right = 8;
    static constexpr u8 chest_pickup_offset_y = 5;
    static constexpr u8 chest_sprite_width = 16;
    static constexpr u8 chest_sprite_height = 16;
    static constexpr u16 chest_physical_width = 12;
    static constexpr u16 chest_physical_height = 8;
    static constexpr SpritesheetType chest_spritesheet_type = SpritesheetType::SPIKES_COLLECTIBLES;

    Chest(int x, int y) : BaseItem(
            x,
            y,
            chest_sprite_width,
            chest_sprite_height,
            chest_spritesheet_type,
            chest_physical_width,
            chest_physical_height,
            chest_pickup_offset_x_left,
            chest_pickup_offset_x_right,
            chest_pickup_offset_y
    ) {
        _friction = 0.1f;
        _bouncing_factor_y = ICollidable::default_bouncing_factor_y * 0.5f;
        _max_x_speed = 1.25f;
        _pos_update_delta = 15;
        init_sprites();
    }

    // BaseItem overrides

    void update_item_specific() override;

    // IRenderable overrides

    void init_sprites() override;

    // Other, item specific

    void spawn_treasure();

    void match_animation();

    bool _dropped_loot{};

};

#endif //SPELUNKYDS_CHEST_H
