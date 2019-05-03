//fixme this shoudn't be a class

//
// Created by xdbeef on 18.03.18.
//
#include <cmath>
#include "Collisions.hpp"
#include "../entities/singletons/MainDude.hpp"
#include "../tiles/TileOrientation.hpp"
#include "../preprocessor/Debug.h"
#include "../GameState.hpp"

//(TILE_WIDTH * a), (TILE_HEIGHT * b) makes x,y point placed in the left-upper corner of the tile from passed *map_tiles[a][b].
//Sprite's x,y positions that are given to the following functions are expected to be upper-left corner.
//The x,y coordinates go from up to down on the y's, and from left to right on the x's - (0,0) is upper-left corner of
//the map.

//Collision from upper side of the tile, with rectangle given by x,y, and width.
bool Collisions::checkUpperCollision(MapTile *neighboringTiles[9], int *xPos, int *yPos, double *ySpeed, int width,
                                     float bouncing_factor) {

    bool w1;
    bool w2;

    for (int a = 0; a < 9; a++) {

        if (!neighboringTiles[a]->exists || !neighboringTiles[a]->collidable)
            continue;

        w1 = *yPos < (neighboringTiles[a]->y * TILE_HEIGHT) + TILE_HEIGHT && (*yPos > (neighboringTiles[a]->y * TILE_HEIGHT));
        w2 = *xPos > (neighboringTiles[a]->x * TILE_WIDTH) - width &&
             (*xPos < (neighboringTiles[a]->x * TILE_WIDTH) + TILE_WIDTH);

        if (w1 && w2) {
            *ySpeed = (-1) * bouncing_factor * *ySpeed;
            *yPos = (neighboringTiles[a]->y * TILE_HEIGHT) + TILE_HEIGHT;
            return true;
        }

    }

    return false;
}

//Collision from bottom side of the tile, with rectangle given by x,y, width and height.
bool Collisions::checkBottomCollision(MapTile *neighboringTiles[9], int *xPos, int *yPos, double *ySpeed, int width,
                                      int height, float bouncing_factor) {

    bool w1;
    bool w2;

    for (int a = 0; a < 9; a++) {

        if (neighboringTiles[a] == nullptr || !neighboringTiles[a]->collidable)
            continue;

        bool collision;

        if (width >= 16 && height >= 16) {
            w1 = (*xPos > (neighboringTiles[a]->x * TILE_WIDTH) - (width * 0.75) &&
                  *xPos < (neighboringTiles[a]->x * TILE_WIDTH) + (width * 0.75));
            w2 = (*yPos <= neighboringTiles[a]->y * TILE_HEIGHT) && *yPos + height >= (neighboringTiles[a]->y * TILE_HEIGHT);
            collision = w1 && w2;
        } else {
            w1 = (*xPos > (neighboringTiles[a]->x * TILE_WIDTH) - (width) &&
                  *xPos < (neighboringTiles[a]->x * TILE_WIDTH) + (TILE_WIDTH));
            w2 = (*yPos <= neighboringTiles[a]->y * TILE_HEIGHT) && *yPos + height >= (neighboringTiles[a]->y * TILE_HEIGHT);
            collision = w1 && w2;
        }

        if (collision) {
            *ySpeed = (-1) * bouncing_factor * *ySpeed;

            if (fabs(*ySpeed) < 0.15f)
                *ySpeed = 0;

            *yPos = (neighboringTiles[a]->y * TILE_HEIGHT) - height;
            return true;
        }
    }

    return false;
}

//Collision from left side of the tile, with rectangle given by x,y, width and height.
bool Collisions::checkLeftCollision(MapTile *neighboringTiles[9], int *xPos, int *yPos, double *xSpeed, int width,
                                    int height, float bouncing_factor) {

    bool w1;
    bool w2;

    for (int a = 0; a < 9; a++) {

        if (!neighboringTiles[a]->exists || !neighboringTiles[a]->collidable)
            continue;

        if (width == 16) {
            w2 = *xPos < (neighboringTiles[a]->x * TILE_WIDTH) - 0.75 * width &&
                 *xPos > (neighboringTiles[a]->x * TILE_WIDTH) - width;
        } else {
            w2 = (*xPos < (neighboringTiles[a]->x * TILE_WIDTH) - width + (TILE_WIDTH * 0.75) &&
                  (*xPos + width > (neighboringTiles[a]->x * TILE_WIDTH)));
        }

        w1 = (*yPos > (neighboringTiles[a]->y * TILE_HEIGHT) - height &&
              (*yPos < (neighboringTiles[a]->y * TILE_HEIGHT) + TILE_HEIGHT));

        if (w1 && w2) {
            if (bouncing_factor > 0)
                *xSpeed = (-1) * bouncing_factor * *xSpeed;
            *xPos = (neighboringTiles[a]->x * TILE_WIDTH) - width;
            return true;
        }
    }

    return false;
}

