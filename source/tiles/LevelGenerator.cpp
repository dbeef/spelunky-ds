//
// Created by xdbeef on 04.03.18.
//

#include <tgmath.h>
#include "LevelGenerator.h"
#include "../Globals.h"
#include "SplashScreenType.h"
#include "../rooms/EntranceRooms.h"
#include "../rooms/ExitRooms.h"
#include "../rooms/ClosedRooms.h"
#include "../rooms/LeftRightRooms.h"
#include "../rooms/LeftRightDownRooms.h"
#include "../rooms/LeftRightUpRooms.h"
#include "../rooms/SplashScreens.h"
#include "../rooms/RoomType.h"


/**
 * http:www.cplusplus.com/reference/cstdlib/rand/, liczba z przedziału 0-3
 * 0,0 is upper-left corner of the map
 * @param seed
 */
void LevelGenerator::newLayout(int seed) {

    for (int x = 0; x < MAP_GAME_WIDTH_TILES; x++) {
        for (int y = 0; y < MAP_GAME_HEIGHT_TILES; y++) {
            mapTiles[x][y] = 0;
        }
    }
    for (int a = 0; a < ROOMS_X; a++) {
        for (int b = 0; b < ROOMS_Y; b++) {
            layout_room_types[a][b] = 0;
        }
    }

    srand(timerElapsed(1));

    int curr_x = rand() % 3;
    //    std::cout << " " << curr_x << '\n';

    int curr_y = ROOMS_Y - 1;
    int direction = 0;

    layout_room_types[curr_x][curr_y] = 4;

    while (curr_y >= 0) {


        if (direction == 0) {
            if (curr_y == 0)
                direction = rand() % 2 + 1;
            else {

                int l_chances = 4;
                int r_chances = 4;

                if (curr_x == ROOMS_X - 1) {
                    l_chances = 25;
                }
                if (curr_x == 0) {
                    r_chances = 25;
                }

                int r = (rand() % (r_chances + l_chances)) + 1;

                if (r < l_chances)
                    direction = 1;
                else if (r >= l_chances && r <= l_chances + r_chances)
                    direction = 2;
                else {
                    direction = 3;
                }

            }
        }

        if (direction == 1) {

            if (curr_x == 0) {
                direction = 3;
            } else {
                curr_x--;
                layout_room_types[curr_x][curr_y] = 1;
            }
        } else if (direction == 2) {

            if (curr_x == ROOMS_X - 1) {
                direction = 3;
            } else {
                curr_x++;
                layout_room_types[curr_x][curr_y] = 1;
            }

        } else if (direction == 3) {

            if (curr_y > 0) {

                layout_room_types[curr_x][curr_y] = 2;

                if (curr_y >= 1) {
                    layout_room_types[curr_x][curr_y - 1] = 3;
                }


                direction = 0;
                curr_y--;

                if (curr_y == 0)
                    layout_room_types[curr_x][curr_y] = 5;

            } else {
                break;
            }
        }
    }

    //todo post-generation things, like founding patterns, i.e if there's a well of '0' type rooms, then make
    //a special, long room

}

