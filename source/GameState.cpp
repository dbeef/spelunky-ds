//
// Created by xdbeef on 27.05.18.
//


#include <cstdio>
#include "GameState.hpp"
#include "GameLoop.hpp"
#include "tiles/SplashScreenType.hpp"
#include "../build/soundbank.h"
#include "entities/creatures/Damsel.hpp"
#include "entities/decorations/Smooch.hpp"
#include "sound/Sound.hpp"
#include "tiles/BaseMap.hpp"
#include "tiles/LevelGenerator.hpp"
#include "tiles/PopulatingUtils.hpp"
#include "time/Timer.h"
#include "graphics/OamUtils.hpp"
#include "graphics/Brightness.hpp"
#include "graphics/SpriteUtils.hpp"
#include "tiles/LevelRenderer.hpp"

GameState *GameState::_instance = nullptr;

GameState::GameState() {

    // some of the moving objects aren't rendered as entities, like the ArrowTrap, which is render as a map tile,
    // though the maximum number of entities that can be rendered on NDS is 128
    creatures.reserve(64);
    sprite_infos.reserve(128);
    decorations.reserve(64);
    treasures.reserve(8);

    // set up logics
    _current_scene = Scene::MAIN_MENU;
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
    _current_scene = Scene::MAIN_MENU;
    Camera::instance().detach_from_main_dude();
    populate_main_menu();
}

void GameState::start_scores() {

    _current_scene = Scene::SCORES;

    MainDude::instance().reset_state();
    Hud::instance().draw_scores();
    Camera::instance().detach_from_main_dude();
}

void GameState::start_level_summary() {

    _current_scene = Scene::LEVEL_SUMMARY;

    Hud::instance().total_time_spent += Hud::instance().time_spent_on_level;
    Hud::instance().level++;

    InputHandler::instance().block_user_input();
    InputHandler::instance().keys.l_bumper_held = true;
    InputHandler::instance().keys.right_key_held = true;

    Camera::instance().detach_from_main_dude();
    Hud::instance().clear_console();
    Hud::instance().draw_on_level_done();

    if (damsels_rescued_this_level > 0) {
        Hud::instance().hearts += damsels_rescued_this_level;
        damsels_rescued_this_level = 0;
        smooching = true;

        auto *damsel = new Damsel(0, 0);
        damsel->call_for_help = false;
        creatures.push_back(damsel);

        MapTile *entrance;
        Level::instance().get_first_tile_of_given_type(MapTileType::ENTRANCE, entrance);
        if (entrance != nullptr) {
            damsel->_x = (entrance->x * 16) + 7 * 16;
            damsel->_y = entrance->y * 16;
            //144
            //448
        }

    }

}

void GameState::start_next_level() {

    _current_scene = Scene::LEVEL;

    Hud::instance().items_offset_y = 7;
    MainDude::instance().holding_item = false;
    MainDude::instance().spawn_carried_items();
    Hud::instance().init_sprites();
    populate_cave_npcs();
    populate_cave_moniez();
    killed_npcs.clear();
    collected_treasures.clear();
    Hud::instance().money_on_this_level = 0;
    Hud::instance().draw_level_hud();

    InputHandler::instance().unblock_user_input();
}

