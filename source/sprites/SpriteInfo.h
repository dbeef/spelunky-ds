//
// Created by xdbeef on 26.03.18.
//

#ifndef SPELUNKYDS_SPRITEINFO_H
#define SPELUNKYDS_SPRITEINFO_H


#include <nds/arm9/sprite.h>
#include "SpritesheetTypes.h"
#include "../memory/OamType.h"

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
    SpritesheetType spriteType;

    u16 oam_address;
    OamType oamType;

    void updateFrame(u8 *tile, int size);

    /*void init();

    void assignValues(OamType o, bool reuse_tiles, bool reuse_pal, int size, int pal_len, const unsigned short *pal_pointer, SpritesheetType s);

    bool reuse_tiles;
    bool reuse_pal;
    int size;
    int pal_len;
    const unsigned short *pal_pointer;*/
};


#endif //SPELUNKYDS_SPRITEINFO_H
