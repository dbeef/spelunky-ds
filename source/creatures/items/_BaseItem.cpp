//
// Created by xdbeef on 01.01.19.
//

#include "_BaseItem.h"
#include "../SpriteUtils.hpp"

BaseItem::BaseItem(
        int x, int y,
        u16 sprite_width, u16 sprite_height, SpritesheetType spritesheet_type,
        u16 physical_width, u16 physical_height
) :
        IRenderable(sprite_width, sprite_height, spritesheet_type),
        ICollidable(physical_width, physical_height) {
    set_xy(x, y);
}

void BaseItem::update_sprites_position() {
    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);
    sprite_utils::set_entry_xy(_main_sprite_info, static_cast<u16>(main_x), static_cast<u16>(main_y));
    sprite_utils::set_entry_xy(_sub_sprite_info, static_cast<u16>(sub_x), static_cast<u16>(sub_y));

}

void BaseItem::delete_sprites() {
    delete _main_sprite_info;
    delete _sub_sprite_info;
    _main_sprite_info = nullptr;
    _sub_sprite_info = nullptr;
}

void BaseItem::update() {
    if (!_hold_by_main_dude) update_collidable();
    update_item_specific();
}
