//
// Created by xdbeef on 04.03.18.
//

#ifndef SPELUNKYDS_LEVELGENERATOR_H
#define SPELUNKYDS_LEVELGENERATOR_H

#include "map_tile.h"

#define ROOMS_X 3
#define ROOMS_Y 3

#define SPLASH_SCREEN_WIDTH 16
#define SPLASH_SCREEN_HEIGHT 12

#define ROOM_TILE_WIDTH_SPLASH_SCREEN 16
#define ROOM_TILE_HEIGHT_SPLASH_SCREEN 12

#define ROOM_TILE_WIDTH_GAME 10
#define ROOM_TILE_HEIGHT_GAME 10

#define MAP_GAME_HEIGHT_TILES 32
#define MAP_GAME_WIDTH_TILES 32

#define LINE_WIDTH 64
//Offset of 2 tiles, 8 px each
#define OFFSET_X 2
//Offset of 128 tiles, 8px  each (2 lines, 64 each)
#define OFFSET_Y 128


class  LevelGenerator {

public:

    MapTile *map_tiles[32][32];
    int layout_room_types[ROOMS_X][ROOMS_Y];
    int layout_room_ids[ROOMS_X][ROOMS_Y];

    void newLayout(int seed);

    void tiles_to_map();

    void generate_frame();

    void generate_rooms();

    void generate_splash_screen(int room_type);

    void match_tile(MapTile *t, int value);

    void get_first_tile(MapTileType mapTileType, MapTile *&m);

    void render_tiles_on_base_map();
};

#endif //SPELUNKYDS_LEVELGENERATOR_H

