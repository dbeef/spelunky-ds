//
// Created by xdbeef on 06.05.18.
//

#ifndef SPELUNKYDS_TIME_UTILS_H
#define SPELUNKYDS_TIME_UTILS_H

//With selected divisor value as 1024:
//1 tick = 32.7284 kHz
//32.7284 ticks must be counted every millisecond
//1 / 32.7284 = 0,030554503, so every ~0.03 millisecond gives 1 tick

namespace time {
    void update_ms_since_last_frame();
    void stop();
    void start();
}
#endif //SPELUNKYDS_TIME_UTILS_H
