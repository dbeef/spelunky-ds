//
// Created by dbeef on 5/4/19.
//

#ifndef SPELUNKYDS_DEAD_HPP
#define SPELUNKYDS_DEAD_HPP


#include "../DudeActionState.hpp"

class Dead : public DudeActionState {

public:

    Dead() : DudeActionState(_MainDudeState::DEAD) {}

    ~Dead() override = default;

    void handle_input() override;

    void update() override;

    void enter() override;
};

#endif //SPELUNKYDS_DEAD_HPP
