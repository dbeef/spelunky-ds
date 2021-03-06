//
// Created by xdbeef on 04.03.18.
//

#include <cstdlib>
#include <cstring>
#include "Level.hpp"
#include "SplashScreenType.hpp"
#include "../rooms/EntranceRooms.hpp"
#include "../rooms/ExitRooms.hpp"
#include "../rooms/ClosedRooms.hpp"
#include "../rooms/LeftRightRooms.hpp"
#include "../rooms/LeftRightDownRooms.hpp"
#include "../rooms/LeftRightUpRooms.hpp"
#include "../rooms/SplashScreenRooms.hpp"
#include "../rooms/AltarRoom.hpp"
#include "../rooms/RoomType.hpp"
#include "../rooms/ShopRooms.hpp"
#include "Direction.hpp"
#include "../GameState.hpp"
#include "BaseMap.hpp"

Level* Level::_instance = nullptr;

void Level::init() {
    SPELUNKYDS_BREAKING_ASSERT(!_instance);
    _instance = new Level();
    SPELUNKYDS_BREAKING_ASSERT(_instance);
}

void Level::dispose() {
    SPELUNKYDS_BREAKING_ASSERT(_instance);
    delete _instance;
}

Level::Level(){
    for (auto &map_tile : map_tiles)
        for (auto &b : map_tile)
            b = new MapTile();
}

Level::~Level(){
    for (auto &map_tile : map_tiles)
        for (auto &b : map_tile)
            delete b;
}

/**
 * Updates the map_tiles array with a frame composed of undestructable tiles over the edges of the map.
 */
void Level::generate_frame() {

    //upper row
    for (int a = 0; a < MAP_GAME_HEIGHT_TILES; a++) {
//        delete (map_tiles[a][0]);
//        auto *t = new MapTile();
        map_tiles[a][0]->match_tile(MapTileType::CAVE_REGULAR);
        map_tiles[a][0]->map_index[0] = a * 2;
        map_tiles[a][0]->map_index[1] = a * 2 + 1;
        map_tiles[a][0]->map_index[2] = LINE_WIDTH + a * 2;
        map_tiles[a][0]->map_index[3] = LINE_WIDTH + a * 2 + 1;
        map_tiles[a][0]->destroyable = false; //make it non-destroyable explicitly
        map_tiles[a][0]->x = a;
        map_tiles[a][0]->y = 0;
        map_tiles[a][0]->exists = true;
    }

    //bottom row
    for (int a = 0; a < MAP_GAME_WIDTH_TILES; a++) {
//        delete (map_tiles[31][a]);
//        auto *t = new MapTile();
        map_tiles[31][a]->match_tile(MapTileType::CAVE_REGULAR);
        map_tiles[31][a]->map_index[0] = a * 2 * LINE_WIDTH - 2 + LINE_WIDTH;
        map_tiles[31][a]->map_index[1] = a * 2 * LINE_WIDTH - 1 + LINE_WIDTH;
        map_tiles[31][a]->map_index[2] = a * 2 * LINE_WIDTH + LINE_WIDTH - 2 + LINE_WIDTH;
        map_tiles[31][a]->map_index[3] = a * 2 * LINE_WIDTH + LINE_WIDTH - 1 + LINE_WIDTH;
        map_tiles[31][a]->destroyable = false; //make it non-destroyable explicitly
        map_tiles[31][a]->x = 31;
        map_tiles[31][a]->y = a;
        map_tiles[31][a]->exists = true;
    }

    //right row
    for (int a = 0; a < MAP_GAME_HEIGHT_TILES; a++) {
//        delete (map_tiles[31][a]);
//        auto *t = new MapTile();
        map_tiles[a][31]->match_tile(MapTileType::CAVE_REGULAR);
        map_tiles[a][31]->map_index[0] = 62 * LINE_WIDTH + a * 2;
        map_tiles[a][31]->map_index[1] = 62 * LINE_WIDTH + a * 2 + 1;
        map_tiles[a][31]->map_index[2] = 63 * LINE_WIDTH + a * 2;
        map_tiles[a][31]->map_index[3] = 63 * LINE_WIDTH + a * 2 + 1;
        map_tiles[a][31]->destroyable = false; //make it non-destroyable explicitly
        map_tiles[a][31]->x = a;
        map_tiles[a][31]->y = 31;
        map_tiles[a][31]->exists = true;
    }

    //left row
    for (int a = 0; a < MAP_GAME_WIDTH_TILES; a++) {
//        delete (map_tiles[0][a]);
//        auto *t = new MapTile();
        map_tiles[0][a]->match_tile(MapTileType::CAVE_REGULAR);
        map_tiles[0][a]->map_index[0] = a * 2 * LINE_WIDTH + 0;
        map_tiles[0][a]->map_index[1] = a * 2 * LINE_WIDTH + 1;
        map_tiles[0][a]->map_index[2] = a * 2 * LINE_WIDTH + LINE_WIDTH;
        map_tiles[0][a]->map_index[3] = a * 2 * LINE_WIDTH + LINE_WIDTH + 1;
        map_tiles[0][a]->destroyable = false; //make it non-destroyable explicitly
        map_tiles[0][a]->x = 0;
        map_tiles[0][a]->y = a;
        map_tiles[0][a]->exists = true;
    }
}

