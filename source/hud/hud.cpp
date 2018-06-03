//
// Created by xdbeef on 24.03.18.
//

#include <nds/arm9/sprite.h>
#include <cstdio>
#include <iostream>
#include <nds/arm9/console.h>
#include <maxmod9.h>
#include <algorithm>
#include "hud.h"
#include "../../build/gfx_hud.h"
#include "../globals_declarations.h"
#include "../../build/soundbank.h"
#include "../sprites/enemies/snake.h"
#include "../sprites/enemies/bat.h"
#include "../sprites/enemies/spider.h"
#include "../sprites/collectibles/moniez.h"

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

void Hud::hide() {
    heartSpriteInfo->entry->isHidden = true;
    dollarSpriteInfo->entry->isHidden = true;
    bombSpriteInfo->entry->isHidden = true;
    ropeSpriteInfo->entry->isHidden = true;
    holdingItemSpriteInfo->entry->isHidden = true;
    consoleClear();
}

void Hud::show() {
    heartSpriteInfo->entry->isHidden = false;
    dollarSpriteInfo->entry->isHidden = false;
    bombSpriteInfo->entry->isHidden = false;
    ropeSpriteInfo->entry->isHidden = false;
    holdingItemSpriteInfo->entry->isHidden = false;
    draw();
}

void Hud::initSprites() {

    heartSpriteInfo = global::main_oam_manager->initSprite(gfx_hudPal, gfx_hudPalLen, nullptr, 16 * 16, 16, HUD, true,
                                                           false, LAYER_LEVEL::TOP);
    dollarSpriteInfo = global::main_oam_manager->initSprite(gfx_hudPal, gfx_hudPalLen, nullptr, 16 * 16, 16, HUD, true,
                                                            false, LAYER_LEVEL::TOP);
    bombSpriteInfo = global::main_oam_manager->initSprite(gfx_hudPal, gfx_hudPalLen, nullptr, 16 * 16, 16, HUD, true,
                                                          false, LAYER_LEVEL::TOP);
    ropeSpriteInfo = global::main_oam_manager->initSprite(gfx_hudPal, gfx_hudPalLen, nullptr, 16 * 16, 16, HUD, true,
                                                          false, LAYER_LEVEL::TOP);
    holdingItemSpriteInfo = global::main_oam_manager->initSprite(gfx_hudPal, gfx_hudPalLen, nullptr, 16 * 16, 16, HUD,
                                                                 true,
                                                                 false, LAYER_LEVEL::TOP);

    u8 *frameGfxHeart = (u8 *) gfx_hudTiles + 0 * 16 * 16 / 2;
    heartSpriteInfo->updateFrame(frameGfxHeart, 16 * 16);

    u8 *frameGfxHoldingItem = (u8 *) gfx_hudTiles + 1 * 16 * 16 / 2;
    holdingItemSpriteInfo->updateFrame(frameGfxHoldingItem, 16 * 16);

    u8 *frameGfxRope = (u8 *) gfx_hudTiles + 2 * 16 * 16 / 2;
    ropeSpriteInfo->updateFrame(frameGfxRope, 16 * 16);

    u8 *frameGfxBomb = (u8 *) gfx_hudTiles + 3 * 16 * 16 / 2;
    bombSpriteInfo->updateFrame(frameGfxBomb, 16 * 16);

    u8 *frameGfxDollar = (u8 *) gfx_hudTiles + 4 * 16 * 16 / 2;
    dollarSpriteInfo->updateFrame(frameGfxDollar, 16 * 16);

    set_position();
}

void Hud::init() {
    consoleClear();
    initSprites();

    std::cout << '\n' << "   " << hearts << "    " << bombs << "    " << ropes << "    " << dollars;

}

