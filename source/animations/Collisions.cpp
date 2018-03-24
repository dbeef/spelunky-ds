//
// Created by xdbeef on 18.03.18.
//
// TODO Add b64 encoded illustration for the following collisions.
#include <iostream>
#include "Collisions.h"
#include "MainDude.h"

#define TILE_H 16
#define TILE_W 16

//(TILE_W * a), (TILE_H * b) makes x,y point placed in the left-upper corner of the tile from passed *mapTiles[a][b].
//Sprite's x,y positions that are given to the following functions are expected to be upper-left corner.
//The x,y coordinates go from up to down on the y's, and from left to right on the x's - (0,0) is upper-left corner of
//the map.

//Collision from upper side of the tile, with rectangle given by x,y, and width.
bool Collisions::checkUpperCollision(MapTile *mapTiles[32][32], int *xPos, int *yPos, double *ySpeed, int width) {

    bool upperCollision = false;

    for (int x = 0; x < 32; x++) {
        for (int y = 0; y < 32; y++) {

            if (mapTiles[x][y] == 0 /*|| !mapTiles[x][y]->collidable*/)
                continue;

            if (!upperCollision) {
                bool w1 = (*yPos < (y * TILE_H) + TILE_H && (*yPos > (y * TILE_H)));
                bool w2 = (*xPos > (x * TILE_W) - width && (*xPos < (x * TILE_W) + TILE_W));
                upperCollision = w1 && w2;

                if (upperCollision) {
                    fprintf(stdout, "UPPER-SIDE TILE COLLISION \n");
                    *ySpeed = 0;
                    *yPos = (y * TILE_H) + TILE_H;
                }

            } else
                return true;
        }
    }
    return upperCollision;
}

//Collision from bottom side of the tile, with rectangle given by x,y, width and height.
bool Collisions::checkBottomCollision(MapTile *mapTiles[32][32], int *xPos, int *yPos, double *ySpeed, int width,
                                      int height) {

    bool bottomCollision = false;

    for (int x = 0; x < 32; x++) {
        for (int y = 0; y < 32; y++) {

            if (mapTiles[x][y] == 0 /*|| !mapTiles[x][y]->collidable*/)
                continue;

            if (!bottomCollision) {
                bool w1 = (*xPos > (x * TILE_W) - width * 0.75 && *xPos < (x * TILE_W) + width * 0.75);
                bool w2 = (*yPos <= y * TILE_H) && *yPos + height >= (y * TILE_H);

                bottomCollision = w1 && w2;

                if (bottomCollision) {
                    *ySpeed = 0;
                    *yPos = (y * TILE_H) - TILE_H;
                }
            } else
                return true;
        }
    }

    return bottomCollision;
}

//Collision from left side of the tile, with rectangle given by x,y, width and height.
bool
Collisions::checkLeftCollision(MapTile *mapTiles[32][32], int *xPos, int *yPos, double *xSpeed, int width, int height) {

    bool leftCollision = false;

    for (int x = 0; x < 32; x++) {
        for (int y = 0; y < 32; y++) {

            if (mapTiles[x][y] == 0 /*|| !mapTiles[x][y]->collidable*/)
                continue;

            if (!leftCollision) {
                bool w1 = (*yPos > (y * TILE_H) - height && (*yPos < (y * TILE_H) + TILE_H));
                bool w2 = (*xPos < (x * TILE_W) - 0.75 * width && (*xPos > (x * TILE_W) - width));
                leftCollision = w1 && w2;

                if (leftCollision) {
                    fprintf(stdout, "LEFT-SIDE TILE COLLISION \n");
                    *xSpeed = 0;
                    *xPos = (x * TILE_W) - TILE_W;
                }

            } else return true;
        }
    }

    return leftCollision;
}

//Collision from right side of the tile, with rectangle given by x,y, width and height.
bool Collisions::checkRightCollision(MapTile *mapTiles[32][32], int *xPos, int *yPos, double *xSpeed, int width,
                                     int height) {

    bool rightCollision = false;

    for (int x = 0; x < 32; x++) {
        for (int y = 0; y < 32; y++) {

            if (mapTiles[x][y] == 0 /*|| !mapTiles[x][y]->collidable*/)
                continue;


            if (!rightCollision) {
                bool w1 = (*yPos > (y * TILE_H) - height && (*yPos < (y * TILE_H) + TILE_H));
                bool w2 = (*xPos < (x * TILE_W) + width && (*xPos > (x * TILE_W) + 0.75 * width));
                rightCollision = w1 && w2;

                if (rightCollision) {
                    fprintf(stdout, "RIGHT-SIDE TILE COLLISION \n");
                    *xSpeed = 0;
                    *xPos = (x * TILE_W) + TILE_W;
                }

            } else
                return true;
        }
    }
    return rightCollision;
}

bool Collisions::isStandingOnEdge(MapTile *mapTiles[32][32], int *xPos, int *yPos, double *xSpeed, int width,
                                  int height) {

    bool standingOnEdge = false;

    for (int x = 0; x < 32; x++) {
        for (int y = 0; y < 32; y++) {

            if (mapTiles[x][y] == 0 /*|| !mapTiles[x][y]->collidable*/)
                continue;


            if (!standingOnEdge) {

                if (x < 32) {

                    bool w0 = mapTiles[x + 1][y] == 0;
                    bool w1 = (*yPos <= y * TILE_H) && *yPos + height >= (y * TILE_H);
                    bool w2 = (*xPos >= (x * TILE_W)) && (*xPos < (x * TILE_W) + TILE_W);
                    standingOnEdge = w0 && w1 && w2;

                    if (standingOnEdge) {
//                        std::cout << "STANDING ON EDGE - RIGHT" << '\n';
                    }
                }

                if (x > 0) {

                    bool w0 = mapTiles[x - 1][y] == 0;
                    bool w1 = (*yPos <= y * TILE_H) && *yPos + height >= (y * TILE_H);
                    bool w2 = (*xPos <= (x * TILE_W)) && (*xPos > (x * TILE_W) - width);
                    standingOnEdge = w0 && w1 && w2;
                    if (standingOnEdge) {
//                        std::cout << "STANDING ON EDGE - LEFT" << '\n';
                    }

                }


            } else
                return true;
        }
    }
    return standingOnEdge;
}