/**
 * Initialises every MapTile in the map_tiles[][] array with its position on the map,
 * so it could be allowed to call LevelGenerator::tiles_to_map.
 * It utilizes given splash screen type, to know if any tile should be planted on the place it iterates,
 * and what MapTileType it should have.
 */
void Level::initialise_tiles_from_splash_screen(SplashScreenType splash_type) {

    int tab[SPLASH_SCREEN_HEIGHT][SPLASH_SCREEN_WIDTH];
    bool offset_on_upper_screen{};

    if (splash_type == SplashScreenType::ON_LEVEL_DONE_UPPER ||
        splash_type == SplashScreenType::SCORES_UPPER ||
        splash_type == SplashScreenType::MAIN_MENU_UPPER) {

        offset_on_upper_screen = true;

        if (splash_type == SplashScreenType::MAIN_MENU_UPPER)
            memcpy(tab, main_menu_upper, sizeof(main_menu_upper));
        else if (splash_type == SplashScreenType::ON_LEVEL_DONE_UPPER)
            memcpy(tab, on_level_done_upper, sizeof(on_level_done_upper));
        else if (splash_type == SplashScreenType::SCORES_UPPER)
            memcpy(tab, scores_upper, sizeof(scores_upper));
    }

    if (splash_type == SplashScreenType::ON_LEVEL_DONE_LOWER ||
        splash_type == SplashScreenType::SCORES_LOWER ||
        splash_type == SplashScreenType::MAIN_MENU_LOWER) {

        if (splash_type == SplashScreenType::ON_LEVEL_DONE_LOWER)
            memcpy(tab, on_level_done_lower, sizeof(on_level_done_lower));
        else if (splash_type == SplashScreenType::MAIN_MENU_LOWER)
            memcpy(tab, main_menu_lower, sizeof(main_menu_lower));
        else if (splash_type == SplashScreenType::SCORES_LOWER)
            memcpy(tab, scores_lower, sizeof(scores_lower));
    }

    //Now we initialise every tile in the splash screen and give it a map_index, which describes its location
    for (int tab_y = 0; tab_y < SPLASH_SCREEN_HEIGHT; tab_y++) {
        for (int tab_x = 0; tab_x < SPLASH_SCREEN_WIDTH; tab_x++) {

            if (tab[tab_y][tab_x] != 0) {

                //offset to the position in current room
                u16 room_offset =
                        static_cast<u16>(
                                2 * ROOM_TILE_HEIGHT_SPLASH_SCREEN *
                                LINE_WIDTH * ((ROOMS_Y - offset_on_upper_screen) - 1) - 4 * OFFSET_Y);
                //pos x and y in pixels of the tile in the current room
                u16 pos_x = static_cast<u16>((tab_x * 2) / 2);
                //NDS engine has different coordinate system than our room layout map,
                //so we invert the Y axis by ((ROOMS_Y - offset_on_upper_screen) - 1)
                u16 pos_y = static_cast<u16>(
                        tab_y + ROOM_TILE_HEIGHT_SPLASH_SCREEN * ((ROOMS_Y - offset_on_upper_screen) - 1) - 4);

//                delete (map_tiles[pos_x][pos_y]);
//                auto *t = new MapTile();
                map_tiles[pos_x][pos_y]->match_tile(static_cast<MapTileType>(tab[tab_y][tab_x]));
                map_tiles[pos_x][pos_y]->map_index[0] = room_offset + (tab_x * 2) + (tab_y * LINE_WIDTH * 2);
                map_tiles[pos_x][pos_y]->map_index[1] = room_offset + (tab_x * 2) + (tab_y * LINE_WIDTH * 2) + 1;
                map_tiles[pos_x][pos_y]->map_index[2] =
                        room_offset + (tab_x * 2) + (LINE_WIDTH + (tab_y * LINE_WIDTH * 2));
                map_tiles[pos_x][pos_y]->map_index[3] =
                        room_offset + (tab_x * 2) + (LINE_WIDTH + (tab_y * LINE_WIDTH * 2)) + 1;
                map_tiles[pos_x][pos_y]->x = pos_x;
                map_tiles[pos_x][pos_y]->y = pos_y;
                map_tiles[pos_x][pos_y]->exists = true;

            }
        }
    }
}

