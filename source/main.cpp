#include <nds.h>
#include <cstdio>
#include <nds/arm9/video.h>
#include <nds/arm9/sprite.h>
#include <nds/arm9/console.h>
#include <array>
#include <iostream>
#include <cstring>
#include "level_layout/Scrolling.h"
#include "../build/cavebg.h"
#include "level_layout/BaseCaveMap.h"
#include "time/Timer.h"
#include "Globals.h"
#include "level_layout/MapUtils.h"
#include "console/TextManager.h"

static u16 *fresh_map;

namespace global {
    InputHandler *input_handler;
    Camera *camera;
    LevelGenerator *level_generator;
    OAMManager *main_oam_manager;
    OAMManager *sub_oam_manager;
    Hud *hud;
}

int main(void) {

    Timer *t = new Timer();
    TextManager *textManager = new TextManager();


    videoSetMode(MODE_0_2D);
    videoSetModeSub(MODE_0_2D);

    vramSetBankA(VRAM_A_MAIN_BG_0x06000000);
    //vram c is used by the console
    vramSetBankC(VRAM_C_SUB_BG_0x06200000);
    vramSetBankF(VRAM_F_MAIN_SPRITE_0x06400000);
    vramSetBankD(VRAM_D_SUB_SPRITE);

    oamInit(&oamMain, SpriteMapping_1D_128, false);
    oamInit(&oamSub, SpriteMapping_1D_128, false);

    int bg_main = bgInit(2, BgType_Text4bpp, BgSize_B8_512x512, 22, 4);
    int bg_sub = bgInitSub(3, BgType_Text4bpp, BgSize_B8_512x512, 18, 4);

    dmaCopy(cavebgTiles, bgGetGfxPtr(bg_main), sizeof(cavebgTiles));
    dmaCopy(cavebgTiles, bgGetGfxPtr(bg_sub), sizeof(cavebgTiles));

    fresh_map = (u16 *) std::malloc(sizeof(u16[4096]));
    dmaCopyHalfWords(3, map, fresh_map, sizeof(map));

    global::level_generator = new LevelGenerator();
    global::level_generator->newLayout(timerElapsed(0));
    global::level_generator->mapBackground();
    global::level_generator->mapFrame();
    global::level_generator->generateRooms();
    global::level_generator->tilesToMap();

    sectorize_map();

    dmaCopyHalfWords(DMA_CHANNEL, map, bgGetMapPtr(bg_main), sizeof(map));
    dmaCopyHalfWords(DMA_CHANNEL, map, bgGetMapPtr(bg_sub), sizeof(map));
    dmaCopy(cavebgPal, BG_PALETTE, cavebgPalLen);
    dmaCopy(cavebgPal, BG_PALETTE_SUB, cavebgPalLen);
    textManager->initConsole();

    spelunker::scroll(bg_main, bg_sub, fresh_map);

    t->stop();

    return 0;
}

