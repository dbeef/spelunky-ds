#include <nds.h>
#include <nds/arm9/video.h>
#include <nds/arm9/sprite.h>
#include <nds/arm9/console.h>
#include <maxmod9.h>
#include "../build/cavebg.h"
#include "../build/soundbank.h"
#include "../build/soundbank_bin.h"
#include "Globals.h"
#include "tiles/MapUtils.h"
#include "tiles/SplashScreenType.h"
#include "GameLoop.h"
#include "tiles/BaseCaveMap.h"

int main(void) {

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

    dmaCopy(cavebgTiles, bgGetGfxPtr(global::bg_main_address), sizeof(cavebgTiles));
    dmaCopy(cavebgTiles, bgGetGfxPtr(global::bg_sub_address), sizeof(cavebgTiles));


    dmaCopyHalfWords(3, global::base_map, global::current_map, sizeof(global::base_map));

    global::level_generator->newLayout(timerElapsed(0));
    global::level_generator->generateSplashScreen(SplashScreenType::MAIN_MENU_UPPER);
    global::level_generator->generateSplashScreen(SplashScreenType::MAIN_MENU_LOWER);
    global::level_generator->tilesToMap();

    sectorize_map();

    dmaCopyHalfWords(DMA_CHANNEL, global::current_map, bgGetMapPtr(global::bg_main_address),
                     sizeof(global::current_map));
    dmaCopyHalfWords(DMA_CHANNEL, global::current_map, bgGetMapPtr(global::bg_sub_address),
                     sizeof(global::current_map));

    global::textManager->initConsole();

    dmaCopy(cavebgPal, BG_PALETTE, cavebgPalLen);
    dmaCopy(cavebgPal, BG_PALETTE_SUB, cavebgPalLen);

    gameloop::scroll();

    t->stop();

    return 0;
}

