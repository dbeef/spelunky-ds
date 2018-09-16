#include <nds.h>
#include <cstdlib>
#include "game_loop.hpp"
#include "tiles/populating_utils.hpp"
#include "time/time_utils.h"
#include "memory/oam_utils.hpp"

constexpr int BOUNDARY_VALUE = 64; /* This is the default boundary value (can be set in REG_DISPCNT) */
constexpr int OFFSET_MULTIPLIER_MAIN = BOUNDARY_VALUE / sizeof(SPRITE_GFX[0]);
constexpr int OFFSET_MULTIPLIER_SUB = BOUNDARY_VALUE / sizeof(SPRITE_GFX_SUB[0]);

void gameloop::scroll() {

    global::main_oam_manager->initOAMTable(SPRITE_GFX, SPRITE_PALETTE, OAM, OFFSET_MULTIPLIER_MAIN, OamType::MAIN);
    global::sub_oam_manager->initOAMTable(SPRITE_GFX_SUB, SPRITE_PALETTE_SUB, OAM_SUB,
            OFFSET_MULTIPLIER_SUB, OamType::SUB);
    global::main_dude->init();
    global::camera->x = 0;
    global::camera->y = 127;
    global::main_dude->x = 224;
    global::main_dude->y = 300;
    global::sprites.push_back(global::main_dude);

    global::hud->init();

    populate_main_menu();

    while (true) {

        time_utils::update_ms_since_last_frame();

        global::input_handler->updateInput();

        if (global::game_state->bombed) {
            global::current_level->update_level();
            global::game_state->bombed = false;
            for (int a = 0; a < global::sprites.size(); a++)
                (*global::sprites.at(a)).bottomCollision = false;
        }

        global::camera->update();

        unsigned long size = global::sprites_to_add.size();

        for (int a = 0; a < size; a++) {
            if (global::sprites_to_add.at(a)) {
                MovingObject *o = global::sprites_to_add.at(a);
                global::sprites.push_back(o);
            }
        }


        for (int a = 0; a < global::sprites.size(); a++) {

//            if (size)
//                (*global::sprites.at(a)).introduce_yourself();

            //TODO Add checking if ready to dispose here, and remove checking in MovingObject subclasses
            if (global::sprites.at(a) /*&& !global::sprites.at(a)->ready_to_dispose *//*&& !global::sprites.at(a)->killed*/) {
//                printf("START ");
//                (*global::sprites.at(a)).introduce_yourself();
                (*global::sprites.at(a)).update();
                (*global::sprites.at(a)).draw();
            }

        }
//        printf("DONE ALL\n");

        if (size)
            global::sprites_to_add.clear();

        global::game_state->handle_transition_screen_smooch();

        global::main_dude->handle_key_input();
        global::hud->update();

        swiWaitForVBlank();

        //-> this should be done after Vblank (or crash!)
        if (global::game_state->just_started_game) {
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

            global::game_state->change_brightness_timer += *global::timer;

            if (global::game_state->change_brightness_timer > 100) {

                global::game_state->brightness_level++;

                if (global::game_state->brightness_level > 16)
                    exit(0);

                setBrightness(3, global::game_state->brightness_level);

            }
        }

        global::camera->write_current_position_to_graphics_engines();
        global::main_oam_manager->updateOAM();
        global::sub_oam_manager->updateOAM();
        oam_utils::clean_unused_oam();
    }
}
