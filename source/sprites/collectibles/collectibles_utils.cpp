//
// Created by xdbeef on 17.06.18.
//

#include <cstdlib>
#include <cstdio>
#include "collectibles_utils.h"
#include "../../globals_declarations.h"
#include "../items/jetpack.h"
#include "mitt.h"
#include "../items/spike_shoes.h"
#include "shotgun.h"
#include "glove.h"
#include "compass.h"
#include "../items/cape.h"
#include "spring_shoes.h"
#include "pistol.h"

ShoppingObject * collectibles_utils::spawn_item(int x, int y, int r, bool bought) {
    //drop an item
    Pistol *pistol = nullptr;
    SpringShoes *springShoes = nullptr;
    Cape *cape = nullptr;
    Compass *compass = nullptr;
    Glove *glove = nullptr;
    Shotgun *shotgun = nullptr;
    SpikeShoes *spikeShoes = nullptr;
    Mitt *mitt = nullptr;
    Jetpack *jetpack = nullptr;

    switch (r) {
        case 0:
            pistol = new Pistol();
            pistol->bought = bought;
            spawn_set_up(x, y, pistol);
            break;
        case 1:
            springShoes = new SpringShoes();
            springShoes->bought = bought;
            spawn_set_up(x, y, springShoes);
            break;
        case 2:
            cape = new Cape();
            cape->bought = bought;
            spawn_set_up(x, y, cape);
            break;
        case 3:
            compass = new Compass();
            compass->bought = bought;
            spawn_set_up(x, y, compass);
            break;
        case 4:
            glove = new Glove();
            glove->bought = bought;
            spawn_set_up(x, y, glove);
            break;
        case 5:
            shotgun = new Shotgun();
            shotgun->bought = bought;
            spawn_set_up(x, y, shotgun);
            break;
        case 6:
            spikeShoes = new SpikeShoes();
            spikeShoes->bought = bought;
            spawn_set_up(x, y, spikeShoes);
            break;
        case 7:
            mitt = new Mitt();
            mitt->bought = bought;
            spawn_set_up(x, y, mitt);
            break;
        case 8:
            jetpack = new Jetpack();
            jetpack->bought = bought;
            spawn_set_up(x, y, jetpack);
            break;
        default:
            break;
    }

    ShoppingObject *c[9] = {pistol, springShoes, cape, compass, shotgun, spikeShoes, mitt, jetpack, glove};

    for (auto &a : c)
        if (a != nullptr)
            return a;

    return nullptr;
}

void collectibles_utils::spawn_set_up(int x, int y, MovingObject *m) {
    m->init();
    m->x = x + (int) (0.5 * (16 - m->physical_width));
    m->y = y;
    global::sprites_to_add.push_back(m);
}

void collectibles_utils::spawn_random_item(int x, int y) {
    int r = rand() % 9;
//    r = 6;
    spawn_item(x, y, r, true);
}
