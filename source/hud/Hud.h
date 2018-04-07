//
// Created by xdbeef on 24.03.18.
//

#ifndef SPELUNKYDS_HUD_H
#define SPELUNKYDS_HUD_H

#include <nds/jtypes.h>
#include "../level_layout/SpriteInfo.h"
#include "../animations/OAMManager.h"

class Hud {

public:
    void draw();
    void init(OAMManager *mainOam);
    SpriteInfo* heartSpriteInfo;
    SpriteInfo* bombSpriteInfo;
    SpriteInfo* dollarSpriteInfo;
    SpriteInfo* ropeSpriteInfo;
    SpriteInfo* holdingItemSpriteInfo;
//    u16 *heartGfxMemMain;
//    u16 *bombGfxMemMain;
//    u16 *ropeGfxMemMain;
//    u16 *goldGfxMemMain;
//    u16 *holdingObjFrameGfxMemMain;
};


#endif //SPELUNKYDS_HUD_H
