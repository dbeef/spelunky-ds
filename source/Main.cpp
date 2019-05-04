#include <nds.h>
#include <cstdio>
#include <nds/arm9/exceptions.h>

#include "graphics/Brightness.hpp"
#include "sound/Sound.hpp"
#include "time/Timer.h"
#include "graphics/OamManager.hpp"
#include "tiles/LevelRenderer.hpp"
#include "hud/Hud.hpp"
#include "GameLoop.hpp"
#include "GameState.hpp"

void configure_vram_banks();

void init_singletons();

void dispose_singletons();

// Sets up game and proceeds to game loop.
// Disposes game on game loop break.
int main() {

#ifndef NDEBUG
    defaultExceptionHandler();
#endif

    // Setting screen full white to smooth transition from game launcher,
    // which happens to transit to white color when starting game.
    swiWaitForVBlank();
    brightness::set_maximum_brightness();

    init_singletons();

    sound::load_sounds();
    sound::start_menu_music();

    // Batch of hardware-related calls. Needs to be called in this sequence.
    configure_vram_banks();
    OAMManager::init_sprite_hardware();
    LevelRenderer::instance().init_background_hardware();
    LevelRenderer::instance().copy_background_tiles();
    Hud::instance().init_console();
    LevelRenderer::instance().copy_background_palette();

    Timer::start();
    gameloop::run();
    Timer::stop();

    dispose_singletons();

    return 0;
}

void dispose_singletons() {
    GameState::dispose();
    InputHandler::dispose();
    Camera::dispose();
    Hud::dispose();
    OAMManager::dispose();
    MainDude::dispose();
    Whip::dispose();
    Level::dispose();
    LevelRenderer::dispose();
}

void init_singletons() {
    Level::init();
    GameState::init();
    InputHandler::init();
    Camera::init();
    Hud::init();
    OAMManager::init();
    MainDude::init();
    Whip::init();
    LevelRenderer::init();
}

void configure_vram_banks() {
    // Current VRAM bank configuration:
    // https://mtheall.com/vram.html#T0=3&NT0=128&MB0=2&TB0=8&T3=1&NT3=32&MB3=22&TB3=4&S3=3
    videoSetMode(MODE_0_2D);
    videoSetModeSub(MODE_0_2D);

    vramSetBankA(VRAM_A_MAIN_BG_0x06000000);
    // vram C is used by console (by default)
    vramSetBankC(VRAM_C_SUB_BG_0x06200000);
    vramSetBankE(VRAM_E_MAIN_SPRITE);
    vramSetBankD(VRAM_D_SUB_SPRITE);
}