void LevelGenerator::generateFrame() {

    for (int a = 0; a < MAP_GAME_HEIGHT_TILES; a++) {
        delete (mapTiles[a][0]);
        MapTile *t = new MapTile();
        matchTile(t, MapTileType::CAVE_REGULAR);
        t->map_index[0] = a * 2;
        t->map_index[1] = a * 2 + 1;
        t->map_index[2] = LINE_WIDTH + a * 2;
        t->map_index[3] = LINE_WIDTH + a * 2 + 1;
        t->collidable = true;
        t->destroyable = false;
        t->x = a;
        t->y = 0;
        this->mapTiles[a][0] = t;
    }

    for (int a = 0; a < MAP_GAME_HEIGHT_TILES; a++) {
        delete (mapTiles[a][MAP_GAME_WIDTH_TILES - 1]);
        MapTile *t = new MapTile();
        matchTile(t, MapTileType::CAVE_REGULAR);
        t->map_index[0] = 62 * LINE_WIDTH + a * 2;
        t->map_index[1] = 62 * LINE_WIDTH + a * 2 + 1;
        t->map_index[2] = 63 * LINE_WIDTH + a * 2;
        t->map_index[3] = 63 * LINE_WIDTH + a * 2 + 1;
        t->collidable = true;
        t->destroyable = false;
        t->x = a;
        t->y = 31;
        this->mapTiles[a][31] = t;
    }

    for (int a = 0; a < MAP_GAME_WIDTH_TILES; a++) {
        delete (mapTiles[0][a]);
        MapTile *t = new MapTile();
        matchTile(t, MapTileType::CAVE_REGULAR);
        t->map_index[0] = a * 2 * LINE_WIDTH + 0;
        t->map_index[1] = a * 2 * LINE_WIDTH + 1;
        t->map_index[2] = a * 2 * LINE_WIDTH + LINE_WIDTH;
        t->map_index[3] = a * 2 * LINE_WIDTH + LINE_WIDTH + 1;
        t->collidable = true;
        t->destroyable = false;
        t->x = 0;
        t->y = a;
        this->mapTiles[0][a] = t;
    }

    for (int a = 0; a < MAP_GAME_WIDTH_TILES; a++) {
        delete (mapTiles[MAP_GAME_HEIGHT_TILES - 1][a]);
        MapTile *t = new MapTile();

        matchTile(t, MapTileType::CAVE_REGULAR);

        t->map_index[0] = a * 2 * LINE_WIDTH - 2 + LINE_WIDTH;
        t->map_index[1] = a * 2 * LINE_WIDTH - 1 + LINE_WIDTH;
        t->map_index[2] = a * 2 * LINE_WIDTH + LINE_WIDTH - 2 + LINE_WIDTH;
        t->map_index[3] = a * 2 * LINE_WIDTH + LINE_WIDTH - 1 + LINE_WIDTH;
        t->collidable = true;
        t->destroyable = false;
        t->x = 31;
        t->y = a;
        this->mapTiles[31][a] = t;
    }
}

void LevelGenerator::tilesToMap() {
    for (int x = 0; x < MAP_GAME_WIDTH_TILES; x++) {
        for (int y = 0; y < MAP_GAME_HEIGHT_TILES; y++) {
            MapTile *t = this->mapTiles[x][y];
            for (int k = 0; k < 4; k++)
                if (t && t->values[k] != 0)
                    global::current_map[t->map_index[k]] = t->values[k];
        }
    }
}

void LevelGenerator::generateSplashScreen(int room_type) {
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

                delete (mapTiles[pos_x][pos_y]);
                MapTile *t = new MapTile();
                matchTile(t, tab[tab_y][tab_x]);
                t->map_index[0] = room_offset + (tab_x * 2) + (tab_y * LINE_WIDTH * 2);
                t->map_index[1] = room_offset + (tab_x * 2) + (tab_y * LINE_WIDTH * 2) + 1;
                t->map_index[2] = room_offset + (tab_x * 2) + (LINE_WIDTH + (tab_y * LINE_WIDTH * 2));
                t->map_index[3] = room_offset + (tab_x * 2) + (LINE_WIDTH + (tab_y * LINE_WIDTH * 2)) + 1;
                t->x = pos_x;
                t->y = pos_y;
                this->mapTiles[pos_x][pos_y] = t;
            }
        }
    }
}

