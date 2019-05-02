//
// Created by xdbeef on 25.02.18.
//

#ifndef SPELUNKYDS_SCROLLING_H
#define SPELUNKYDS_SCROLLING_H

#include <nds/arm9/sprite.h>
#include "tiles/Level.hpp"
#include "graphics/SpriteInfo.h"
#include "entities/creatures/Snake.hpp"
#include "entities/items/Jar.hpp"
#include "entities/items/Rock.hpp"

namespace gameloop {

    void run();

    void on_bomb_explosion();

    void update_entities();
}
#endif //SPELUNKYDS_SCROLLING_H
