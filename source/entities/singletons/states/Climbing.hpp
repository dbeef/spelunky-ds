//
// Created by dbeef on 5/4/19.
//

#ifndef SPELUNKYDS_CLIMBING_HPP
#define SPELUNKYDS_CLIMBING_HPP


#include "_DudeStateHandler.hpp"

class Climbing : public _DudeStateHandler {

public:

    Climbing() : _DudeStateHandler(_MainDudeState::CLIMBING) {}

    ~Climbing() override = default;

    void handle_input() override;

    void update() override;

    void enter() override;

    enum {
        LADDER,
        ROPE
    } _type{};
};


#endif //SPELUNKYDS_CLIMBING_HPP
