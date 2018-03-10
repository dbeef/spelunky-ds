//
// Created by xdbeef on 04.03.18.
//

#include <tgmath.h>
#include "LevelGenerator.h"

extern u16 map[4096];

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

    std::cout << " " << seed << '\n';


    for (int a = 0; a < ROOMS_X; a++) {
        for (int b = 0; b < ROOMS_Y; b++) {
            layout[a][b] = 0;
        }
    }

    //value of '3' crashes program
    int curr_x = rand() % 3;
    std::cout << " " << curr_x << '\n';

    int curr_y = ROOMS_Y - 1;
    int direction = 0;

    layout[curr_x][curr_y] = 1;

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
            } else
                break;

        }
    }

    for (int b = ROOMS_Y - 1; b >= 0; b--) {
        for (int a = 0; a < ROOMS_X; a++) {
            std::cout << " " << layout[a][b];
        }
        std::cout << ('\n');
    }

}

void LevelGenerator::mapFrame() {

    int frame_index = 0;
    for (int a = 0; a < 32; a++) {
        delete (mapTiles[a][0]);
        MapTile *t = new MapTile();
        t->values[0] = 20;
        t->values[1] = 21;
        t->values[2] = 22;
        t->values[3] = 23;
        t->map_index[0] = frame_index;
        t->map_index[1] = frame_index + 1;
        t->map_index[2] = LINE_WIDTH + frame_index;
        t->map_index[3] = LINE_WIDTH + frame_index + 1;
        this->mapTiles[a][0] = t;
        frame_index += 2;
    }

    frame_index = 0;
    for (int a = 0; a < 32; a++) {
        delete (mapTiles[a][31]);
        MapTile *t = new MapTile();
        t->values[0] = 20;
        t->values[1] = 21;
        t->values[2] = 22;
        t->values[3] = 23;
        t->map_index[0] = 62 * LINE_WIDTH + frame_index;
        t->map_index[1] = 62 * LINE_WIDTH + frame_index + 1;
        t->map_index[2] = 63 * LINE_WIDTH + frame_index;
        t->map_index[3] = 63 * LINE_WIDTH + frame_index + 1;
        this->mapTiles[a][31] = t;
        frame_index += 2;
    }

    frame_index = 0;
    for (int a = 0; a < 32; a++) {
        delete (mapTiles[0][a]);
        MapTile *t = new MapTile();
        t->values[0] = 20;
        t->values[1] = 21;
        t->values[2] = 22;
        t->values[3] = 23;
        t->map_index[0] = frame_index * LINE_WIDTH + 0;
        t->map_index[1] = frame_index * LINE_WIDTH + 1;
        t->map_index[2] = frame_index * LINE_WIDTH + LINE_WIDTH;
        t->map_index[3] = frame_index * LINE_WIDTH + LINE_WIDTH + 1;
        this->mapTiles[0][a] = t;
        frame_index += 2;
    }

    frame_index = 0;
    for (int a = 0; a < 32; a++) {
        delete (mapTiles[31][a]);
        MapTile *t = new MapTile();
        t->values[0] = 20;
        t->values[1] = 21;
        t->values[2] = 22;
        t->values[3] = 23;
        t->map_index[0] = frame_index * LINE_WIDTH - 2;
        t->map_index[1] = frame_index * LINE_WIDTH - 1;
        t->map_index[2] = frame_index * LINE_WIDTH + LINE_WIDTH - 2;
        t->map_index[3] = frame_index * LINE_WIDTH + LINE_WIDTH - 1;
        this->mapTiles[31][a] = t;
        frame_index += 2;
    }
}

void LevelGenerator::tilesToMap() {
    for (int x = 0; x < 32; x++) {
        for (int y = 0; y < 32; y++) {
            MapTile *t = this->mapTiles[x][y];
            for (int k = 0; k < 4; k++)
                map[t->map_index[k]] = t->values[k];
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
                std::cout << " " << px << " " << py << " " << camera_x << " " << camera_y << '\n';
            }
        }
    }
    tilesToMap();
}

void LevelGenerator::generateRooms() {

    bool tab[10][10];

    for (int b = ROOMS_Y - 1; b >= 0; b--) {
        for (int a = 0; a < ROOMS_X; a++) {

            int room_type = layout[a][b];

            if (room_type == 0)
                memcpy(tab, tab_0, sizeof(tab_0));
            if (room_type == 1)
                memcpy(tab, tab_1, sizeof(tab_1));
            if (room_type == 2)
                memcpy(tab, tab_2, sizeof(tab_2));
            if (room_type == 3)
                memcpy(tab, tab_3, sizeof(tab_3));

            for (int tab_y = 0; tab_y < 10; tab_y++) {
                for (int tab_x = 0; tab_x < 10; tab_x++) {
                    if (tab[tab_y][tab_x] == 1) {

                        u16 room_offset =
                                OFFSET_X + OFFSET_Y + 2 * ROOM_WIDTH * a +
                                2 * ROOM_HEIGHT * LINE_WIDTH * ((ROOMS_Y - b) - 1);
                        u16 pos_x = (OFFSET_X + tab_x * 2 + 2 * ROOM_WIDTH * a) / 2;
                        u16 pos_y = (OFFSET_X + tab_y * 2 + 2 * ROOM_HEIGHT * ((ROOMS_Y - b) - 1)) / 2;

                        std::cout << pos_x << " " << pos_y << '\n';
                        delete (mapTiles[pos_x][pos_y]);
                        MapTile *t = new MapTile();
                        t->values[0] = 20;
                        t->values[1] = 21;
                        t->values[2] = 22;
                        t->values[3] = 23;
                        t->map_index[0] = room_offset + (tab_x * 2) + (tab_y * LINE_WIDTH * 2);
                        t->map_index[1] = room_offset + (tab_x * 2) + (tab_y * LINE_WIDTH * 2) + 1;
                        t->map_index[2] = room_offset + (tab_x * 2) + (LINE_WIDTH + (tab_y * LINE_WIDTH * 2));
                        t->map_index[3] = room_offset + (tab_x * 2) + (LINE_WIDTH + (tab_y * LINE_WIDTH * 2)) + 1;
                        this->mapTiles[pos_x][pos_y] = t;
                    }
                }
            }
        }
    }
}
