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

    //some of the moving objects aren't rendered as entities, like the ArrowTrap, which is render as a map tile,
    //so the the list is bigger, though the maximum number of entities that can be rendered on NDS is 128
    creatures.reserve(64);
    //some of the entities in this list may just wait to be disposed, that's why this size is slightly bigger that 128
    sprite_infos.reserve(128);
    //I assume there won't be a situation when we'll add more than 64 objects in a single frame

    decorations.reserve(64);

    treasures.reserve(8);

    temp_map = new u16[4096];
}

void GameState::reset_main_dude() {
    main_dude->carrying_spring_shoes = false;
    main_dude->carrying_compass = false;
    main_dude->carrying_glove = false;
    main_dude->carrying_mitt = false;
    main_dude->carrying_spike_shoes = false;
    main_dude->carrying_cape = false;
    main_dude->carrying_shotgun = false;
    main_dude->carrying_pistol = false;
    main_dude->carrying_jetpack = false;
    main_dude->dead = false;
    main_dude->stunned = false;
    main_dude->hanging_on_tile_right = false;
    main_dude->hanging_on_tile_left = false;
}

void GameState::start_new_game() {

    reset_main_dude();
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
    GameState::instance().in_main_menu = true;
    GameState::instance().levels_transition_screen = false;
    GameState::instance().scores_screen = false;
    populate_main_menu();
}

void GameState::start_scores() {

    reset_main_dude();

    GameState::instance().scores_screen = true;
    hud->draw_scores();
    camera->follow_main_dude = false;
}

void GameState::start_level_transition_screen() {
    hud->total_time_spent += hud->time_spent_on_level;
    hud->level++;

    GameState::instance().levels_transition_screen = true;
    GameState::instance().splash_screen = true;

    input_handler->stop_handling = true;
    input_handler->l_bumper_held = true;
    input_handler->right_key_held = true;

    camera->follow_main_dude = false;
    consoleClear();
    hud->draw_on_level_done();


    if (GameState::instance().damsels_rescued_this_level > 0) {
        hud->hearts += GameState::instance().damsels_rescued_this_level;
        GameState::instance().damsels_rescued_this_level = 0;
        GameState::instance().smooching = true;

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
    GameState::instance().levels_transition_screen = false;
    GameState::instance().in_main_menu = false;
    killed_npcs.clear();
    collected_treasures.clear();
    hud->money_on_this_level = 0;
    hud->draw_level_hud();

}

void GameState::handle_changing_screens() {

    if ((main_dude->dead && input_handler->y_key_down) ||
        (main_dude->animFrame >= 16 && !GameState::instance().splash_screen)) {

        main_dude->animFrame = 0;

        // copy base cave background to current map array
        dmaCopyHalfWords(DEFAULT_DMA_CHANNEL, base_map, current_map, sizeof(base_map));

        generate_new_level_layout();

        if (GameState::instance().in_main_menu || GameState::instance().levels_transition_screen) {

            //next level or starting game

            if (GameState::instance().in_main_menu) {
                sound::stop_menu_music();
                sound::start_cave_music();
            } else
                sound::stop_cave_music();


            current_level->generate_frame();
            current_level->initialise_tiles_from_room_layout();
            set_position_to(MapTileType::ENTRANCE);


        } else {

            current_level->clean_map_layout();

            //flushing the buffer so it wouldn't update in the scores/main menu/transition screen if != 0
            hud->money_on_this_level += hud->dollars_buffer;
            hud->dollars_buffer = 0;

            //splash screen; scores or level transition

            if (GameState::instance().scores_screen) {

                GameState::instance().robbed_killed_shopkeeper = false;

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
                set_position_to(MapTileType::EXIT);
            } else {

                current_level->initialise_tiles_from_splash_screen(SplashScreenType::ON_LEVEL_DONE_UPPER);
                current_level->initialise_tiles_from_splash_screen(SplashScreenType::ON_LEVEL_DONE_LOWER);
                set_position_to(MapTileType::ENTRANCE);
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

        if (GameState::instance().in_main_menu || GameState::instance().levels_transition_screen) {

            if (GameState::instance().in_main_menu) {
                GameState::instance().start_new_game();
            } else {
                sound::start_cave_music();
            }

            GameState::instance().start_next_level(); //initializes hud entities

        } else {

            if (GameState::instance().scores_screen)
                GameState::instance().start_main_menu();
            else if (main_dude->dead) {
                GameState::instance().start_scores();
            } else
                GameState::instance().start_level_transition_screen();
        }

        main_dude->exiting_level = false;


    } else if (main_dude->animFrame >= 16 && GameState::instance().splash_screen) {

        main_dude->main_sprite_info->entry->isHidden = true;
        main_dude->sub_sprite_info->entry->isHidden = true;
        input_handler->stop_handling = false;

        if (input_handler->y_key_down) {
            GameState::instance().splash_screen = false;
        }

    }
}

/**
 * Sets main dude's position to the first tile of given type that occurs during iteration through the map
 */
void GameState::set_position_to(MapTileType t) {

    MapTile *entrance;
    current_level->get_first_tile_of_given_type(t, entrance);

    if (entrance != nullptr && entrance->exists) {
        main_dude->_x = entrance->x * 16;
        main_dude->_y = entrance->y * 16;
    } else {
        main_dude->_x = 0;
        main_dude->_y = 144;
    }

    camera->follow_main_dude = true;
    camera->instant_focus();

}

/**
 * TODO Move this to the Damsel class.
 */
void GameState::handle_transition_screen_smooch() {
    if (smooching) {
        if (144 - main_dude->_x <= 16) {
            smooch_timer += *timer;
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