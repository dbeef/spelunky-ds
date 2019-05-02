#include <nds.h>
#include <cstdlib>
#include <nds/arm9/exceptions.h>
#include "GameLoop.hpp"
#include "tiles/PopulatingUtils.hpp"
#include "time/Timer.h"
#include "entities/animations/Blood.hpp"
#include "entities/items/Arrow.hpp"
#include "entities/main_dude/MainDudeConsts.h"
#include "GameState.hpp"
#include "graphics/OamUtils.hpp"
#include "graphics/Brightness.hpp"

void gameloop::run() {

    GameState::instance().camera->x = CAMERA_MENU_START_X;
    GameState::instance().camera->y = CAMERA_MENU_START_Y;
    GameState::instance().main_dude = new MainDude(MAIN_DUDE_MENU_START_POSITION_X, MAIN_DUDE_MENU_START_POSITION_Y);

    GameState::instance().hud->init();
    populate_main_menu();

    // If exited, SpelunkyDS returns 0 and gets back to the menu.
    while (true) {

        Timer::update_ms_since_last_frame();
        GameState::instance().input_handler->updateInput();

        if (GameState::instance().bombed) {
            on_bomb_explosion();
            GameState::instance().bombed = false;
        }

        GameState::instance().camera->update();

        GameState::instance().main_dude->update();
        GameState::instance().main_dude->whip->update();

        update_entities();

        GameState::instance().handle_transition_screen_smooch();
        GameState::instance().main_dude->handle_key_input();
        GameState::instance().hud->update();

        swiWaitForVBlank();
        brightness::normalize_brightness();

        GameState::instance().camera->write_current_position_to_graphics_engines();
        GameState::instance().main_oam_manager->updateOAM();
        GameState::instance().sub_oam_manager->updateOAM();
        oam_utils::clean_unused_oam();
    }

}

void gameloop::on_bomb_explosion() {
    GameState::instance().current_level->update_level();

    for (auto &creature : GameState::instance().creatures)
        (*creature)._bottom_collision = false;
    for (auto &item : GameState::instance().items)
        (*item)._bottom_collision = false;
    for (auto &treasures : GameState::instance().treasures)
        (*treasures)._bottom_collision = false;
    GameState::instance().main_dude->_bottom_collision = false;
}

void gameloop::update_entities() {

    // Iterating using indexes since entities may be
    // pushed back while iterating, invalidating iterators.

    {
        const std::size_t size = GameState::instance().creatures.size();
        for (std::size_t index = 0; index < size; ++index) {
            auto *&creature = GameState::instance().creatures.at(index);
            if (creature && !creature->_ready_to_dispose) {
                creature->update();
            }
        }
    }
    {
        const std::size_t size = GameState::instance().items.size();
        for (std::size_t index = 0; index < size; ++index) {
            auto *&item = GameState::instance().items.at(index);
            if (item && !item->_ready_to_dispose) {
                item->update();
            }
        }
    }
    {
        const std::size_t size = GameState::instance().decorations.size();
        for (std::size_t index = 0; index < size; ++index) {
            auto *&decoration = GameState::instance().decorations.at(index);
            if (decoration) {
                decoration->update();
            }
        }
    }
    {
        const std::size_t size = GameState::instance().treasures.size();
        for (std::size_t index = 0; index < size; ++index) {
            auto *&treasure = GameState::instance().treasures.at(index);
            if (treasure && !treasure->_ready_to_dispose) {
                treasure->update();
            }
        }
    }
}

