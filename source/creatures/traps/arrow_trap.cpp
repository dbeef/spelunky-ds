//
// Created by xdbeef on 15.05.18.
//

#include "../../../build/gfx_spike_collectibles_flame.h"
#include <cstdlib>
#include <maxmod9.h>
#include "spikes.h"
#include "../../collisions/collisions.hpp"
#include "../../globals_declarations.hpp"
#include "../../../build/soundbank.h"
#include "../animations/blood.hpp"
#include "arrow_trap.h"
#include "../items/arrow.hpp"

void ArrowTrap::draw() {

    //todo check if tile still exists

    if (activated)
        return;

    if (check_if_can_be_triggered(global::main_dude))
        return;

    for (unsigned long a = 0; a < global::creatures.size(); a++) {
        //check if there's any moving object in 6 tiles starting from the trap at the same height
        if (check_if_can_be_triggered(global::creatures.at(a)))
            return;
    }
}


void ArrowTrap::init() {
    initSprite();
}

void ArrowTrap::initSprite() {
}

ArrowTrap::ArrowTrap() {
    this->sprite_height = ARROW_TRAP_SPRITE_HEIGHT;
    this->sprite_width = ARROW_TRAP_SPRITE_WIDTH;
    this->physical_height = ARROW_TRAP_PHYSICAL_HEIGHT;
    this->physical_width = ARROW_TRAP_PHYSICAL_WIDTH;
}

void ArrowTrap::spawn_arrow() {
    auto *arrow = new Arrow();
    global::creatures_to_add.push_back(arrow);
    arrow->sprite_state = sprite_state;

    if (sprite_state == SpriteState::W_LEFT) {
        arrow->x = x - 7;
        arrow->xSpeed = -4;
    } else if (sprite_state == SpriteState::W_RIGHT) {
        arrow->x = x + ARROW_TRAP_PHYSICAL_WIDTH + 7;
        arrow->xSpeed = 4;
    }

    arrow->init();
    arrow->y = y + 7;

}

void ArrowTrap::deleteSprite() {}

bool ArrowTrap::check_if_can_be_triggered(BaseCreature *obj) {

    if (obj->xSpeed == 0 && obj->ySpeed == 0)
        return false;

    if (obj->y >= y && obj->y < y + ARROW_TRAP_PHYSICAL_HEIGHT) {


        if (sprite_state == SpriteState::W_LEFT) {

            if (obj->x <= x && obj->x > x - (7 * ARROW_TRAP_PHYSICAL_WIDTH)) {


                activated = true;
                spawn_arrow();
                mmEffect(SFX_XARROWTRAP);
                return true;

            }

        } else if (sprite_state == SpriteState::W_RIGHT) {

            if (obj->x >= x + ARROW_TRAP_PHYSICAL_WIDTH &&
                obj->x < x + ARROW_TRAP_PHYSICAL_WIDTH + (7 * ARROW_TRAP_PHYSICAL_WIDTH)) {

                //todo checking like in the caveman


                activated = true;
                spawn_arrow();
                mmEffect(SFX_XARROWTRAP);
                return true;
            }

        }

    }
    return false;

}
