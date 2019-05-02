//
// Created by xdbeef on 17.06.18.
//

#include <cstdlib>
#include <cstdio>
#include "_ItemsUtils.hpp"
#include "../../GameState.hpp"
#include "Jetpack.hpp"
#include "Mitt.hpp"
#include "SpikeShoes.hpp"
#include "Shotgun.hpp"
#include "Glove.hpp"
#include "Compass.hpp"
#include "Cape.hpp"
#include "SpringShoes.hpp"
#include "Pistol.hpp"

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

    switch (6) {
        case 0:
            pistol = new Pistol(0, 0);
            pistol->_bought = bought;
            spawn_set_up(x, y, pistol);
            break;
        case 1:
            springShoes = new SpringShoes(0, 0);
            springShoes->_bought = bought;
            spawn_set_up(x, y, springShoes);
            break;
        case 2:
            cape = new Cape(0, 0);
            cape->_bought = bought;
            spawn_set_up(x, y, cape);
            break;
        case 3:
            compass = new Compass(0, 0);
            compass->_bought = bought;
            spawn_set_up(x, y, compass);
            break;
        case 4:
            glove = new Glove(0, 0);
            glove->_bought = bought;
            spawn_set_up(x, y, glove);
            break;
        case 5:
            shotgun = new Shotgun(0, 0);
            shotgun->_bought = bought;
            spawn_set_up(x, y, shotgun);
            break;
        case 6:
            spikeShoes = new SpikeShoes(0, 0);
            spikeShoes->_bought = bought;
            spawn_set_up(x, y, spikeShoes);
            break;
        case 7:
            mitt = new Mitt(0, 0);
            mitt->_bought = bought;
            spawn_set_up(x, y, mitt);
            break;
        case 8:
            jetpack = new Jetpack(0, 0);
            jetpack->_bought = bought;
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

void collectibles_utils::spawn_set_up(int x, int y, BaseCreature *m) {
    m->_x = x + (int) (0.5 * (16 - m->_physical_width));
    m->_y = y;
    GameState::instance().creatures.push_back(m);
}

void collectibles_utils::spawn_set_up(int x, int y, BaseItem *m) {
    m->_x = x + (int) (0.5 * (16 - m->_physical_width));
    m->_y = y;
    GameState::instance().items.push_back(m);
}

void collectibles_utils::spawn_random_item(int x, int y) {
    int r = rand() % 9;
    spawn_item(x, y, r, true);
}
