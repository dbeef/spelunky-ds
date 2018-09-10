//
// Created by xdbeef on 04.04.18.
//

#include <maxmod9.h>
#include "../../globals_declarations.h"
#include "../../../build/gfx_blood_rock_rope_poof.h"
#include "rope.h"
#include "../../../build/soundbank.h"
#include "../../collisions/collisions.h"
#include "../../tiles/map_utils.h"

#define ROPE_POS_INC_DELTA 15
#define MAX_ROPE_CHAIN_SIZE 8
#define MAX_ROPE_CHAIN_SIZE_EXTENDED 12

void Rope::draw() {


    if (ready_to_dispose) {
        mainSpriteInfo->entry->isHidden = true;
        subSpriteInfo->entry->isHidden = true;
        return;
    } else {
        mainSpriteInfo->entry->isHidden = false;
        subSpriteInfo->entry->isHidden = false;
    }

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

    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);

    mainSpriteInfo->entry->x = main_x;
    mainSpriteInfo->entry->y = main_y;

    subSpriteInfo->entry->x = sub_x;
    subSpriteInfo->entry->y = sub_y;

}


void Rope::init() {
    initSprite();
    notThrown();
}

void Rope::notThrown() {
    frameGfx = (u8 *) gfx_blood_rock_rope_poofTiles + 9 * sprite_width * sprite_height / 2;
    subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
    mainSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
}

void Rope::throwingFinished() {
    frameGfx = (u8 *) gfx_blood_rock_rope_poofTiles + (10 * sprite_width * sprite_height / 2);
    subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
    mainSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
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

    MapTile *tiles[9];
    Collisions::getNeighboringTiles(global::level_generator->map_tiles, x_current_pos_in_tiles, y_current_pos_in_tiles,
                                    tiles);

    bottomCollision = Collisions::checkBottomCollision(tiles, &x, &y, &ySpeed, physical_width, physical_height, true,
                                                       BOUNCING_FACTOR_Y);
    leftCollision = Collisions::checkLeftCollision(tiles, &x, &y, &xSpeed, physical_width, physical_height, true,
                                                   BOUNCING_FACTOR_X);
    rightCollision = Collisions::checkRightCollision(tiles, &x, &y, &xSpeed, physical_width, physical_height, true,
                                                     BOUNCING_FACTOR_X);
    upperCollision = Collisions::checkUpperCollision(tiles, &x, &y, &ySpeed, physical_width, false, 0);

    if (upperCollision) {
        if (!finished) {
            throwingFinished();
            finished = true;

            int temp_y = floor_div(this->y + (0.5 * physical_height), TILE_H);

            RopeElement *element = new RopeElement();
            element->init();

            element->x = x;
            element->y = temp_y * TILE_H;

            element->draw();

            ropeChain.push_back(element);

        }
    }

}

bool Rope::isThereChainForThisTile(int rope_y) {
    for (int a = 0; a < ropeChain.size(); a++) {
        if (ropeChain.at(a)->y == rope_y)
            return true;
    }
    return false;
}

void Rope::initSprite() {


    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_blood_rock_rope_poofPal, gfx_blood_rock_rope_poofPalLen,
                                                        nullptr, sprite_width * sprite_height, 8, BLOOD_ROCK_ROPE_POOF,
                                                        true,
                                                        false,
                                                        LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_blood_rock_rope_poofPal, gfx_blood_rock_rope_poofPalLen,
                                                          nullptr, sprite_width * sprite_height, 8,
                                                          BLOOD_ROCK_ROPE_POOF,
                                                          true, false,
                                                          LAYER_LEVEL::MIDDLE_TOP);

    mainSpriteInfo->entry->isHidden = false;
    subSpriteInfo->entry->isHidden = false;

    notThrown();

    for (int a = 0; a < ropeChain.size(); a++) {
        ropeChain.at(a)->initSprite();
    }
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

            RopeElement *element = new RopeElement();
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

