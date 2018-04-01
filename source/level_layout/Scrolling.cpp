#include <nds.h>
#include "Scrolling.h"
#include "MapUtils.h"
#include "../Consts.h"
#include "../animations/MainDude.h"
#include "../hud/Hud.h"
#include "SpriteInfo.h"
#include "../../build/heart.h"
#include <nds/arm9/input.h>
#include <nds/arm9/sprite.h>
#include <nds/arm9/cache.h>
#include <assert.h>

extern u16 map[4096];

static const int BYTES_PER_16_COLOR_TILE = 32;
static const int COLORS_PER_PALETTE = 16;
static const int BOUNDARY_VALUE = 32; /* This is the default boundary value
                                           * (can be set in REG_DISPCNT) */
static const int OFFSET_MULTIPLIER = BOUNDARY_VALUE /
                                     sizeof(SPRITE_GFX[0]);


void spelunker::scroll(int bg_main, int bg_sub, int width, int height, LevelGenerator *l, u16 *fresh_map) {
    int keys_held = 0;
    int keys_down = 0;
    int sx = 0;
    int sy = 0;

    int timer = 0;
    int camera_timer = 0;


//    SpriteInfo *heartInfo = new SpriteInfo();
//    OAMTable *oam = new OAMTable();
//    initOAMTable(oam);
//    initSprites(oam, heartInfo);
//
    MainDude *mainDude = new MainDude();
    mainDude->x = 100;
    mainDude->y = 100;

    mainDude->init();

//    Hud *hud = new Hud();
//    hud->initHud();

    touchPosition touch;
    while (true) {
        scanKeys();

        timer += timerElapsed(0) / TICKS_PER_SECOND;
        camera_timer += timerElapsed(0) / TICKS_PER_SECOND;

        keys_held = keysHeld();
        keys_down = keysDown();

        if ((keys_held & KEY_B) ) {

            timer = 0;
            dmaCopyHalfWords(DMA_CHANNEL, fresh_map, map, sizeof(map));
//            l->newLayout(timerElapsed(0));
//            l->mapBackground();
//            l->mapFrame();
//            l->generateRooms();
            mainDude->clearTilesOnRight(l->mapTiles);
            fprintf(stdout, "GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG \n");

            l->tilesToMap();
            sectorize_map();
            dmaCopyHalfWords(DMA_CHANNEL, map, bgGetMapPtr(bg_main), sizeof(map));
            dmaCopyHalfWords(DMA_CHANNEL, map, bgGetMapPtr(bg_sub), sizeof(map));
        }

//        if (keys_held & KEY_X) sy -= 3;
//        if (keys_held & KEY_B) sy += 3;
//        if (keys_held & KEY_Y) sx -= 3;
//        if (keys_held & KEY_A) sx += 3;

//        sx = mainDude->x - 128;
//        sy = mainDude->y - 96;


        static int BOUNDARY_X = 32;
        static int BOUNDARY_Y = 16;

        int center_x = mainDude->x - 128;
        int center_y = mainDude->y - 96;


        //todo Bound sx/sy delta to mainDude.xSpeed/mainDude.ySpeed, or to value of (center-x/y - sx/y)

        //todo timer
//        if(camera_timer > 10) {

            if (abs(center_x - sx) > BOUNDARY_X) {
                if (center_x > sx)
                    sx += 1;
                else
                    sx -= 1;
            }

            if (abs(center_y - sy) > BOUNDARY_Y) {
                if (center_y > sy)
                    sy += 1;
                else
                    sy -= 1;
            }

//            camera_timer = 0;
//        }

        if (sx < 0) sx = 0;
        if (sx >= width - 256) sx = width - 1 - 256;
        if (sy < 0) sy = 0;
        if (sy >= height - 192 - 192) sy = height - 1 - 192 - 192;
/*

        if (sx < 0) sx = 0;
        if (sx >= width - 256) sx = width - 1 - 256;
        if (sy < 0) sy = 0;
        if (sy >= height - 192) sy = height - 1 - 192;
*/

        swiWaitForVBlank();
        //
        //updateOAM(oam);
        //
        mainDude->update(sx, sy, keys_held, keys_down, l);

        oamUpdate(&oamSub);
        oamUpdate(&oamMain);

        bgSetScroll(bg_main, sx, sy);
        bgSetScroll(bg_sub, sx, sy + 192);

        bgUpdate();

/*
        touchRead(&touch);
        if (Timer > 500) {
            if (touch.px != 0 && touch.py != 0) {
                dmaCopyHalfWords(DMA_CHANNEL, fresh_map, map, sizeof(map));
                l->clearCollidedTile(touch.px, touch.py, sx, sy, bg_main);
                sectorize_map();
                dmaCopyHalfWords(DMA_CHANNEL, map, bgGetMapPtr(bg_main), sizeof(map));
            }
            Timer = 0;
        }*/
    }
}

