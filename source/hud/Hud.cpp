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

void Hud::initHud(const unsigned int tiles[]) {
    frameGfx = (u8*) tiles;
}

void Hud::drawHud() {

    int frame;
    u8* offset;

    frame = 1;
    offset = frameGfx + frame * 64*64;

    heartSpriteInfo->entry->x = HEART_POSITION_X;
    heartSpriteInfo->entry->y = HEART_POSITION_Y;
    heartSpriteInfo->updateFrame(frameGfx);
}