//
// Created by dbeef on 5/4/19.
//

#ifndef SPELUNKYDS_MAINDUDESTATE_H
#define SPELUNKYDS_MAINDUDESTATE_H

enum class _MainDudeState {
    // Running, crawling, pushing or standing. Can be in air.
    WALKING,
    // Either rope or ladder.
    CLIMBING,
    HANGING_ON_EDGE,
    STUNNED,
    DEAD,
    // Meaning main dude entered doors, in scores, main menu or level.
    EXITING_LEVEL
};


#endif //SPELUNKYDS_MAINDUDESTATE_H
