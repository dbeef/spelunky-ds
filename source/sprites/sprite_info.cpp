//
// Created by xdbeef on 26.03.18.
//

#include <nds.h>
#include "sprite_info.h"
#include "../globals_declarations.hpp"

void SpriteInfo::updateFrame(u8 *tile, u32 size) {
    dmaCopyHalfWords(3, tile, &sprite_address[entry->gfxIndex * this->offset_multiplier], size);
}