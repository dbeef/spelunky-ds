//
// Created by xdbeef on 27.05.18.
//

#include <maxmod9.h>
#include <cstdio>
#include "GameState.hpp"
#include "GameLoop.hpp"
#include "tiles/LevelRenderingUtils.hpp"
#include "tiles/SplashScreenType.hpp"
#include "memory/OamUtils.hpp"
#include "../build/soundbank.h"
#include "entities/creatures/Damsel.hpp"
#include "entities/decorations/Smooch.hpp"
#include "sound/Sound.hpp"
#include "tiles/BaseMap.hpp"
#include "tiles/LevelGenerator.hpp"
#include "tiles/PopulatingUtils.hpp"
#include "entities/main_dude/MainDudeConsts.h"
#include "time/Timer.h"

GameState *GameState::_instance = nullptr;

GameState::GameState() {

    input_handler = new InputHandler();
    camera = new Camera();
    main_dude = nullptr;
    current_level = new Level();
    current_level->init_map_tiles();
    main_oam_manager = new OAMManager();
    sub_oam_manager = new OAMManager();
    hud = new Hud();
    temp_map = new u16[4096];

    SPELUNKYDS_BREAKING_ASSERT(input_handler);
    SPELUNKYDS_BREAKING_ASSERT(camera);
    SPELUNKYDS_BREAKING_ASSERT(main_dude);
    SPELUNKYDS_BREAKING_ASSERT(current_level);
    SPELUNKYDS_BREAKING_ASSERT(main_oam_manager);
    SPELUNKYDS_BREAKING_ASSERT(sub_oam_manager);
    SPELUNKYDS_BREAKING_ASSERT(hud);
    SPELUNKYDS_BREAKING_ASSERT(temp_map);

    // some of the moving objects aren't rendered as entities, like the ArrowTrap, which is render as a map tile,
    // though the maximum number of entities that can be rendered on NDS is 128
    creatures.reserve(64);
    sprite_infos.reserve(128);
    decorations.reserve(64);
    treasures.reserve(8);

    // set up logics
    in_main_menu = true;
    just_started_game = true;
}

void GameState::start_new_game() {

    main_dude->reset_state();
    hud->hearts = 4;
    hud->ropes = 4;
    hud->bombs = 4;
    hud->money = 0;
    hud->items_offset_y = 7;
    hud->game_over_timer = 0;
    hud->disable_all_prompts();
    hud->set_hud_sprites_attributes();
    hud->draw_level_hud();
}

void GameState::start_main_menu() {
    main_dude->climbing = false;
    camera->follow_main_dude = false;
    in_main_menu = true;
    levels_transition_screen = false;
    scores_screen = false;
    populate_main_menu();
}

void GameState::start_scores() {

    main_dude->reset_state();

    scores_screen = true;
    hud->draw_scores();
    camera->follow_main_dude = false;
}

void GameState::start_level_transition_screen() {
    hud->total_time_spent += hud->time_spent_on_level;
    hud->level++;

    levels_transition_screen = true;
    splash_screen = true;

    input_handler->stop_handling = true;
    input_handler->l_bumper_held = true;
    input_handler->right_key_held = true;

    camera->follow_main_dude = false;
    consoleClear();
    hud->draw_on_level_done();


    if (damsels_rescued_this_level > 0) {
        hud->hearts += damsels_rescued_this_level;
        damsels_rescued_this_level = 0;
        smooching = true;

        Damsel *damsel = new Damsel(0, 0);
        damsel->call_for_help = false;
        creatures.push_back(damsel);

        MapTile *entrance;
        current_level->get_first_tile_of_given_type(MapTileType::ENTRANCE, entrance);
        if (entrance != nullptr) {
            damsel->_x = (entrance->x * 16) + 7 * 16;
            damsel->_y = entrance->y * 16;
            //144
            //448
        }

    }

}

void GameState::start_next_level() {

    hud->items_offset_y = 7;
    main_dude->holding_item = false;
    main_dude->spawn_carried_items();
    hud->init_sprites();
    populate_cave_npcs();
    populate_cave_moniez();
    levels_transition_screen = false;
    in_main_menu = false;
    killed_npcs.clear();
    collected_treasures.clear();
    hud->money_on_this_level = 0;
    hud->draw_level_hud();

}

