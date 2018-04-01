//
// Created by xdbeef on 01.04.18.
//

#include <nds.h>
#include <nds/arm9/cache.h>
#include <assert.h>
#include "OAMManager.h"

static const int BYTES_PER_16_COLOR_TILE = 32;
static const int COLORS_PER_PALETTE = 16;
static const int BOUNDARY_VALUE = 32; /* This is the default boundary value (can be set in REG_DISPCNT) */
static const int OFFSET_MULTIPLIER = BOUNDARY_VALUE / sizeof(SPRITE_GFX[0]);

void OAMManager::initOAMTable() {
    oam = new OAMTable();
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
    updateOAM();
}

void OAMManager::updateOAM() {
    DC_FlushAll();
    dmaCopyHalfWords(3,
                     oam->oamBuffer,
                     OAM,
                     SPRITE_COUNT * sizeof(SpriteEntry));
}

SpriteInfo *OAMManager::initSprite(const unsigned short pallette[], int palLen, const unsigned int tiles[], int tilesLen) {

    /* Keep track of the available tiles */

    /* Create the ship sprite. */
    
    assert(current_oam_id < SPRITE_COUNT);
    SpriteInfo *spriteInfo = new SpriteInfo();/*&spriteInfo[current_oam_id];*/
    SpriteEntry *spriteEntry = &oam->oamBuffer[current_oam_id];

    /* Initialize spriteInfo */
    spriteInfo->oamId = current_oam_id;
    spriteInfo->width = 16;
    spriteInfo->height = 16;
    spriteInfo->angle = 462;
    spriteInfo->entry = spriteEntry;
    /*
     *  Configure attribute 0.
     *
     *  OBJCOLOR_16 will make a 16-color sprite. We specify that we want an
     *  affine sprite (via isRotateScale) here because we would like to rotate
     *  the ship.
     */
    spriteEntry->y = 60;
    spriteEntry->isRotateScale = true;
    /* This assert is a check to see a matrix is available to store the affine
     * transformation matrix for this sprite. Of course, you don't have to have
     * the matrix id match the affine id, but if you do make them match, this
     * assert can be helpful. */
    assert(!spriteEntry->isRotateScale || (spriteInfo->oamId < MATRIX_COUNT));
    spriteEntry->isSizeDouble = false;
//    spriteEntry->objMode = OBJMODE_NORMAL;
    spriteEntry->isMosaic = false;
    spriteEntry->colorMode = OBJCOLOR_256;
    spriteEntry->shape = OBJSHAPE_SQUARE;

    /*
     *  Configure attribute 1.
     *
     *  rotationIndex refers to the loation of affine transformation matrix. We
     *  set it to a location computed with a macro. OBJSIZE_64, in our case
     *  since we are making a square sprite, creates a 64x64 sprite.
     */
    spriteEntry->x = 50;
    spriteEntry->rotationIndex = spriteInfo->oamId;
    spriteEntry->size = OBJSIZE_16;
    /*
     *  Configure attribute 2.
     *
     *  Configure which tiles the sprite will use, which priority layer it will
     *  be placed onto, which palette the sprite should use, and whether or not
     *  to show the sprite.
     */
    spriteEntry->gfxIndex = current_oam_id;
    current_oam_id += tilesLen / BYTES_PER_16_COLOR_TILE;
    spriteEntry->priority = OBJPRIORITY_0;
    spriteEntry->palette = spriteInfo->oamId;

    //In case of animations, copy only the current frame
    dmaCopyHalfWords(3,
                     pallette,
                     &SPRITE_PALETTE[spriteInfo->oamId *
                                     COLORS_PER_PALETTE],
                     palLen);
    dmaCopyHalfWords(3,
                     tiles,
                     &SPRITE_GFX[spriteEntry->gfxIndex * OFFSET_MULTIPLIER],
                     tilesLen);

    sprites[current_oam_id] = spriteInfo;
    return spriteInfo;
}
