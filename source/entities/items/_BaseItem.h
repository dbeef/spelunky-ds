//
// Created by xdbeef on 01.01.19.
//

#ifndef SPELUNKYDS_BASEITEM_H
#define SPELUNKYDS_BASEITEM_H


#include "../SpriteInfo.h"
#include "../../interfaces/IRenderable.h"
#include "../../interfaces/ICollidable.h"
#include "../../interfaces/IPickupable.h"

class BaseItem : public IPickupable {

public:

    // Constructor of inheriting class should call init_sprites.
    BaseItem(
            int x, int y,
            u16 sprite_width, u16 sprite_height, SpritesheetType spritesheet_type,
            u16 physical_width, u16 physical_height,
            int x_pickup_offset_left, int x_pickup_offset_right, int y_pickup_offset
    );

    void update();

    // To be implemented by inheriting class:

    virtual void update_item_specific() = 0;

    bool is_activated() { return _activated; };

    // IRenderable overrides

    // Not overriding init_sprites, that should be done in inheriting class.

    void delete_sprites() override;

    void update_sprites_position() override;

    // ICollidable overrides

    bool can_apply_friction() const override { return _bottom_collision; }

    bool can_apply_gravity() const override { return true; }

    virtual ~BaseItem() = default;

    const int _x_pickup_offset_left;
    const int _x_pickup_offset_right;
    const int _y_pickup_offset;

    SpriteInfo *_main_sprite_info{};
    SpriteInfo *_sub_sprite_info{};
};


#endif //SPELUNKYDS_BASEITEM_H
