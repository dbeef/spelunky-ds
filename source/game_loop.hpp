//
// Created by xdbeef on 25.02.18.
//

#ifndef SPELUNKYDS_SCROLLING_H
#define SPELUNKYDS_SCROLLING_H

#include <nds/arm9/sprite.h>
#include "tiles/level.hpp"
#include "creatures/sprite_info.h"
#include "creatures/collectibles/moniez.hpp"
#include "globals_declarations.hpp"
#include "creatures/enemies/snake.hpp"
#include "creatures/items/jar.hpp"
#include "creatures/items/rock.hpp"

namespace gameloop {

    void run();

    void manage_brightness();

}
#endif //SPELUNKYDS_SCROLLING_H
