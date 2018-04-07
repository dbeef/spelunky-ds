//
// Created by xdbeef on 07.04.18.
//

#ifndef SPELUNKYDS_INPUTHANDLER_H
#define SPELUNKYDS_INPUTHANDLER_H


class InputHandler {
public:
    void updateInput();
    int keys_held;
    int keys_down;

    bool left_key_held;
    bool right_key_held;
    bool down_key_held;
    bool b_key_held;

    bool r_bumper_down;
    bool l_bumper_down;

};


#endif //SPELUNKYDS_INPUTHANDLER_H