void GameState::handle_changing_screens() {

//    if ((MainDude::instance()._current_state->_state == _MainDudeState::DEAD && InputHandler::instance().keys.y_key_down) ||
//        (MainDude::instance().animFrame >= 16)) {
//
//        MainDude::instance().animFrame = 0;
//
//        generate_new_level_layout();
//
//        if (_current_scene == Scene::MAIN_MENU || _current_scene == Scene::LEVEL_SUMMARY) {
//
//            //next level or starting game
//
//            if (_current_scene == Scene::MAIN_MENU) {
//                sound::stop_menu_music();
//                sound::start_cave_music();
//            } else
//                sound::stop_cave_music();
//
//
//            Level::instance().generate_frame();
//            Level::instance().initialise_tiles_from_room_layout();
//            MainDude::instance().set_position_to(MapTileType::ENTRANCE);
//
//
//        } else {
//
//            Level::instance().clean_map_layout();
//
//            //flushing the buffer so it wouldn't update in the scores/main menu/transition screen if != 0
//            Hud::instance().money_on_this_level += Hud::instance().dollars_buffer;
//            Hud::instance().dollars_buffer = 0;
//
//            //splash screen; scores or level transition
//
//            if (_current_scene == Scene::SCORES) {
//
//                robbed_or_killed_shopkeeper = false;
//
//                sound::stop_cave_music();
//                sound::start_menu_music();
//
//                Level::instance().initialise_tiles_from_splash_screen(SplashScreenType::MAIN_MENU_UPPER);
//                Level::instance().initialise_tiles_from_splash_screen(SplashScreenType::MAIN_MENU_LOWER);
//                // FIXME: Should call method below or use constexpr position
////                set_position_to(MapTileType::ENTRANCE);
//                MainDude::instance()._x = 113;
//                MainDude::instance()._y = 288;
//                // FIXME: Shouldn't detach?
//                Camera::instance().follow_main_dude();
//                Camera::instance().instant_focus();
//
//            } else if (MainDude::instance()._current_state->_state == _MainDudeState::DEAD) {
//                Level::instance().initialise_tiles_from_splash_screen(SplashScreenType::SCORES_UPPER);
//                Level::instance().initialise_tiles_from_splash_screen(SplashScreenType::SCORES_LOWER);
//                MainDude::instance().set_position_to(MapTileType::EXIT);
//            } else {
//                Level::instance().initialise_tiles_from_splash_screen(SplashScreenType::ON_LEVEL_DONE_UPPER);
//                Level::instance().initialise_tiles_from_splash_screen(SplashScreenType::ON_LEVEL_DONE_LOWER);
//                MainDude::instance().set_position_to(MapTileType::ENTRANCE);
//            }
//
//        }
//
//        int temp_x = MainDude::instance()._x;
//        int temp_y = MainDude::instance()._y;
//
//        MainDude::instance()._x = temp_x;
//        MainDude::instance()._y = temp_y;
//
//        Hud::instance().clear_console();
//
//        // changing scene, so delete all MovingObjects you have, and according SpriteInfos
//        oam_utils::delete_all_sprites();
//
//        // init sprites since they've been disposed
//        MainDude::instance().init_sprites();
//        Whip::instance().init_sprites();
//
//        if (_current_scene == Scene::MAIN_MENU || _current_scene == Scene::LEVEL_SUMMARY) {
//
//            if (_current_scene == Scene::MAIN_MENU) {
//                start_new_game();
//            } else {
//                sound::start_cave_music();
//            }
//
//            start_next_level(); //initializes hud entities
//
//        } else {
//
//            if (_current_scene == Scene::SCORES) {
//                start_main_menu();
//            } else if (MainDude::instance()._current_state->_state == _MainDudeState::DEAD) {
//                start_scores();
//            } else {
//                start_level_summary();
//            }
//        }
//
//        MainDude::instance()._current_state = _MainDudeState::STANDING;
//
//        LevelRenderer::instance().render();
//
//    } else if (MainDude::instance().animFrame >= 16) {
//        sprite_utils::set_visibility(false,
//                                     MainDude::instance().main_sprite_info,
//                                     MainDude::instance().sub_sprite_info);
//    }
}

/**
 * TODO Move this to the Damsel class.
 */
void GameState::handle_transition_screen_smooch() {
    if (smooching) {
        if (144 - MainDude::instance()._x <= 16) {
            smooch_timer += Timer::getDeltaTime();
            InputHandler::instance().keys.right_key_held = false;
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

bool GameState::update() {

    handle_transition_screen_smooch();

    brightness::update_brightness();
    return !(GameState::instance().exiting_game && brightness::is_maximum_brightness());
}
