//
// Created by xdbeef on 18.03.18.
//

#ifndef SPELUNKYDS_COLLISIONS_H
#define SPELUNKYDS_COLLISIONS_H


#include "../level_layout/MapTile.h"

class Collisions {

public:
    static bool checkUpperCollision(MapTile *neighboringTiles[9], int *xPos, int *yPos, double *ySpeed, int width);

    static bool checkBottomCollision(MapTile *neighboringTiles[9], int *xPos, int *yPos, double *ySpeed, int width, int height);

    static bool checkLeftCollision(MapTile *neighboringTiles[9], int *xPos, int *yPos, double *xSpeed, int width, int height, bool jumping);

    static bool checkRightCollision(MapTile *neighboringTiles[9], int *xPos, int *yPos, double *xSpeed, int width, int height, bool jumping);

    static bool isStandingOnEdge(MapTile *neighboringTiles[9], int *xPos, int *yPos, double *xSpeed, int width, int height);


};


#endif //SPELUNKYDS_COLLISIONS_H
