//
// Created by xdbeef on 04.03.18.
//

#ifndef SPELUNKYDS_LEVELGENERATOR_H
#define SPELUNKYDS_LEVELGENERATOR_H

#define SPLASH_SCREEN_WIDTH 16
#define SPLASH_SCREEN_HEIGHT 12

#include <cstdlib>
#include <iostream>
#include <nds.h>
#include <nds/arm9/background.h>
#include <cstring>
#include "MapTile.h"

#define ROOMS_X 3
#define ROOMS_Y 3

class LevelGenerator {

public:

    MapTile *mapTiles[32][32];
    int layout[ROOMS_X][ROOMS_Y];

    void newLayout(int seed);

    void tilesToMap();

    void generateFrame();

    void generateRooms();

    void generateSplashScreen(int room_type);

    void matchTile(MapTile *t, int value);

    void getFirstTile(MapTileType mapTileType, MapTile *&m);
};

#endif //SPELUNKYDS_LEVELGENERATOR_H

