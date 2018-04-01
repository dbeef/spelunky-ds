#include <nds.h>
#include "Scrolling.h"
#include "MapUtils.h"
#include "../Consts.h"
#include "../animations/MainDude.h"
#include "../animations/OAMManager.h"
#include "../../build/spelunker.h"
#include "../hud/Hud.h"
#include "../../build/heart.h"
#include <nds/arm9/input.h>
#include "../../build/dollar.h"
#include "../../build/bomb.h"
#include "../../build/rope.h"
#include "../../build/holding_item.h"

extern u16 map[4096];

static const int BOUNDARY_VALUE = 128; /* This is the default boundary value (can be set in REG_DISPCNT) */
static const int OFFSET_MULTIPLIER_MAIN = BOUNDARY_VALUE / sizeof(SPRITE_GFX[0]);
static const int OFFSET_MULTIPLIER_SUB = BOUNDARY_VALUE / sizeof(SPRITE_GFX_SUB[0]);

void spelunker::scroll(int bg_main, int bg_sub, LevelGenerator *l, u16 *fresh_map) {
    int keys_held = 0;
    int keys_down = 0;

    int timer = 0;
    int camera_timer = 0;

    Camera *camera = new Camera();

    OAMManager *mainOamManager = new OAMManager();
    mainOamManager->initOAMTable(SPRITE_GFX, SPRITE_PALETTE, OAM, OFFSET_MULTIPLIER_MAIN);

    OAMManager *subOamManager = new OAMManager();
    subOamManager->initOAMTable(SPRITE_GFX_SUB, SPRITE_PALETTE_SUB, OAM_SUB, OFFSET_MULTIPLIER_SUB);


    MainDude *mainDude = new MainDude();
    mainDude->x = 100;
    mainDude->y = 100;
    mainDude->main_spriteInfo = mainOamManager->initSprite(spelunkerPal, spelunkerPalLen, spelunkerTiles,
                                                           spelunkerTilesLen);
    mainDude->sub_spriteInfo = subOamManager->initSprite(spelunkerPal, spelunkerPalLen, spelunkerTiles,
                                                         spelunkerTilesLen);
    mainDude->init();
    Hud *hud = new Hud();
    hud->heartSpriteInfo = mainOamManager->initSprite(heartPal, heartPalLen, heartTiles, heartTilesLen);
    hud->dollarSpriteInfo = mainOamManager->initSprite(dollarPal, dollarPalLen, dollarTiles, dollarTilesLen);
    hud->bombSpriteInfo = mainOamManager->initSprite(bombPal, bombPalLen, bombTiles, bombTilesLen);
    hud->ropeSpriteInfo = mainOamManager->initSprite(ropePal, ropePalLen, ropeTiles, ropeTilesLen);
    hud->holdingItemSpriteInfo = mainOamManager->
            initSprite(holding_itemPal, holding_itemPalLen, holding_itemTiles, holding_itemTilesLen);
    hud->initHud();

    while (true) {
        scanKeys();

        timer += timerElapsed(0) / TICKS_PER_SECOND;
        camera_timer += timerElapsed(0) / TICKS_PER_SECOND;

        keys_held = keysHeld();
        keys_down = keysDown();

        if ((keys_held & KEY_B)) {

            timer = 0;
            dmaCopyHalfWords(DMA_CHANNEL, fresh_map, map, sizeof(map));
            l->newLayout(timerElapsed(0));
            l->mapBackground();
            l->mapFrame();

            l->generateRooms();
//            mainDude->clearTilesOnRight(l->mapTiles);

            l->tilesToMap();
            sectorize_map();
            dmaCopyHalfWords(DMA_CHANNEL, map, bgGetMapPtr(bg_main), sizeof(map));
            dmaCopyHalfWords(DMA_CHANNEL, map, bgGetMapPtr(bg_sub), sizeof(map));
        }


        swiWaitForVBlank();

        mainDude->update(camera, keys_held, keys_down, l);
        hud->drawHud();

        camera->updatePosition(mainDude->x, mainDude->y);
        camera->setScroll(bg_main, bg_sub);

        mainOamManager->updateOAM();
        subOamManager->updateOAM();

    }
}




