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

    void init();

    SpriteInfo *heartSpriteInfo = nullptr;
    SpriteInfo *bombSpriteInfo = nullptr;
    SpriteInfo *dollarSpriteInfo = nullptr;
    SpriteInfo *ropeSpriteInfo = nullptr;
    SpriteInfo *holdingItemSpriteInfo = nullptr;
};


#endif //SPELUNKYDS_HUD_H
