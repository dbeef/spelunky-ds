//
// Created by xdbeef on 04.04.18.
//

#include <maxmod9.h>
#include "../../globals_declarations.hpp"
#include "../../../build/gfx_blood_rock_rope_poof.h"
#include "rope.hpp"
#include "../../../build/soundbank.h"
#include "../../collisions/collisions.hpp"
#include "../../tiles/level_rendering_utils.hpp"
#include "../sprite_utils.hpp"

#define ROPE_POS_INC_DELTA 15
#define MAX_ROPE_CHAIN_SIZE 8
#define MAX_ROPE_CHAIN_SIZE_EXTENDED 12

void Rope::draw() {

    if (ready_to_dispose) return;

    expand_timer += *global::timer;

    for (int a = ropeChain.size() - 1; a >= 0; a--) {

        ropeChain.at(a)->draw();
        ropeChain.at(a)->update();

        if (finished) {
            if (expand_timer > 40) {
                if (!ropeChain.at(a)->active) {
                    ropeChain.at(a)->active = true;
                    expand_timer = 0;
                }
            }
        }
    }

    if (activated && !thrown && !finished) {
        mmEffect(SFX_XTHROW);

        thrown = true;
        throwingTimer = 0;

        x = floor_div(global::main_dude->x + 0.5 * MAIN_DUDE_PHYSICAL_WIDTH, TILE_W) * TILE_W + physical_width * 0.5;
        y -= 16;
        ySpeed = -4;
    }
    if (thrown && !finished) {
        throwingTimer += *global::timer;
        add_rope_if_needed();
    }

    //checking if main dude can climb over the rope

    bool onTopOfClimbingSpace = global::main_dude->y < y + 6;
    global::main_dude->on_top_of_climbing_space = onTopOfClimbingSpace;

    for (int a = 0; a < ropeChain.size(); a++) {

        if (ropeChain.at(a)->x + 5 > global::main_dude->x &&
            ropeChain.at(a)->x + 5 < global::main_dude->x + MAIN_DUDE_PHYSICAL_WIDTH &&
            ropeChain.at(a)->y + 5 > global::main_dude->y &&
            ropeChain.at(a)->y - 5 < global::main_dude->y + MAIN_DUDE_PHYSICAL_HEIGHT &&
            ropeChain.at(a)->active
            && !onTopOfClimbingSpace) {
            if (!global::input_handler->r_bumper_down) {
                global::main_dude->can_climb_rope = true;
                if (global::input_handler->up_key_held) {
                    global::main_dude->x = ropeChain.at(a)->x - 4;
                }
            }

            break;
        }
    }

    if (!finished) {
        kill_mobs_if_thrown(1);
    }

    set_position();
}


void Rope::init() {
    initSprite();
    notThrown();
}

void Rope::notThrown() {
    frameGfx = sprite_utils::get_frame((u8 *) gfx_blood_rock_rope_poofTiles, ROPE_SPRITE_SIZE, 9);
    sprite_utils::update_frame(frameGfx, ROPE_SPRITE_SIZE, mainSpriteInfo, subSpriteInfo);
}

void Rope::throwingFinished() {
    frameGfx = sprite_utils::get_frame((u8 *) gfx_blood_rock_rope_poofTiles, ROPE_SPRITE_SIZE, 10);
    sprite_utils::update_frame(frameGfx, ROPE_SPRITE_SIZE, mainSpriteInfo, subSpriteInfo);
}

void Rope::updateSpeed() {

    limit_speed(MAX_X_SPEED_ROPE, MAX_Y_SPEED_ROPE);

    pos_inc_timer += *global::timer;

    bool change_pos = (pos_inc_timer > ROPE_POS_INC_DELTA) && !hold_by_main_dude && !finished;

    if (change_pos) {
        update_position();
        apply_friction(0.055);
        pos_inc_timer = 0;
    }

}

void Rope::updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) {

    MapTile *t[9];
    Collisions::getNeighboringTiles(global::current_level->map_tiles,
                                    x_current_pos_in_tiles, y_current_pos_in_tiles, t);

    bottomCollision = Collisions::checkBottomCollision(t, &x, &y, &ySpeed, physical_width, physical_height, true, 0.35);
    leftCollision = Collisions::checkLeftCollision(t, &x, &y, &xSpeed, physical_width, physical_height, true, 0.15);
    rightCollision = Collisions::checkRightCollision(t, &x, &y, &xSpeed, physical_width, physical_height, true, 0.15);
    upperCollision = Collisions::checkUpperCollision(t, &x, &y, &ySpeed, physical_width, false, 0);

    if (upperCollision) {
        if (!finished) {
            throwingFinished();
            finished = true;

            int temp_y = floor_div(this->y + (0.5 * physical_height), TILE_H);

            auto element = new RopeElement();
            element->init();
            element->x = x;
            element->y = temp_y * TILE_H;
            element->draw();

            ropeChain.push_back(element);
        }
    }

}

bool Rope::isThereChainForThisTile(int rope_y) {
    for (auto &a : ropeChain) {
        if (a->y == rope_y)
            return true;
    }
    return false;
}

void Rope::initSprite() {

    delete mainSpriteInfo;
    delete subSpriteInfo;

    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_blood_rock_rope_poofPal, gfx_blood_rock_rope_poofPalLen,
                                                        nullptr, ROPE_SPRITE_SIZE, 8, BLOOD_ROCK_ROPE_POOF, true,
                                                        false, LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_blood_rock_rope_poofPal, gfx_blood_rock_rope_poofPalLen,
                                                          nullptr, ROPE_SPRITE_SIZE, 8, BLOOD_ROCK_ROPE_POOF, true,
                                                          false, LAYER_LEVEL::MIDDLE_TOP);

    sprite_utils::set_visibility(true, mainSpriteInfo, subSpriteInfo);

    notThrown();

    for (auto &a : ropeChain)
        a->initSprite();
}

Rope::Rope() {
    physical_height = ROPE_PHYSICAL_HEIGHT;
    physical_width = ROPE_PHYSICAL_WIDTH;
    sprite_height = ROPE_SPRITE_HEIGHT;
    sprite_width = ROPE_SPRITE_WIDTH;
}

void Rope::add_rope_if_needed() {

    int temp_y = floor_div(this->y + (0.5 * physical_height), TILE_H);

    if (!isThereChainForThisTile(temp_y * TILE_H)) {

        if (temp_y * TILE_H > y) {

            auto *element = new RopeElement();
            element->init();

            element->x = x;
            element->y = temp_y * TILE_H;

            element->draw();

            ropeChain.push_back(element);

            if ((extended_rope && ropeChain.size() == MAX_ROPE_CHAIN_SIZE_EXTENDED) ||
                (!extended_rope && ropeChain.size() == MAX_ROPE_CHAIN_SIZE)) {
                throwingFinished();
                finished = true;
            }

        }
    }
}

void Rope::set_position() {
    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);
    sprite_utils::set_entry_xy(mainSpriteInfo, main_x, main_y);
    sprite_utils::set_entry_xy(subSpriteInfo, sub_x, sub_y);
}

