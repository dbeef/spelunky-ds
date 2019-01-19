//
// Created by xdbeef on 16.05.18.
//

#ifndef SPELUNKYDS_CRATE_H
#define SPELUNKYDS_CRATE_H

#include "../items/_BaseItem.h"
#include "../_interfaces/ShoppingObject.h"

//http://spelunky.wikia.com/wiki/Crate
class Crate: public BaseItem {

public:

    static constexpr u8 crate_pickup_offset_x_left = 4;
    static constexpr u8 crate_pickup_offset_x_right = 8;
    static constexpr u8 crate_pickup_offset_y = 5;
    static constexpr u8 crate_sprite_width = 16;
    static constexpr u8 crate_sprite_height = 16;
    static constexpr u16 crate_physical_width = 12;
    static constexpr u16 crate_physical_height = 10;
    static constexpr SpritesheetType crate_spritesheet_type = SpritesheetType::SPIKES_COLLECTIBLES;

    Crate(int x, int y) : BaseItem(
            x,
            y,
            crate_sprite_width,
            crate_sprite_height,
            crate_spritesheet_type,
            crate_physical_width,
            crate_physical_height,
            crate_pickup_offset_x_left,
            crate_pickup_offset_x_right,
            crate_pickup_offset_y
    ) {
        init_sprites();
    }

    // BaseItem overrides

    void update_item_specific() override;

    // IRenderable overrides

    void init_sprites() override;

    // Other, creature specific

    void drop_loot();

    void play_collectible_animation();
    
    int _anim_frame{};
    double _anim_frame_timer{};
    bool _dropped_loot{};
    
};

#endif //SPELUNKYDS_CRATE_H
