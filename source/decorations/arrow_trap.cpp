//
// Created by xdbeef on 15.05.18.
//

#include <cstdlib>
#include <maxmod9.h>

#include "../../build/gfx_spike_collectibles_flame.h"
#include "../../build/soundbank.h"

#include "../creatures/traps/spikes.h"
#include "../collisions/collisions.hpp"
#include "../globals_declarations.hpp"
#include "../creatures/animations/blood.hpp"
#include "../creatures/items/arrow.hpp"
#include "arrow_trap.h"

void ArrowTrap::update_decoration_specific() {

    // TODO Check if tile still exists, if not, it's ready to dispose and can't be triggered.
    // This checking should be activated on every bomb explosion.

    if (_activated) return;

    if (check_if_can_be_triggered(global::main_dude)) return;

    for (auto &creature : global::creatures)
        if (check_if_can_be_triggered(creature)) return;
}

void ArrowTrap::spawn_arrow() const {

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

// TODO Exclude blood/fire particles from triggering traps.
bool ArrowTrap::check_if_can_be_triggered(BaseCreature *obj) {

    // So it wouldn't be activated by any rock, jar or any other item that
    // just stands in front of the trap.
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

                // TODO Checking like in the caveman:
                // make sure there's no obstacle between creature and trap before triggering.

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
        BaseDecoration(x, y, 0, 0, SpritesheetType::NONE),
        _sprite_state(sprite_state) {
    init_sprites();
}

void ArrowTrap::introduce_yourself() const {
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

