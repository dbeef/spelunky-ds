//
// Created by xdbeef on 06.05.18.
//
#include "../globals_declarations.h"

namespace time {

    //note -> calling timerElapsed(0) flushes the value, so by the next timerElapsed(0) that
    //happens in the same frame, you will get 0 - make sure then, that you call it only once every frame!
    void update_ms_since_last_frame() {
        *global::timer = (timerElapsed(0) / TICKS_PER_SECOND);
    }

    void stop() {
        timerStop(0);
        timerStop(1);
    }

    void start() {
        timerStart(0, ClockDivider_1024, 1000, 0);
        timerStart(1, ClockDivider_1, 10, 0);
    }

}