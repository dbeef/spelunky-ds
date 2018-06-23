//
// Created by xdbeef on 26.03.18.
//

#include <nds.h>
#include "sprite_info.h"
#include "../globals_declarations.h"


void SpriteInfo::updateFrame(u8 *tile, int size) {
    //In case of collisions, copy only the current frame
    dmaCopyHalfWords(3,
                     tile,
                     &sprite_address[entry->gfxIndex * this->offset_multiplier],
                     size);


}
/*

void SpriteInfo::init() {

    SpriteInfo *s = nullptr;

    if (oamType == oam_type::MAIN)
        s = global::main_oam_manager->initSprite(pal_pointer, pal_len, nullptr, size * size, size, spritesheet_type,
                                                 reuse_pal,
                                                 reuse_tiles);
    else
        s = global::sub_oam_manager->initSprite(pal_pointer, pal_len, nullptr, size * size, size, spritesheet_type, reuse_pal,
                                                reuse_tiles);

    this->entry = s->entry;
    this->angle = s->angle;
    this->width = s->width;
    this->height = s->height;
    this->offset_multiplier = s->offset_multiplier;
    this->sprite_address = s->sprite_address;
    this->oamId_palette = s->oamId_palette;
    this->oamId_tiles = s->oamId_tiles;
    this->oam_address = s->oam_address;
}

void
SpriteInfo::assignValues(oam_type o, bool reuse_tiles, bool reuse_pal, int size, int pal_len, const unsigned short *pal_pointer, SpritesheetType s) {
    this->oamType = o;
    this->reuse_tiles = reuse_tiles;
    this->reuse_pal = reuse_pal;
    this->size = size;
    this->pal_len = pal_len;
    this->pal_pointer = pal_pointer;
    this->spritesheet_type = s;
}

*/
