//
// Created by xdbeef on 10.03.18.
//

#ifndef SPELUNKYDS_MAPTILE_H
#define SPELUNKYDS_MAPTILE_H

class MapTile {
public:
    //There's 512 px x 512 px map
    //That makes 64x64 of (8x8) tiles
    //I decided that the smallest, logical object is 2x2 of 8px tiles, which makes 16 x 16
    //(practical reasons, compromise between having as much tiles as possible on screen and readability)
    int values[4];
    int map_index[4];
    int x; //32
    int y; //32
    bool collidable;
    bool destroyable;
    MapTile();
};


#endif //SPELUNKYDS_MAPTILE_H
