//
// Created by dbeef on 5/4/19.
//

#ifndef SPELUNKYDS_EXITINGLEVEL_HPP
#define SPELUNKYDS_EXITINGLEVEL_HPP


#include "_DudeStateHandler.hpp"

class ExitingLevel : public _DudeStateHandler {

public:

    ExitingLevel() : _DudeStateHandler(_MainDudeState::EXITING_LEVEL) {}

    ~ExitingLevel() override = default;

    void handle_input() override;

    void update() override;

    void enter() override;
};


#endif //SPELUNKYDS_EXITINGLEVEL_HPP
