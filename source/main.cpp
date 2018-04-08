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
    InputHandler *input_handler = new InputHandler();
    Camera *camera = new Camera();
    LevelGenerator *level_generator = new LevelGenerator();
    OAMManager *main_oam_manager = new OAMManager();
    OAMManager *sub_oam_manager = new OAMManager();
    Hud *hud = new Hud();
    int bg_main_address = 0;
    int bg_sub_address = 0;
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

    global::bg_main_address = bgInit(2, BgType_Text4bpp, BgSize_B8_512x512, 22, 4);
    global::bg_sub_address = bgInitSub(3, BgType_Text4bpp, BgSize_B8_512x512, 18, 4);

    dmaCopy(cavebgTiles, bgGetGfxPtr(global::bg_main_address), sizeof(cavebgTiles));
    dmaCopy(cavebgTiles, bgGetGfxPtr(global::bg_sub_address), sizeof(cavebgTiles));

    fresh_map = (u16 *) std::malloc(sizeof(u16[4096]));
    dmaCopyHalfWords(3, map, fresh_map, sizeof(map));

    global::level_generator->newLayout(timerElapsed(0));
    global::level_generator->mapBackground();
    global::level_generator->mapFrame();
    global::level_generator->generateRooms();
    global::level_generator->tilesToMap();

    sectorize_map();

    dmaCopyHalfWords(DMA_CHANNEL, map, bgGetMapPtr(global::bg_main_address), sizeof(map));
    dmaCopyHalfWords(DMA_CHANNEL, map, bgGetMapPtr(global::bg_sub_address), sizeof(map));
    dmaCopy(cavebgPal, BG_PALETTE, cavebgPalLen);
    dmaCopy(cavebgPal, BG_PALETTE_SUB, cavebgPalLen);
    textManager->initConsole();

    spelunker::scroll(fresh_map);

    t->stop();

    return 0;
}

