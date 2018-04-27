//
// Created by xdbeef on 04.03.18.
//

#include <tgmath.h>
#include "LevelGenerator.h"
#include "../Globals.h"


#define ROOM_WIDTH 10
#define ROOM_HEIGHT 10
#define GRIT_TILE_WIDTH 8
#define GRIT_TILE_HEIGHT 8
#define MAPPY_TILE_WIDTH 16
#define MAPPY_TILE_HEIGHT 16
#define LINE_WIDTH 64
//Offset of 2 tiles, 8 px each
#define OFFSET_X 2
//Offset of 128 tiles, 8px  each (2 lines, 64 each)
#define OFFSET_Y 128

/**
 * Wartość layout
 * 0 brak wyjść
 * 1 wyjście w lewo + prawo
 * 2 wyjście w lewo, prawo dół
 * 3 wyjście w dół, lewo, prawo, w górę
 * Dest
 * idziemy w lewo
 * idziemy w prawo
 * idziemy w dół
 * http:www.cplusplus.com/reference/cstdlib/rand/, liczba z przedziału 0-3
 * punkt 0,0 to lewy górny róg mapy
 * @param seed 
 */
void LevelGenerator::newLayout(int seed) {

    for (int x = 0; x < 32; x++) {
        for (int y = 0; y < 32; y++) {
            mapTiles[x][y] = 0;
        }
    }

    srand(timerElapsed(1));

//    std::cout << " " << seed << '\n';

    for (int a = 0; a < ROOMS_X; a++) {
        for (int b = 0; b < ROOMS_Y; b++) {
            layout[a][b] = 0;
        }
    }

    //value of '3' crashes program
    int curr_x = rand() % 3;
//    std::cout << " " << curr_x << '\n';

    int curr_y = ROOMS_Y - 1;
    int direction = 0;

    layout[curr_x][curr_y] = 5;

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
                layout[curr_x][curr_y] = 1;
            }
        } else if (direction == 2) {

            if (curr_x == ROOMS_X - 1) {
                direction = 3;
            } else {
                curr_x++;
                layout[curr_x][curr_y] = 1;
            }

        } else if (direction == 3) {

            if (curr_y > 0) {

                layout[curr_x][curr_y] = 2;

                if (curr_y >= 1) {
                    layout[curr_x][curr_y - 1] = 3;
                }


                direction = 0;
                curr_y--;

                if (curr_y == 0)
                    layout[curr_x][curr_y] = 6;

            } else {
                break;
            }
        }
    }

    for (int b = ROOMS_Y - 1; b >= 0; b--) {
        for (int a = 0; a < ROOMS_X; a++) {
//            std::cout << " " << layout[a][b];
        }
//        std::cout << ('\n');
    }

}

