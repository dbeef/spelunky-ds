#include <nds.h>
#include "Scrolling.h"
#include "MapUtils.h"
#include "../Consts.h"
#include "../animations/MainDude.h"
#include "../hud/Hud.h"
#include "SpriteInfo.h"
#include "../../build/heart.h"
#include "../animations/Camera.h"
#include "../animations/OAMManager.h"
#include "../../build/spelunker.h"
#include <nds/arm9/input.h>
#include <nds/arm9/sprite.h>
#include <nds/arm9/cache.h>
#include <assert.h>

extern u16 map[4096];

void spelunker::scroll(int bg_main, int bg_sub, LevelGenerator *l, u16 *fresh_map) {
    int keys_held = 0;
    int keys_down = 0;

    int timer = 0;
    int camera_timer = 0;

    Camera *camera = new Camera();
    OAMManager *oamManager = new OAMManager();
    oamManager->initOAMTable();

    MainDude *mainDude = new MainDude();
    mainDude->x = 100;
    mainDude->y = 100;
    mainDude->spriteInfo = oamManager->initSprite(spelunkerPal, spelunkerPalLen, spelunkerTiles, spelunkerTilesLen);
    mainDude->init();

//    Hud *hud = new Hud();
//    hud->initHud();


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
        //
        //updateOAM(oam);
        //
        mainDude->update(camera, keys_held, keys_down, l);
        camera->updatePosition(mainDude->x, mainDude->y);
        camera->setScroll(bg_main, bg_sub);


        oamUpdate(&oamSub);
        oamUpdate(&oamMain);

    }
}




