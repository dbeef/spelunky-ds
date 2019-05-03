#include <nds.h>

#include "GameLoop.hpp"
#include "GameState.hpp"
#include "graphics/OamUtils.hpp"
#include "tiles/PopulatingUtils.hpp"
#include "time/Timer.h"

void gameloop::run() {

    Camera::instance().set_main_menu_position();
    Camera::instance().detach_from_main_dude();

    MainDude::instance().init_sprites();
    Whip::instance().init_sprites();

    Hud::instance().init_sprites();
    Hud::instance().clear_console();

    populate_main_menu();

    // If exited, SpelunkyDS returns 0 and gets back to game laucher.
    while (GameState::instance().update()) {

        Timer::update_ms_since_last_frame();
        InputHandler::instance().update_input();

        Camera::instance().update();
        Hud::instance().update();

        update_entities();

        OAMManager::main().update();
        OAMManager::sub().update();

        oam_utils::clean_unused_oam();

        swiWaitForVBlank();
    }
}

void gameloop::update_entities() {

    // TODO: Merge those functions.
    MainDude::instance().handle_key_input();
    MainDude::instance().update();

    Whip::instance().update();

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
