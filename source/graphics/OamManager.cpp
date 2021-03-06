//
// Created by xdbeef on 01.04.18.
//

#include <nds.h>
#include <nds/arm9/cache.h>
#include "OamManager.hpp"
#include "../camera/LayerLevel.hpp"
#include "../preprocessor/Debug.h"
#include "../GameState.hpp"

/**
 * "Another advantage of using 16-color sprites is the ability to use 16 different palettes."
 * https://patater.com/files/projects/manual/manual.html
 * There's a maximum of 4 bit palettes and it is 16!
 */

/**
 * When there's a sprite that can be disposed, clear all sprites in oam manager by calling clearAllSprites,
 * then call loadOAM on every sprite in GameState::instance().sprites
 */
static const int BYTES_PER_16_COLOR_TILE = 32;
static const int COLORS_PER_PALETTE = 16;

OAMManager *OAMManager::_main_instance = nullptr;
OAMManager *OAMManager::_sub_instance = nullptr;

void OAMManager::init() {
    SPELUNKYDS_BREAKING_ASSERT(!_main_instance);
    SPELUNKYDS_BREAKING_ASSERT(!_sub_instance);
    _main_instance = new OAMManager(OamType::MAIN);
    _sub_instance = new OAMManager(OamType::SUB);
    SPELUNKYDS_BREAKING_ASSERT(_main_instance);
    SPELUNKYDS_BREAKING_ASSERT(_sub_instance);
}

void OAMManager::dispose() {
    SPELUNKYDS_BREAKING_ASSERT(_main_instance);
    SPELUNKYDS_BREAKING_ASSERT(_sub_instance);
    delete _main_instance;
    delete _sub_instance;
}

void OAMManager::clear_sprite_attributes() {

    current_oam_id_palette = 0;
    current_oam_id_tiles = 0;
    nextAvailableTileIdx = 0;

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

    update();
}

void OAMManager::update() {
    DC_FlushAll();
    dmaCopyHalfWords(3, oam->oamBuffer, oam_address, SPRITE_COUNT * sizeof(SpriteEntry));
}

