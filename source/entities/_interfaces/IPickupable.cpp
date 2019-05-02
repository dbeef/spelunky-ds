//
// Created by dbeef on 13.01.19.
//

#include "IPickupable.h"
#include "../../collisions/Collisions.hpp"
#include "../../GameState.hpp"

//for opening chests and crates
bool IPickupable::check_if_can_be_opened() {
    if (!_activated &&
        Collisions::checkCollisionWithMainDudeWidthBoundary(_x, _y, _physical_width, _physical_height, 8) &&
        GameState::instance().input_handler->up_key_held && GameState::instance().input_handler->y_key_down) {

        _activated = true;
        GameState::instance().input_handler->y_key_down = false;

        return true;
    } else
        return false;
}

//TODO Merge check_if_can_be_pickuped with set_pickuped_position
//check, if main dude can pickup this item (to hands, not to inventory)
void IPickupable::check_if_can_be_pickuped() {

    if (_hold_by_main_dude &&
        GameState::instance().input_handler->y_key_down &&
        GameState::instance().input_handler->down_key_held &&
        GameState::instance().main_dude->_bottom_collision) {

        //leave item on ground

        _hold_by_main_dude = false;
        GameState::instance().main_dude->holding_item = false;
        GameState::instance().input_handler->y_key_down = false;
        _bottom_collision = false;
        GameState::instance().main_dude->_currently_held_creature = nullptr;

    } else if (GameState::instance().input_handler->y_key_down &&
               GameState::instance().input_handler->down_key_held &&
               !GameState::instance().main_dude->holding_item &&
               Collisions::checkCollisionWithMainDude(_x, _y, _physical_width, _physical_height)) {

        //pickup item from the ground

        GameState::instance().main_dude->holding_item = true;
        GameState::instance().main_dude->_currently_held_pickupable = this;
        _hold_by_main_dude = true;
        GameState::instance().input_handler->y_key_down = false;

    }

};

//check, if main dude can pickup to the inventory (not to the hands)
bool IPickupable::check_if_can_be_equipped() {

    bool q = (GameState::instance().input_handler->y_key_down && GameState::instance().input_handler->down_key_held &&
              !GameState::instance().main_dude->holding_item) &&
             Collisions::checkCollisionWithMainDude(_x, _y, _sprite_width, _sprite_height);

    if (q) {
        GameState::instance().input_handler->y_key_down = false;
    }

    return q;

};