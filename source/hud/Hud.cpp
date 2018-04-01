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

#define DOLLAR_POSITION_X 130
#define DOLLAR_POSITION_Y 10

#define HOLDING_ITEM_FRAME_POSITION_X 10
#define HOLDING_ITEM_FRAME_POSITION_Y 25

void Hud::initHud() {
    heartSpriteInfo->entry->x = HEART_POSITION_X;
    heartSpriteInfo->entry->y = HEART_POSITION_Y;

    bombSpriteInfo->entry->x = BOMB_POSITION_X;
    bombSpriteInfo->entry->y = BOMB_POSITION_Y;

    dollarSpriteInfo->entry->x = DOLLAR_POSITION_X;
    dollarSpriteInfo->entry->y = DOLLAR_POSITION_Y;

    ropeSpriteInfo->entry->x = ROPE_POSITION_X;
    ropeSpriteInfo->entry->y = ROPE_POSITION_Y;

    holdingItemSpriteInfo->entry->x = HOLDING_ITEM_FRAME_POSITION_X;
    holdingItemSpriteInfo->entry->y = HOLDING_ITEM_FRAME_POSITION_Y;
}

void Hud::drawHud() {
    //nothing
}