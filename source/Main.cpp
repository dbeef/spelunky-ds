#include <nds.h>
#include <nds/arm9/video.h>
#include <nds/arm9/sprite.h>
#include <nds/arm9/console.h>
#include <nds/arm9/exceptions.h>
#include "../build/gfx_cavebg.h"

#include "tiles/SplashScreenType.hpp"
#include "GameLoop.hpp"
#include "sound/Sound.hpp"
#include "console/ConsoleUtils.hpp"
#include "time/Timer.h"
#include "GameState.hpp"

/**
 * Set-up before the gameloop can be run.
 * TODO Link current VRAM bank configuration.
 */
int main() {

    GameState::init();

    // waiting for a next frame so it would be allowed to change the brightness
    swiWaitForVBlank();
    // setting screen white to smooth transition from game launcher
    GameState::instance().set_maximum_brightness();

    sound::load_sounds();
    sound::start_menu_music();

    Timer::start();

    videoSetMode(MODE_0_2D);
    videoSetModeSub(MODE_0_2D);

    vramSetBankA(VRAM_A_MAIN_BG_0x06000000);
    //vram C is used by the console (by default):
    vramSetBankC(VRAM_C_SUB_BG_0x06200000);

    vramSetBankE(VRAM_E_MAIN_SPRITE);
    vramSetBankD(VRAM_D_SUB_SPRITE);

    oamInit(&oamMain, SpriteMapping_1D_64, false);
    oamInit(&oamSub, SpriteMapping_1D_64, false);

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

    console::init();

    //Set-up main menu scene from tiles
    GameState::instance().current_level->initialise_tiles_from_splash_screen(SplashScreenType::MAIN_MENU_UPPER);
    GameState::instance().current_level->initialise_tiles_from_splash_screen(SplashScreenType::MAIN_MENU_LOWER);
    GameState::instance().current_level->update_level();

    //Copy cave background palette to the graphics memory
    dmaCopy(gfx_cavebgPal, BG_PALETTE, gfx_cavebgPalLen);
    dmaCopy(gfx_cavebgPal, BG_PALETTE_SUB, gfx_cavebgPalLen);

    //Initialise sprite memory
    constexpr int BOUNDARY_VALUE = 64; /* This is the default boundary value (can be set in REG_DISPCNT) */
    constexpr int OFFSET_MULTIPLIER_MAIN = BOUNDARY_VALUE / sizeof(SPRITE_GFX[0]);
    constexpr int OFFSET_MULTIPLIER_SUB = BOUNDARY_VALUE / sizeof(SPRITE_GFX_SUB[0]);

    GameState::instance().main_oam_manager->initOAMTable(SPRITE_GFX, SPRITE_PALETTE, OAM, OFFSET_MULTIPLIER_MAIN, OamType::MAIN);
    GameState::instance().sub_oam_manager->initOAMTable(SPRITE_GFX_SUB, SPRITE_PALETTE_SUB, OAM_SUB, OFFSET_MULTIPLIER_SUB,
                                          OamType::SUB);

    gameloop::run();
    Timer::stop();

    GameState::dispose();

    return 0;
}

