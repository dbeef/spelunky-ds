//
// Created by xdbeef on 18.03.18.
//

#ifndef SPELUNKYDS_COLLISIONS_H
#define SPELUNKYDS_COLLISIONS_H


#include "../tiles/map_tile.h"

#define TILE_H 16
#define TILE_W 16

class Collisions {

public:

    //TODO Make equivalent functions that won't make x/y speed = 0 when collision == true (actually, just fix these)

    static bool checkUpperCollision(map_tile *neighboringTiles[9], int *xPos, int *yPos, double *ySpeed, int width, bool bounce);

    static bool
    checkBottomCollision(map_tile *neighboringTiles[9], int *xPos, int *yPos, double *ySpeed, int width, int height, bool bounce);

    static bool
    checkLeftCollision(map_tile *neighboringTiles[9], int *xPos, int *yPos, double *xSpeed, int width, int height, bool bounce);

    static bool
    checkRightCollision(map_tile *neighboringTiles[9], int *xPos, int *yPos, double *xSpeed, int width, int height, bool bounce);

    static bool isStandingOnLeftEdge(map_tile *neighboringTiles[9], int x, int width, int tileX);
    static bool isStandingOnRightEdge(map_tile *neighboringTiles[9], int x, int width, int tileX);

    static void getNeighboringTiles(map_tile *mapTiles[32][32], int xx, int yy, map_tile *out_neighboringTiles[9]);

    static void bombNeighboringTiles(map_tile *mapTiles[32][32], int xx, int yy);

    static void getCenterTile(int x_position, int y_position, int height, int width, int *x_tile, int *y_tile);

    static bool checkCollisionWithMainDude(int x, int y, int width, int height);

    static bool checkCollisionWithMainDudeWidthBoundary(int x, int y, int width, int height, int x_boundary);

    static bool checkCollisionWithMainDudeWhip(int x, int y, int width, int height);

    static bool checkCollisionBodies(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);
};


#endif //SPELUNKYDS_COLLISIONS_H
