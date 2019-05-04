//
// Created by dbeef on 5/4/19.
//

#ifndef SPELUNKYDS_DUDESTATEHANDLER_H
#define SPELUNKYDS_DUDESTATEHANDLER_H

#include "_MainDudeState.hpp"

class _DudeStateHandler {
public:

    explicit _DudeStateHandler(_MainDudeState state) : _state(state) { }

    virtual ~_DudeStateHandler() = default;

    virtual void enter() = 0;

    virtual void handle_input() = 0;

    virtual void update() = 0;

    const _MainDudeState _state;
};


#endif //SPELUNKYDS_DUDESTATEHANDLER_H
