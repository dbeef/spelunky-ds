//
// Created by xdbeef on 07.04.18.
//
#include <nds.h>
#include <nds/arm9/input.h>
#include <cstring>
#include "InputHandler.hpp"

InputHandler *InputHandler::_instance;

void InputHandler::init() {
    SPELUNKYDS_BREAKING_ASSERT(!_instance);
    _instance = new InputHandler();
    SPELUNKYDS_BREAKING_ASSERT(_instance);
}

void InputHandler::dispose() {
    SPELUNKYDS_BREAKING_ASSERT(_instance);
    delete _instance;
}

void InputHandler::update_input() {

    if (_block_input) return;

    scanKeys();
    keys_held = keysHeld();
    keys_down = keysDown();

    reset_key_state();

    if (keys_held) {

        keys.right_key_held = (keys_held & KEY_RIGHT) != 0;
        keys.left_key_held = (keys_held & KEY_LEFT) != 0;
        keys.down_key_held = (keys_held & KEY_DOWN) != 0;
        keys.up_key_held = (keys_held & KEY_UP) != 0;
        keys.b_key_held = (keys_held & KEY_B) != 0;
        keys.y_key_held = (keys_held & KEY_Y) != 0;
        keys.l_bumper_held = (keys_held & KEY_L) != 0;
        keys.r_bumper_held = (keys_held & KEY_R) != 0;
    }

    if (keys_down) {
        keys.r_bumper_down = (keys_down & KEY_R) != 0;
        keys.l_bumper_down = (keys_down & KEY_L) != 0;
        keys.x_key_down = (keys_down & KEY_X) != 0;
        keys.y_key_down = (keys_down & KEY_Y) != 0;
        keys.a_key_down = (keys_down & KEY_A) != 0;
        keys.b_key_down = (keys_down & KEY_B) != 0;
    }
}

void InputHandler::reset_key_state() {
    keys = {};
}
