//
// Created by xdbeef on 18.03.18.
//
#include <iostream>
#include "Collisions.h"
#include "MainDude.h"
#include "../level_layout/MapUtils.h"
#include "../Globals.h"

#define BOUNCING_FACTOR_X 0.15
#define BOUNCING_FACTOR_Y 0.45
#define BOUNCING_STOP_SPEED 0.05


//(TILE_W * a), (TILE_H * b) makes x,y point placed in the left-upper corner of the tile from passed *mapTiles[a][b].
//Sprite's x,y positions that are given to the following functions are expected to be upper-left corner.
//The x,y coordinates go from up to down on the y's, and from left to right on the x's - (0,0) is upper-left corner of
//the map.

//Collision from upper side of the tile, with rectangle given by x,y, and width.
bool Collisions::checkUpperCollision(MapTile *neighboringTiles[9], int *xPos, int *yPos, double *ySpeed, int width,
                                     bool bounce) {

    bool upperCollision = false;
    bool w1;
    bool w2;

    for (int a = 0; a < 9; a++) {

        if (neighboringTiles[a] == 0 || !neighboringTiles[a]->collidable)
            continue;

        if (!upperCollision) {
            w1 = (*yPos < (neighboringTiles[a]->y * TILE_H) + TILE_H && (*yPos > (neighboringTiles[a]->y * TILE_H)));
            w2 = (*xPos > (neighboringTiles[a]->x * TILE_W) - width &&
                  (*xPos < (neighboringTiles[a]->x * TILE_W) + TILE_W));
            upperCollision = w1 && w2;

            if (upperCollision) {


                if (bounce) {
                    *ySpeed = (-1) * BOUNCING_FACTOR_Y * *ySpeed;
//                    if (*ySpeed < BOUNCING_STOP_SPEED)
//                        *ySpeed = 0;
                } else
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
                                      int height, bool bounce) {

    bool bottomCollision = false;
    bool w1;
    bool w2;

    for (int a = 0; a < 9; a++) {

        if (neighboringTiles[a] == 0 || !neighboringTiles[a]->collidable)
            continue;

        if (!bottomCollision) {
            if (width >= 16 && height >= 16) {
                w1 = (*xPos > (neighboringTiles[a]->x * TILE_W) - (width * 0.75) &&
                      *xPos < (neighboringTiles[a]->x * TILE_W) + (width * 0.75));
                w2 = (*yPos <= neighboringTiles[a]->y * TILE_H) && *yPos + height >= (neighboringTiles[a]->y * TILE_H);
            } else {
                w1 = (*xPos > (neighboringTiles[a]->x * TILE_W) - (width) &&
                      *xPos < (neighboringTiles[a]->x * TILE_W) + (width));
                w2 = (*yPos <= neighboringTiles[a]->y * TILE_H) && *yPos + height >= (neighboringTiles[a]->y * TILE_H);
            }

            bottomCollision = w1 && w2;

            if (bottomCollision) {

                if (bounce) {
                    *ySpeed = (-1) * BOUNCING_FACTOR_Y * *ySpeed;
//                    if (*ySpeed < BOUNCING_STOP_SPEED)
//                        *ySpeed = 0;
                } else
                    *ySpeed = 0;


                *yPos = (neighboringTiles[a]->y * TILE_H) - height;
            }
        } else
            return true;
    }

    return bottomCollision;
}

//Collision from left side of the tile, with rectangle given by x,y, width and height.
bool Collisions::checkLeftCollision(MapTile *neighboringTiles[9], int *xPos, int *yPos, double *xSpeed, int width,
                                    int height, bool bounce) {

    bool leftCollision = false;
    bool w1;
    bool w2;

    for (int a = 0; a < 9; a++) {

        if (neighboringTiles[a] == 0 || !neighboringTiles[a]->collidable)
            continue;

        if (!leftCollision) {

            if (width == 16) {
                w2 = (*xPos < (neighboringTiles[a]->x * TILE_W) - 0.75 * width &&
                      (*xPos > (neighboringTiles[a]->x * TILE_W) - width));
            } else {
                w2 = (*xPos < (neighboringTiles[a]->x * TILE_W) - 0.1 * width &&
                      (*xPos > (neighboringTiles[a]->x * TILE_W) - width));
            }

            w1 = (*yPos > (neighboringTiles[a]->y * TILE_H) - height &&
                  (*yPos < (neighboringTiles[a]->y * TILE_H) + TILE_H));

            leftCollision = w1 && w2;

            if (leftCollision) {

                if (bounce) {
                    *xSpeed = (-1) * BOUNCING_FACTOR_X * *xSpeed;
//                    if (*xSpeed < BOUNCING_STOP_SPEED)
//                        *xSpeed = 0;
                } else
                    *xSpeed = 0;


                *xPos = (neighboringTiles[a]->x * TILE_W) - width;
            }

        } else {
            return true;
        };
    }

    return leftCollision;
}

//Collision from right side of the tile, with rectangle given by x,y, width and height.
bool Collisions::checkRightCollision(MapTile *neighboringTiles[9], int *xPos, int *yPos, double *xSpeed, int width,
                                     int height, bool bounce) {
    bool rightCollision = false;
    bool w1;
    bool w2;
    for (int a = 0; a < 9; a++) {

        if (neighboringTiles[a] == 0 || !neighboringTiles[a]->collidable)
            continue;

        if (!rightCollision) {

            if (width == 16) {
                w2 = (*xPos < (neighboringTiles[a]->x * TILE_W) + width &&
                      (*xPos > (neighboringTiles[a]->x * TILE_W) + 0.75 * width));
            } else {
                w2 = (*xPos < (neighboringTiles[a]->x * TILE_W) + 16 &&
                      (*xPos > (neighboringTiles[a]->x * TILE_W) + 0.75 * width));
            }

            w1 = (*yPos > (neighboringTiles[a]->y * TILE_H) - height &&
                  (*yPos < (neighboringTiles[a]->y * TILE_H) + TILE_H));


            rightCollision = w1 && w2;

            if (rightCollision) {

                if (bounce) {
                    *xSpeed = (-1) * BOUNCING_FACTOR_X * *xSpeed;
//                    if (*xSpeed < BOUNCING_STOP_SPEED)
//                        *xSpeed = 0;
                } else
                    *xSpeed = 0;

                *xPos = (neighboringTiles[a]->x * TILE_W) + TILE_W;
            }

        } else {
            return true;
        }
    }

    return rightCollision;
}

bool Collisions::isStandingOnLeftEdge(MapTile *neighboringTiles[9], int x, int width, int tileX) {
    return (!neighboringTiles[7] && x <= (tileX * 16));
}
bool Collisions::isStandingOnRightEdge(MapTile *neighboringTiles[9], int x, int width, int tileX) {
    return  (!neighboringTiles[8] && x >= (tileX * 16) );
}

void Collisions::getNeighboringTiles(MapTile *mapTiles[32][32], int xx, int yy, MapTile *neighboringTiles[9]) {

    MapTile *left_middle = mapTiles[xx - 1][yy];
    MapTile *right_middle = mapTiles[xx + 1][yy];
    MapTile *up_middle = mapTiles[xx][yy - 1];
    MapTile *down_middle = mapTiles[xx][yy + 1];
    MapTile *center = mapTiles[xx][yy];
    MapTile *left_up = mapTiles[xx - 1][yy - 1];
    MapTile *right_up = mapTiles[xx + 1][yy - 1];
    MapTile *left_down = mapTiles[xx - 1][yy + 1];
    MapTile *right_down = mapTiles[xx + 1][yy + 1];

    neighboringTiles[0] = left_middle;
    neighboringTiles[1] = right_middle;
    neighboringTiles[2] = up_middle;
    neighboringTiles[3] = down_middle;
    neighboringTiles[4] = center;
    neighboringTiles[5] = left_up;
    neighboringTiles[6] = right_up;
    neighboringTiles[7] = left_down;
    neighboringTiles[8] = right_down;
}

void Collisions::bombNeighboringTiles(MapTile *mapTiles[32][32], int xx, int yy) {

    if (mapTiles[xx - 1][yy]->destroyable)
        mapTiles[xx - 1][yy] = nullptr;
    if (mapTiles[xx + 1][yy]->destroyable)
        mapTiles[xx + 1][yy] = nullptr;
    if (mapTiles[xx][yy - 1]->destroyable)
        mapTiles[xx][yy - 1] = nullptr;
    if (mapTiles[xx][yy + 1]->destroyable)
        mapTiles[xx][yy + 1] = nullptr;
    if (mapTiles[xx][yy]->destroyable)
        mapTiles[xx][yy] = nullptr;
    if (mapTiles[xx - 1][yy - 1]->destroyable)
        mapTiles[xx - 1][yy - 1] = nullptr;
    if (mapTiles[xx + 1][yy - 1]->destroyable)
        mapTiles[xx + 1][yy - 1] = nullptr;
    if (mapTiles[xx - 1][yy + 1]->destroyable)
        mapTiles[xx - 1][yy + 1] = nullptr;
    if (mapTiles[xx + 1][yy + 1]->destroyable)
        mapTiles[xx + 1][yy + 1] = nullptr;
    if (mapTiles[xx][yy + 2]->destroyable)
        mapTiles[xx][yy + 2] = nullptr;

}

void Collisions::getCenterTile(int x_position, int y_position, int height, int width, int *x_tile, int *y_tile) {
    floor_div(x_position + 0.5 * width, 16, x_tile);
    floor_div(y_position + 0.5 * height, 16, y_tile);
}

bool Collisions::checkCollisionWithMainDude(int x, int y, int width, int height) {
    return x + width + 4 >= global::main_dude->x && x + width < global::main_dude->x + MAIN_DUDE_WIDTH + 8 &&
           y + height > global::main_dude->y && y < global::main_dude->y + MAIN_DUDE_HEIGHT;
}
bool Collisions::checkCollisionWithMainDudeWidthBoundary(int x, int y, int width, int height, int x_boundary) {
    return  global::main_dude->x + 16 > x &&  global::main_dude->x <= x + width &&
           y + height > global::main_dude->y && y <= global::main_dude->y + MAIN_DUDE_HEIGHT;
}
