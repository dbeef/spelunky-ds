//
// Created by dbeef on 5/4/19.
//

#ifndef SPELUNKYDS_HANGINGONEDGE_HPP
#define SPELUNKYDS_HANGINGONEDGE_HPP

#include "_DudeStateHandler.hpp"

class HangingOnEdge : public _DudeStateHandler {

public:

    HangingOnEdge() : _DudeStateHandler(_MainDudeState::HANGING_ON_EDGE) {}

    ~HangingOnEdge() override = default;

    void handle_input() override;

    void update() override;

    void enter() override;
};

#endif //SPELUNKYDS_HANGINGONEDGE_HPP
