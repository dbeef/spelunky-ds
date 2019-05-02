//
// Created by xdbeef on 27.05.18.
//


#include <cstdio>
#include "GameState.hpp"
#include "GameLoop.hpp"
#include "tiles/LevelRenderingUtils.hpp"
#include "tiles/SplashScreenType.hpp"
#include "../build/soundbank.h"
#include "entities/creatures/Damsel.hpp"
#include "entities/decorations/Smooch.hpp"
#include "sound/Sound.hpp"
#include "tiles/BaseMap.hpp"
#include "tiles/LevelGenerator.hpp"
#include "tiles/PopulatingUtils.hpp"
#include "entities/singletons/MainDudeConsts.h"
#include "time/Timer.h"
#include "graphics/OamUtils.hpp"

GameState *GameState::_instance = nullptr;

GameState::GameState() {

    current_level = new Level();
    current_level->init_map_tiles();
    temp_map = new u16[4096];

    SPELUNKYDS_BREAKING_ASSERT(current_level);
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

    MainDude::instance().reset_state();
    Hud::instance().hearts = 4;
    Hud::instance().ropes = 4;
    Hud::instance().bombs = 4;
    Hud::instance().money = 0;
    Hud::instance().items_offset_y = 7;
    Hud::instance().game_over_timer = 0;
    Hud::instance().disable_all_prompts();
    Hud::instance().set_hud_sprites_attributes();
    Hud::instance().draw_level_hud();
}

void GameState::start_main_menu() {
    MainDude::instance().climbing = false;
    Camera::instance().follow_main_dude = false;
    in_main_menu = true;
    levels_transition_screen = false;
    scores_screen = false;
    populate_main_menu();
}

void GameState::start_scores() {

    MainDude::instance().reset_state();

    scores_screen = true;
    Hud::instance().draw_scores();
    Camera::instance().follow_main_dude = false;
}

void GameState::start_level_transition_screen() {
    Hud::instance().total_time_spent += Hud::instance().time_spent_on_level;
    Hud::instance().level++;

    levels_transition_screen = true;
    splash_screen = true;

    InputHandler::instance().stop_handling = true;
    InputHandler::instance().l_bumper_held = true;
    InputHandler::instance().right_key_held = true;

    Camera::instance().follow_main_dude = false;
    consoleClear();
    Hud::instance().draw_on_level_done();


    if (damsels_rescued_this_level > 0) {
        Hud::instance().hearts += damsels_rescued_this_level;
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

    Hud::instance().items_offset_y = 7;
    MainDude::instance().holding_item = false;
    MainDude::instance().spawn_carried_items();
    Hud::instance().init_sprites();
    populate_cave_npcs();
    populate_cave_moniez();
    levels_transition_screen = false;
    in_main_menu = false;
    killed_npcs.clear();
    collected_treasures.clear();
    Hud::instance().money_on_this_level = 0;
    Hud::instance().draw_level_hud();

}

void GameState::handle_changing_screens() {

    if ((MainDude::instance().dead && InputHandler::instance().y_key_down) ||
        (MainDude::instance().animFrame >= 16 && !splash_screen)) {

        MainDude::instance().animFrame = 0;

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
            MainDude::instance().set_position_to(MapTileType::ENTRANCE);


        } else {

            current_level->clean_map_layout();

            //flushing the buffer so it wouldn't update in the scores/main menu/transition screen if != 0
            Hud::instance().money_on_this_level += Hud::instance().dollars_buffer;
            Hud::instance().dollars_buffer = 0;

            //splash screen; scores or level transition

            if (scores_screen) {

                robbed_killed_shopkeeper = false;

                sound::stop_cave_music();
                sound::start_menu_music();

                current_level->initialise_tiles_from_splash_screen(SplashScreenType::MAIN_MENU_UPPER);
                current_level->initialise_tiles_from_splash_screen(SplashScreenType::MAIN_MENU_LOWER);
//                set_position_to(MapTileType::ENTRANCE);
                MainDude::instance()._x = 113;
                MainDude::instance()._y = 288;
                Camera::instance().follow_main_dude = true;
                Camera::instance().instant_focus();

            } else if (MainDude::instance().dead) {
                current_level->initialise_tiles_from_splash_screen(SplashScreenType::SCORES_UPPER);
                current_level->initialise_tiles_from_splash_screen(SplashScreenType::SCORES_LOWER);
                MainDude::instance().set_position_to(MapTileType::EXIT);
            } else {

                current_level->initialise_tiles_from_splash_screen(SplashScreenType::ON_LEVEL_DONE_UPPER);
                current_level->initialise_tiles_from_splash_screen(SplashScreenType::ON_LEVEL_DONE_LOWER);
                MainDude::instance().set_position_to(MapTileType::ENTRANCE);
            }

        }

        current_level->write_tiles_to_map();

        sectorize_map();

        // copy current map to rendering engines
        dmaCopyHalfWords(DEFAULT_DMA_CHANNEL, current_map, bgGetMapPtr(bg_main_address),
                         sizeof(current_map));
        dmaCopyHalfWords(DEFAULT_DMA_CHANNEL, current_map, bgGetMapPtr(bg_sub_address),
                         sizeof(current_map));

        bool dead = MainDude::instance().dead;
        int temp_x = MainDude::instance()._x;
        int temp_y = MainDude::instance()._y;

        // changing scene, so delete all MovingObjects you have, and according SpriteInfos
        oam_utils::delete_all_sprites();

        // init sprites since they've been disposed
        MainDude::instance().init_sprites();
        Whip::instance().init_sprites();

        MainDude::instance().dead = dead; //TODO Move this fields out to the game_state! or do the thing above, same result

        MainDude::instance()._x = temp_x;
        MainDude::instance()._y = temp_y;

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
            else if (MainDude::instance().dead) {
                start_scores();
            } else
                start_level_transition_screen();
        }

        MainDude::instance().exiting_level = false;


    } else if (MainDude::instance().animFrame >= 16 && splash_screen) {

        MainDude::instance().main_sprite_info->entry->isHidden = true;
        MainDude::instance().sub_sprite_info->entry->isHidden = true;
        InputHandler::instance().stop_handling = false;

        if (InputHandler::instance().y_key_down) {
            splash_screen = false;
        }

    }
}

/**
 * TODO Move this to the Damsel class.
 */
void GameState::handle_transition_screen_smooch() {
    if (smooching) {
        if (144 - MainDude::instance()._x <= 16) {
            smooch_timer += Timer::getDeltaTime();
            InputHandler::instance().right_key_held = false;
            if (!spawned_smooch) {
                sound::kiss();
                spawned_smooch = true;
                auto *s = new Smooch(144, 436);
                decorations.push_back(s);
            }
        }
        //144
        //448
    }
}

GameState::~GameState() {
    delete current_level;
    delete temp_map;
}

void GameState::init() {
    SPELUNKYDS_BREAKING_ASSERT(!_instance);
    _instance = new GameState();
    SPELUNKYDS_BREAKING_ASSERT(_instance);
}

void GameState::dispose() {
    SPELUNKYDS_BREAKING_ASSERT(_instance);
    delete _instance;
}
