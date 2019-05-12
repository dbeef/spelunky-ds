//
// Created by dbeef on 5/4/19.
//

#ifndef SPELUNKYDS_HANGINGONEDGE_HPP
#define SPELUNKYDS_HANGINGONEDGE_HPP

#include "../DudeActionState.hpp"

class HangingOnEdge : public DudeActionState {

public:

    HangingOnEdge() : DudeActionState(_MainDudeState::HANGING_ON_EDGE) {}

    ~HangingOnEdge() override = default;

    void handle_input() override;

    void update() override;

    void enter() override;
};

#endif //SPELUNKYDS_HANGINGONEDGE_HPP
