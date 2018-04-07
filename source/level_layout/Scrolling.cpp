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
#include "../../build/bomb_unarmed.h"
#include "../../build/rope.h"
#include "../../build/holding_item.h"
#include "../../build/pre_whip_left.h"
#include "../../build/pre_whip_right.h"
#include "../../build/whip_left.h"
#include "../../build/whip_right.h"

extern u16 map[4096];

static const int BOUNDARY_VALUE = 128; /* This is the default boundary value (can be set in REG_DISPCNT) */
static const int OFFSET_MULTIPLIER_MAIN = BOUNDARY_VALUE / sizeof(SPRITE_GFX[0]);
static const int OFFSET_MULTIPLIER_SUB = BOUNDARY_VALUE / sizeof(SPRITE_GFX_SUB[0]);

void spelunker::scroll(int bg_main, int bg_sub, LevelGenerator *l, u16 *fresh_map) {
    int keys_held = 0;
    int keys_down = 0;

    double timer = 0;

    Camera *camera = new Camera();

    OAMManager *mainOamManager = new OAMManager();
    mainOamManager->initOAMTable(SPRITE_GFX, SPRITE_PALETTE, OAM, OFFSET_MULTIPLIER_MAIN);

    OAMManager *subOamManager = new OAMManager();
    subOamManager->initOAMTable(SPRITE_GFX_SUB, SPRITE_PALETTE_SUB, OAM_SUB, OFFSET_MULTIPLIER_SUB);


    MainDude *mainDude = new MainDude();
    mainDude->x = 100;
    mainDude->timer = &timer;
    mainDude->y = 100;
    mainDude->levelGenerator = l;
    mainDude->camera = camera;

    mainDude->main_spriteInfo = mainOamManager->initSprite(spelunkerPal, spelunkerPalLen, spelunkerTiles,
                                                           spelunkerTilesLen, 16);

    mainDude->main_pre_whip_left_spriteInfo = mainOamManager->initSprite(pre_whip_leftPal, pre_whip_leftPalLen,
                                                                         pre_whip_leftTiles, pre_whip_leftTilesLen, 16);

    mainDude->main_pre_whip_right_spriteInfo = mainOamManager->initSprite(pre_whip_rightPal, pre_whip_rightPalLen,
                                                                          pre_whip_rightTiles, pre_whip_rightTilesLen,
                                                                          16);

    mainDude->main_whip_left_spriteInfo = mainOamManager->initSprite(whip_leftPal, whip_leftPalLen,
                                                                     whip_leftTiles, whip_leftTilesLen, 16);

    mainDude->main_whip_right_spriteInfo = mainOamManager->initSprite(whip_rightPal, whip_rightPalLen,
                                                                      whip_rightTiles, whip_rightTilesLen, 16);

    mainDude->sub_spriteInfo = subOamManager->initSprite(spelunkerPal, spelunkerPalLen, spelunkerTiles,
                                                         spelunkerTilesLen, 16);

    mainDude->sub_pre_whip_left_spriteInfo = subOamManager->initSprite(pre_whip_leftPal, pre_whip_leftPalLen,
                                                                       pre_whip_leftTiles, pre_whip_leftTilesLen, 16);
    mainDude->sub_pre_whip_right_spriteInfo = subOamManager->initSprite(pre_whip_rightPal, pre_whip_rightPalLen,
                                                                        pre_whip_rightTiles, pre_whip_rightTilesLen,
                                                                        16);
    mainDude->sub_whip_left_spriteInfo = subOamManager->initSprite(whip_leftPal, whip_leftPalLen,
                                                                   whip_leftTiles, whip_leftTilesLen, 16);
    mainDude->sub_whip_right_spriteInfo = subOamManager->initSprite(whip_rightPal, whip_rightPalLen,
                                                                    whip_rightTiles, whip_rightTilesLen, 16);

//    mainDude->main_spriteInfo->entry->priority = OBJPRIORITY_1;
//    mainDude->sub_spriteInfo->entry->priority = OBJPRIORITY_1;

    Bomb *bomb = new Bomb();
    bomb->init(70, 100, true, l, &timer);
    bomb->xSpeed = 3;
    bomb->subSpriteInfo = subOamManager->initSprite(bomb_unarmedPal, bomb_unarmedPalLen,
                                                    bomb_unarmedTiles, bomb_unarmedTilesLen, 8);
    bomb->mainSpriteInfo = mainOamManager->initSprite(bomb_unarmedPal, bomb_unarmedPalLen,
                                                      bomb_unarmedTiles, bomb_unarmedTilesLen, 8);
    bomb->carried = true;

    mainDude->init();
    mainDude->bomb = bomb;
    Hud *hud = new Hud();
    hud->heartSpriteInfo = mainOamManager->initSprite(heartPal, heartPalLen, heartTiles, heartTilesLen, 16);
    hud->dollarSpriteInfo = mainOamManager->initSprite(dollarPal, dollarPalLen, dollarTiles, dollarTilesLen, 16);
    hud->bombSpriteInfo = mainOamManager->initSprite(bombPal, bombPalLen, bombTiles, bombTilesLen, 16);
    hud->ropeSpriteInfo = mainOamManager->initSprite(ropePal, ropePalLen, ropeTiles, ropeTilesLen, 16);
    hud->holdingItemSpriteInfo = mainOamManager->
            initSprite(holding_itemPal, holding_itemPalLen, holding_itemTiles, holding_itemTilesLen, 16);
    hud->initHud();

    while (true) {
        scanKeys();

        timer = timerElapsed(0) / TICKS_PER_SECOND;

        keys_held = keysHeld();
        keys_down = keysDown();

        if ((keys_held & KEY_B)) {

            dmaCopyHalfWords(DMA_CHANNEL, fresh_map, map, sizeof(map));
            l->newLayout(timer);
            l->mapBackground();
            l->mapFrame();

            l->generateRooms();
//            mainDude->clearTilesOnRight(levelGenerator->mapTiles);

            l->tilesToMap();
            sectorize_map();
            dmaCopyHalfWords(DMA_CHANNEL, map, bgGetMapPtr(bg_main), sizeof(map));
            dmaCopyHalfWords(DMA_CHANNEL, map, bgGetMapPtr(bg_sub), sizeof(map));

            mainDude->bottomCollision = false;
            bomb->bottomCollision = false;
        }


        swiWaitForVBlank();
        mainDude->updateOther();
        mainDude->handleKeyInput(keys_held, keys_down);
        bomb->update(camera);
        hud->drawHud();

        camera->updatePosition(mainDude->x, mainDude->y);
        camera->setScroll(bg_main, bg_sub);

        mainOamManager->updateOAM();
        subOamManager->updateOAM();

    }
}



