//
// Created by xdbeef on 16.09.18.
//

#include "simple_sprite_wrapper.h"
#include "sprite_utils.h"
#include "../globals_declarations.h"

void SimpleSpriteWrapper::set_position() {
    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);
    sprite_utils::set_entry_xy(main_sprite_info, main_x, main_y);
    sprite_utils::set_entry_xy(sub_sprite_info, sub_x, sub_y);
}

