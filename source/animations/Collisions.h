//
// Created by xdbeef on 18.03.18.
//

#ifndef SPELUNKYDS_COLLISIONS_H
#define SPELUNKYDS_COLLISIONS_H


#include "../level_layout/MapTile.h"

class Collisions {

public:
    static bool checkUpperCollision(MapTile *mapTiles[32][32], int *xPos, int *yPos, double *ySpeed, int width);

    static bool checkBottomCollision(MapTile *mapTiles[32][32], int *xPos, int *yPos, double *ySpeed, int width, int height);

    static bool checkLeftCollision(MapTile *mapTiles[32][32], int *xPos, int *yPos, double *xSpeed, int width, int height);

    static bool checkRightCollision(MapTile *mapTiles[32][32], int *xPos, int *yPos, double *xSpeed, int width, int height);

    static bool isStandingOnEdge(MapTile *mapTiles[32][32], int *xPos, int *yPos, double *xSpeed, int width, int height);


};


#endif //SPELUNKYDS_COLLISIONS_H
