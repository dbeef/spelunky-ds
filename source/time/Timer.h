//
// Created by xdbeef on 04.03.18.
//

#ifndef SPELUNKYDS_TIMER_H
#define SPELUNKYDS_TIMER_H

//With selected divisor value as 1024:
//1 tick = 32.7284 kHz
//32.7284 ticks must be counted every millisecond
//1 / 32.7284 = 0,030554503, so every ~0.03 millisecond gives 1 tick

class Timer {

public:

    Timer();

    u16 get_ms_since_last_frame();

    void stop();
    void start();
};


#endif //SPELUNKYDS_TIMER_H
