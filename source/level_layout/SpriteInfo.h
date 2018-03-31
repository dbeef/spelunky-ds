//
// Created by xdbeef on 26.03.18.
//

#ifndef SPELUNKYDS_SPRITEINFO_H
#define SPELUNKYDS_SPRITEINFO_H


#include <nds/arm9/sprite.h>

class SpriteInfo {
public:
    int oamId;
    int width;
    int height;
    int angle;
    SpriteEntry *entry;
};


#endif //SPELUNKYDS_SPRITEINFO_H
