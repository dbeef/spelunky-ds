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
) :
        IRenderable(sprite_width, sprite_height, spritesheet_type),
        ICollidable(physical_width, physical_height),
        _x_pickup_offset_left(x_pickup_offset_left),
        _x_pickup_offset_right(x_pickup_offset_right),
        _y_pickup_offset(y_pickup_offset) {
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
    if (!_hold_by_main_dude) {
        update_collidable();
    } else
        set_pickuped_position_not_checking(2, 8, 6);

    check_if_can_be_pickuped();
    update_item_specific();
}


//for opening chests and crates
bool BaseItem::check_if_can_be_opened() {
    if (!_activated &&
        Collisions::checkCollisionWithMainDudeWidthBoundary(_x, _y, _physical_width, _physical_height, 8) &&
        global::input_handler->up_key_held && global::input_handler->y_key_down) {
        _activated = true;
        global::input_handler->y_key_down = false;
        return true;
    } else
        return false;
}

//TODO Merge check_if_can_be_pickuped with set_pickuped_position
//check, if main dude can pickup this item (to hands, not to inventory)
void BaseItem::check_if_can_be_pickuped() {

    if (_hold_by_main_dude &&
        global::input_handler->y_key_down &&
        global::input_handler->down_key_held &&
        global::main_dude->_bottom_collision) {

        //leave item on ground

        _hold_by_main_dude = false;
        global::main_dude->holding_item = false;
        global::input_handler->y_key_down = false;
        _bottom_collision = false;
        global::main_dude->_currently_held_item = nullptr;
    } else if (global::input_handler->y_key_down &&
               global::input_handler->down_key_held &&
               !global::main_dude->holding_item &&
               Collisions::checkCollisionWithMainDude(_x, _y, _physical_width, _physical_height)) {

        //pickup item from the ground
        global::main_dude->holding_item = true;
        _hold_by_main_dude = true;
        global::input_handler->y_key_down = false;
        global::main_dude->_currently_held_item = this;
    }

};

//check, if main dude can pickup to the inventory (not to the hands)
bool BaseItem::check_if_can_be_equipped() {

    bool q = (global::input_handler->y_key_down && global::input_handler->down_key_held &&
              !global::main_dude->holding_item) &&
             Collisions::checkCollisionWithMainDude(_x, _y, _sprite_width, _sprite_height);

    if (q) {
        global::input_handler->y_key_down = false;
    }

    return q;

};