SpriteInfo *
OAMManager::initSprite(const unsigned short pallette[], int palLen, const unsigned int tiles[], int tilesLen,
                       ObjSize size, SpritesheetType type, bool reuse_palette, bool reuse_tiles, LAYER_LEVEL l) {

    SPELUNKYDS_ASSERT(current_oam_id_palette < SPRITE_COUNT && current_oam_id_tiles < SPRITE_COUNT);

    auto *spriteInfo = new SpriteInfo();
    SpriteEntry *spriteEntry = &oam->oamBuffer[current_oam_id_tiles];

    /* Initialize spriteInfo */
    spriteInfo->oamType = _oam_type;
    spriteInfo->spriteType = type;
    spriteInfo->offset_multiplier = this->offset_multiplier;
    spriteInfo->sprite_address = this->sprite_address;
    spriteInfo->oamId_tiles = current_oam_id_tiles;
    spriteInfo->oamId_palette = current_oam_id_palette;
    spriteInfo->width = size;
    spriteInfo->height = size;
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
    spriteEntry->isRotateScale = false;
    /* This assert is a check to see a matrix is available to store the affine
     * transformation matrix for this sprite. Of course, you don't have to have
     * the matrix id match the affine id, but if you do make them match, this
     * assert can be helpful. */

    SPELUNKYDS_ASSERT(!spriteEntry->isRotateScale || (spriteInfo->oamId_tiles < MATRIX_COUNT))

    spriteEntry->isSizeDouble = false;
    spriteEntry->isMosaic = false;
    spriteEntry->colorMode = OBJCOLOR_16;
    spriteEntry->shape = OBJSHAPE_SQUARE;
    spriteEntry->priority = OBJPRIORITY_0;

    switch (l) {
        case (LAYER_LEVEL::BOTTOM):
            spriteEntry->priority = OBJPRIORITY_3;
            break;
        case (LAYER_LEVEL::MIDDLE_BOT):
            spriteEntry->priority = OBJPRIORITY_2;
            break;
        case (LAYER_LEVEL::MIDDLE_TOP):
            spriteEntry->priority = OBJPRIORITY_1;
            break;
        case (LAYER_LEVEL::TOP):
            spriteEntry->priority = OBJPRIORITY_0;
            break;
        default:
            spriteEntry->priority = OBJPRIORITY_0;
            break;
    }

    /*
     *  Configure attribute 1.
     *
     *  rotationIndex refers to the loation of affine transformation matrix. We
     *  set it to a location computed with a macro. OBJSIZE_64, in our case
     *  since we are making a square sprite, creates a 64x64 sprite.
     */

    spriteEntry->x = 50;
    spriteEntry->rotationIndex = spriteInfo->oamId_tiles;
    spriteEntry->size = size;

    /*
     *  Configure attribute 2.
     *
     *  Configure which tiles the sprite will use, which priority layer it will
     *  be placed onto, which palette the sprite should use, and whether or not
     *  to show the sprite.
     */
    if (reuse_palette || reuse_tiles) {
        //Re-using already loaded palletes / tiles
        for (unsigned long a = 0; a < GameState::instance().sprite_infos.size(); a++) {
            if (GameState::instance().sprite_infos.at(a)) {

                if ((*GameState::instance().sprite_infos.at(a)).spriteType == type &&
                    (*GameState::instance().sprite_infos.at(a)).oamType == _oam_type) {

                    if (reuse_palette) {
                        spriteInfo->oamId_palette = (*GameState::instance().sprite_infos.at(a)).oamId_palette;
                        spriteEntry->palette = (*GameState::instance().sprite_infos.at(a)).oamId_palette;
                    }
                    if (reuse_tiles) {
                        spriteEntry->gfxIndex = (*(*GameState::instance().sprite_infos.at(a)).entry).gfxIndex;
                    }
                    break;
                }
            }
        }

    }

    if (!spriteEntry->gfxIndex) {
        spriteEntry->gfxIndex = nextAvailableTileIdx;
        nextAvailableTileIdx += tilesLen / BYTES_PER_16_COLOR_TILE;
        dmaCopyHalfWords(3, tiles, &sprite_address[spriteEntry->gfxIndex * this->offset_multiplier], tilesLen);
    }


    if (!spriteEntry->palette) {


        spriteEntry->palette = spriteInfo->oamId_palette;
        spriteInfo->oamId_palette = current_oam_id_palette;

        dmaCopyHalfWords(3,
                         pallette,
                         &palette_address[spriteInfo->oamId_palette *
                                          COLORS_PER_PALETTE],
                         palLen);

        current_oam_id_palette++;

    }

    spriteInfo->oam_address = *oam_address;

    GameState::instance().sprite_infos.push_back(spriteInfo);

    current_oam_id_tiles++;

    return spriteInfo;
}

OAMManager::OAMManager(OamType oam_type) : _oam_type(oam_type) {

    const constexpr int BOUNDARY_VALUE = 64; /* This is the default boundary value (can be set in REG_DISPCNT) */
    const constexpr int OFFSET_MULTIPLIER_MAIN = BOUNDARY_VALUE / sizeof(SPRITE_GFX[0]);
    const constexpr int OFFSET_MULTIPLIER_SUB = BOUNDARY_VALUE / sizeof(SPRITE_GFX_SUB[0]);

    switch (_oam_type) {
        case OamType::MAIN: {
            this->sprite_address = SPRITE_GFX;
            this->palette_address = SPRITE_PALETTE;
            this->oam_address = OAM;
            this->offset_multiplier = OFFSET_MULTIPLIER_MAIN;
            break;
        }
        case OamType::SUB: {
            this->sprite_address = SPRITE_GFX_SUB;
            this->palette_address = SPRITE_PALETTE_SUB;
            this->oam_address = OAM_SUB;
            this->offset_multiplier = OFFSET_MULTIPLIER_SUB;
            break;
        }
    }

    oam = new OAMTable();
    clear_sprite_attributes();
    update();
}

void OAMManager::init_sprite_hardware() {
    //Initialise sprite graphics
    oamInit(&oamMain, SpriteMapping_1D_64, false);
    oamInit(&oamSub, SpriteMapping_1D_64, false);
}