/**
 * Initialises every MapTile in the map_tiles[][] array with its position on the map,
 * so it could be allowed to call LevelGenerator::tiles_to_map.
 * It utilizes current room layout, to know if any tile should be planted on the place it iterates,
 * and what MapTileType it should have.
 */
void Level::initialise_tiles_from_room_layout() {

    //a room, 10x10 tiles
    int tab[10][10];
    int r;

    //iterate through every room we have
    for (int room_y = ROOMS_Y - 1; room_y >= 0; room_y--) {
        for (int room_x = 0; room_x < ROOMS_X; room_x++) {

            //basing on the room type, randomly select a variation of this room
            //and copy it to the temporary tab[10][10] array
            RoomType& room_type = layout[room_x][room_y];
            r = rand() % 6;
            layout_room_ids[room_x][room_y] = r; //-1 if completely disabling NPC's in this room

            //copying specific room variation
            switch (room_type) {
                case RoomType::R_CLOSED:
                    memcpy(tab, closed_rooms[r], sizeof(closed_rooms[r]));
                    break;
                case RoomType::R_LEFT_RIGHT:
                    memcpy(tab, left_right_rooms[r], sizeof(left_right_rooms[r]));
                    break;
                case RoomType::R_LEFT_RIGHT_DOWN:
                    memcpy(tab, left_right_down_rooms[r], sizeof(left_right_down_rooms[r]));
                    break;
                case RoomType::R_LEFT_RIGHT_UP:
                    memcpy(tab, left_right_up_rooms[r], sizeof(left_right_up_rooms[r]));
                    break;
                case RoomType::R_ENTRANCE:
                    memcpy(tab, entrance_room[r], sizeof(entrance_room[r]));
                    break;
                case RoomType::R_EXIT:
                    memcpy(tab, exit_room[r], sizeof(exit_room[r]));
                    break;
                case RoomType::R_SHOP_LEFT:
                    memcpy(tab, shops[0], sizeof(shops[0]));
                    break;
                case RoomType::R_SHOP_RIGHT:
                    memcpy(tab, shops[1], sizeof(shops[1]));
                    break;
                case RoomType::R_SHOP_LEFT_MUGSHOT:
                    memcpy(tab, shops_mugshots[0], sizeof(shops_mugshots[0]));
                    break;
                case RoomType::R_SHOP_RIGHT_MUGSHOT:
                    memcpy(tab, shops_mugshots[1], sizeof(shops_mugshots[1]));
                    break;
                case RoomType::R_ALTAR:
                    memcpy(tab, altar_room[0], sizeof(altar_room[1]));
                    break;
                default:
                    break;
            }

            //Now we initialise every tile in map and give it a map_index, which describes its location
            for (int tab_y = 0; tab_y < ROOM_TILE_HEIGHT_GAME; tab_y++) {
                for (int tab_x = 0; tab_x < ROOM_TILE_WIDTH_GAME; tab_x++) {

                    if (tab[tab_y][tab_x] != 0) {

                        //pos x and y in pixels of the tile in the current room
                        u16 pos_x = static_cast<u16>((OFFSET_X + tab_x * 2 + 2 * ROOM_TILE_WIDTH_GAME * room_x) / 2);
                        //NDS engine has different coordinate system than our room layout map,
                        //so we invert the Y axis by ((ROOMS_Y - room_y) - 1))
                        u16 pos_y = static_cast<u16>(
                                (OFFSET_X + tab_y * 2 + 2 * ROOM_TILE_HEIGHT_GAME * ((ROOMS_Y - room_y) - 1)) / 2);

                        //offset to the position in current room
                        u16 room_offset = static_cast<u16>(
                                OFFSET_X + OFFSET_Y +
                                2 * ROOM_TILE_WIDTH_GAME * room_x +
                                2 * ROOM_TILE_HEIGHT_GAME * LINE_WIDTH * ((ROOMS_Y - room_y) - 1));

//                        delete (map_tiles[pos_x][pos_y]);
//
//                        auto t = new MapTile();
                        map_tiles[pos_x][pos_y]->match_tile(static_cast<MapTileType>(tab[tab_y][tab_x]));

                        map_tiles[pos_x][pos_y]->map_index[0] = room_offset + (tab_x * 2) + (tab_y * LINE_WIDTH * 2);
                        map_tiles[pos_x][pos_y]->map_index[1] =
                                room_offset + (tab_x * 2) + (tab_y * LINE_WIDTH * 2) + 1;
                        map_tiles[pos_x][pos_y]->map_index[2] =
                                room_offset + (tab_x * 2) + (LINE_WIDTH + (tab_y * LINE_WIDTH * 2));
                        map_tiles[pos_x][pos_y]->map_index[3] =
                                room_offset + (tab_x * 2) + (LINE_WIDTH + (tab_y * LINE_WIDTH * 2)) + 1;

                        map_tiles[pos_x][pos_y]->x = pos_x;
                        map_tiles[pos_x][pos_y]->y = pos_y;
                        map_tiles[pos_x][pos_y]->exists = true;

                    }
                }
            }
        }
    }
}

/**
 * Sets given MapTile to the first tile on map that matches given MapTileType.
 * Searching begins on the upper-left corner of the map and goes right-down.
 * https://stackoverflow.com/questions/416162/assignment-inside-function-that-is-passed-as-pointer
 * @param mapTileType
 * @param m
 */
void Level::get_first_tile_of_given_type(MapTileType mapTileType, MapTile *&m) {
    for (int a = 0; a < MAP_GAME_WIDTH_TILES; a++) {
        for (int b = 0; b < MAP_GAME_HEIGHT_TILES; b++) {
            if (map_tiles[a][b]->exists && map_tiles[a][b]->mapTileType == mapTileType) {
                m = map_tiles[a][b];
                break;
            }
        }
    }
}

void Level::clean_map_layout() {
    //clean current layout
    for (auto &map_tile : Level::instance().map_tiles)
        for (auto &y : map_tile) {
            y->exists = false;
            y->destroyable = true; //tiles are destroyable by default
        }
}
