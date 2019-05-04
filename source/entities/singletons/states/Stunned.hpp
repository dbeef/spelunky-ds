//
// Created by dbeef on 5/4/19.
//

#ifndef SPELUNKYDS_STUNNED_HPP
#define SPELUNKYDS_STUNNED_HPP


#include "_DudeStateHandler.hpp"

class Stunned : public _DudeStateHandler {

public:

    Stunned() : _DudeStateHandler(_MainDudeState::STUNNED) {}

    ~Stunned() override = default;

    void handle_input() override;

    void update() override;

    void enter() override;
};

#endif //SPELUNKYDS_STUNNED_HPP
