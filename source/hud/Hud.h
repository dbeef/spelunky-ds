//
// Created by xdbeef on 24.03.18.
//

#ifndef SPELUNKYDS_HUD_H
#define SPELUNKYDS_HUD_H

#include <nds/jtypes.h>
#include "../level_layout/SpriteInfo.h"

class Hud {

public:
    void drawHud();
    void initHud();
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
