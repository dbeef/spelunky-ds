//
// Created by xdbeef on 04.03.18.
//

#include <cstring>
#include <cstdlib>
#include <nds.h>
#include "../globals_declarations.h"

/**
 * https://stackoverflow.com/questions/11072244/c-multiple-definitions-of-a-variable
 */

int floor_div(int a, int b) {
    div_t r = div(a, b);
    if (r.rem != 0 && ((a < 0) ^ (b < 0)))
        r.quot--;
    return r.quot;
}

void floor_div(int a, int b, int *return_value) {
    *return_value = floor_div(a, b);
}


//fixme black-ish background is composed of 16 tiles (4 normal sized tiles),
// that's why it appears to be corrupted after sectorizing
//make it 4 different tiles
void sectorize_map() {

//    printf("BEFORE SECTORIZE \n");
//    for (int a = 0; a < 60; a++)
//        swiWaitForVBlank();

    //malloc always needed on global::current_map declaration! Otherwise won't work
//    u16 *map_temp = (u16 *) std::malloc(sizeof(u16[4096]));
//    u16 *map_temp = new u16[4096];
    dmaCopyHalfWords(3, global::current_map, global::temp_map, sizeof(global::current_map));

    int b = 0;

    for (int a = 0; a < 2048; a++) {

        if (a % 64 >= 32)
            a += 32;

        if (a < 2048) {
            global::current_map[b] = global::temp_map[a];
            b++;
        }
    }

    for (int a = 0; a < 2048; a++) {

        if (a % 64 <= 32)
            a += 32;

        if (a < 2048) {
            global::current_map[b] = global::temp_map[a];
            b++;
        }
    }


    for (int a = 2048; a < 4096; a++) {

        if (a % 64 >= 32)
            a += 32;

        if (a < 4096) {
            global::current_map[b] = global::temp_map[a];
            b++;
        }

    }

    for (int a = 2048; a < 4096; a++) {

        if (a % 64 <= 32)
            a += 32;

        if (a < 4096) {
            global::current_map[b] = global::temp_map[a];
            b++;
        }
    }

//    free(map_temp);
//    delete[] map_temp;

//    printf("AFTER SECTORIZE \n");
//    for (int a = 0; a < 60; a++)
//        swiWaitForVBlank();
}