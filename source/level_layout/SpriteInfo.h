//
// Created by xdbeef on 26.03.18.
//

#ifndef SPELUNKYDS_SPRITEINFO_H
#define SPELUNKYDS_SPRITEINFO_H


#include <nds/arm9/sprite.h>
#include "../sprites/SpriteTypes.h"
#include "../animations/OamType.h"

class SpriteInfo {
public:
    u16 *sprite_address;
    int offset_multiplier;

    int oamId_palette;
    int oamId_tiles;

    int width;
    int height;
    int angle;
    SpriteEntry *entry;
    SpriteType spriteType;

    u16 oam_address;
    OamType oamType;

    void updateFrame(u8 *tile, int size);
};


#endif //SPELUNKYDS_SPRITEINFO_H