void spelunker::initOAMTable(OAMTable *oam) {
    /*
     * For all 128 sprites on the DS, disable and clear any attributes they
     * might have. This prevents any garbage from being displayed and gives
     * us a clean slate to work with.
     */
    for (int i = 0; i < SPRITE_COUNT; i++) {
        oam->oamBuffer[i].attribute[0] = ATTR0_DISABLED;
        oam->oamBuffer[i].attribute[1] = 0;
        oam->oamBuffer[i].attribute[2] = 0;
    }
    for (int i = 0; i < MATRIX_COUNT; i++) {
        /* If you look carefully, you'll see this is that affine trasformation
         * matrix again. We initialize it to the identity matrix, as we did
         * with backgrounds
         */
        oam->matrixBuffer[i].hdx = 1 << 8;
        oam->matrixBuffer[i].hdy = 0;
        oam->matrixBuffer[i].vdx = 0;
        oam->matrixBuffer[i].vdy = 1 << 8;
    }
    updateOAM(oam);
}

void spelunker::updateOAM(OAMTable *oam) {
    DC_FlushAll();
    dmaCopyHalfWords(3,
                     oam->oamBuffer,
                     OAM,
                     SPRITE_COUNT * sizeof(SpriteEntry));
}

void spelunker::initSprites(OAMTable *oam, SpriteInfo *spriteInfo) {

    /* Keep track of the available tiles */
    int nextAvailableTileIdx = 0;
//

    /* Create the ship sprite. */
    static const int SHUTTLE_OAM_ID = 0;
    assert(SHUTTLE_OAM_ID < SPRITE_COUNT);
    SpriteInfo *shuttleInfo = &spriteInfo[SHUTTLE_OAM_ID];
    SpriteEntry *shuttle = &oam->oamBuffer[SHUTTLE_OAM_ID];

    /* Initialize shuttleInfo */
    shuttleInfo->oamId = SHUTTLE_OAM_ID;
    shuttleInfo->width = 16;
    shuttleInfo->height = 16;
    shuttleInfo->angle = 462;
    shuttleInfo->entry = shuttle;
    /*
     *  Configure attribute 0.
     *
     *  OBJCOLOR_16 will make a 16-color sprite. We specify that we want an
     *  affine sprite (via isRotateScale) here because we would like to rotate
     *  the ship.
     */
    shuttle->y = 60;
    shuttle->isRotateScale = true;
    /* This assert is a check to see a matrix is available to store the affine
     * transformation matrix for this sprite. Of course, you don't have to have
     * the matrix id match the affine id, but if you do make them match, this
     * assert can be helpful. */
    assert(!shuttle->isRotateScale || (shuttleInfo->oamId < MATRIX_COUNT));
    shuttle->isSizeDouble = false;
//    shuttle->objMode = OBJMODE_NORMAL;
    shuttle->isMosaic = false;
    shuttle->colorMode = OBJCOLOR_256;
    shuttle->shape = OBJSHAPE_SQUARE;

    /*
     *  Configure attribute 1.
     *
     *  rotationIndex refers to the loation of affine transformation matrix. We
     *  set it to a location computed with a macro. OBJSIZE_64, in our case
     *  since we are making a square sprite, creates a 64x64 sprite.
     */
    shuttle->x = 50;
    shuttle->rotationIndex = shuttleInfo->oamId;
    shuttle->size = OBJSIZE_16;
    /*
     *  Configure attribute 2.
     *
     *  Configure which tiles the sprite will use, which priority layer it will
     *  be placed onto, which palette the sprite should use, and whether or not
     *  to show the sprite.
     */
    shuttle->gfxIndex = nextAvailableTileIdx;
    nextAvailableTileIdx += heartTilesLen / BYTES_PER_16_COLOR_TILE;
    shuttle->priority = OBJPRIORITY_0;
    shuttle->palette = shuttleInfo->oamId;

    dmaCopyHalfWords(3,
                     heartPal,
                     &SPRITE_PALETTE[shuttleInfo->oamId *
                                     COLORS_PER_PALETTE],
                     heartPalLen);
    dmaCopyHalfWords(3,
                     heartTiles,
                     &SPRITE_GFX[shuttle->gfxIndex * OFFSET_MULTIPLIER],
                     heartTilesLen);

}


