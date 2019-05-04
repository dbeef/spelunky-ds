//
// Created by dbeef on 5/4/19.
//

#ifndef SPELUNKYDS_WALKING_HPP
#define SPELUNKYDS_WALKING_HPP


#include "_DudeStateHandler.hpp"

class Walking : public _DudeStateHandler {

public:

    Walking() : _DudeStateHandler(_MainDudeState::WALKING) {}

    ~Walking() override = default;

    void handle_input() override;

    void update() override;

    void enter() override;
};


#endif //SPELUNKYDS_WALKING_HPP
