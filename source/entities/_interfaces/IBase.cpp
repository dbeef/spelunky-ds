//
// Created by xdbeef on 29.12.18.
//

#include "IBase.h"
#include "../../GameState.hpp"

void IBase::set_pickuped_position_not_checking(
        int pickup_offset_x_left,
        int pickup_offset_x_right,
        int pickup_offset_y) {

    _y = MainDude::instance()._y + pickup_offset_y;

    if (MainDude::instance().sprite_state == Orientation::LEFT) {
        _x = MainDude::instance()._x - pickup_offset_x_left;
    } else
        _x = MainDude::instance()._x + pickup_offset_x_right;

}