//call only when something changed
void Hud::draw() {
    consoleClear();

    if (!global::main_dude->dead) {
        std::cout << '\n' << "   " << hearts << "    " << bombs << "    " << ropes << "    " << dollars;
        if (dollars_buffer != 0) {
            std::cout << '\n' << "   " << "    " << "    " << "    " << "   " << "+" << dollars_buffer;
/*


        std::cout << "\n \n \n " << "IPM: " << global::main_oam_manager->current_oam_id_palette << " "
                  << "IPS: " << global::sub_oam_manager->current_oam_id_palette << " "
                  << "ITM: " << global::main_oam_manager->current_oam_id_tiles << " "
                  << "ITS: " << global::sub_oam_manager->current_oam_id_tiles
                  << "N: " << global::sub_oam_manager->nextAvailableTileIdx;

        } else {
        std::cout << "\n \n \n " << "IPM: " << global::main_oam_manager->current_oam_id_palette << " "
                  << "IPS: " << global::sub_oam_manager->current_oam_id_palette << " "
                  << "ITM: " << global::main_oam_manager->current_oam_id_tiles << " "
                  << "ITS: " << global::sub_oam_manager->current_oam_id_tiles
                  << "N: " << global::sub_oam_manager->nextAvailableTileIdx;
*/

        }
    }

    if (global::main_dude->dead && global::main_dude->time_since_last_damage > DAMAGE_PROTECTION_TIME) {

        hide();

        std::cout << '\n' << '\n' << '\n' << '\n' << '\n' << "    " << "    " << "  " << "GAME OVER";

        if (game_over_timer > 2000) {
            std::cout << '\n' << '\n' << '\n' << "    " << "    " << "  " << "FINAL SCORE:";
        }

        if (game_over_timer > 3000) {
            std::cout << '\n' << "    " << "    " << "  " << "$" << dollars;
            std::cout << '\n' << '\n' << '\n' << "    " << "" << "PRESS Y FOR HIGH SCORES.";
        }
    }

    //TODO Debug flag for this:
}

void Hud::draw_on_level_done() {
    consoleClear();

    int seconds_on_level = time_spent_on_level / 1000;
    int seconds_total = total_time_spent / 1000;

    int minutes_on_level = seconds_on_level / 60;
    int minutes_total = seconds_total / 60;

    seconds_on_level = seconds_on_level - minutes_on_level * 60;
    seconds_total = seconds_total - minutes_total * 60;


    std::cout << '\n' << '\n' << '\n' << '\n' << '\n' << "   " << "  " << "LEVEL " << level << " COMPLETED!" << '\n'
              << '\n';
    std::cout << "   " << "  " << "TIME : " << minutes_on_level << ":";
    printf("%02d", seconds_on_level);
    std::cout << " - " << minutes_total << ":";
    printf("%02d", seconds_total);
    std::cout << '\n' << '\n';
    std::cout << "   " << "  " << "LOOT : " << '\n' << '\n';
    std::cout << "   " << "  " << "KILLS : " << '\n' << '\n';
    std::cout << "   " << "  " << "MONEY : " << money_on_this_level << " - " << dollars << '\n' << '\n';

    std::sort(global::killed_npcs.begin(), global::killed_npcs.end());
    std::sort(global::collected_loot.begin(), global::collected_loot.end());

    for (int a = 0; a < global::killed_npcs.size(); a++) {
        if (global::killed_npcs.at(a) == SpritesheetType::SNAKE) {
            Snake *snake = new Snake();
            snake->init();
            global::sprites.push_back(snake);
            snake->x = 95 + (a * 8);
            snake->y = 208;
            snake->ready_to_dispose = true;
            snake->set_position();
            snake->set_sprite_left();
        } else if (global::killed_npcs.at(a) == SpritesheetType::BAT_JETPACK) {
            Bat *bat = new Bat();
            bat->init();
            global::sprites.push_back(bat);
            bat->x = 95 + (a * 8);
            bat->y = 208;
            bat->ready_to_dispose = true;
            bat->set_sprite_flying_left();
            bat->set_position();
        } else if (global::killed_npcs.at(a) == SpritesheetType::SPIDER) {
            Spider *spider = new Spider();
            spider->init();
            global::sprites.push_back(spider);
            spider->x = 95 + (a * 8);
            spider->y = 208;
            spider->ready_to_dispose = true;
            spider->set_sprite_falling();
            spider->set_position();
        }
    }

    for (int a = 0; a < global::collected_loot.size(); a++) {
        if (global::collected_loot.at(a) == SpriteType::S_MONIEZ_TRIPLE_GOLDBARS) {
            Moniez *moniez = new Moniez();
            moniez->sprite_height = 16;
            moniez->sprite_width = 16;
            moniez->spritesheet_type = MONIEZ_GOLDBARS;
            moniez->value = 1000;
            moniez->init();
            global::sprites.push_back(moniez);
            moniez->x = 90 + (a * 8);
            moniez->y = 190;
            moniez->ready_to_dispose = true;
            moniez->set_position();

        } else if (global::collected_loot.at(a) == SpriteType::S_MONIEZ_RUBY_BIG_RED) {

            Moniez *moniez = new Moniez();
            moniez->sprite_height = 8;
            moniez->sprite_width = 8;
            moniez->spritesheet_type = MONIEZ_RUBY;
            moniez->value = 1200;
            moniez->initSprite();
            global::sprites.push_back(moniez);
            moniez->x = 96 + (a * 8);
            moniez->y = 198;
            moniez->ready_to_dispose = true;
            moniez->set_position();

        } else if (global::collected_loot.at(a) == SpriteType::S_MONIEZ_RUBY_BIG_GREEN) {

            Moniez *moniez = new Moniez();
            moniez->sprite_height = 8;
            moniez->sprite_width = 8;
            moniez->spritesheet_type = MONIEZ_RUBY;
            moniez->value = 1200;
            moniez->initSprite();
            global::sprites.push_back(moniez);
            moniez->x = 96 + (a * 8);
            moniez->y = 198;
            moniez->ready_to_dispose = true;
            moniez->set_position();

        } else if (global::collected_loot.at(a) == SpriteType::S_MONIEZ_RUBY_BIG_BLUE) {

            Moniez *moniez = new Moniez();
            moniez->sprite_height = 8;
            moniez->sprite_width = 8;
            moniez->spritesheet_type = MONIEZ_RUBY;
            moniez->value = 1200;
            moniez->initSprite();
            global::sprites.push_back(moniez);
            moniez->x = 96 + (a * 8);
            moniez->y = 198;
            moniez->ready_to_dispose = true;
            moniez->set_position();
        }


    }


}

