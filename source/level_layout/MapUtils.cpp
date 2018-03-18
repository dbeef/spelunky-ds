//
// Created by xdbeef on 04.03.18.
//

#include <cstring>
#include <cstdlib>
#include <nds.h>

/**
 * https://stackoverflow.com/questions/11072244/c-multiple-definitions-of-a-variable
 */
extern u16 map[4096];

void sectorize_map() {

    //malloc always needed on map declaration! Otherwise won't work
    u16 *map_temp = (u16 *) std::malloc(sizeof(u16[4096]));
    dmaCopyHalfWords(3, map, map_temp, sizeof(map));

    int b = 0;

    for (int a = 0; a < 2048; a++) {

        if (a % 64 >= 32)
            a += 32;

        if (a < 2048) {
            map[b] = map_temp[a];
            b++;
        }
    }

    for (int a = 0; a < 2048; a++) {

        if (a % 64 <= 32)
            a += 32;

        if (a < 2048) {
            map[b] = map_temp[a];
            b++;
        }
    }


    for (int a = 2048; a < 4096; a++) {

        if (a % 64 >= 32)
            a += 32;

        if (a < 4096) {
            map[b] = map_temp[a];
            b++;
        }

    }

    for (int a = 2048; a < 4096; a++) {

        if (a % 64 <= 32)
            a += 32;

        if (a < 4096) {
            map[b] = map_temp[a];
            b++;
        }
    }

}