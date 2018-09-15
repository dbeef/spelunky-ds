//
// Created by xdbeef on 04.03.18.
//

#include <cstdlib>
#include <cstring>
#include "level_generator.h"
#include "../globals_declarations.h"
#include "splash_screen_type.hpp"
#include "../rooms/entrance_rooms.hpp"
#include "../rooms/exit_rooms.hpp"
#include "../rooms/closed_rooms.hpp"
#include "../rooms/left_right_rooms.hpp"
#include "../rooms/left_right_down_rooms.hpp"
#include "../rooms/left_right_up_rooms.hpp"
#include "../rooms/splash_screens.hpp"
#include "../rooms/room_type.hpp"
#include "map_utils.h"
#include "../rooms/shops.hpp"
#include "direction.hpp"

/**
 * Copies current map to the main/sub screen engine, and before that, formats it in a way that makes
 * it understandable by the engines.
 */
void LevelGenerator::render_tiles_on_base_map() {
    //copy the base map to the current map, which means we start the current map with only the cave background
    dmaCopyHalfWords(DEFAULT_DMA_CHANNEL, global::base_map, global::current_map, sizeof(global::base_map));
    //now we write our tiles onto the current map
    tiles_to_map();
    //sectorizing map, so it would be correctly rendered
    sectorize_map();
    //and copy the map to both main and sub screen engine
    dmaCopyHalfWords(DEFAULT_DMA_CHANNEL, global::current_map, bgGetMapPtr(global::bg_main_address),
                     sizeof(global::current_map));
    dmaCopyHalfWords(DEFAULT_DMA_CHANNEL, global::current_map, bgGetMapPtr(global::bg_sub_address),
                     sizeof(global::current_map));
}

/**
 * HOW IT WORKS TODO
 *
 * http:www.cplusplus.com/reference/cstdlib/rand
 * 0,0 is upper-left corner of the map
 */
