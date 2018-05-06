//
// Created by xdbeef on 07.04.18.
//
#include <nds.h>
#include <nds/arm9/input.h>
#include <iostream>
#include "input_handler.h"

void InputHandler::updateInput() {

    if(stop_handling)
        return;

    scanKeys();
    keys_held = keysHeld();
    keys_down = keysDown();

    if (keys_held) {

        right_key_held = (keys_held & KEY_RIGHT) != 0;
        left_key_held = (keys_held & KEY_LEFT) != 0;
        down_key_held = (keys_held & KEY_DOWN) != 0;
        up_key_held = (keys_held & KEY_UP) != 0;
        b_key_held = (keys_held & KEY_B) != 0;
        y_key_held = (keys_held & KEY_Y) != 0;

//        seed += right_key_held + left_key_held + down_key_held + up_key_held + b_key_held + y_key_held;

    } else {
        left_key_held = false;
        right_key_held = false;
        down_key_held = false;
        b_key_held = false;
        y_key_held = false;
        up_key_held = false;
    }

    if (keys_down) {
        r_bumper_down = (keys_down & KEY_R) != 0;
        l_bumper_down = (keys_down & KEY_L) != 0;
        x_key_down = (keys_down & KEY_X) != 0;
        y_key_down = (keys_down & KEY_Y) != 0;
        a_key_down = (keys_down & KEY_A) != 0;

//        seed += r_bumper_down + l_bumper_down + x_key_down + y_key_down + a_key_down;

    } else {
        r_bumper_down = false;
        l_bumper_down = false;
        x_key_down = false;
        y_key_down = false;
        a_key_down = false;
    }

}
