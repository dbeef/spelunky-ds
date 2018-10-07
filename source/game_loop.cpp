#include <nds.h>
#include <cstdlib>
#include "game_loop.hpp"
#include "tiles/populating_utils.hpp"
#include "time/time_utils.h"
#include "memory/oam_utils.hpp"
#include "sprites/animations/blood.hpp"
#include "sprites/items/arrow.hpp"

void gameloop::run() {

    global::main_dude->init();
    //init positions with main menu values:
    global::camera->x = 0;
    global::camera->y = 127;
    global::main_dude->x = 224;
    global::main_dude->y = 300; //TODO Some constexpr file for these
//    global::sprites.push_back(global::main_dude);

    global::hud->init();
    populate_main_menu(); //TODO Game state function for matching this?

    //the game loop. If exited, SpelunkyDS returns 0 and gets back to the menu
    while (true) {

        time_utils::update_ms_since_last_frame();
        global::input_handler->updateInput();

        if (global::game_state->bombed) { //TODO Can't this be done in the Bomb class?
            global::current_level->update_level();
            global::game_state->bombed = false;
            for (int a = 0; a < global::sprites.size(); a++)
                (*global::sprites.at(a)).bottomCollision = false;
            global::main_dude->bottomCollision = false;
        }

        global::camera->update();

        unsigned long size = global::sprites_to_add.size();

        for (int a = 0; a < size; a++) {
            if (global::sprites_to_add.at(a)) {
                MovingObject *o = global::sprites_to_add.at(a);
                global::sprites.push_back(o);
            }
        }

        if (size)
            global::sprites_to_add.clear();

        global::main_dude->update();
        global::main_dude->draw();
        global::main_dude->whip->update();
        global::main_dude->whip->draw();

        for (int a = 0; a < global::sprites.size(); a++) {
            //TODO Debugging flag
//            if (size)
//                (*global::sprites.at(a)).introduce_yourself();

            //TODO Add checking if ready to dispose here, and remove checking in MovingObject subclasses
            if (global::sprites.at(a)
                /*&& !global::sprites.at(a)->ready_to_dispose *//*&& !global::sprites.at(a)->killed*/) {
//                printf("START ");
//                (*global::sprites.at(a)).introduce_yourself();
                (*global::sprites.at(a)).update();
                (*global::sprites.at(a)).draw();
            }

        }

//        printf("DONE ALL\n");

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
