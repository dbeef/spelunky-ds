//
// Created by xdbeef on 07.04.18.
// https://stackoverflow.com/questions/120876/what-are-the-rules-for-calling-the-superclass-constructor
//

#include <maxmod9.h>
#include <cmath>
#include <cstdio>

#include "../../../build/soundbank.h"
#include "../../memory/OamManager.hpp"
#include "../../camera/Camera.hpp"
#include "../../tiles/LevelRenderingUtils.hpp"
#include "../../collisions/Collisions.hpp"
#include "../../sound/Sound.hpp"
#include "_BaseCreature.h"
#include "../animations/Blood.hpp"
#include "../../GameState.hpp"

void BaseCreature::spawn_blood() const {
    for (int a = 0; a < 4; a++) {
        auto *blood = new Blood(_x, _y);

        if (rand() % 2 == 0)
            blood->_x_speed = (-0.8 - ((float) (rand() % 20) / 10)) / a;
        else
            blood->_x_speed = (0.8 + ((float) (rand() % 20) / 10)) / a;

        blood->_y_speed = (-1.5 - (((float) (rand() % 20) / 5))) / a;
        GameState::instance().creatures.push_back(blood);
    }
}

void BaseCreature::kill_if_whip(int dmg_to_apply) {
    if (GameState::instance().main_dude->using_whip && !killed && GameState::instance().main_dude->whip->_whiping_timer > 120) {
        if (Collisions::checkCollisionWithMainDudeWhip(_x, _y, _sprite_width, _sprite_height)) {
            apply_dmg(dmg_to_apply);
        }
    }
}

void BaseCreature::kill_if_main_dude_jumped_on_you(int dmg_to_apply) {
    if (!killed && Collisions::checkCollisionWithMainDude(_x, _y, _sprite_width, _sprite_height) &&
        GameState::instance().main_dude->_y_speed > 0 &&
        GameState::instance().main_dude->_y_speed - 4 < _y) {

        apply_dmg(dmg_to_apply);
        GameState::instance().main_dude->_y_speed = -2;
        GameState::instance().main_dude->jumping_timer = 0;

    }
}

//for opening chests and crates
bool BaseCreature::check_if_can_be_opened() {
    if (!activated &&
        Collisions::checkCollisionWithMainDudeWidthBoundary(_x, _y, _physical_width, _physical_height, 8) &&
        GameState::instance().input_handler->up_key_held && GameState::instance().input_handler->y_key_down) {

        activated = true;
        GameState::instance().input_handler->y_key_down = false;

        return true;
    } else
        return false;
}

//TODO Merge check_if_can_be_pickuped with set_pickuped_position
//check, if main dude can pickup this item (to hands, not to inventory)
void BaseCreature::check_if_can_be_pickuped() {

    if (hold_by_main_dude &&
        GameState::instance().input_handler->y_key_down &&
        GameState::instance().input_handler->down_key_held &&
        GameState::instance().main_dude->_bottom_collision) {

        //leave item on ground

        hold_by_main_dude = false;
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
        GameState::instance().main_dude->_currently_held_creature = this;
        hold_by_main_dude = true;
        GameState::instance().input_handler->y_key_down = false;

    }

};

//check, if main dude can pickup to the inventory (not to the hands)
bool BaseCreature::check_if_can_be_equipped() {

    bool q = (GameState::instance().input_handler->y_key_down && GameState::instance().input_handler->down_key_held &&
              !GameState::instance().main_dude->holding_item) &&
             Collisions::checkCollisionWithMainDude(_x, _y, _sprite_width, _sprite_height);

    if (q) {
        GameState::instance().input_handler->y_key_down = false;
    }

    return q;

};


//this should be applied, when item is being carried by main dude
void BaseCreature::set_pickuped_position(int pickup_offset_x, int pickup_offset_y) {

    if (hold_by_main_dude) {

        _y = GameState::instance().main_dude->_y + pickup_offset_y;

        if (GameState::instance().main_dude->sprite_state == Orientation::LEFT) {
            _x = GameState::instance().main_dude->_x - pickup_offset_x;
        } else
            _x = GameState::instance().main_dude->_x + pickup_offset_x;


        sprite_state = GameState::instance().main_dude->sprite_state;
    }

}

//this should be applied, when item is being carried by main dude
void BaseCreature::set_pickuped_position(int pickup_offset_x_left, int pickup_offset_x_right, int pickup_offset_y) {

    if (hold_by_main_dude) {

        _y = GameState::instance().main_dude->_y + pickup_offset_y;

        if (GameState::instance().main_dude->sprite_state == Orientation::LEFT) {
            _x = GameState::instance().main_dude->_x - pickup_offset_x_left;
        } else
            _x = GameState::instance().main_dude->_x + pickup_offset_x_right;


        sprite_state = GameState::instance().main_dude->sprite_state;
    }

}

//this should be applied, when item is being carried by another moving object
void
BaseCreature::set_pickuped_position_on_another_moving_obj(int pickup_offset_x, int pickup_offset_y,
                                                          BaseCreature *m) const {

    m->_y = _y + pickup_offset_y;

    if (sprite_state == Orientation::LEFT) {
        m->_x = _x - pickup_offset_x;
    } else
        m->_x = _x + pickup_offset_x;

}

//this should be applied, when item is being carried by another moving object
void
BaseCreature::set_pickuped_position_on_another_moving_obj(int pickup_offset_x, int pickup_offset_y, BaseItem *m) const {

    m->_y = _y + pickup_offset_y;

    if (sprite_state == Orientation::LEFT) {
        m->_x = _x - pickup_offset_x;
    } else
        m->_x = _x + pickup_offset_x;

}


//this should be applied, when item is being carried by main dude
void BaseCreature::set_pickuped_position_not_checking(int pickup_offset_x, int pickup_offset_y) {

    _y = GameState::instance().main_dude->_y + pickup_offset_y;
    if (GameState::instance().main_dude->sprite_state == Orientation::LEFT) {
        _x = GameState::instance().main_dude->_x - pickup_offset_x;
    } else
        _x = GameState::instance().main_dude->_x + pickup_offset_x;
}

BaseCreature::BaseCreature(int x, int y, const u16 sprite_width, const u16 sprite_height,
                           const SpritesheetType spritesheet_type, u16 physical_width, u16 physical_height,
                           CreatureType creature_type) :
        IRenderable(sprite_width, sprite_height, spritesheet_type),
        ICollidable(physical_width, physical_height),
        _creature_type(creature_type) {
    set_xy(x, y);
}


//https://stackoverflow.com/questions/120876/what-are-the-rules-for-calling-the-superclass-constructor