void LevelGenerator::generateRooms() {

    int tab[10][10];
    int r;

    for (int b = ROOMS_Y - 1; b >= 0; b--) {
        for (int a = 0; a < ROOMS_X; a++) {

            int room_type = layout_room_types[a][b];
            r = rand() % 3;
            layout_room_ids[a][b] = r;
            //-1 if no NPC's

            switch (room_type) {
                case RoomType::ROOM_CLOSED:
                    memcpy(tab, closed_rooms[r], sizeof(closed_rooms[r]));
                    break;
                case RoomType::ROOM_LEFT_RIGHT:
                    memcpy(tab, left_right_rooms[r], sizeof(left_right_rooms[r]));
                    break;
                case RoomType::ROOM_LEFT_RIGHT_DOWN:
                    memcpy(tab, left_right_down_rooms[r], sizeof(left_right_down_rooms[r]));
                    break;
                case RoomType::ROOM_LEFT_RIGHT_UP:
                    memcpy(tab, left_right_up_rooms[r], sizeof(left_right_up_rooms[r]));
                    break;
                case RoomType::ROOM_ENTRANCE:
                    memcpy(tab, entrance_room[r], sizeof(entrance_room[r]));
                    break;
                case RoomType::ROOM_EXIT:
                    memcpy(tab, exit_room[r], sizeof(exit_room[r]));
                    break;
                default:
                    break;
            }

            for (int tab_y = 0; tab_y < ROOM_TILE_HEIGHT_GAME; tab_y++) {
                for (int tab_x = 0; tab_x < ROOM_TILE_WIDTH_GAME; tab_x++) {

                    if (tab[tab_y][tab_x] != 0) {

                        u16 room_offset =
                                OFFSET_X + OFFSET_Y + 2 * ROOM_TILE_WIDTH_GAME * a +
                                2 * ROOM_TILE_HEIGHT_GAME * LINE_WIDTH * ((ROOMS_Y - b) - 1);
                        u16 pos_x = (OFFSET_X + tab_x * 2 + 2 * ROOM_TILE_WIDTH_GAME * a) / 2;
                        u16 pos_y = (OFFSET_X + tab_y * 2 + 2 * ROOM_TILE_HEIGHT_GAME * ((ROOMS_Y - b) - 1)) / 2;

                        delete (mapTiles[pos_x][pos_y]);
                        MapTile *t = new MapTile();
                        matchTile(t, tab[tab_y][tab_x]);
                        t->map_index[0] = room_offset + (tab_x * 2) + (tab_y * LINE_WIDTH * 2);
                        t->map_index[1] = room_offset + (tab_x * 2) + (tab_y * LINE_WIDTH * 2) + 1;
                        t->map_index[2] = room_offset + (tab_x * 2) + (LINE_WIDTH + (tab_y * LINE_WIDTH * 2));
                        t->map_index[3] = room_offset + (tab_x * 2) + (LINE_WIDTH + (tab_y * LINE_WIDTH * 2)) + 1;
                        t->x = pos_x;
                        t->y = pos_y;
                        this->mapTiles[pos_x][pos_y] = t;
                    }
                }
            }
        }
    }
}

