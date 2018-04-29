//
// Created by xdbeef on 24.03.18.
//

#include <nds/arm9/sprite.h>
#include <cstdio>
#include <iostream>
#include <nds/arm9/console.h>
#include <maxmod9.h>
#include "Hud.h"
#include "../../build/hud.h"
#include "../Globals.h"
#include "../../build/soundbank.h"

#define HEART_POSITION_X 5
#define HEART_POSITION_Y 5

#define BOMB_POSITION_X 45
#define BOMB_POSITION_Y 5

#define ROPE_POSITION_X 85
#define ROPE_POSITION_Y 5

#define DOLLAR_POSITION_X 125
#define DOLLAR_POSITION_Y 5

#define HOLDING_ITEM_FRAME_POSITION_X 5
#define HOLDING_ITEM_FRAME_POSITION_Y 20

void Hud::init() {

    consoleClear();

    heartSpriteInfo = global::main_oam_manager->initSprite(hudPal, hudPalLen, nullptr, 16 * 16, 16, HUD, true, false);
    dollarSpriteInfo = global::main_oam_manager->initSprite(hudPal, hudPalLen, nullptr, 16 * 16, 16, HUD, true, false);
    bombSpriteInfo = global::main_oam_manager->initSprite(hudPal, hudPalLen, nullptr, 16 * 16, 16, HUD, true, false);
    ropeSpriteInfo = global::main_oam_manager->initSprite(hudPal, hudPalLen, nullptr, 16 * 16, 16, HUD, true, false);
    holdingItemSpriteInfo = global::main_oam_manager->initSprite(hudPal, hudPalLen, nullptr, 16 * 16, 16, HUD, true,
                                                                 false);

    u8 *frameGfxHeart = (u8 *) hudTiles + 0 * 16 * 16 / 2;
    heartSpriteInfo->updateFrame(frameGfxHeart, 16 * 16);

    u8 *frameGfxHoldingItem = (u8 *) hudTiles + 1 * 16 * 16 / 2;
    holdingItemSpriteInfo->updateFrame(frameGfxHoldingItem, 16 * 16);

    u8 *frameGfxRope = (u8 *) hudTiles + 2 * 16 * 16 / 2;
    ropeSpriteInfo->updateFrame(frameGfxRope, 16 * 16);

    u8 *frameGfxBomb = (u8 *) hudTiles + 3 * 16 * 16 / 2;
    bombSpriteInfo->updateFrame(frameGfxBomb, 16 * 16);

    u8 *frameGfxDollar = (u8 *) hudTiles + 4 * 16 * 16 / 2;
    dollarSpriteInfo->updateFrame(frameGfxDollar, 16 * 16);


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

    hearts = 4;
    ropes = 4;
    bombs = 4;
    dollars = 0;

    std::cout << '\n' << "   " << hearts << "    " << bombs << "    " << ropes << "    " << dollars;

}

//call only when something changed
void Hud::draw() {
    consoleClear();
    std::cout << '\n' << "   " << hearts << "    " << bombs << "    " << ropes << "    " << dollars;
    if (dollars_buffer != 0) {
        std::cout << '\n' << "   " << "    " << "    " << "    " << "   " << "+" << dollars_buffer;

        std::cout << "\n \n \n " << "IPM: " << global::main_oam_manager->current_oam_id_palette << " "
                  << "IPS: " << global::sub_oam_manager->current_oam_id_palette << " "
                  << "ITM: " << global::main_oam_manager->current_oam_id_tiles << " "
                  << "ITS: " << global::sub_oam_manager->current_oam_id_tiles;

    } else {

        std::cout << "\n \n \n " << "IPM: " << global::main_oam_manager->current_oam_id_palette << " "
                  << "IPS: " << global::sub_oam_manager->current_oam_id_palette << " "
                  << "ITM: " << global::main_oam_manager->current_oam_id_tiles << " "
                  << "ITS: " << global::sub_oam_manager->current_oam_id_tiles;
    }


    //TODO Debug flag for this:
}

void Hud::drawSplashScreenOnLevelDone() {
    consoleClear();

    int seconds_on_level = time_spent_on_level / 1000;
    int seconds_total = total_time_spent / 1000;

    int minutes_on_level = seconds_on_level / 60;
    int minutes_total = seconds_total / 60;

    seconds_on_level = seconds_on_level - minutes_on_level * 60;
    seconds_total = seconds_total - minutes_total * 60;

    std::cout << '\n' << '\n' << '\n' << '\n' << '\n' << "   " << "  " << "LEVEL " << level << " COMPLETED!" << '\n'
              << '\n';
    std::cout << "   " << "  " << "TIME : " << minutes_on_level << ":" << seconds_on_level << " - " << minutes_total<< ":" << seconds_total<< '\n' << '\n';
    std::cout << "   " << "  " << "LOOT : " << '\n' << '\n';
    std::cout << "   " << "  " << "KILLS : " << '\n' << '\n';
    std::cout << "   " << "  " << "MONEY : " << money_on_this_level << " - " << dollars << '\n' << '\n';

}

void Hud::updateMoniez() {

    if (collecting_timer > 0) {
        collecting_timer -= *global::timer;

        if (collecting_timer <= 0) {
            collecting_timer = 0;
            dollars_timer = 0;
        }
    }

    if (collecting_timer == 0 && dollars_buffer != 0) {
        dollars_timer += *global::timer;
    }

    if (dollars_timer >= 50) {
        if (dollars_buffer >= 100) {
            dollars += 100;
            dollars_buffer -= 100;
        } else {
            dollars += dollars_buffer % 100;
            dollars_buffer -= dollars_buffer % 100;
        }
        dollars_timer = 0;
        draw();
    }


    if (!global::splash_screen)
        time_spent_on_level += *global::timer;
}

void Hud::collectedMoniez(int value) {
    collecting_timer += 500;
    dollars_buffer += value;
    money_on_this_level += value;
    draw();
}
