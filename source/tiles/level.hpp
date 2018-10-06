//
// Created by xdbeef on 04.03.18.
//

#ifndef SPELUNKYDS_LEVELGENERATOR_H
#define SPELUNKYDS_LEVELGENERATOR_H

#include "map_tile.hpp"
#include "direction.hpp"
#include "splash_screen_type.hpp"
#include "../rooms/RoomType.hpp"

//TODO Move from macros to constexpr in other classes too.

constexpr u16 ROOMS_X = 3;
constexpr u16 ROOMS_Y = 3;

constexpr u16 SPLASH_SCREEN_WIDTH = 16;
constexpr u16 SPLASH_SCREEN_HEIGHT = 12;

constexpr u16 ROOM_TILE_WIDTH_SPLASH_SCREEN = 16;
constexpr u16 ROOM_TILE_HEIGHT_SPLASH_SCREEN = 12;

constexpr u16 ROOM_TILE_WIDTH_GAME = 10;
constexpr u16 ROOM_TILE_HEIGHT_GAME = 10;

constexpr u16 MAP_GAME_HEIGHT_TILES = 32;
constexpr u16 MAP_GAME_WIDTH_TILES = 32;

constexpr u16 LINE_WIDTH = 64;
constexpr u16 OFFSET_X = 2; //Offset of 2 tiles, 8 px each
constexpr u16 OFFSET_Y = 128; //Offset of 128 tiles, 8px  each (2 lines, 64 each)

class Level {

public:

    //TODO Allocate memory on heap for the whole 32x32 tiles, never delete them later. That may help memory fragmentation.
    MapTile *map_tiles[32][32];

    //holds information on what room type is at specific array index
    RoomType layout[ROOMS_X][ROOMS_Y];

    //holds information on specific variation of room type, that is given from 'layout' array
    //i.e, we have 6 possible 'closed' rooms declared in the closed_rooms.hpp,
    //so this array lets us know, that we have a 'closed' room number 3 (for example) at some place.
    u16 layout_room_ids[ROOMS_X][ROOMS_Y];

    //sets all tiles to !existing
    void clean_map_layout();

    void init_map_tiles();

    void update_level();

    void write_tiles_to_map();

    void generate_frame();

    void initialise_tiles_from_room_layout();

    void initialise_tiles_from_splash_screen(SplashScreenType splashScreenType);

    void get_first_tile_of_given_type(MapTileType mapTileType, MapTile *&m);

    void copy_current_map_to_engines();

    void write_cave_background_to_map();
};

#endif //SPELUNKYDS_LEVELGENERATOR_H

