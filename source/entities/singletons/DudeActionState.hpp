//
// Created by dbeef on 5/4/19.
//

#ifndef SPELUNKYDS_DUDESTATEHANDLER_H
#define SPELUNKYDS_DUDESTATEHANDLER_H

enum class DudePossibleActions {
    WALKING, // Running, crawling, pushing or standing. Can be in air.
    CLIMBING, // Either rope or ladder.
    HANGING_ON_EDGE,
    STUNNED,
    DEAD,
    EXITING_LEVEL // Meaning main dude entered doors, in scores, main menu or level.
};

class DudeActionState {
public:

    explicit DudeActionState(_MainDudeState state) : _state(state) { }

    virtual ~DudeActionState() = default;

    virtual void enter() = 0;

    virtual void handle_input() = 0;

    virtual void update() = 0;

    const _MainDudeState _state;
};

#endif //SPELUNKYDS_DUDESTATEHANDLER_H
