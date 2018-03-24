//
// Created by xdbeef on 24.03.18.
//

#include <nds/arm9/sprite.h>
#include <nds.h>
#include "Hud.h"
#include "../../build/heart.h"

#define HEART_POSITION_X 10
#define HEART_POSITION_Y 10

#define BOMB_POSITION_X 50
#define BOMB_POSITION_Y 10

#define ROPE_POSITION_X 90
#define ROPE_POSITION_Y 10

#define GOLD_POSITION_X 130
#define GOLD_POSITION_Y 10

#define HOLDING_OBJ_FRAME_POSITION_X 10
#define HOLDING_OBJ_FRAME_POSITION_Y 50

void Hud::initHud() {
    heartGfxMemMain = oamAllocateGfx(&oamMain, SpriteSize_16x16, SpriteColorFormat_256Color);
    dmaCopy(heartPal, SPRITE_PALETTE, 512);
}

void Hud::drawHud() {
    dmaCopy(heartTiles, heartGfxMemMain, 16 * 16);
    oamSet(&oamMain, 0, HEART_POSITION_X, HEART_POSITION_Y, 0, 0, SpriteSize_16x16, SpriteColorFormat_256Color,
           heartGfxMemMain, -1, false, false, false, false, false);
}