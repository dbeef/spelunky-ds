//
// Created by dbeef on 5/12/19.
//

#ifndef SPELUNKYDS_DUDEEQUIPMENTSTATE_HPP
#define SPELUNKYDS_DUDEEQUIPMENTSTATE_HPP

enum class DudePossibleEquipment {
    HOLDING_ITEM,
    NOT_HOLDING_ITEM
};

class DudeEquipmentState {
public:

    explicit DudeEquipmentState(DudePossibleEquipment state) : _state(state) { }

    virtual ~DudeEquipmentState() = default;

    virtual void enter() = 0;

    virtual void handle_input() = 0;

    virtual void update() = 0;

    const DudePossibleEquipment _state;
};

#endif //SPELUNKYDS_DUDEEQUIPMENTSTATE_HPP
