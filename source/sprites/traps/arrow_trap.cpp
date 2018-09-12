//
// Created by xdbeef on 15.05.18.
//

#include "../../../build/gfx_spike_collectibles_flame.h"
#include <cstdlib>
#include <maxmod9.h>
#include "spikes.h"
#include "../../collisions/collisions.h"
#include "../../globals_declarations.h"
#include "../../../build/soundbank.h"
#include "../animations/blood.h"
#include "arrow_trap.h"
#include "../items/arrow.h"

void ArrowTrap::draw() {

    //todo check if tile still exists

    if (activated)
        return;

    for (int a = 0; a < global::sprites.size(); a++) {
        //check if there's any moving object in 6 tiles starting from the trap at the same height

        if (global::sprites.at(a)->xSpeed == 0 && global::sprites.at(a)->ySpeed == 0)
            continue;

        if (global::sprites.at(a)->y >= y && global::sprites.at(a)->y < y + ARROW_TRAP_PHYSICAL_HEIGHT) {


            if (sprite_state == SpriteState::W_LEFT) {

                if (global::sprites.at(a)->x <= x && global::sprites.at(a)->x > x - (7 * ARROW_TRAP_PHYSICAL_WIDTH)) {


                    activated = true;
                    spawn_arrow();
                    mmEffect(SFX_XARROWTRAP);
                    break;
                }

            } else if (sprite_state == SpriteState::W_RIGHT) {

                if (global::sprites.at(a)->x >= x + ARROW_TRAP_PHYSICAL_WIDTH &&
                    global::sprites.at(a)->x < x + ARROW_TRAP_PHYSICAL_WIDTH + (7 * ARROW_TRAP_PHYSICAL_WIDTH)) {

                    //todo checking like in the caveman


                    activated = true;
                    spawn_arrow();
                    mmEffect(SFX_XARROWTRAP);
                    break;
                }

            }

        }

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
    global::sprites_to_add.push_back(arrow);
    arrow->sprite_state = sprite_state;

    if (sprite_state == SpriteState::W_LEFT) {
        arrow->x = x - 7;
        arrow->xSpeed = -4;
    } else if (sprite_state == SpriteState::W_RIGHT) {
        arrow->x = x + ARROW_TRAP_PHYSICAL_WIDTH + 7;
        arrow->xSpeed = 4;
    }

    arrow->init();
    arrow->y = y + 4;

}