void LevelGenerator::matchTile(MapTile *t, int value) {

    t->destroyable = true;
    t->collidable = true;

    switch (value) {
        case MapTileType::CAVE_REGULAR:
            t->values[0] = 20;
            t->values[1] = 21;
            t->values[2] = 22;
            t->values[3] = 23;
            t->mapTileType = MapTileType::CAVE_REGULAR;
            break;
        case MapTileType::CAVE_UP_ORIENTED:
            t->values[0] = 40;
            t->values[1] = 41;
            t->values[2] = 42;
            t->values[3] = 43;
            t->mapTileType = MapTileType::CAVE_UP_ORIENTED;
            break;
        case MapTileType::STONE_BLOCK:
            t->values[0] = 24;
            t->values[1] = 25;
            t->values[2] = 26;
            t->values[3] = 27;
            t->mapTileType = MapTileType::STONE_BLOCK;
            break;

        case MapTileType::CAVE_SOME_GOLD:
            t->values[0] = 32;
            t->values[1] = 33;
            t->values[2] = 34;
            t->values[3] = 35;
            t->mapTileType = MapTileType::CAVE_SOME_GOLD;
            break;
        case MapTileType::CAVE_MUCH_GOLD:
            t->values[0] = 36;
            t->values[1] = 37;
            t->values[2] = 38;
            t->values[3] = 39;
            t->mapTileType = MapTileType::CAVE_MUCH_GOLD;
            break;
        case MapTileType::CAVE_ROCK:
            t->values[0] = 16;
            t->values[1] = 17;
            t->values[2] = 18;
            t->values[3] = 19;
            t->mapTileType = MapTileType::CAVE_ROCK;
            break;
        case MapTileType::LADDER:

            t->values[0] = 48;
            t->values[1] = 49;
            t->values[2] = 50;
            t->values[3] = 51;

            t->collidable = false;
            t->destroyable = false;
            t->mapTileType = MapTileType::LADDER;
            break;
        case MapTileType::LADDER_DECK:

            t->values[0] = 52;
            t->values[1] = 53;
            t->values[2] = 54;
            t->values[3] = 55;

            t->collidable = false;
            t->destroyable = false;
            t->mapTileType = MapTileType::LADDER_DECK;
            break;

        case MapTileType::ARROW_TRAP_LEFT:

            t->values[0] = 56;
            t->values[1] = 57;
            t->values[2] = 58;
            t->values[3] = 59;
            t->mapTileType = MapTileType::ARROW_TRAP_LEFT;
            break;

        case MapTileType::ARROW_TRAP_RIGHT:
            t->values[0] = 60;
            t->values[1] = 61;
            t->values[2] = 62;
            t->values[3] = 63;
            t->mapTileType = MapTileType::ARROW_TRAP_RIGHT;
            break;
        case MapTileType::ENTRANCE:

            t->values[0] = 64;
            t->values[1] = 65;
            t->values[2] = 66;
            t->values[3] = 67;
            t->collidable = false;
            t->destroyable = false;
            t->mapTileType = MapTileType::ENTRANCE;
            break;

        case MapTileType::EXIT:
            t->values[0] = 68;
            t->values[1] = 69;
            t->values[2] = 70;
            t->values[3] = 71;
            t->collidable = false;
            t->destroyable = false;
            t->mapTileType = MapTileType::EXIT;
            break;

        case MapTileType::CAVE_DOWN_ORIENTED:
            t->values[0] = 28;
            t->values[1] = 29;
            t->values[2] = 30;
            t->values[3] = 31;
            t->mapTileType = MapTileType::CAVE_DOWN_ORIENTED;
            break;
        case MapTileType::CAVE_UP_DOWN_ORIENTED:
            t->values[0] = 44;
            t->values[1] = 45;
            t->values[2] = 46;
            t->values[3] = 47;
            t->mapTileType = MapTileType::CAVE_UP_DOWN_ORIENTED;
            break;

        case MapTileType::CONSOLE_LEFT_BAR_TOP_ROUNDED:
            t->values[0] = 72;
            t->values[1] = 73;
            t->values[2] = 74;
            t->values[3] = 75;
            t->collidable = false;
            t->destroyable = false;
            t->mapTileType = MapTileType::CONSOLE_LEFT_BAR_TOP_ROUNDED;
            break;
        case MapTileType::CONSOLE_RIGHT_BAR_TOP_ROUNDED:
            t->values[0] = 76;
            t->values[1] = 77;
            t->values[2] = 78;
            t->values[3] = 79;
            t->collidable = false;
            t->destroyable = false;
            t->mapTileType = MapTileType::CONSOLE_RIGHT_BAR_TOP_ROUNDED;
            break;
        case MapTileType::CONSOLE_LEFT_BAR_BOT_ROUNDED:

            t->values[0] = 80;
            t->values[1] = 81;
            t->values[2] = 82;
            t->values[3] = 83;
            t->collidable = false;
            t->destroyable = false;
            t->mapTileType = MapTileType::CONSOLE_LEFT_BAR_BOT_ROUNDED;
            break;

        case MapTileType::CONSOLE_RIGHT_BAR_BOT_ROUNDED:

            t->values[0] = 84;
            t->values[1] = 85;
            t->values[2] = 86;
            t->values[3] = 87;
            t->collidable = false;
            t->destroyable = false;
            t->mapTileType = MapTileType::CONSOLE_RIGHT_BAR_BOT_ROUNDED;
            break;

        case MapTileType::CONSOLE_TOP_BAR:
            t->values[0] = 88;
            t->values[1] = 89;
            t->values[2] = 90;
            t->values[3] = 91;
            t->collidable = false;
            t->destroyable = false;
            t->mapTileType = MapTileType::CONSOLE_TOP_BAR;
            break;

        case MapTileType::CONSOLE_BOTTOM_BAR:

            t->values[0] = 92;
            t->values[1] = 93;
            t->values[2] = 94;
            t->values[3] = 95;
            t->collidable = false;
            t->destroyable = false;
            t->mapTileType = MapTileType::CONSOLE_BOTTOM_BAR;
            break;

        case MapTileType::CONSOLE_LEFT_BAR:
            t->values[0] = 96;
            t->values[1] = 97;
            t->values[2] = 98;
            t->values[3] = 99;
            t->collidable = false;
            t->destroyable = false;
            t->mapTileType = MapTileType::CONSOLE_LEFT_BAR;
            break;

        case MapTileType::CONSOLE_RIGHT_BAR:
            t->values[0] = 100;
            t->values[1] = 101;
            t->values[2] = 102;
            t->values[3] = 103;
            t->collidable = false;
            t->destroyable = false;
            t->mapTileType = MapTileType::CONSOLE_RIGHT_BAR;
            break;
        case MapTileType::CONSOLE_BLACK_BACKGROUND:
            t->values[0] = 104;
            t->values[1] = 105;
            t->values[2] = 106;
            t->values[3] = 107;
            t->collidable = false;
            t->destroyable = false;
            t->mapTileType = MapTileType::CONSOLE_BLACK_BACKGROUND;
            break;
        case MapTileType::CAVE_SMOOTH:
            t->values[0] = 108;
            t->values[1] = 109;
            t->values[2] = 110;
            t->values[3] = 111;
            t->collidable = true;
            t->destroyable = true;
            t->mapTileType = MapTileType::CAVE_SMOOTH;
            break;
        case MapTileType::SCORES_STAR_DOOR:
            t->values[0] = 112;
            t->values[1] = 113;
            t->values[2] = 114;
            t->values[3] = 115;
            t->collidable = false;
            t->destroyable = false;
            t->mapTileType = MapTileType::SCORES_STAR_DOOR;
            break;
        case MapTileType::SCORES_SUN_DOOR:
            t->values[0] = 116;
            t->values[1] = 117;
            t->values[2] = 118;
            t->values[3] = 119;
            t->collidable = false;
            t->destroyable = false;
            t->mapTileType = MapTileType::SCORES_SUN_DOOR;
            break;
        case MapTileType::SCORES_MOON_DOOR:
            t->values[0] = 120;
            t->values[1] = 121;
            t->values[2] = 122;
            t->values[3] = 123;
            t->collidable = false;
            t->destroyable = false;
            t->mapTileType = MapTileType::SCORES_MOON_DOOR;
            break;
        case MapTileType::SCORES_CHANGING_DOOR:
            t->values[0] = 124;
            t->values[1] = 125;
            t->values[2] = 126;
            t->values[3] = 127;
            t->collidable = false;
            t->destroyable = false;
            t->mapTileType = MapTileType::SCORES_CHANGING_DOOR;
            break;
        default:
            break;
    }


}

void LevelGenerator::getFirstTile(MapTileType mapTileType, MapTile *&m) {
//    https://stackoverflow.com/questions/416162/assignment-inside-function-that-is-passed-as-pointer?utm_medium=organic&utm_source=google_rich_qa&utm_campaign=google_rich_qa
    for (int a = 0; a < MAP_GAME_WIDTH_TILES; a++) {
        for (int b = 0; b < MAP_GAME_HEIGHT_TILES; b++) {
            if (mapTiles[a][b] != 0 && mapTiles[a][b]->mapTileType == mapTileType) {
                m = mapTiles[a][b];
                break;
            }
        }
    }
}