//Collision from right side of the tile, with rectangle given by x,y, width and height.

//Collision from right side of the tile, with rectangle given by x,y, width and height.
bool Collisions::checkRightCollision(MapTile *neighboringTiles[9], int *xPos, int *yPos, double *xSpeed, int width,
                                     int height, float bouncing_factor) {
    bool w1;
    bool w2;
    for (int a = 0; a < 9; a++) {

        if (neighboringTiles[a] == nullptr || !neighboringTiles[a]->collidable)
            continue;

        if (width == 16) {
            w2 = (*xPos < (neighboringTiles[a]->x * TILE_WIDTH) + width &&
                  (*xPos > (neighboringTiles[a]->x * TILE_WIDTH) + 0.75 * width));
        } else {
            w2 = (*xPos < (neighboringTiles[a]->x * TILE_WIDTH) + TILE_WIDTH &&
                  (*xPos > (neighboringTiles[a]->x * TILE_WIDTH) + 0.75 * width));
        }

        w1 = (*yPos > (neighboringTiles[a]->y * TILE_HEIGHT) - height &&
              (*yPos < (neighboringTiles[a]->y * TILE_HEIGHT) + TILE_HEIGHT));

        if (w1 && w2) {
            if (bouncing_factor > 0)
                *xSpeed = (-1) * bouncing_factor * *xSpeed;
            *xPos = (neighboringTiles[a]->x * TILE_WIDTH) + TILE_WIDTH;
            return true;
        }

    }

    return false;
}

bool Collisions::isStandingOnLeftEdge(MapTile *neighboringTiles[9], int x, int width, int tileX) {
    return (!neighboringTiles[static_cast<uint16>(TileOrientation::LEFT_DOWN)] /*&& !neighboringTiles[7]->collidable && x <= (tileX * 16)*/);
}

bool Collisions::isStandingOnRightEdge(MapTile *neighboringTiles[9], int x, int width, int tileX) {
    return (!neighboringTiles[static_cast<uint16>(TileOrientation::RIGHT_DOWN)] /*&& !neighboringTiles[8]->collidable && x >= (tileX * 16)*/);
}


void Collisions::getNeighboringTiles(MapTile *mapTiles[32][32], int xx, int yy, MapTile *out_neighboringTiles[9]) {

    SPELUNKYDS_BREAKING_ASSERT(xx >= 0 && yy >= 0 && xx < 32 && yy < 32);

    MapTile *left_middle = nullptr,
            *right_middle = nullptr,
            *up_middle = nullptr,
            *down_middle = nullptr,
            *center = nullptr,
            *left_up = nullptr,
            *right_up = nullptr,
            *left_down = nullptr,
            *right_down = nullptr;

    left_middle = mapTiles[xx - 1][yy]->exists ? mapTiles[xx - 1][yy] : nullptr;
    right_middle = mapTiles[xx + 1][yy]->exists ? mapTiles[xx + 1][yy] : nullptr;
    up_middle = mapTiles[xx][yy - 1]->exists ? mapTiles[xx][yy - 1] : nullptr;
    down_middle = mapTiles[xx][yy + 1]->exists ? mapTiles[xx][yy + 1] : nullptr;
    center = mapTiles[xx][yy]->exists ? mapTiles[xx][yy] : nullptr;
    left_up = mapTiles[xx - 1][yy - 1]->exists ? mapTiles[xx - 1][yy - 1] : nullptr;
    right_up = mapTiles[xx + 1][yy - 1]->exists ? mapTiles[xx + 1][yy - 1] : nullptr;
    left_down = mapTiles[xx - 1][yy + 1]->exists ? mapTiles[xx - 1][yy + 1] : nullptr;
    right_down = mapTiles[xx + 1][yy + 1]->exists ? mapTiles[xx + 1][yy + 1] : nullptr;

    out_neighboringTiles[static_cast<uint16>(TileOrientation::LEFT_MIDDLE)] = left_middle;
    out_neighboringTiles[static_cast<uint16>(TileOrientation::RIGHT_MIDDLE)] = right_middle;
    out_neighboringTiles[static_cast<uint16>(TileOrientation::UP_MIDDLE)] = up_middle;
    out_neighboringTiles[static_cast<uint16>(TileOrientation::DOWN_MIDDLE)] = down_middle;
    out_neighboringTiles[static_cast<uint16>(TileOrientation::CENTER)] = center;
    out_neighboringTiles[static_cast<uint16>(TileOrientation::LEFT_UP)] = left_up;
    out_neighboringTiles[static_cast<uint16>(TileOrientation::RIGHT_UP)] = right_up;
    out_neighboringTiles[static_cast<uint16>(TileOrientation::LEFT_DOWN)] = left_down;
    out_neighboringTiles[static_cast<uint16>(TileOrientation::RIGHT_DOWN)] = right_down;
}

