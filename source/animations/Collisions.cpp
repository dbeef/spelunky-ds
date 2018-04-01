//
// Created by xdbeef on 18.03.18.
//
#include <iostream>
#include "Collisions.h"
#include "MainDude.h"
#include "../level_layout/MapUtils.h"

#define TILE_H 16
#define TILE_W 16

//(TILE_W * a), (TILE_H * b) makes x,y point placed in the left-upper corner of the tile from passed *mapTiles[a][b].
//Sprite's x,y positions that are given to the following functions are expected to be upper-left corner.
//The x,y coordinates go from up to down on the y's, and from left to right on the x's - (0,0) is upper-left corner of
//the map.

//Collision from upper side of the tile, with rectangle given by x,y, and width.
bool Collisions::checkUpperCollision(MapTile *neighboringTiles[9], int *xPos, int *yPos, double *ySpeed, int width) {

    bool upperCollision = false;
    bool w1;
    bool w2;

    for (int a = 0; a < 9; a++) {

        if (neighboringTiles[a] == 0)
            continue;

        if (!upperCollision) {
            w1 = (*yPos < (neighboringTiles[a]->y * TILE_H) + TILE_H && (*yPos > (neighboringTiles[a]->y * TILE_H)));
            w2 = (*xPos > (neighboringTiles[a]->x * TILE_W) - width &&
                  (*xPos < (neighboringTiles[a]->x * TILE_W) + TILE_W));
            upperCollision = w1 && w2;

            if (upperCollision) {
                *ySpeed = 0;
                *yPos = (neighboringTiles[a]->y * TILE_H) + TILE_H;
            }

        } else {
            return true;
        }
    }

    return upperCollision;
}

//Collision from bottom side of the tile, with rectangle given by x,y, width and height.
bool Collisions::checkBottomCollision(MapTile *neighboringTiles[9], int *xPos, int *yPos, double *ySpeed, int width,
                                      int height) {

    bool bottomCollision = false;
    bool w1;
    bool w2;

    for (int a = 0; a < 9; a++) {

        if(neighboringTiles[a] == 0)
            continue;

        if (!bottomCollision) {
            w1 = (*xPos > (neighboringTiles[a]->x * TILE_W) - width * 0.75 &&
                  *xPos < (neighboringTiles[a]->x * TILE_W) + width * 0.75);
            w2 = (*yPos <= neighboringTiles[a]->y * TILE_H) && *yPos + height >= (neighboringTiles[a]->y * TILE_H);

            bottomCollision = w1 && w2;

            if (bottomCollision) {
                *ySpeed = 0;
                *yPos = (neighboringTiles[a]->y * TILE_H) - TILE_H;
            }
        } else
            return true;
    }

    return bottomCollision;
}

//Collision from left side of the tile, with rectangle given by x,y, width and height.
bool Collisions::checkLeftCollision(MapTile *neighboringTiles[9], int *xPos, int *yPos, double *xSpeed, int width,
                               int height) {

    bool leftCollision = false;
    bool w1;
    bool w2;

    for (int a = 0; a < 9; a++) {

        if(neighboringTiles[a] == 0)
            continue;

        if (!leftCollision) {

            w2 = (*xPos < (neighboringTiles[a]->x * TILE_W) - 0.75 * width &&
                  (*xPos > (neighboringTiles[a]->x * TILE_W) - width));
            w1 = (*yPos > (neighboringTiles[a]->y * TILE_H) - height &&
                  (*yPos < (neighboringTiles[a]->y * TILE_H) + TILE_H));

            leftCollision = w1 && w2;

            if (leftCollision) {
                *xSpeed = 0;
                *xPos = (neighboringTiles[a]->x * TILE_W) - TILE_W;
            }

        } else {
            return true;
        };
    }

    return leftCollision;
}

//Collision from right side of the tile, with rectangle given by x,y, width and height.
bool Collisions::checkRightCollision(MapTile *neighboringTiles[9], int *xPos, int *yPos, double *xSpeed, int width,
                                     int height) {
    bool rightCollision = false;
    bool w1;
    bool w2;
    for (int a = 0; a < 9; a++) {

        if(neighboringTiles[a] == 0)
            continue;

        if (!rightCollision) {

            w2 = (*xPos < (neighboringTiles[a]->x * TILE_W) + width &&
                  (*xPos > (neighboringTiles[a]->x * TILE_W) + 0.75 * width));

            w1 = (*yPos > (neighboringTiles[a]->y * TILE_H) - height &&
                  (*yPos < (neighboringTiles[a]->y * TILE_H) + TILE_H));

            rightCollision = w1 && w2;

            if (rightCollision) {
                *xSpeed = 0;
                *xPos = (neighboringTiles[a]->x * TILE_W) + TILE_W;
            }

        } else {
            return true;
        }
    }

    return rightCollision;
}

//fixme
bool Collisions::isStandingOnEdge(MapTile *neighboringTiles[9], int *xPos, int *yPos, double *xSpeed, int width,
                                  int height) {

    bool standingOnEdge = false;
    bool w0 = false;
    bool w1 = false;
    bool w2 = false;

    for (int a = 0; a < 32; a++) {

        if (neighboringTiles[a] == 0 /*|| !mapTiles[x][y]->collidable*/)
            continue;


        if (!standingOnEdge) {
            return false;
/*
                if (x < 32) {

                    w0 = mapTiles[x + 1][y] == 0;
                    w1 = (*yPos <= y * TILE_H) && *yPos + height >= (y * TILE_H);
                    w2 = (*xPos >= (x * TILE_W)) && (*xPos < (x * TILE_W) + TILE_W);
                    standingOnEdge = w0 && w1 && w2;

                    if (standingOnEdge) {
                        std::cout << "STANDING ON EDGE - RIGHT" << '\n';
                    }
                }

                if (x > 0) {

                    w0 = mapTiles[x - 1][y] == 0;
                    w1 = (*yPos <= y * TILE_H) && *yPos + height >= (y * TILE_H);
                    w2 = (*xPos <= (x * TILE_W)) && (*xPos > (x * TILE_W) - width);
                    standingOnEdge = w0 && w1 && w2;
                    if (standingOnEdge) {
                        std::cout << "STANDING ON EDGE - LEFT" << '\n';
                    }

                }*/


        } else
            return true;
    }
    return standingOnEdge;
}
