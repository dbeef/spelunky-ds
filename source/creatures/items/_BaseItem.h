//
// Created by xdbeef on 01.01.19.
//

#ifndef SPELUNKYDS_BASEITEM_H
#define SPELUNKYDS_BASEITEM_H


#include "../../interfaces/IRenderable.h"
#include "../../interfaces/ICollidable.h"
#include "../SpriteInfo.h"

class BaseItem : public IRenderable, public ICollidable {

public:

    // Constructor of inheriting class should call init_sprites.
    BaseItem(
            int x, int y,
            u16 sprite_width, u16 sprite_height, SpritesheetType spritesheet_type,
            u16 physical_width, u16 physical_height
    );

    void update();

    // To be implemented by inheriting class:

    virtual void update_item_specific() = 0;

    virtual bool is_activated() = 0;

    // IRenderable overrides

    // Not overriding init_sprites, that should be done in inheriting class.

    void delete_sprites() override;

    void update_sprites_position() override;

    // ICollidable overrides

    bool can_apply_friction() const override { return true; }

    bool can_apply_gravity() const override { return true; }

    virtual ~BaseItem() = default;

    bool _hold_by_main_dude{};
    SpriteInfo *_main_sprite_info{};
    SpriteInfo *_sub_sprite_info{};
};


#endif //SPELUNKYDS_BASEITEM_H