void Collisions::bombNeighboringTiles(MapTile *mapTiles[32][32], int xx, int yy) {
    mapTiles[xx - 1][yy]->exists = !mapTiles[xx - 1][yy]->destroyable;
    mapTiles[xx + 1][yy]->exists = !mapTiles[xx + 1][yy]->destroyable;
    mapTiles[xx][yy - 1]->exists = !mapTiles[xx][yy - 1]->destroyable;
    mapTiles[xx][yy + 1]->exists = !mapTiles[xx][yy + 1]->destroyable;
    mapTiles[xx][yy]->exists = !mapTiles[xx][yy]->destroyable;
    mapTiles[xx - 1][yy - 1]->exists = !mapTiles[xx - 1][yy - 1]->destroyable;
    mapTiles[xx + 1][yy - 1]->exists = !mapTiles[xx + 1][yy - 1]->destroyable;
    mapTiles[xx - 1][yy + 1]->exists = !mapTiles[xx - 1][yy + 1]->destroyable;
    mapTiles[xx + 1][yy + 1]->exists = !mapTiles[xx + 1][yy + 1]->destroyable;
    mapTiles[xx][yy + 2]->exists = !mapTiles[xx][yy + 2]->destroyable;
}

bool Collisions::checkCollisionWithMainDude(int x, int y, int width, int height) {
    return checkCollisionBodies(x, y, width, height, MainDude::instance()._x, MainDude::instance()._y, 16, 16);
}

bool Collisions::checkCollisionWithMainDudeWidthBoundary(int x, int y, int width, int height, int x_boundary) {
    return MainDude::instance()._x + 16 > x && MainDude::instance()._x <= x + width &&
           y + height > MainDude::instance()._y && y <= MainDude::instance()._y + MainDude::main_dude_physical_height;
}

bool Collisions::checkCollisionWithMainDudeWhip(int x, int y, int width, int height) {
    if (MainDude::instance().sprite_state == Orientation::LEFT) {
        return x + width >= MainDude::instance()._x - WHIP_WIDTH &&
               x + width < MainDude::instance()._x + MainDude::main_dude_physical_width &&
               y + height > MainDude::instance()._y && y < MainDude::instance()._y + MainDude::main_dude_physical_height;
    } else if (MainDude::instance().sprite_state == Orientation::RIGHT) {
        return x + width >= MainDude::instance()._x &&
               x + width < MainDude::instance()._x + MainDude::main_dude_physical_width + WHIP_WIDTH * 2 &&
               y + height > MainDude::instance()._y && y < MainDude::instance()._y + MainDude::main_dude_physical_height;
    } else return false;

}

//x1,y1 is a left >upper< corner of the body, same for the x2,y2
bool Collisions::checkCollisionBodies(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2) {
    return x1 + w1 > x2 && x1 < x2 + w2 && y1 + h1 > y2 && y1 < y2 + h2;
}

//x1,y1 is a left >lower< corner of the body, same for the x2,y2
bool Collisions::checkCollisionBodiesLeftLowerCorner(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2) {
    return x1 + w1 > x2 && x1 < x2 + w2 && y1 > y2 - h2 && y1 - h1 < y2;
}

//returns 9 tiles from given tile x/y
void Collisions::getTilesOnRightFromXY(int xx, int yy, MapTile **out_neighboringTiles) {
    for (int a = 0; a < 9; a++) {
        if (a + xx <= 31)
            out_neighboringTiles[a] = Level::instance().map_tiles[a + xx][yy];
        else
            out_neighboringTiles[a] = nullptr;
    }
}

//returns 9 tiles from given tile x/y
void Collisions::getTilesOnLeftFromXY(int xx, int yy, MapTile **out_neighboringTiles) {
    for (int a = 0; a < 9; a++) {
        if (xx - a >= 0)
            out_neighboringTiles[a] = Level::instance().map_tiles[xx - a][yy];
        else
            out_neighboringTiles[a] = nullptr;
    }
}

