//
// Created by xdbeef on 15.05.18.
//

#include <cstdlib>
#include <maxmod9.h>

#include "../../build/gfx_spike_collectibles_flame.h"
#include "../creatures/traps/spikes.h"
#include "../collisions/collisions.hpp"
#include "../globals_declarations.hpp"
#include "../../build/soundbank.h"
#include "../creatures/animations/blood.hpp"
#include "arrow_trap.h"
#include "../creatures/items/arrow.hpp"

void ArrowTrap::update_decoration_specific() {

    //TODO Check if tile still exists, if not, it's ready to dispose and can't be triggered.

    if (_activated) return;
    
    if (check_if_can_be_triggered(global::main_dude)) return;

    for (unsigned long a = 0; a < global::creatures.size(); a++) {
        if (check_if_can_be_triggered(global::creatures.at(a))) return;
    }
}

void ArrowTrap::spawn_arrow() {
    
    auto *arrow = new Arrow(0, 0);
    global::creatures_to_add.push_back(arrow);
    arrow->sprite_state = _sprite_state;

    if (_sprite_state == SpriteState::W_LEFT) {
        arrow->_x = _x - 9;
        arrow->_x_speed = -4;
    } else if (_sprite_state == SpriteState::W_RIGHT) {
        arrow->_x = _x + arrow_trap_physical_width + 7;
        arrow->_x_speed = 4;
    }

    arrow->_y = _y + 7;

}

//check if there's any moving object in 6 tiles starting from the trap at the same height
bool ArrowTrap::check_if_can_be_triggered(BaseCreature *obj) {

    if (obj->_x_speed == 0 && obj->_y_speed == 0)
        return false;

    if (obj->_y >= _y && obj->_y < _y + arrow_trap_physical_height) {


        if (_sprite_state == SpriteState::W_LEFT) {

            if (obj->_x <= _x && obj->_x > _x - (7 * arrow_trap_physical_width)) {


                _activated = true;
                spawn_arrow();
                mmEffect(SFX_XARROWTRAP);
                return true;

            }

        } else if (_sprite_state == SpriteState::W_RIGHT) {

            if (obj->_x >= _x + arrow_trap_physical_width &&
                obj->_x < _x + arrow_trap_physical_width + (7 * arrow_trap_physical_width)) {

                //todo checking like in the caveman
                _activated = true;
                spawn_arrow();
                mmEffect(SFX_XARROWTRAP);
                return true;
            }

        }

    }
    return false;

}

ArrowTrap::ArrowTrap(int x, int y, SpriteState sprite_state) :
        BaseDecoration(x, y, 0, 0, SpritesheetType::NONE), _sprite_state(sprite_state) {
    init_sprites();
}