void LevelGenerator::mapBackground() {
/*
    for (int a = 0; a < 32; a += 2)
        for (int b = 0; b < 32; b += 2) {
            delete (mapTiles[b][a]);
            MapTile *t = new MapTile();
            t->values[0] = 0;
            t->values[1] = 1;
            t->values[2] = 2;
            t->values[3] = 3;
            t->map_index[0] = (b * 2) + 0 + (a * LINE_WIDTH);
            t->map_index[1] = (b * 2) + 1 + (a * LINE_WIDTH);
            t->map_index[2] = (b * 2) + 0 + (a * LINE_WIDTH) + LINE_WIDTH;
            t->map_index[3] = (b * 2) + 1 + (a * LINE_WIDTH) + LINE_WIDTH;
            t->collidable = false;
            this->mapTiles[b][a] = t;
        }

    for (int a = 0; a < 32; a += 2)
        for (int b = 1; b < 32; b += 2) {
            delete (mapTiles[b][a]);
            MapTile *t = new MapTile();
            t->values[0] = 4;
            t->values[1] = 5;
            t->values[2] = 6;
            t->values[3] = 7;
            t->map_index[0] = (b * 2) + 0 + (a * LINE_WIDTH);
            t->map_index[1] = (b * 2) + 1 + (a * LINE_WIDTH);
            t->map_index[2] = (b * 2) + 0 + (a * LINE_WIDTH) + LINE_WIDTH;
            t->map_index[3] = (b * 2) + 1 + (a * LINE_WIDTH) + LINE_WIDTH;
            t->collidable = false;
            this->mapTiles[b][a] = t;
        }

    for (int a = 1; a < 32; a += 2)
        for (int b = 0; b < 32; b += 2) {
            delete (mapTiles[b][a]);
            MapTile *t = new MapTile();
            t->values[0] = 8;
            t->values[1] = 9;
            t->values[2] = 10;
            t->values[3] = 11;
            t->map_index[0] = (b * 2) + 0 + (a * LINE_WIDTH);
            t->map_index[1] = (b * 2) + 1 + (a * LINE_WIDTH);
            t->map_index[2] = (b * 2) + 0 + (a * LINE_WIDTH) + LINE_WIDTH;
            t->map_index[3] = (b * 2) + 1 + (a * LINE_WIDTH) + LINE_WIDTH;
            t->collidable = false;
            this->mapTiles[b][a] = t;
        }

    for (int a = 1; a < 32; a += 2)
        for (int b = 1; b < 32; b += 2) {
            delete (mapTiles[b][a]);
            MapTile *t = new MapTile();
            t->values[0] = 12;
            t->values[1] = 13;
            t->values[2] = 14;
            t->values[3] = 15;
            t->map_index[0] = (b * 2) + 0 + (a * LINE_WIDTH);
            t->map_index[1] = (b * 2) + 1 + (a * LINE_WIDTH);
            t->map_index[2] = (b * 2) + 0 + (a * LINE_WIDTH) + LINE_WIDTH;
            t->map_index[3] = (b * 2) + 1 + (a * LINE_WIDTH) + LINE_WIDTH;
            t->collidable = false;
            this->mapTiles[b][a] = t;
        }*/
}

void LevelGenerator::mapFrame() {

    for (int a = 0; a < 32; a++) {
        delete (mapTiles[a][0]);
        MapTile *t = new MapTile();
        t->mapTileType = MapTileType::REGULAR;

        t->values[0] = 20;
        t->values[1] = 21;
        t->values[2] = 22;
        t->values[3] = 23;
        t->map_index[0] = a * 2;
        t->map_index[1] = a * 2 + 1;
        t->map_index[2] = LINE_WIDTH + a * 2;
        t->map_index[3] = LINE_WIDTH + a * 2 + 1;
        t->collidable = true;
        t->x = a;
        t->y = 0;
        this->mapTiles[a][0] = t;
    }

    for (int a = 0; a < 32; a++) {
        delete (mapTiles[a][31]);
        MapTile *t = new MapTile();
        t->mapTileType = MapTileType::REGULAR;

        t->values[0] = 20;
        t->values[1] = 21;
        t->values[2] = 22;
        t->values[3] = 23;
        t->map_index[0] = 62 * LINE_WIDTH + a * 2;
        t->map_index[1] = 62 * LINE_WIDTH + a * 2 + 1;
        t->map_index[2] = 63 * LINE_WIDTH + a * 2;
        t->map_index[3] = 63 * LINE_WIDTH + a * 2 + 1;
        t->collidable = true;
        t->x = a;
        t->y = 31;
        this->mapTiles[a][31] = t;
    }

    for (int a = 0; a < 32; a++) {
        delete (mapTiles[0][a]);
        MapTile *t = new MapTile();
        t->mapTileType = MapTileType::REGULAR;

        t->values[0] = 20;
        t->values[1] = 21;
        t->values[2] = 22;
        t->values[3] = 23;
        t->map_index[0] = a * 2 * LINE_WIDTH + 0;
        t->map_index[1] = a * 2 * LINE_WIDTH + 1;
        t->map_index[2] = a * 2 * LINE_WIDTH + LINE_WIDTH;
        t->map_index[3] = a * 2 * LINE_WIDTH + LINE_WIDTH + 1;
        t->collidable = true;
        t->x = 0;
        t->y = a;
        this->mapTiles[0][a] = t;
    }

    for (int a = 0; a < 32; a++) {
        delete (mapTiles[31][a]);
        MapTile *t = new MapTile();
        t->mapTileType = MapTileType::REGULAR;
        t->values[0] = 20;
        t->values[1] = 21;
        t->values[2] = 22;
        t->values[3] = 23;
        t->map_index[0] = a * 2 * LINE_WIDTH - 2 + LINE_WIDTH;
        t->map_index[1] = a * 2 * LINE_WIDTH - 1 + LINE_WIDTH;
        t->map_index[2] = a * 2 * LINE_WIDTH + LINE_WIDTH - 2 + LINE_WIDTH;
        t->map_index[3] = a * 2 * LINE_WIDTH + LINE_WIDTH - 1 + LINE_WIDTH;
        t->collidable = true;
        t->x = 31;
        t->y = a;
        this->mapTiles[31][a] = t;
    }
}

