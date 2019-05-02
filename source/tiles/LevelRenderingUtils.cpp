//
// Created by xdbeef on 04.03.18.
//

#include <cstring>
#include <cstdlib>
#include <nds.h>
#include "../GameState.hpp"

//https://stackoverflow.com/questions/11072244/c-multiple-definitions-of-a-variable

//TODO Move this function to math_utils
int floor_div(int a, int b) {
    div_t r = div(a, b);
    if (r.rem != 0 && ((a < 0) ^ (b < 0)))
        r.quot--;
    return r.quot;
}

void sectorize_map() {

    dmaCopyHalfWords(3, GameState::instance().current_map, GameState::instance().temp_map, sizeof(GameState::instance().current_map));

    int b = 0;

    for (int a = 0; a < 2048; a++) {

        if (a % 64 >= 32)
            a += 32;

        if (a < 2048) {
            GameState::instance().current_map[b] = GameState::instance().temp_map[a];
            b++;
        }
    }

    for (int a = 0; a < 2048; a++) {

        if (a % 64 <= 32)
            a += 32;

        if (a < 2048) {
            GameState::instance().current_map[b] = GameState::instance().temp_map[a];
            b++;
        }
    }


    for (int a = 2048; a < 4096; a++) {

        if (a % 64 >= 32)
            a += 32;

        if (a < 4096) {
            GameState::instance().current_map[b] = GameState::instance().temp_map[a];
            b++;
        }

    }

    for (int a = 2048; a < 4096; a++) {

        if (a % 64 <= 32)
            a += 32;

        if (a < 4096) {
            GameState::instance().current_map[b] = GameState::instance().temp_map[a];
            b++;
        }
    }

}