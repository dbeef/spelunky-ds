//
// Created by xdbeef on 07.04.18.
//
#include <nds.h>
#include <nds/arm9/input.h>
#include "InputHandler.h"

void InputHandler::updateInput() {

    scanKeys();
    keys_held = keysHeld();
    keys_down = keysDown();

    if (keys_held) {

        right_key_held = (keys_held & KEY_RIGHT) != 0;
        left_key_held = (keys_held & KEY_LEFT) != 0;
        down_key_held = (keys_held & KEY_DOWN) != 0;
        up_key_held = (keys_held & KEY_UP) != 0;
        b_key_held = (keys_held & KEY_B) != 0;

    } else {
        left_key_held = false;
        right_key_held = false;
        down_key_held = false;
        b_key_held = false;
    }

    if (keys_down) {
        r_bumper_down = (keys_down & KEY_R) != 0;
        l_bumper_down = (keys_down & KEY_L) != 0;
        x_key_down = (keys_down & KEY_X) != 0;
        y_key_down = (keys_down & KEY_Y) != 0;
    } else {
        r_bumper_down = false;
        l_bumper_down = false;
        x_key_down = false;
        y_key_down = false;
    }

}
