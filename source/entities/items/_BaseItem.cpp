//
// Created by xdbeef on 01.01.19.
//

#include "_BaseItem.h"
#include "../SpriteUtils.hpp"
#include "../../collisions/Collisions.hpp"
#include "../../GlobalsDeclarations.hpp"

BaseItem::BaseItem(
        int x, int y,
        u16 sprite_width, u16 sprite_height, SpritesheetType spritesheet_type,
        u16 physical_width, u16 physical_height,
        int x_pickup_offset_left, int x_pickup_offset_right, int y_pickup_offset
) : IPickupable(x, y, sprite_width, sprite_height, spritesheet_type, physical_width, physical_height),
    _x_pickup_offset_left(x_pickup_offset_left),
    _x_pickup_offset_right(x_pickup_offset_right),
    _y_pickup_offset(y_pickup_offset) {
    set_xy(x, y);
}

void BaseItem::update_sprites_position() {
    if (_render_in_hud) {
        _main_sprite_info->entry->priority = OBJPRIORITY_0;
        sprite_utils::set_entry_xy(_main_sprite_info, static_cast<u16>(_x), static_cast<u16>(_y));
        _sub_sprite_info->entry->isHidden = true;
        _main_sprite_info->entry->isHidden = false;
    } else {
        int main_x, main_y, sub_x, sub_y;
        get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);
        sprite_utils::set_entry_xy(_main_sprite_info, static_cast<u16>(main_x), static_cast<u16>(main_y));
        sprite_utils::set_entry_xy(_sub_sprite_info, static_cast<u16>(sub_x), static_cast<u16>(sub_y));
    }
}

void BaseItem::delete_sprites() {
    delete _main_sprite_info;
    delete _sub_sprite_info;
    _main_sprite_info = nullptr;
    _sub_sprite_info = nullptr;
}

void BaseItem::update() {
    if (!_hold_by_main_dude) {
        update_collidable();
    } else
        set_pickuped_position_not_checking(_x_pickup_offset_left, _x_pickup_offset_right, _y_pickup_offset);

    check_if_can_be_pickuped();
    update_item_specific();
}
