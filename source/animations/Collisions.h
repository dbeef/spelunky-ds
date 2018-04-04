//
// Created by xdbeef on 18.03.18.
//

#ifndef SPELUNKYDS_COLLISIONS_H
#define SPELUNKYDS_COLLISIONS_H


#define TILE_H 16
#define TILE_W 16

#include "../level_layout/MapTile.h"

class Collisions {

public:
    static bool checkUpperCollision(MapTile *neighboringTiles[9], int *xPos, int *yPos, double *ySpeed, int width);

    static bool checkBottomCollision(MapTile *neighboringTiles[9], int *xPos, int *yPos, double *ySpeed, int width, int height);

    static bool checkLeftCollision(MapTile *neighboringTiles[9], int *xPos, int *yPos, double *xSpeed, int width, int height);

    static bool checkRightCollision(MapTile *neighboringTiles[9], int *xPos, int *yPos, double *xSpeed, int width, int height);

    static bool isStandingOnEdge(MapTile *neighboringTiles[9], int *xPos, int *yPos, double *xSpeed, int width, int height);

    static void getNeighboringTiles(MapTile *mapTiles[32][32], int xx, int yy, MapTile *neighboringTiles[9]);

    static void getCenterTile(int x_position, int y_position, int height, int width, int *x_tile, int *y_tile);
};


#endif //SPELUNKYDS_COLLISIONS_H
