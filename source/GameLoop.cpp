#include <nds.h>
#include <cstdlib>
#include <nds/arm9/exceptions.h>
#include "GameLoop.hpp"
#include "tiles/PopulatingUtils.hpp"
#include "time/TimeUtils.h"
#include "memory/OamUtils.hpp"
#include "entities/animations/Blood.hpp"
#include "entities/items/Arrow.hpp"
#include "entities/main_dude/MainDudeConsts.h"

void gameloop::run() {

    global::camera->x = CAMERA_MENU_START_X;
    global::camera->y = CAMERA_MENU_START_Y;
    global::main_dude = new MainDude(MAIN_DUDE_MENU_START_POSITION_X, MAIN_DUDE_MENU_START_POSITION_Y);

    global::hud->init();
    populate_main_menu();

    // If exited, SpelunkyDS returns 0 and gets back to the menu.
    while (true) {

        time_utils::update_ms_since_last_frame();
        global::input_handler->updateInput();

        if (global::game_state->bombed) {
            on_bomb_explosion();
            global::game_state->bombed = false;
        }

        global::camera->update();

        global::main_dude->update();
        global::main_dude->whip->update();

        update_entities();

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

void gameloop::on_bomb_explosion() {
    global::current_level->update_level();

    for (auto &creature : global::creatures)
        (*creature)._bottom_collision = false;
    for (auto &item : global::items)
        (*item)._bottom_collision = false;
    for (auto &treasures : global::treasures)
        (*treasures)._bottom_collision = false;
    global::main_dude->_bottom_collision = false;
}

void gameloop::update_entities() {

    // Iterating using indexes since entities may be
    // pushed back while iterating, invalidating iterators.

    {
        const std::size_t size = global::creatures.size();
        for (std::size_t index = 0; index < size; ++index) {
            auto *&creature = global::creatures.at(index);
            if (creature && !creature->_ready_to_dispose) {
                creature->update();
            }
        }
    }
    {
        const std::size_t size = global::items.size();
        for (std::size_t index = 0; index < size; ++index) {
            auto *&item = global::items.at(index);
            if (item && !item->_ready_to_dispose) {
                item->update();
            }
        }
    }
    {
        const std::size_t size = global::decorations.size();
        for (std::size_t index = 0; index < size; ++index) {
            auto *&decoration = global::decorations.at(index);
            if (decoration) {
                decoration->update();
            }
        }
    }
    {
        const std::size_t size = global::treasures.size();
        for (std::size_t index = 0; index < size; ++index) {
            auto *&treasure = global::treasures.at(index);
            if (treasure && !treasure->_ready_to_dispose) {
                treasure->update();
            }
        }
    }
}

