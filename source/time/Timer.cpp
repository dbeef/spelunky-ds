//
// Created by xdbeef on 04.03.18.
//

#include <nds.h>
#include "Timer.h"
#include "../Globals.h"

u16 Timer::getMillisecondsElapsed() {
    return timerElapsed(0) / TICKS_PER_SECOND;
}

void Timer::stop() {
    timerStop(0);
}

Timer::Timer() {
    timerStart(0, ClockDivider_1024, 1000, 0);
    timerStart(1, ClockDivider_1, 10, 0);
}