void LevelGenerator::tilesToMap() {
    for (int x = 0; x < 32; x++) {
        for (int y = 0; y < 32; y++) {
            MapTile *t = this->mapTiles[x][y];
            for (int k = 0; k < 4; k++)
                if (t && t->values[k] != 0)
                    global::current_map[t->map_index[k]] = t->values[k];
        }
    }
}

void LevelGenerator::clearCollidedTile(int px, int py, int camera_x, int camera_y, int bg) {
    for (int x = 0; x < 32; x++) {
        for (int y = 0; y < 32; y++) {

            MapTile *t = this->mapTiles[x][y];

            int viewport_x = px + camera_x;
            int viewport_y = py + camera_y;

            bool x_collision = viewport_x >= (x * 16) && viewport_x <= (x * 16) + 16;
            bool y_collision = viewport_y >= (y * 16) && viewport_y <= (y * 16) + 16;

            if (x_collision && y_collision) {
                t->values[0] = 0;
                t->values[1] = 0;
                t->values[2] = 0;
                t->values[3] = 0;
//                std::cout << " " << px << " " << py << " " << camera_x << " " << camera_y << '\n';
            }
        }
    }
    tilesToMap();
}

void LevelGenerator::generateRooms() {

    int tab[10][10];
    int r;

    for (int b = ROOMS_Y - 1; b >= 0; b--) {
        for (int a = 0; a < ROOMS_X; a++) {

            int room_type = layout[a][b];

            if (room_type == 5) {
                memcpy(tab, entrance_room, sizeof(entrance_room));
            }
            if (room_type == 6) {
                memcpy(tab, exit_room, sizeof(exit_room));
            }
            if (room_type == 0) {
                r = (rand() % 3) + 1;
                if (r == 1)
                    memcpy(tab, tab_0_1, sizeof(tab_0_1));
                else if (r == 2)
                    memcpy(tab, tab_0_2, sizeof(tab_0_2));
                else if (r == 3)
                    memcpy(tab, tab_0_3, sizeof(tab_0_3));
            }

            if (room_type == 1) {
                r = (rand() % 3) + 1;
                if (r == 1)
                    memcpy(tab, tab_1_1, sizeof(tab_1_1));
                else if (r == 2)
                    memcpy(tab, tab_1_2, sizeof(tab_1_2));
                else if (r == 3)
                    memcpy(tab, tab_1_3, sizeof(tab_1_3));

            }
            if (room_type == 2) {
                r = (rand() % 3) + 1;
                if (r == 1)
                    memcpy(tab, tab_2_1, sizeof(tab_2_1));
                else if (r == 2)
                    memcpy(tab, tab_2_2, sizeof(tab_2_2));
                else if (r == 3)
                    memcpy(tab, tab_2_3, sizeof(tab_2_3));
            }
            if (room_type == 3) {
                r = (rand() % 3) + 1;
                if (r == 1)
                    memcpy(tab, tab_3_1, sizeof(tab_3_1));
                else if (r == 2)
                    memcpy(tab, tab_3_2, sizeof(tab_3_2));
                else if (r == 3)
                    memcpy(tab, tab_3_3, sizeof(tab_3_3));

            }
            for (int tab_y = 0; tab_y < 10; tab_y++) {
                for (int tab_x = 0; tab_x < 10; tab_x++) {
                    if (tab[tab_y][tab_x] != 0) {

                        u16 room_offset =
                                OFFSET_X + OFFSET_Y + 2 * ROOM_WIDTH * a +
                                2 * ROOM_HEIGHT * LINE_WIDTH * ((ROOMS_Y - b) - 1);
                        u16 pos_x = (OFFSET_X + tab_x * 2 + 2 * ROOM_WIDTH * a) / 2;
                        u16 pos_y = (OFFSET_X + tab_y * 2 + 2 * ROOM_HEIGHT * ((ROOMS_Y - b) - 1)) / 2;

//                        std::cout << pos_x << " " << pos_y << '\n';
                        delete (mapTiles[pos_x][pos_y]);
                        MapTile *t = new MapTile();
                        t->destroyable = true;
                        t->collidable = true;
                        t->mapTileType = MapTileType::REGULAR;

                        if (tab[tab_y][tab_x] == 1) {
                            t->values[0] = 20;
                            t->values[1] = 21;
                            t->values[2] = 22;
                            t->values[3] = 23;
                        } else if (tab[tab_y][tab_x] == 2) {
                            t->values[0] = 40;
                            t->values[1] = 41;
                            t->values[2] = 42;
                            t->values[3] = 43;
                        } else if (tab[tab_y][tab_x] == 5) {
                            t->values[0] = 24;
                            t->values[1] = 25;
                            t->values[2] = 26;
                            t->values[3] = 27;
                        } else if (tab[tab_y][tab_x] == 6) {
                            t->values[0] = 32;
                            t->values[1] = 33;
                            t->values[2] = 34;
                            t->values[3] = 35;
                        } else if (tab[tab_y][tab_x] == 7) {
                            t->values[0] = 36;
                            t->values[1] = 37;
                            t->values[2] = 38;
                            t->values[3] = 39;
                        } else if (tab[tab_y][tab_x] == 8) {
                            t->values[0] = 16;
                            t->values[1] = 17;
                            t->values[2] = 18;
                            t->values[3] = 19;
                        } else if (tab[tab_y][tab_x] == 9) {

                            t->values[0] = 48;
                            t->values[1] = 49;
                            t->values[2] = 50;
                            t->values[3] = 51;

                            t->collidable = false;
                            t->destroyable = false;
                            t->mapTileType = MapTileType::LADDER;

                        } else if (tab[tab_y][tab_x] == 10) {

                            t->values[0] = 52;
                            t->values[1] = 53;
                            t->values[2] = 54;
                            t->values[3] = 55;

                            t->collidable = false;
                            t->destroyable = false;
                            t->mapTileType = MapTileType::LADDER_WITH_DECK;

                        } else if (tab[tab_y][tab_x] == 12) {

                            t->values[0] = 56;
                            t->values[1] = 57;
                            t->values[2] = 58;
                            t->values[3] = 59;

                        } else if (tab[tab_y][tab_x] == 13) {

                            t->values[0] = 60;
                            t->values[1] = 61;
                            t->values[2] = 62;
                            t->values[3] = 63;

                        } else if (tab[tab_y][tab_x] == 11) {

                            t->values[0] = 64;
                            t->values[1] = 65;
                            t->values[2] = 66;
                            t->values[3] = 67;
                            t->collidable = false;
                            t->destroyable = false;
                            t->mapTileType = MapTileType::ENTRANCE;

                        } else if (tab[tab_y][tab_x] == 14) {

                            t->values[0] = 68;
                            t->values[1] = 69;
                            t->values[2] = 70;
                            t->values[3] = 71;
                            t->collidable = false;
                            t->destroyable = false;
                            t->mapTileType = MapTileType::EXIT;

                        } else if (tab[tab_y][tab_x] == 3) {
                            t->values[0] = 28;
                            t->values[1] = 29;
                            t->values[2] = 30;
                            t->values[3] = 31;
                        } else if (tab[tab_y][tab_x] == 4) {
                            t->values[0] = 44;
                            t->values[1] = 45;
                            t->values[2] = 46;
                            t->values[3] = 47;
                        }

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

void LevelGenerator::getEntranceTile(MapTile *&m) {
    https://stackoverflow.com/questions/416162/assignment-inside-function-that-is-passed-as-pointer?utm_medium=organic&utm_source=google_rich_qa&utm_campaign=google_rich_qa
    for (int a = 0; a < 32; a++) {
        for (int b = 0; b < 32; b++) {
            if (mapTiles[a][b] != 0 && mapTiles[a][b]->mapTileType == MapTileType::ENTRANCE) {
                m = mapTiles[a][b];
                break;
            }
        }
    }

}
