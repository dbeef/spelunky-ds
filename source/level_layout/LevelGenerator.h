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
#include "MapTile.h"

#define ROOMS_X 3
#define ROOMS_Y 3

class LevelGenerator {

public:

    MapTile *mapTiles[32][32];
    int layout[ROOMS_X][ROOMS_Y];

    void mapBackground();

    void newLayout(int seed);

    void tilesToMap();

    void mapFrame();

    void generateRooms();

    void clearCollidedTile(int x, int y, int camera_x, int camera_y, int bg);
};

//0 - nothing (background)
//1 - just regular tile
//2 - tile up-oriented
//3 - tile down-oriented
//4 - tile both up and down oriented
//5 - block
//6 - tile with gold
//7 - tile with more gold
//8 - regular tile with rock

static int tab_0[10][10] = {
        {1, 3, 3, 3, 3, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 3, 6, 1, 3, 7},
        {8, 0, 0, 0, 0, 0, 3, 1, 0, 6},
        {1, 0, 0, 0, 0, 0, 0, 3, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {6, 0, 0, 0, 0, 0, 0, 0, 0, 8},
        {6, 0, 0, 0, 0, 0, 5, 0, 0, 1},
        {8, 0, 2, 2, 2, 0, 5, 0, 0, 1},
        {1, 2, 7, 1, 8, 2, 2, 0, 2, 6},
        {1, 1, 1, 1, 1, 1, 1, 2, 1, 6},
};

static int tab_1[10][10] = {
        {3, 3, 3, 3, 3, 3, 3, 3, 3, 1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 2, 0, 0, 0, 0, 0, 0},
        {2, 2, 2, 1, 2, 2, 0, 0, 0, 5},
        {3, 6, 1, 3, 0, 0, 0, 0, 2, 2},
        {0, 3, 3, 0, 0, 0, 0, 4, 7, 3},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 2, 2, 0, 0, 0, 0},
        {0, 2, 2, 2, 1, 1, 2, 0, 0, 0},
        {2, 1, 7, 6, 8, 1, 1, 2, 2, 2},
};

static int tab_2[10][10] = {

        {3, 3, 3, 3, 3, 3, 3, 3, 3, 7},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 3},
        {0, 0, 0, 2, 0, 0, 0, 0, 0, 0},
        {2, 2, 2, 1, 4, 4, 0, 0, 0, 0},
        {3, 1, 1, 3, 0, 0, 0, 0, 2, 2},
        {0, 3, 3, 0, 0, 0, 0, 4, 3, 8},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 4, 4, 0, 0, 0, 0},
        {0, 5, 0, 0, 0, 0, 0, 0, 0, 0},
        {2, 2, 0, 0, 0, 0, 0, 0, 2, 2},
};

static int tab_3[10][10] = {
        {3, 3, 0, 0, 0, 0, 0, 0, 0, 6},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 3},
        {0, 0, 0, 4, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 2, 2, 4, 0, 0, 0, 0, 2, 2},
        {0, 3, 3, 0, 0, 0, 0, 0, 3, 3},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 4, 4, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 5, 0, 0},
        {2, 2, 2, 2, 2, 2, 2, 2, 2, 2}
};


#endif //SPELUNKYDS_LEVELGENERATOR_H

