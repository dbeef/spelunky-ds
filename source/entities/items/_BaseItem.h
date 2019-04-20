//
// Created by xdbeef on 01.01.19.
//

#ifndef SPELUNKYDS_BASEITEM_H
#define SPELUNKYDS_BASEITEM_H

#include "../_interfaces/IRenderable.h"
#include "../_interfaces/ICollidable.h"
#include "../_interfaces/IPickupable.h"

class SpriteInfo;

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

    virtual void print_classname() = 0;

    virtual void update_item_specific() = 0;

    // IRenderable overrides

    // Not overriding init_sprites, that should be done in inheriting class.

    void delete_sprites() override;

    void update_sprites_position() override;

    // ICollidable overrides

    bool can_apply_friction() const override { return _bottom_collision; }

    bool can_apply_gravity() const override { return true; }

    virtual ~BaseItem() = default;

    // Providing pickup offsets to be applied when main dude's holding this item:

    const int _x_pickup_offset_left;
    const int _x_pickup_offset_right;
    const int _y_pickup_offset;

    // Providing SpriteInfo's here instead of in inheriting class, since all items in the
    // game are smaller than 64x64px, there's no need to use more sprites to represent an item.

    SpriteInfo *_main_sprite_info{};
    SpriteInfo *_sub_sprite_info{};

    // Set to true if this item can be drawn on HUD instead like a normal item.
    // Equipped items are those, which on pickup are transferred to the inventory,
    // therefore they're drawn on HUD.
    // i.e mitt or spring shoes become hud items when they're pickuped, and rock or arrow don't.
    bool _render_in_hud{};
};


#endif //SPELUNKYDS_BASEITEM_H
