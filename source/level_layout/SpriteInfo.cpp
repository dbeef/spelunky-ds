//
// Created by xdbeef on 26.03.18.
//

#include <nds.h>
#include "SpriteInfo.h"


void SpriteInfo::updateFrame(u8 *tile ) {
    //In case of animations, copy only the current frame
    dmaCopyHalfWords(3,
                     tile,
                     &sprite_address[entry->gfxIndex * this->offset_multiplier],
                     16*16);
}