//
// Created by xdbeef on 25.02.18.
//

#ifndef SPELUNKYDS_SCROLLING_H
#define SPELUNKYDS_SCROLLING_H

#include <nds/arm9/sprite.h>
#include "tiles/LevelGenerator.h"
#include "sprites/SpriteInfo.h"
#include "sprites/collectibles/Moniez.h"
#include "Globals.h"
#include "sprites/enemies/Snake.h"
#include "sprites/items/Jar.h"
#include "sprites/items/Rock.h"

namespace gameloop {

    void scroll();

    void populateCaveNpcs();

    void populateCaveItems();

    void populateCaveMoniez();

    void populate_main_menu();
}
#endif //SPELUNKYDS_SCROLLING_H
