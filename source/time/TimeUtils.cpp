//
// Created by xdbeef on 06.05.18.
//
#include "../GameState.hpp"

#define TICKS_PER_SECOND 30.5554503

namespace time_utils {

    //note -> calling timerElapsed(0) flushes the value, so by the next timerElapsed(0) that
    //happens in the same frame, you will get 0 - make sure then, that you call it only once every frame!
    void update_ms_since_last_frame() {
        *GameState::instance().timer = (timerElapsed(0) / TICKS_PER_SECOND);
    }

    void stop() {
        timerStop(0);
        timerStop(1);
    }

    void start() {
        //With selected divisor value as 1024:
        //1 tick = 32.7284 kHz
        //32.7284 ticks must be counted every millisecond
        //1 / 32.7284 = 0,030554503, so every ~0.03 millisecond gives 1 tick
        timerStart(0, ClockDivider_1024, 1000, nullptr);
        timerStart(1, ClockDivider_1, 10, nullptr);
    }

}