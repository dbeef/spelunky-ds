#include <nds.h>
#include <cstdlib>
#include "GameLoop.hpp"
#include "tiles/PopulatingUtils.hpp"
#include "time/TimeUtils.h"
#include "memory/OamUtils.hpp"
#include "creatures/animations/Blood.hpp"
#include "creatures/items/Arrow.hpp"

static void update_creatures_to_add() {
    unsigned long size = global::creatures_to_add.size();
    for (unsigned long a = 0; a < size; a++) {
        if (global::creatures_to_add.at(a)) {
            BaseCreature *o = global::creatures_to_add.at(a);
            global::creatures.push_back(o);
        }
    }
    if (size)
        global::creatures_to_add.clear();
}

static void update_decorations_to_add() {
    unsigned long size = global::decorations_to_add.size();
    for (unsigned long a = 0; a < size; a++) {
        if (global::decorations_to_add.at(a)) {
            BaseDecoration *o = global::decorations_to_add.at(a);
            global::decorations.push_back(o);
        }
    }
    if (size)
        global::decorations_to_add.clear();
}

static void update_treasures_to_add() {
    unsigned long size = global::treasures_to_add.size();
    for (unsigned long a = 0; a < size; a++) {
        if (global::treasures_to_add.at(a)) {
            BaseTreasure *o = global::treasures_to_add.at(a);
            global::treasures.push_back(o);
        }
    }
    if (size)
        global::treasures_to_add.clear();
}

void gameloop::run() {

    //init positions with main menu values:
    global::camera->x = 0;
    global::camera->y = 127;
//    global::creatures.push_back(global::main_dude);

    global::main_dude = new MainDude(224, 300); //TODO Some constexpr file for xy

    global::hud->init();
    populate_main_menu(); //TODO Game state function for matching this?

    //the game loop. If exited, SpelunkyDS returns 0 and gets back to the menu
    while (true) {

        time_utils::update_ms_since_last_frame();
        global::input_handler->updateInput();

        if (global::game_state->bombed) { //TODO Can't this be done in the Bomb class?
            global::current_level->update_level();
            global::game_state->bombed = false;
            for (unsigned long a = 0; a < global::creatures.size(); a++)
                (*global::creatures.at(a))._bottom_collision = false;
            global::main_dude->_bottom_collision = false;
        }

        global::camera->update();

        update_creatures_to_add();
        update_decorations_to_add();
        update_treasures_to_add();

        global::main_dude->update();
        global::main_dude->whip->update();

        for (auto &creature : global::creatures) {
            if (creature) {
                creature->update();
            }
        }

        for (auto &decoration : global::decorations) {
            if (decoration) {
                decoration->update();
            }
        }

        for (auto &decoration : global::treasures) {
            if (decoration) {
                decoration->update();
            }
        }

        global::game_state->handle_transition_screen_smooch();
        global::main_dude->handle_key_input();
        global::hud->update();

        swiWaitForVBlank();
        manage_brightness();

        global::camera->write_current_position_to_graphics_engines();
        global::main_oam_manager->updateOAM();
        global::sub_oam_manager->updateOAM();
        oam_utils::clean_unused_oam();
    }

}

//!> this should be done after Vblank (otherwise - crash!)
void gameloop::manage_brightness() {

    if (global::game_state->just_started_game) {
        //just started the game, so lowering the brightness to the normal level.
        //game starts with the maximum brightness, so the transition between DSiMenu++ would look smoother
        global::game_state->change_brightness_timer += *global::timer;

        if (global::game_state->change_brightness_timer > 100) {

            global::game_state->brightness_level--;

            if (global::game_state->brightness_level == 0)
                global::game_state->just_started_game = false;
            else
                setBrightness(3, global::game_state->brightness_level);

        }
    }

    if (global::game_state->in_main_menu && global::game_state->exiting_game) {
        //exiting game, so increasing the brightness to the maximum level so the transition
        //between the game and DSiMenu++ would be smoother
        global::game_state->change_brightness_timer += *global::timer;

        if (global::game_state->change_brightness_timer > 100) {

            global::game_state->brightness_level++;

            if (global::game_state->brightness_level > 16)
                exit(0);

            setBrightness(3, global::game_state->brightness_level);

        }
    }

}
