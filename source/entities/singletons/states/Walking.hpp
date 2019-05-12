//
// Created by dbeef on 5/4/19.
//

#ifndef SPELUNKYDS_WALKING_HPP
#define SPELUNKYDS_WALKING_HPP


#include "../DudeActionState.hpp"

class Walking : public DudeActionState {

public:

    Walking() : DudeActionState(_MainDudeState::WALKING) {}

    ~Walking() override = default;

    void handle_input() override;

    void update() override;

    void enter() override;
};


#endif //SPELUNKYDS_WALKING_HPP
