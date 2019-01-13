//
// Created by xdbeef on 25.02.18.
//

#ifndef SPELUNKYDS_SCROLLING_H
#define SPELUNKYDS_SCROLLING_H

#include <nds/arm9/sprite.h>
#include "tiles/Level.hpp"
#include "entities/SpriteInfo.h"
#include "GlobalsDeclarations.hpp"
#include "entities/creatures/Snake.hpp"
#include "entities/items/Jar.hpp"
#include "entities/items/Rock.hpp"

namespace gameloop {

    void run();

    void manage_brightness();

}
#endif //SPELUNKYDS_SCROLLING_H
