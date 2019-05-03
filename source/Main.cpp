#include <nds.h>
#include <nds/arm9/video.h>
#include <nds/arm9/sprite.h>
#include <nds/arm9/console.h>
#include <nds/arm9/exceptions.h>
#include "../build/gfx_cavebg.h"

#include "tiles/SplashScreenType.hpp"
#include "GameLoop.hpp"
#include "sound/Sound.hpp"
#include "time/Timer.h"
#include "GameState.hpp"
#include "graphics/Brightness.hpp"

#include <cstdio>

int main() {

    // FIXME: This function mixes abstraction levels,
    //  rewrite it for the sake of clean code.

#ifndef NDEBUG
    defaultExceptionHandler();
#endif

    GameState::init();
    InputHandler::init();
    Camera::init();
    Hud::init();
    OAMManager::init();
    MainDude::init();
    Whip::init();

    // waiting for a next frame so it would be allowed to change the brightness
    swiWaitForVBlank();
    // setting screen white to smooth transition from game launcher
    brightness::set_maximum_brightness();

    sound::load_sounds();
    sound::start_menu_music();

    Timer::start();

    // TODO Link current VRAM bank configuration.

    videoSetMode(MODE_0_2D);
    videoSetModeSub(MODE_0_2D);

    vramSetBankA(VRAM_A_MAIN_BG_0x06000000);
    //vram C is used by the console (by default):
    vramSetBankC(VRAM_C_SUB_BG_0x06200000);

    vramSetBankE(VRAM_E_MAIN_SPRITE);
    vramSetBankD(VRAM_D_SUB_SPRITE);

    oamInit(&oamMain, SpriteMapping_1D_64, false);
    oamInit(&oamSub, SpriteMapping_1D_64, false);

    // TODO: Decouple Level state from tile rendering logics and put these
    //   calls in TileRenderer constructor or whatever class will come out of this decoupling.
    //https://devkitpro.org/viewtopic.php?f=38&t=8720
    // OBJPRIORITY_0 > BG_PRIORITY_0 >
    // OBJPRIORITY_1 > BG_PRIORITY_1 >
    // OBJPRIORITY_2 > BG_PRIORITY_2 >
    // OBJPRIORITY_3 > BG_PRIORITY_3.
    GameState::instance().bg_main_address = bgInit(BG_PRIORITY_3, BgType_Text8bpp, BgSize_B8_512x512, 22, 4);
    GameState::instance().bg_sub_address = bgInitSub(BG_PRIORITY_3, BgType_Text8bpp, BgSize_B8_512x512, 18, 4);

    bgSetPriority(GameState::instance().bg_main_address, BG_PRIORITY_3);
    bgSetPriority(GameState::instance().bg_sub_address, BG_PRIORITY_3);

    //Copy cave background tiles to the graphics memory
    dmaCopy(gfx_cavebgTiles, bgGetGfxPtr(GameState::instance().bg_main_address), sizeof(gfx_cavebgTiles));
    dmaCopy(gfx_cavebgTiles, bgGetGfxPtr(GameState::instance().bg_sub_address), sizeof(gfx_cavebgTiles));

// TODO: Find a way to print to desmume; nocash is Windows only.
// https://wiki.desmume.org/index.php?title=Faq_095#How_can_I_do_a_printf_to_the_emulator_console.3F
#ifdef PRINT_TO_NOCASH_EMULATOR
    consoleDebugInit(DebugDevice_NOCASH);
    fprintf(stderr, "This should be printed if ran on No$GBA");
#else
    Hud::instance().init_console();
#endif

    //Set-up main menu scene from tiles
    GameState::instance().current_level->initialise_tiles_from_splash_screen(SplashScreenType::MAIN_MENU_UPPER);
    GameState::instance().current_level->initialise_tiles_from_splash_screen(SplashScreenType::MAIN_MENU_LOWER);
    GameState::instance().current_level->update_level();

    //Copy cave background palette to the graphics memory
    dmaCopy(gfx_cavebgPal, BG_PALETTE, gfx_cavebgPalLen);
    dmaCopy(gfx_cavebgPal, BG_PALETTE_SUB, gfx_cavebgPalLen);

    //Initialise sprite graphics
    constexpr int BOUNDARY_VALUE = 64; /* This is the default boundary value (can be set in REG_DISPCNT) */
    constexpr int OFFSET_MULTIPLIER_MAIN = BOUNDARY_VALUE / sizeof(SPRITE_GFX[0]);
    constexpr int OFFSET_MULTIPLIER_SUB = BOUNDARY_VALUE / sizeof(SPRITE_GFX_SUB[0]);

    // TODO: constexpr arguments, can be in called method definition.
    OAMManager::main().initOAMTable(SPRITE_GFX, SPRITE_PALETTE, OAM, OFFSET_MULTIPLIER_MAIN);
    OAMManager::sub().initOAMTable(SPRITE_GFX_SUB, SPRITE_PALETTE_SUB, OAM_SUB, OFFSET_MULTIPLIER_SUB);

    gameloop::run();

    Timer::stop();
    GameState::dispose();
    InputHandler::dispose();
    Camera::dispose();
    Hud::dispose();
    OAMManager::dispose();
    MainDude::dispose();
    Whip::dispose();

    return 0;
}