void GameState::handle_changing_screens() {

    if ((main_dude->dead && input_handler->y_key_down) ||
        (main_dude->animFrame >= 16 && !splash_screen)) {

        main_dude->animFrame = 0;

        // copy base cave background to current map array
        dmaCopyHalfWords(DEFAULT_DMA_CHANNEL, base_map, current_map, sizeof(base_map));

        generate_new_level_layout();

        if (in_main_menu || levels_transition_screen) {

            //next level or starting game

            if (in_main_menu) {
                sound::stop_menu_music();
                sound::start_cave_music();
            } else
                sound::stop_cave_music();


            current_level->generate_frame();
            current_level->initialise_tiles_from_room_layout();
            main_dude->set_position_to(MapTileType::ENTRANCE);


        } else {

            current_level->clean_map_layout();

            //flushing the buffer so it wouldn't update in the scores/main menu/transition screen if != 0
            hud->money_on_this_level += hud->dollars_buffer;
            hud->dollars_buffer = 0;

            //splash screen; scores or level transition

            if (scores_screen) {

                robbed_killed_shopkeeper = false;

                sound::stop_cave_music();
                sound::start_menu_music();

                current_level->initialise_tiles_from_splash_screen(SplashScreenType::MAIN_MENU_UPPER);
                current_level->initialise_tiles_from_splash_screen(SplashScreenType::MAIN_MENU_LOWER);
//                set_position_to(MapTileType::ENTRANCE);
                main_dude->_x = 113;
                main_dude->_y = 288;
                camera->follow_main_dude = true;
                camera->instant_focus();

            } else if (main_dude->dead) {
                current_level->initialise_tiles_from_splash_screen(SplashScreenType::SCORES_UPPER);
                current_level->initialise_tiles_from_splash_screen(SplashScreenType::SCORES_LOWER);
                main_dude->set_position_to(MapTileType::EXIT);
            } else {

                current_level->initialise_tiles_from_splash_screen(SplashScreenType::ON_LEVEL_DONE_UPPER);
                current_level->initialise_tiles_from_splash_screen(SplashScreenType::ON_LEVEL_DONE_LOWER);
                main_dude->set_position_to(MapTileType::ENTRANCE);
            }

        }

        current_level->write_tiles_to_map();

        sectorize_map();

        // copy current map to rendering engines
        dmaCopyHalfWords(DEFAULT_DMA_CHANNEL, current_map, bgGetMapPtr(bg_main_address),
                         sizeof(current_map));
        dmaCopyHalfWords(DEFAULT_DMA_CHANNEL, current_map, bgGetMapPtr(bg_sub_address),
                         sizeof(current_map));

        bool dead = main_dude->dead;
        int temp_x = main_dude->_x;
        int temp_y = main_dude->_y;

        //changing scene, so delete all MovingObjects you have, and according SpriteInfos
        oam_utils::delete_all_sprites();

        //new MainDude since we deleted it. TODO Don't push main dude to the entities and update it separately
        main_dude = new MainDude(MAIN_DUDE_MENU_START_POSITION_X, MAIN_DUDE_MENU_START_POSITION_Y);

        main_dude->dead = dead; //TODO Move this fields out to the game_state! or do the thing above, same result

        main_dude->_x = temp_x;
        main_dude->_y = temp_y;

        consoleClear();

        if (in_main_menu || levels_transition_screen) {

            if (in_main_menu) {
                start_new_game();
            } else {
                sound::start_cave_music();
            }

            start_next_level(); //initializes hud entities

        } else {

            if (scores_screen)
                start_main_menu();
            else if (main_dude->dead) {
                start_scores();
            } else
                start_level_transition_screen();
        }

        main_dude->exiting_level = false;


    } else if (main_dude->animFrame >= 16 && splash_screen) {

        main_dude->main_sprite_info->entry->isHidden = true;
        main_dude->sub_sprite_info->entry->isHidden = true;
        input_handler->stop_handling = false;

        if (input_handler->y_key_down) {
            splash_screen = false;
        }

    }
}

/**
 * TODO Move this to the Damsel class.
 */
void GameState::handle_transition_screen_smooch() {
    if (smooching) {
        if (144 - main_dude->_x <= 16) {
            smooch_timer += Timer::getDeltaTime();
            input_handler->right_key_held = false;
            if (!spawned_smooch) {
                mmEffect(SFX_XKISS);
                spawned_smooch = true;
                auto *s = new Smooch(144, 436);
                decorations.push_back(s);
            }
        }
        //144
        //448
    }
}

//!> this should be done after Vblank (otherwise - crash!)
void GameState::normalize_brightness() {

    if (just_started_game) {
        //just started the game, so lowering the brightness to the normal level.
        //game starts with the maximum brightness, so the transition between DSiMenu++ would look smoother
        change_brightness_timer += Timer::getDeltaTime();

        if (change_brightness_timer > 100) {

            _brightness_level--;

            if (_brightness_level == 0)
                just_started_game = false;
            else
                setBrightness(3, _brightness_level);

        }
    }

    if (in_main_menu && exiting_game) {
        //exiting game, so increasing the brightness to the maximum level so the transition
        //between the game and DSiMenu++ would be smoother
        change_brightness_timer += Timer::getDeltaTime();

        if (change_brightness_timer > 100) {

            _brightness_level++;

            if (_brightness_level > 16)
                exit(0);

            setBrightness(3, _brightness_level);

        }
    }
}

void GameState::set_maximum_brightness() {
    setBrightness(3, GameState::instance()._brightness_level);
    _brightness_level = 16;
}

GameState::~GameState() {
    delete main_dude;
    delete input_handler;
    delete camera;
    delete current_level;
    delete main_oam_manager;
    delete sub_oam_manager;
    delete hud;
    delete print_console;
    delete temp_map;
}