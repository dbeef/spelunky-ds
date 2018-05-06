#include <nds.h>
#include <nds/arm9/video.h>
#include <nds/arm9/sprite.h>
#include <nds/arm9/console.h>
#include <maxmod9.h>
#include "../build/gfx_cavebg.h"
#include "../build/soundbank.h"
#include "../build/soundbank_bin.h"
#include "globals_declarations.h"
#include "globals_definitions.h"
#include "tiles/map_utils.h"
#include "tiles/splash_screen_type.h"
#include "game_loop.h"

int main() {

    mmInitDefaultMem((mm_addr) soundbank_bin);
    mmLoadEffect(SFX_MCAVE);
    mmLoadEffect(SFX_MTITLE);
    mmLoadEffect(SFX_XJUMP);
    mmLoadEffect(SFX_XWHIP);
    mmLoadEffect(SFX_XCOIN);
    mmLoadEffect(SFX_XTHROW);
    mmLoadEffect(SFX_XGEM);
    mmLoadEffect(SFX_XEXPLOSION);
    mmLoadEffect(SFX_XSTEPS);
    mmLoadEffect(SFX_XLAND);
    global::menu_music_handler = mmEffect(SFX_MTITLE);

    Timer *t = new Timer();
    t->start();

    videoSetMode(MODE_0_2D);
    videoSetModeSub(MODE_0_2D);

    vramSetBankA(VRAM_A_MAIN_BG_0x06000000);
    //vram c is used by the console
    vramSetBankC(VRAM_C_SUB_BG_0x06200000);


    vramSetBankE(VRAM_E_MAIN_SPRITE);
    vramSetBankD(VRAM_D_SUB_SPRITE);

    oamInit(&oamMain, SpriteMapping_1D_64, false);
    oamInit(&oamSub, SpriteMapping_1D_64, false);

    global::bg_main_address = bgInit(2, BgType_Text8bpp, BgSize_B8_512x512, 22, 4);
    global::bg_sub_address = bgInitSub(3, BgType_Text8bpp, BgSize_B8_512x512, 18, 4);

    dmaCopy(gfx_cavebgTiles, bgGetGfxPtr(global::bg_main_address), sizeof(gfx_cavebgTiles));
    dmaCopy(gfx_cavebgTiles, bgGetGfxPtr(global::bg_sub_address), sizeof(gfx_cavebgTiles));


    dmaCopyHalfWords(3, global::base_map, global::current_map, sizeof(global::base_map));

    global::level_generator->newLayout(timerElapsed(0));
    global::level_generator->generateSplashScreen(splash_screen_type::MAIN_MENU_UPPER);
    global::level_generator->generateSplashScreen(splash_screen_type::MAIN_MENU_LOWER);
    global::level_generator->tilesToMap();

    sectorize_map();

    dmaCopyHalfWords(DMA_CHANNEL, global::current_map, bgGetMapPtr(global::bg_main_address),
                     sizeof(global::current_map));
    dmaCopyHalfWords(DMA_CHANNEL, global::current_map, bgGetMapPtr(global::bg_sub_address),
                     sizeof(global::current_map));

    global::text_manager->initConsole();

    dmaCopy(gfx_cavebgPal, BG_PALETTE, gfx_cavebgPalLen);
    dmaCopy(gfx_cavebgPal, BG_PALETTE_SUB, gfx_cavebgPalLen);

    gameloop::scroll();

    t->stop();

    return 0;
}

