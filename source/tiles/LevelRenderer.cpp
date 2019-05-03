//
// Created by dbeef on 5/3/19.
//

#include "LevelRenderer.hpp"
#include "../GameState.hpp"
#include "BaseMap.hpp"
#include "../../build/gfx_cavebg.h"

LevelRenderer* LevelRenderer::_instance = nullptr;

void LevelRenderer::init() {
    SPELUNKYDS_BREAKING_ASSERT(!_instance);
    _instance = new LevelRenderer();
    SPELUNKYDS_BREAKING_ASSERT(_instance);
}

void LevelRenderer::dispose() {
    SPELUNKYDS_BREAKING_ASSERT(_instance);
    delete _instance;
}

/**
 * Copies clean cave background to the map, then writes current tiles from map_tiles[32][32] to the map,
 * formats it in a way that makes it understandable by the graphics engines and copies it to the place
 * the engines expect the map to be.
 * One should call this function, when he knows that the map changed,
 * i.e a bomb exploded or he moved from main menu to game level.
 */
void LevelRenderer::render() {
    write_cave_background_to_map();
    write_tiles_to_map(); //now we write our tiles onto the current map
    sectorize_map(); //sectorizing map, so it would be correctly rendered
    copy_current_map_to_engines(); //and copy the map to both main and sub screen engine
}

void LevelRenderer::write_cave_background_to_map() {
    //copy the base map to the current map, which means we start the current map with only the cave background
    dmaCopyHalfWords(DEFAULT_DMA_CHANNEL, base_map, current_map, sizeof(base_map));
}

void LevelRenderer::copy_current_map_to_engines() {
    dmaCopyHalfWords(DEFAULT_DMA_CHANNEL, current_map, bgGetMapPtr(_main_background_id),
                     sizeof(current_map));
    dmaCopyHalfWords(DEFAULT_DMA_CHANNEL, current_map, bgGetMapPtr(_sub_background_id),
                     sizeof(current_map));
}

/**
 * Writes bytes that make a graphical representation of every MapTile in map_tiles array to the current map.
 */
void LevelRenderer::write_tiles_to_map() {
    for (int x = 0; x < MAP_GAME_WIDTH_TILES; x++) {
        for (int y = 0; y < MAP_GAME_HEIGHT_TILES; y++) {
            MapTile *t = Level::instance().map_tiles[x][y];
            if (t->exists)
                for (int k = 0; k < 4; k++)
                    current_map[t->map_index[k]] = t->values[k];
        }
    }
}

//https://stackoverflow.com/questions/11072244/c-multiple-definitions-of-a-variable
void LevelRenderer::sectorize_map() {

    dmaCopyHalfWords(3, current_map, temp_map, sizeof(current_map));

    int b = 0;

    for (int a = 0; a < 2048; a++) {

        if (a % 64 >= 32)
            a += 32;

        if (a < 2048) {
            current_map[b] = temp_map[a];
            b++;
        }
    }

    for (int a = 0; a < 2048; a++) {

        if (a % 64 <= 32)
            a += 32;

        if (a < 2048) {
            current_map[b] = temp_map[a];
            b++;
        }
    }


    for (int a = 2048; a < 4096; a++) {

        if (a % 64 >= 32)
            a += 32;

        if (a < 4096) {
            current_map[b] = temp_map[a];
            b++;
        }

    }

    for (int a = 2048; a < 4096; a++) {

        if (a % 64 <= 32)
            a += 32;

        if (a < 4096) {
            current_map[b] = temp_map[a];
            b++;
        }
    }

}

void LevelRenderer::init_background_hardware() {
    // TODO: Decouple Level state from tile rendering logics and put these
    //   calls in TileRenderer constructor or whatever class will come out of this decoupling.
    //https://devkitpro.org/viewtopic.php?f=38&t=8720
    // OBJPRIORITY_0 > BG_PRIORITY_0 >
    // OBJPRIORITY_1 > BG_PRIORITY_1 >
    // OBJPRIORITY_2 > BG_PRIORITY_2 >
    // OBJPRIORITY_3 > BG_PRIORITY_3.
    _main_background_id = bgInit(BG_PRIORITY_3, BgType_Text8bpp, BgSize_B8_512x512, 22, 4);
    _sub_background_id = bgInitSub(BG_PRIORITY_3, BgType_Text8bpp, BgSize_B8_512x512, 18, 4);

    bgSetPriority(LevelRenderer::instance()._main_background_id, BG_PRIORITY_3);
    bgSetPriority(LevelRenderer::instance()._sub_background_id, BG_PRIORITY_3);
}

void LevelRenderer::copy_background_tiles() {
    //Copy cave background tiles to the graphics memory
    dmaCopy(gfx_cavebgTiles, bgGetGfxPtr(LevelRenderer::instance()._main_background_id), sizeof(gfx_cavebgTiles));
    dmaCopy(gfx_cavebgTiles, bgGetGfxPtr(LevelRenderer::instance()._sub_background_id), sizeof(gfx_cavebgTiles));
}

void LevelRenderer::copy_background_palette() {
    //Copy cave background palette to the graphics memory
    dmaCopy(gfx_cavebgPal, BG_PALETTE, gfx_cavebgPalLen);
    dmaCopy(gfx_cavebgPal, BG_PALETTE_SUB, gfx_cavebgPalLen);
}