void Hud::draw_scores() {

    hide();
    std::cout << '\n' << '\n' << '\n' << "    " << "    " << "    " << "    " << "TOP DEFILERS";
    std::cout << '\n' << '\n' << "    " << "    " << "    " << "  " << "MONEY:" << "  " << dollars;
    std::cout << '\n' << '\n' << "    " << "    " << "    " << "  " << "KILLS:" << "  " << 0;
    std::cout << '\n' << '\n' << "    " << "    " << "    " << "  " << "SAVES:" << "  " << 0;
    std::cout << '\n' << '\n' << "    " << "    " << "    " << "    " << "STATISTICS";
    std::cout << '\n' << '\n' << "    " << "    " << "    " << "  " << "PLAYS: " << "  " << 0;
    std::cout << '\n' << '\n' << "    " << "    " << "    " << "  " << "DEATHS:" << "  " << 0;
    std::cout << '\n' << '\n' << "    " << "    " << "    " << "  " << "WINS:  " << "  " << 0;

}

void Hud::update() {

    if (!global::game_state->in_main_menu && !global::game_state->scores_screen &&
        !global::game_state->levels_transition_screen) {
        set_position();
    }

//    std::cout<<*global::timer << '\n';

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


    if (!global::game_state->splash_screen)
        time_spent_on_level += *global::timer;
    if (global::main_dude->dead && global::main_dude->time_since_last_damage > DAMAGE_PROTECTION_TIME) {
        draw();
        game_over_timer += *global::timer;
    }

}

void Hud::collectedMoniez(int value) {
    collecting_timer += 500;
    dollars_buffer += value;
    money_on_this_level += value;
    draw();
}

void Hud::set_position() {

//    std::cout<<"SET POS" << '\n';

    heartSpriteInfo->entry->hFlip = false;
    heartSpriteInfo->entry->vFlip = false;
    heartSpriteInfo->entry->isHidden = false;

    dollarSpriteInfo->entry->hFlip = false;
    dollarSpriteInfo->entry->vFlip = false;
    dollarSpriteInfo->entry->isHidden = false;

    bombSpriteInfo->entry->hFlip = false;
    bombSpriteInfo->entry->vFlip = false;
    bombSpriteInfo->entry->isHidden = false;

    ropeSpriteInfo->entry->hFlip = false;
    ropeSpriteInfo->entry->vFlip = false;
    ropeSpriteInfo->entry->isHidden = false;

    holdingItemSpriteInfo->entry->hFlip = false;
    holdingItemSpriteInfo->entry->vFlip = false;
    holdingItemSpriteInfo->entry->isHidden = false;

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

Hud::Hud() {
    hearts = MAIN_DUDE_HITPOINTS;
    items_offset_y = 0;
}

void Hud::next_item() {
    items_offset_y += 18;
}