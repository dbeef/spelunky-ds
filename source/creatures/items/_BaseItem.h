//
// Created by xdbeef on 01.01.19.
//

#ifndef SPELUNKYDS_BASEITEM_H
#define SPELUNKYDS_BASEITEM_H


#include "../../interfaces/IRenderable.h"
#include "../../interfaces/ICollidable.h"
#include "../SpriteInfo.h"

// _activated flag must be declared explicitly in inheriting classes and returned in is_activated,
// if an item is activable, i.e
// activable item is a shotgun,
// not activable item is a rock.

// vs

// actually putting _activated flag in the base class

class BaseItem : public IRenderable, public ICollidable {

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

    bool can_apply_friction() const override { return true; }

    bool can_apply_gravity() const override { return true; }

    virtual ~BaseItem() = default;

    // FIXME Duplicating functions from the BaseCreature

    bool check_if_can_be_opened();

    void check_if_can_be_pickuped();

    bool check_if_can_be_equipped();

    const int _x_pickup_offset_left;
    const int _x_pickup_offset_right;
    const int _y_pickup_offset;

    bool _activated;
    // TODO Check if this is redundant with main dude's 'currently_held_item' field.
    bool _hold_by_main_dude{};
    SpriteInfo *_main_sprite_info{};
    SpriteInfo *_sub_sprite_info{};
};


#endif //SPELUNKYDS_BASEITEM_H
