//
// Created by xdbeef on 15.05.18.
//

#include <cstdlib>
#include <cstdio>

#include "../../../build/gfx_spike_collectibles_flame.h"
#include "../../../build/soundbank.h"

#include "../creatures/Spikes.h"
#include "../../collisions/Collisions.hpp"
#include "../../GameState.hpp"
#include "../animations/Blood.hpp"
#include "../items/Arrow.hpp"
#include "ArrowTrap.h"
#include "../../sound/Sound.hpp"

void ArrowTrap::update_decoration_specific() {

    // TODO Check if tile still exists, if not, it's ready to dispose and can't be triggered.
    // This checking should be activated on every bomb explosion.

    if (_activated) return;

    if (check_if_can_be_triggered(&MainDude::instance())) return;

    for (auto &creature : GameState::instance().creatures)
        if (check_if_can_be_triggered(creature)) return;
}

void ArrowTrap::spawn_arrow() const {

    auto *arrow = new Arrow(0, 0, _orientation);
    arrow->_y = _y + 7;

    if (_orientation == Orientation::LEFT) {
        arrow->_x = _x - 9;
        arrow->_x_speed = -4;
    } else if (_orientation == Orientation::RIGHT) {
        arrow->_x = _x + arrow_trap_physical_width + 7;
        arrow->_x_speed = 4;
    }

    GameState::instance().items.push_back(arrow);
}

// TODO Exclude blood/fire particles from triggering traps.
bool ArrowTrap::check_if_can_be_triggered(BaseCreature *obj) {

    // So it wouldn't be activated by any rock, jar or any other item that
    // just stands in front of the trap.
    if (obj->_x_speed == 0 && obj->_y_speed == 0)
        return false;

    if (obj->_y >= _y && obj->_y < _y + arrow_trap_physical_height) {

        if (_orientation == Orientation::LEFT) {

            if (obj->_x <= _x && obj->_x > _x - (7 * arrow_trap_physical_width)) {

                _activated = true;
                spawn_arrow();
                sound::arrow_trap();
                return true;

            }

        } else if (_orientation == Orientation::RIGHT) {

            if (obj->_x >= _x + arrow_trap_physical_width &&
                obj->_x < _x + arrow_trap_physical_width + (7 * arrow_trap_physical_width)) {

                // TODO Checking like in the caveman:
                // make sure there's no obstacle between creature and trap before triggering.

                _activated = true;
                spawn_arrow();
                sound::arrow_trap();
                return true;
            }
        }
    }

    return false;
}

ArrowTrap::ArrowTrap(int x, int y, Orientation orientation) :
        BaseDecoration(x, y, 0, 0, SpritesheetType::NONE),
        _orientation(orientation) {
    init_sprites();
}

void ArrowTrap::print_classname() const {
    printf("ARROW TRAP\n");
}

void ArrowTrap::init_sprites() {
    // Empty since arrow trap has no sprites.
}

void ArrowTrap::delete_sprites() {
    // Empty since arrow trap has no sprites.
}

void ArrowTrap::update_sprites_position() {
    // Empty since arrow trap has no sprites.
}