void LevelGenerator::generate_new_rooms_layout() {

    //clean current layout
    for (auto &map_tile : map_tiles)
        for (auto &y : map_tile) {
            delete y;
            y = nullptr;
        }
    for (auto &layout_room_type : layout_room_types)
        for (int &b : layout_room_type) {
            b = room_type::R_CLOSED;
        }

    //generate new seed for the random number generator
    srand(timerElapsed(1));

    //set starting position to the random room in the most upper row
    int curr_x = rand() % 3;
    int curr_y = ROOMS_Y - 1;
    //direction represents where the generator will go in the next loop iteration
    Direction direction;

    bool exit_placed = false;

    //set the starting room as an entrance room
    layout_room_types[curr_x][curr_y] = room_type::R_ENTRANCE;

    //

    if (curr_x == 0)
        //we're on the left side of the map, so go right
        direction = Direction::RIGHT;
    else if (curr_x == 2)
        //we're on the right side of the map, so go left
        direction = Direction::LEFT;
    else
        //we're in the middle, so make a guess where should we gow now
        direction = static_cast<Direction>(rand() % 2); //left or right

    //

    //while we're on the very bottom floor or higher, do
    while (curr_y >= 0) {

        if (direction == Direction::UNDEFINED) {
            layout_room_types[curr_x][curr_y] = room_type::R_LEFT_RIGHT_UP;

            if (curr_y == 0 && !exit_placed && rand() % 2 == 0) {
                exit_placed = true;
                layout_room_types[curr_x][curr_y] = room_type::R_EXIT;
            }

            //

            if (curr_x == 0)
                //we're on the left side of the map, so go right
                direction = Direction::RIGHT;
            else if (curr_x == 2)
                //we're on the right side of the map, so go left
                direction = Direction::LEFT;
            else
                //we're in the middle, so make a guess where should we gow now
                direction = static_cast<Direction>(rand() % 2); //left or right

            //
        }


        if (direction == Direction::LEFT) {

            if (curr_x == 0) {
                //our direction is to go left, but we're already on the left side of the map, so go down
                direction = Direction::DOWN;
            } else {

                //our direction is to go left, and we're not on the left side of the map yet
                curr_x--;
                //
                if (curr_y == 0 && !exit_placed && rand() % 2 == 0) {
                    //we're on the most bottom floor, we didn't plant an exit yet and we've guessed that's the place
                    exit_placed = true;
                    layout_room_types[curr_x][curr_y] = room_type::R_EXIT;
                } else
                    layout_room_types[curr_x][curr_y] = room_type::R_LEFT_RIGHT;

                if (rand() % 3 == 2)
                    //random chance that we change our direction to go down in the next iteration
                    direction = Direction::DOWN;
//
            }

        } else if (direction == Direction::RIGHT) {

            if (curr_x == ROOMS_X - 1) {
                //our direction is to go right, but we're already on the right side of the map, so go down
                direction = Direction::DOWN;
            } else {
                //our direction is to go right, and we're not on the right side of the map yet
                curr_x++;
//
                if (curr_y == 0 && !exit_placed && rand() % 2 == 0) {
                    //we're on the most bottom floor, we didn't plant an exit yet and we've guessed that's the place
                    exit_placed = true;
                    layout_room_types[curr_x][curr_y] = room_type::R_EXIT;
                } else
                    layout_room_types[curr_x][curr_y] = room_type::R_LEFT_RIGHT;

                if (rand() % 3 == 2)
                    //random chance that we change our direction to go down in the next iteration
                    direction = Direction::DOWN;
//
            }

        } else if (direction == Direction::DOWN) {

            if (curr_y > 0) {

                layout_room_types[curr_x][curr_y] = room_type::R_LEFT_RIGHT_DOWN;
                //set next direction as undefined, so it could be guessed in the next iteration
                direction = Direction::UNDEFINED;
                curr_y--;

            } else {

                if (!exit_placed)
                    //we're on the very bottom floor, didn't plant an exit yet and we're
                    //done with iterating through map, so plant an exit
                    layout_room_types[curr_x][curr_y] = room_type::R_EXIT;

                break;
            }
        }
    }



    //todo post-generation things, like founding patterns, i.e if there's a well of '0' type rooms, then make
    //a special, long room

    bool ex = false;
    for (int a = 0; a < ROOMS_X; a++) {
        for (int b = 0; b < ROOMS_Y; b++) {

            if (layout_room_types[a][b] == room_type::R_CLOSED) {

                if (a == 0) {
                    if (layout_room_types[a + 1][b] != room_type::R_CLOSED) {
                        if (global::game_state->robbed_killed_shopkeeper)
                            layout_room_types[a][b] = room_type::R_SHOP_RIGHT_MUGSHOT;
                        else
                            layout_room_types[a][b] = room_type::R_SHOP_RIGHT;
                        ex = true;
                    }
                } else if (a == 2) {
                    if (layout_room_types[a - 1][b] != room_type::R_CLOSED) {
                        if (global::game_state->robbed_killed_shopkeeper)
                            layout_room_types[a][b] = room_type::R_SHOP_LEFT_MUGSHOT;
                        else
                            layout_room_types[a][b] = room_type::R_SHOP_LEFT;
                        ex = true;
                    }
                } else if (a == 1) {
                    if (layout_room_types[a - 1][b] != room_type::R_CLOSED &&
                        layout_room_types[a + 1][b] != room_type::R_CLOSED) {

                        if (rand() % 2 == 0)
                            layout_room_types[a][b] = room_type::R_SHOP_LEFT;
                        else
                            layout_room_types[a][b] = room_type::R_SHOP_RIGHT;

                        ex = true;
                    }
                }

                if (ex)
                    break;
            }


        }
        if (ex)
            break;
    }
}

/**
 * Updates the map_tiles array with a frame composed of undestructable tiles over the edges of the map.
 */
void LevelGenerator::generate_frame() {

    //upper row
    for (int a = 0; a < MAP_GAME_HEIGHT_TILES; a++) {
        delete (map_tiles[a][0]);
        auto *t = new MapTile();
        t->match_tile(MapTileType::CAVE_REGULAR);
        t->map_index[0] = a * 2;
        t->map_index[1] = a * 2 + 1;
        t->map_index[2] = LINE_WIDTH + a * 2;
        t->map_index[3] = LINE_WIDTH + a * 2 + 1;
        t->destroyable = false; //make it non-destroyable explicitly
        t->x = a;
        t->y = 0;
        this->map_tiles[a][0] = t;
    }

    //bottom row
    for (int a = 0; a < MAP_GAME_WIDTH_TILES; a++) {
        delete (map_tiles[MAP_GAME_HEIGHT_TILES - 1][a]);
        auto *t = new MapTile();
        t->match_tile(MapTileType::CAVE_REGULAR);
        t->map_index[0] = a * 2 * LINE_WIDTH - 2 + LINE_WIDTH;
        t->map_index[1] = a * 2 * LINE_WIDTH - 1 + LINE_WIDTH;
        t->map_index[2] = a * 2 * LINE_WIDTH + LINE_WIDTH - 2 + LINE_WIDTH;
        t->map_index[3] = a * 2 * LINE_WIDTH + LINE_WIDTH - 1 + LINE_WIDTH;
        t->destroyable = false; //make it non-destroyable explicitly
        t->x = 31;
        t->y = a;
        this->map_tiles[31][a] = t;
    }

    //right row
    for (int a = 0; a < MAP_GAME_HEIGHT_TILES; a++) {
        delete (map_tiles[a][MAP_GAME_WIDTH_TILES - 1]);
        auto *t = new MapTile();
        t->match_tile(MapTileType::CAVE_REGULAR);
        t->map_index[0] = 62 * LINE_WIDTH + a * 2;
        t->map_index[1] = 62 * LINE_WIDTH + a * 2 + 1;
        t->map_index[2] = 63 * LINE_WIDTH + a * 2;
        t->map_index[3] = 63 * LINE_WIDTH + a * 2 + 1;
        t->destroyable = false; //make it non-destroyable explicitly
        t->x = a;
        t->y = 31;
        this->map_tiles[a][31] = t;
    }

    //left row
    for (int a = 0; a < MAP_GAME_WIDTH_TILES; a++) {
        delete (map_tiles[0][a]);
        auto *t = new MapTile();
        t->match_tile(MapTileType::CAVE_REGULAR);
        t->map_index[0] = a * 2 * LINE_WIDTH + 0;
        t->map_index[1] = a * 2 * LINE_WIDTH + 1;
        t->map_index[2] = a * 2 * LINE_WIDTH + LINE_WIDTH;
        t->map_index[3] = a * 2 * LINE_WIDTH + LINE_WIDTH + 1;
        t->destroyable = false; //make it non-destroyable explicitly
        t->x = 0;
        t->y = a;
        this->map_tiles[0][a] = t;
    }
}

