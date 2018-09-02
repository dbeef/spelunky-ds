#include <nds.h>
#include <nds/arm9/video.h>
#include <nds/arm9/sprite.h>
#include <nds/arm9/console.h>
#include "../build/gfx_cavebg.h"
#include "globals_declarations.h"
#include "globals_definitions.h"
#include "tiles/splash_screen_type.hpp"
#include "game_loop.h"
#include "sound/sound_utils.h"
#include "console/console_utils.h"
#include "time/time_utils.h"

int main() {

    global::init_globals();

    swiWaitForVBlank();
    setBrightness(3, global::game_state->brightness_level);

    global::game_state->in_main_menu = true;

    sound::load_sounds();

    sound::start_menu_music();

    time_utils::start();

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
    //OBJPRIORITY_0 > BG_PRIORITY_0 >
    // OBJPRIORITY_1 > BG_PRIORITY_1 >
    // OBJPRIORITY_2 > BG_PRIORITY_2 >
    // OBJPRIORITY_3 > BG_PRIORITY_3.
    global::bg_main_address = bgInit(BG_PRIORITY_3, BgType_Text8bpp, BgSize_B8_512x512, 22, 4);
    global::bg_sub_address = bgInitSub(BG_PRIORITY_3, BgType_Text8bpp, BgSize_B8_512x512, 18, 4);

    bgSetPriority(global::bg_main_address, BG_PRIORITY_3);
    bgSetPriority(global::bg_sub_address, BG_PRIORITY_3);

    dmaCopy(gfx_cavebgTiles, bgGetGfxPtr(global::bg_main_address), sizeof(gfx_cavebgTiles));
    dmaCopy(gfx_cavebgTiles, bgGetGfxPtr(global::bg_sub_address), sizeof(gfx_cavebgTiles));

    console::init();

    global::level_generator->generate_splash_screen(SplashScreenType::MAIN_MENU_UPPER);
    global::level_generator->generate_splash_screen(SplashScreenType::MAIN_MENU_LOWER);
    global::level_generator->render_tiles_on_base_map();

    dmaCopy(gfx_cavebgPal, BG_PALETTE, gfx_cavebgPalLen);
    dmaCopy(gfx_cavebgPal, BG_PALETTE_SUB, gfx_cavebgPalLen);

    gameloop::scroll();

    time_utils::stop();

    return 0;
}

