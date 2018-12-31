//
// Created by xdbeef on 25.02.18.
//

#ifndef SPELUNKYDS_SCROLLING_H
#define SPELUNKYDS_SCROLLING_H

#include <nds/arm9/sprite.h>
#include "tiles/Level.hpp"
#include "creatures/SpriteInfo.h"
#include "interfaces/Moniez.hpp"
#include "GlobalsDeclarations.hpp"
#include "creatures/enemies/Snake.hpp"
#include "creatures/items/Jar.hpp"
#include "creatures/items/Rock.hpp"

namespace gameloop {

    void run();

    void manage_brightness();

}
#endif //SPELUNKYDS_SCROLLING_H
