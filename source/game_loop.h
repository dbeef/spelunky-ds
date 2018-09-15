//
// Created by xdbeef on 25.02.18.
//

#ifndef SPELUNKYDS_SCROLLING_H
#define SPELUNKYDS_SCROLLING_H

#include <nds/arm9/sprite.h>
#include "tiles/level.h"
#include "sprites/sprite_info.h"
#include "sprites/collectibles/moniez.h"
#include "globals_declarations.h"
#include "sprites/enemies/snake.h"
#include "sprites/items/jar.h"
#include "sprites/items/rock.h"

namespace gameloop {

    void scroll();

    void populate_cave_npcs();

    void populate_cave_moniez();

    void populate_main_menu();
}
#endif //SPELUNKYDS_SCROLLING_H
