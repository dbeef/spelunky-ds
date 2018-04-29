//
// Created by xdbeef on 25.02.18.
//

#ifndef SPELUNKYDS_SCROLLING_H
#define SPELUNKYDS_SCROLLING_H

#include <nds/arm9/sprite.h>
#include "LevelGenerator.h"
#include "SpriteInfo.h"
#include "../animations/Moniez.h"
#include "../Globals.h"
#include "../animations/Snake.h"
#include "../animations/Jar.h"
#include "../animations/Rock.h"

namespace gameloop {

    void scroll();

    void populateCaveNpcs();

    void populateCaveItems();

}
#endif //SPELUNKYDS_SCROLLING_H
