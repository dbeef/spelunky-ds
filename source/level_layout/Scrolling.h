//
// Created by xdbeef on 25.02.18.
//

#ifndef SPELUNKYDS_SCROLLING_H
#define SPELUNKYDS_SCROLLING_H

#include <nds/arm9/sprite.h>
#include "LevelGenerator.h"
#include "SpriteInfo.h"

namespace spelunker {

    void scroll(int bg_main, int bg_sub, int width, int height, LevelGenerator* l, u16* fresh_map);
    void initOAMTable(OAMTable * oam);

    void updateOAM(OAMTable *oam);

    void initSprites(OAMTable *oam, SpriteInfo *spriteInfo);
}

#endif //SPELUNKYDS_SCROLLING_H
