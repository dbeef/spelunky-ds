//
// Created by xdbeef on 29.12.18.
//

#include "IRenderable.h"
#include "../../GameState.hpp"

void IRenderable::get_x_y_viewported(int *out_main_x, int *out_main_y, int *out_sub_x, int *out_sub_y) const {

    *out_main_x = _x - GameState::instance().camera->x;
    *out_main_y = _y - GameState::instance().camera->y;
    *out_sub_x = _x - GameState::instance().camera->x;
    *out_sub_y = _y - GameState::instance().camera->y - SCREEN_HEIGHT;

    if (GameState::instance().camera->y + SCREEN_HEIGHT > this->_y + _sprite_height ||
        GameState::instance().camera->y + SCREEN_HEIGHT + SCREEN_HEIGHT < this->_y - _sprite_height) {
        *out_sub_x = -SCREEN_HEIGHT;
        *out_sub_y = -SCREEN_HEIGHT;
    }
    if (GameState::instance().camera->y > this->_y + _sprite_height ||
        GameState::instance().camera->y + SCREEN_HEIGHT < this->_y - _sprite_height) {
        *out_main_x = -SCREEN_HEIGHT;
        *out_main_y = -SCREEN_HEIGHT;
    }

    if (*out_sub_y + _sprite_height < 0 || *out_sub_x + _sprite_width < 0) {
        *out_sub_x = -SCREEN_HEIGHT;
        *out_sub_y = -SCREEN_HEIGHT;
    }

    if (*out_main_y + _sprite_height < 0 || *out_main_x + _sprite_width < 0) {
        *out_main_x = -SCREEN_HEIGHT;
        *out_main_y = -SCREEN_HEIGHT;
    }

}