//
// Created by xdbeef on 06.05.18.
//

#ifndef SPELUNKYDS_TIME_UTILS_H
#define SPELUNKYDS_TIME_UTILS_H

class Timer {
public:

    static void stop();

    static void start();

    static void update_ms_since_last_frame();

    //note -> calling timerElapsed(0) flushes the value, so by the next timerElapsed(0) that
    //happens in the same frame, you will get 0 - make sure then, that you call it only once every frame!
    inline static double getDeltaTime() { return _timer; }

private:

    static double _timer;
};

#endif //SPELUNKYDS_TIME_UTILS_H
