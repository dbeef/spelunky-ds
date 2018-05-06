//
// Created by xdbeef on 04.03.18.
//

#ifndef SPELUNKYDS_LEVELGENERATOR_H
#define SPELUNKYDS_LEVELGENERATOR_H


#include <cstdlib>
#include <iostream>
#include <nds.h>
#include <nds/arm9/background.h>
#include <cstring>
#include "map_tile.h"

#define ROOMS_X 3
#define ROOMS_Y 3

#define SPLASH_SCREEN_WIDTH 16
#define SPLASH_SCREEN_HEIGHT 12

#define ROOM_TILE_WIDTH_SPLASH_SCREEN 16
#define ROOM_TILE_HEIGHT_SPLASH_SCREEN 12

#define ROOM_TILE_WIDTH_GAME 10
#define ROOM_TILE_HEIGHT_GAME 10

#define MAP_GAME_HEIGHT_TILES 32
#define MAP_GAME_WIDTH_TILES 32

#define LINE_WIDTH 64
//Offset of 2 tiles, 8 px each
#define OFFSET_X 2
//Offset of 128 tiles, 8px  each (2 lines, 64 each)
#define OFFSET_Y 128


class LevelGenerator {

public:

    MapTile *mapTiles[32][32];
    int layout_room_types[ROOMS_X][ROOMS_Y];
    int layout_room_ids[ROOMS_X][ROOMS_Y];

    void newLayout(int seed);

    void tilesToMap();

    void generateFrame();

    void generateRooms();

    void generateSplashScreen(int room_type);

    void matchTile(MapTile *t, int value);

    void getFirstTile(map_tile_type mapTileType, MapTile *&m);
};

#endif //SPELUNKYDS_LEVELGENERATOR_H

