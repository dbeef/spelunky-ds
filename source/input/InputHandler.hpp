//
// Created by xdbeef on 07.04.18.
//

#ifndef SPELUNKYDS_INPUTHANDLER_H
#define SPELUNKYDS_INPUTHANDLER_H

#include "../preprocessor/Debug.h"

class InputHandler {
public:

    static void init();

    static void dispose();

    inline static InputHandler &instance() {
        SPELUNKYDS_BREAKING_ASSERT(_instance);
        return *_instance;
    }

    void updateInput();

    bool stop_handling{};

    bool left_key_held{};
    bool right_key_held{};
    bool down_key_held{};
    bool up_key_held{};
    bool b_key_held{};
    bool y_key_held{};
    bool l_bumper_held{};
    bool r_bumper_held{};
    bool r_bumper_down{};
    bool l_bumper_down{};
    bool x_key_down{};
    bool y_key_down{};
    bool a_key_down{};
    bool b_key_down{};

private:

    static InputHandler *_instance;

    // keys that are currently held
    int keys_held{};
    // recently pressed keys
    int keys_down{};
};


#endif //SPELUNKYDS_INPUTHANDLER_H
