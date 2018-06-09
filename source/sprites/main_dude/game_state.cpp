#include <maxmod9.h>
#include "game_state.h"
#include "../../globals_declarations.h"
#include "../../game_loop.h"
#include "../../tiles/map_utils.h"
#include "../../tiles/splash_screen_type.h"
#include "../../memory/oam_utils.h"
#include "../../../build/soundbank.h"

//
// Created by xdbeef on 27.05.18.
//
void GameState::start_new_game() {
    global::main_dude->carrying_spring_shoes = false;
    global::main_dude->carrying_compass = false;
    global::main_dude->carrying_glove = false;
    global::main_dude->carrying_mitt = false;
    global::main_dude->carrying_spike_shoes = false;
    global::main_dude->carrying_cape = false;
    global::main_dude->carrying_shotgun = false;
    global::main_dude->carrying_pistol = false;
    global::main_dude->carrying_jetpack= false;

    global::hud->hearts = 4;
    global::hud->ropes = 4;
    global::hud->bombs = 4;
    global::hud->dollars = 0;
    global::hud->items_offset_y = 7;
}

void GameState::start_main_menu() {

    global::camera->follow_main_dude = false;
    global::game_state->in_main_menu = true;
    global::game_state->levels_transition_screen = false;
    global::game_state->scores_screen = false;
    gameloop::populate_main_menu();

}

void GameState::start_scores() {
    global::main_dude->dead = false;
    global::game_state->scores_screen = true;
    global::hud->hide();
    global::hud->draw_scores();
    global::camera->follow_main_dude = false;
}

void GameState::start_level_transition_screen() {
    global::hud->total_time_spent += global::hud->time_spent_on_level;
    global::hud->level++;

    global::game_state->levels_transition_screen = true;

    global::game_state->splash_screen = true;

    global::input_handler->stop_handling = true;
    global::input_handler->l_bumper_held = true;
    global::input_handler->right_key_held = true;
    global::camera->follow_main_dude = false;
    global::hud->draw_on_level_done();

}

void GameState::start_next_level() {

    global::hud->items_offset_y = 7;

    global::main_dude->holding_item = false;

    global::main_dude->spawn_carried_items();

    global::hud->init();
    gameloop::populate_cave_npcs();
    gameloop::populate_cave_moniez();
    global::game_state->levels_transition_screen = false;
    global::game_state->in_main_menu = false;
    global::killed_npcs.clear();
    global::collected_loot.clear();
    global::hud->money_on_this_level = 0;

}

void GameState::handle_changing_screens() {

    if ((global::main_dude->dead && global::input_handler->y_key_down) ||
        (global::main_dude->animFrame >= 16 && !global::game_state->splash_screen)) {

        global::main_dude->animFrame = 0;
        dmaCopyHalfWords(DMA_CHANNEL, global::base_map, global::current_map, sizeof(global::current_map));

        global::level_generator->newLayout(*global::timer);

        if (global::game_state->in_main_menu || global::game_state->levels_transition_screen) {

            //next level or starting game

            mmEffectCancel(global::menu_music_handler);
            mmEffectCancel(global::cave_music_handler);
            //global::cave_music_handler = mmEffect(SFX_MCAVE);

            global::level_generator->generate_frame();
            global::level_generator->generate_rooms();
            set_position_to(MapTileType::ENTRANCE);

        } else {

            //splash screen; scores or level transition

            if (global::game_state->scores_screen) {

                mmEffectCancel(global::cave_music_handler);
                //global::menu_music_handler = mmEffect(SFX_MTITLE);
                global::level_generator->generate_splash_screen(SplashScreenType::MAIN_MENU_UPPER);
                global::level_generator->generate_splash_screen(SplashScreenType::MAIN_MENU_LOWER);
//                set_position_to(MapTileType::ENTRANCE);
                global::main_dude->x = 113;
                global::main_dude->y = 288;
                global::camera->follow_main_dude = true;
                global::camera->instant_focus();
            } else if (global::main_dude->dead) {
                global::level_generator->generate_splash_screen(SplashScreenType::SCORES_UPPER);
                global::level_generator->generate_splash_screen(SplashScreenType::SCORES_LOWER);
                set_position_to(MapTileType::EXIT);
            } else {
                global::level_generator->generate_splash_screen(SplashScreenType::ON_LEVEL_DONE_UPPER);
                global::level_generator->generate_splash_screen(SplashScreenType::ON_LEVEL_DONE_LOWER);
                set_position_to(MapTileType::ENTRANCE);
//                global::main_dude->x = 32;
//                global::main_dude->y = 280;
//                global::camera->follow_main_dude = true;
//                global::camera->instant_focus();
            }

        }

        //
        global::level_generator->tiles_to_map();
        sectorize_map();
        dmaCopyHalfWords(DMA_CHANNEL, global::current_map, bgGetMapPtr(global::bg_main_address),
                         sizeof(global::current_map));
        dmaCopyHalfWords(DMA_CHANNEL, global::current_map, bgGetMapPtr(global::bg_sub_address),
                         sizeof(global::current_map));
        //

        oam_utils::delete_all_sprites();

        global::main_dude->init();
        global::sprites.push_back(global::main_dude);

        consoleClear();

        if (global::game_state->in_main_menu || global::game_state->levels_transition_screen) {

            if (global::game_state->in_main_menu) {
                global::game_state->start_new_game();
            }

            global::game_state->start_next_level();

        } else {

            if (global::game_state->scores_screen)
                global::game_state->start_main_menu();
            else if (global::main_dude->dead)
                global::game_state->start_scores();
            else
                global::game_state->start_level_transition_screen();
        }

        global::main_dude->exiting_level = false;
        //mmEffectCancel(SFX_MCAVE);

    } else if (global::main_dude->animFrame >= 16 && global::game_state->splash_screen) {

        global::main_dude->main_spelunker->entry->isHidden = true;
        global::main_dude->sub_spelunker->entry->isHidden = true;
        global::input_handler->stop_handling = false;

        if (global::input_handler->y_key_down) {
            global::game_state->splash_screen = false;
        }

    }
}

void GameState::set_position_to(MapTileType t) {

    MapTile *entrance;
    global::level_generator->get_first_tile(t, entrance);

    if (entrance != nullptr) {
        global::main_dude->x = entrance->x * 16;
        global::main_dude->y = entrance->y * 16;
    } else {
        global::main_dude->x = 113;
        global::main_dude->y = 288;
    }

    global::camera->follow_main_dude = true;
    global::camera->instant_focus();

}