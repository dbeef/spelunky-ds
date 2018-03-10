//
// Created by xdbeef on 04.03.18.
//

#ifndef SPELUNKYDS_TEXTMANAGER_H
#define SPELUNKYDS_TEXTMANAGER_H


#include "../time/Timer.h"

class TextManager {

    Timer timer;
    int intro_timers[3] = {1000, 3500, 6000};
    bool printed_lines[3] = {0, 0, 0};
    bool console_cleared = 0;


    char *intro_lines[3] = {"\n\n\n\n\n\n\n\n  MY LIPS CRACKED AND COVERED\n            IN SAND, \n",
                           " I SPOTTED THE CAVE'S ENTRANCE,",
                           "     AND FELT THE GODS SMILING \n            UPON ME."};

    int console_clear = 1000000;


    void updateTimer(int introTimers[], int timeElapsed);


public:
    void initConsole();
    void printIntro();
};


#endif //SPELUNKYDS_TEXTMANAGER_H