/**
 * Writes bytes that make a graphical representation of every MapTile in map_tiles array to the current map.
 */
void LevelGenerator::tiles_to_map() {
    for (int x = 0; x < MAP_GAME_WIDTH_TILES; x++) {
        for (int y = 0; y < MAP_GAME_HEIGHT_TILES; y++) {

            MapTile *t = this->map_tiles[x][y];
            for (int k = 0; k < 4; k++)
                if (t && t->values[k] != 0)
                    global::current_map[t->map_index[k]] = t->values[k];

        }
    }
}

/**
 * Renders given splash screen to the left-upper corner of the 32x32 tile render space
 */
void LevelGenerator::generate_splash_screen(int room_type) {
    int tab[SPLASH_SCREEN_HEIGHT][SPLASH_SCREEN_WIDTH];
    int a, b = 0;

    if (room_type == ON_LEVEL_DONE_UPPER || room_type == SCORES_UPPER || room_type == MAIN_MENU_UPPER) {
        a = 0;
        b = 1;

        if (room_type == MAIN_MENU_UPPER) {
            memcpy(tab, main_menu_upper, sizeof(main_menu_upper));
        } else if (room_type == ON_LEVEL_DONE_UPPER) {
            memcpy(tab, on_level_done_upper, sizeof(on_level_done_upper));
        } else if (room_type == SCORES_UPPER) {
            memcpy(tab, scores_upper, sizeof(scores_upper));
        }
    }

    if (room_type == ON_LEVEL_DONE_LOWER || room_type == SCORES_LOWER || room_type == MAIN_MENU_LOWER) {
        a = 0;
        b = 0;

        if (room_type == ON_LEVEL_DONE_LOWER) {
            memcpy(tab, on_level_done_lower, sizeof(on_level_done_lower));
        } else if (room_type == MAIN_MENU_LOWER) {
            memcpy(tab, main_menu_lower, sizeof(main_menu_lower));
        } else if (room_type == SCORES_LOWER) {
            memcpy(tab, scores_lower, sizeof(scores_lower));
        }
    }

    for (int tab_y = 0; tab_y < SPLASH_SCREEN_HEIGHT; tab_y++) {
        for (int tab_x = 0; tab_x < SPLASH_SCREEN_WIDTH; tab_x++) {

            if (tab[tab_y][tab_x] != 0) {

                u16 room_offset = 2 * ROOM_TILE_WIDTH_SPLASH_SCREEN * a +
                                  2 * ROOM_TILE_HEIGHT_SPLASH_SCREEN * LINE_WIDTH * ((ROOMS_Y - b) - 1)
                                  - 4 * OFFSET_Y;

                u16 pos_x = (tab_x * 2 + 2 * ROOM_TILE_WIDTH_SPLASH_SCREEN * a) / 2;
                u16 pos_y = tab_y + ROOM_TILE_HEIGHT_SPLASH_SCREEN * ((ROOMS_Y - b) - 1) - 4;

                delete (map_tiles[pos_x][pos_y]);
                auto *t = new MapTile();
                t->match_tile(static_cast<MapTileType>(tab[tab_y][tab_x]));
                t->map_index[0] = room_offset + (tab_x * 2) + (tab_y * LINE_WIDTH * 2);
                t->map_index[1] = room_offset + (tab_x * 2) + (tab_y * LINE_WIDTH * 2) + 1;
                t->map_index[2] = room_offset + (tab_x * 2) + (LINE_WIDTH + (tab_y * LINE_WIDTH * 2));
                t->map_index[3] = room_offset + (tab_x * 2) + (LINE_WIDTH + (tab_y * LINE_WIDTH * 2)) + 1;
                t->x = pos_x;
                t->y = pos_y;
                this->map_tiles[pos_x][pos_y] = t;
            }
        }
    }
}

