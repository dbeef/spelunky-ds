#include <nds.h>
#include "Scrolling.h"
#include "MapUtils.h"
#include "../Consts.h"
#include "../animations/MainDude.h"
#include "../animations/OAMManager.h"
#include "../../build/spelunker.h"
#include "../hud/Hud.h"
#include "../../build/heart.h"
#include "../../build/dollar.h"
#include "../../build/bomb.h"
#include "../../build/bomb_unarmed.h"
#include "../../build/rope.h"
#include "../../build/holding_item.h"
#include "../../build/pre_whip_left.h"
#include "../../build/whip_left.h"

extern u16 map[4096];

static const int BOUNDARY_VALUE = 128; /* This is the default boundary value (can be set in REG_DISPCNT) */
static const int OFFSET_MULTIPLIER_MAIN = BOUNDARY_VALUE / sizeof(SPRITE_GFX[0]);
static const int OFFSET_MULTIPLIER_SUB = BOUNDARY_VALUE / sizeof(SPRITE_GFX_SUB[0]);

void spelunker::scroll(int bg_main, int bg_sub, LevelGenerator *l, u16 *fresh_map) {

    double timer = 0;

    InputHandler *inputHandler = new InputHandler();
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
    mainDude->inputHandler = inputHandler;

    mainDude->main_spelunker = mainOamManager->initSprite(spelunkerPal, spelunkerPalLen, spelunkerTiles,
                                                          spelunkerTilesLen, 16);

    mainDude->main_pre_whip = mainOamManager->initSprite(pre_whip_leftPal, pre_whip_leftPalLen,
                                                         pre_whip_leftTiles, pre_whip_leftTilesLen, 16);

    mainDude->main_whip = mainOamManager->initSprite(whip_leftPal, whip_leftPalLen,
                                                     whip_leftTiles, whip_leftTilesLen, 16);

    mainDude->sub_spelunker = subOamManager->initSprite(spelunkerPal, spelunkerPalLen, spelunkerTiles,
                                                        spelunkerTilesLen, 16);
    mainDude->sub_pre_whip = subOamManager->initSprite(pre_whip_leftPal, pre_whip_leftPalLen,
                                                       pre_whip_leftTiles, pre_whip_leftTilesLen, 16);
    mainDude->sub_whip = subOamManager->initSprite(whip_leftPal, whip_leftPalLen,
                                                   whip_leftTiles, whip_leftTilesLen, 16);

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

        timer = timerElapsed(0) / TICKS_PER_SECOND;

        inputHandler->updateInput();

        if ((inputHandler->b_key_held)) {
            dmaCopyHalfWords(DMA_CHANNEL, fresh_map, map, sizeof(map));
            l->newLayout(timer);
            l->mapBackground();
            l->mapFrame();
            l->generateRooms();
            l->tilesToMap();
            sectorize_map();
            dmaCopyHalfWords(DMA_CHANNEL, map, bgGetMapPtr(bg_main), sizeof(map));
            dmaCopyHalfWords(DMA_CHANNEL, map, bgGetMapPtr(bg_sub), sizeof(map));
            mainDude->bottomCollision = false;
            bomb->bottomCollision = false;
        }

        swiWaitForVBlank();
        mainDude->updateOther();
        mainDude->handleKeyInput();
        bomb->update(camera);
        hud->drawHud();

        camera->updatePosition(mainDude->x, mainDude->y);
        camera->setScroll(bg_main, bg_sub);

        mainOamManager->updateOAM();
        subOamManager->updateOAM();

    }
}