/**
 * Using the layout_room_types that indicate if given room is left-down, left-up, left-up... oriented,
 * copies proper room layout.
 */
void LevelGenerator::generate_rooms() {

    //a room, 10x10 tiles
    int tab[10][10];
    int r;

    for (int b = ROOMS_Y - 1; b >= 0; b--) {
        for (int a = 0; a < ROOMS_X; a++) {

            int room_type = layout_room_types[a][b];
            r = rand() % 6;
            layout_room_ids[a][b] = r;
            //-1 if no NPC's

            //copying specific room layout
            switch (room_type) {
                case room_type::R_CLOSED:
                    memcpy(tab, closed_rooms[r], sizeof(closed_rooms[r]));
                    break;
                case room_type::R_LEFT_RIGHT:
                    memcpy(tab, left_right_rooms[r], sizeof(left_right_rooms[r]));
                    break;
                case room_type::R_LEFT_RIGHT_DOWN:
                    memcpy(tab, left_right_down_rooms[r], sizeof(left_right_down_rooms[r]));
                    break;
                case room_type::R_LEFT_RIGHT_UP:
                    memcpy(tab, left_right_up_rooms[r], sizeof(left_right_up_rooms[r]));
                    break;
                case room_type::R_ENTRANCE:
                    memcpy(tab, entrance_room[r], sizeof(entrance_room[r]));
                    break;
                case room_type::R_EXIT:
                    memcpy(tab, exit_room[r], sizeof(exit_room[r]));
                    break;
                case room_type::R_SHOP_LEFT:
                    memcpy(tab, shops[0], sizeof(shops[0]));
                    break;
                case room_type::R_SHOP_RIGHT:
                    memcpy(tab, shops[1], sizeof(shops[1]));
                    break;
                case room_type::R_SHOP_LEFT_MUGSHOT:
                    memcpy(tab, shops_mugshots[0], sizeof(shops_mugshots[0]));
                    break;
                case room_type::R_SHOP_RIGHT_MUGSHOT:
                    memcpy(tab, shops_mugshots[1], sizeof(shops_mugshots[1]));
                    break;
                default:
                    break;
            }

            //initialises a specific MapTile on given position of the room
            for (int tab_y = 0; tab_y < ROOM_TILE_HEIGHT_GAME; tab_y++) {
                for (int tab_x = 0; tab_x < ROOM_TILE_WIDTH_GAME; tab_x++) {

                    if (tab[tab_y][tab_x] != 0) {

                        u16 room_offset =
                                OFFSET_X + OFFSET_Y + 2 * ROOM_TILE_WIDTH_GAME * a +
                                2 * ROOM_TILE_HEIGHT_GAME * LINE_WIDTH * ((ROOMS_Y - b) - 1);
                        u16 pos_x = (OFFSET_X + tab_x * 2 + 2 * ROOM_TILE_WIDTH_GAME * a) / 2;
                        u16 pos_y = (OFFSET_X + tab_y * 2 + 2 * ROOM_TILE_HEIGHT_GAME * ((ROOMS_Y - b) - 1)) / 2;

                        delete (map_tiles[pos_x][pos_y]);

                        auto t = new MapTile();
                        t->match_tile(static_cast<MapTileType>(tab[tab_y][tab_x]));

                        t->map_index[0] = room_offset + (tab_x * 2) + (tab_y * LINE_WIDTH * 2);
                        t->map_index[1] = room_offset + (tab_x * 2) + (tab_y * LINE_WIDTH * 2) + 1;
                        t->map_index[2] = room_offset + (tab_x * 2) + (LINE_WIDTH + (tab_y * LINE_WIDTH * 2));
                        t->map_index[3] = room_offset + (tab_x * 2) + (LINE_WIDTH + (tab_y * LINE_WIDTH * 2)) + 1;

                        t->x = pos_x;
                        t->y = pos_y;
                        this->map_tiles[pos_x][pos_y] = t;

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
void LevelGenerator::get_first_tile(MapTileType mapTileType, MapTile *&m) {
    for (int a = 0; a < MAP_GAME_WIDTH_TILES; a++) {
        for (int b = 0; b < MAP_GAME_HEIGHT_TILES; b++) {
            if (map_tiles[a][b] != nullptr && map_tiles[a][b]->mapTileType == mapTileType) {
                m = map_tiles[a][b];
                break;
            }
        }
    }
}

