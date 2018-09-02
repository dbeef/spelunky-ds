//
// Created by xdbeef on 24.03.18.
//

#include <nds/arm9/sprite.h>
#include <cstdio>
#include <nds/arm9/console.h>
#include <algorithm>
#include "hud.h"
#include "../../build/gfx_hud.h"
#include "../globals_declarations.h"
#include "../sprites/enemies/snake.h"
#include "../sprites/enemies/bat.h"
#include "../sprites/enemies/spider.h"
#include "../sprites/collectibles/moniez.h"

//FIXME This class needs extensive cleaning!

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

void Hud::hide_hud_sprites() {
    heartSpriteInfo->entry->isHidden = true;
    dollarSpriteInfo->entry->isHidden = true;
    bombSpriteInfo->entry->isHidden = true;
    ropeSpriteInfo->entry->isHidden = true;
    holdingItemSpriteInfo->entry->isHidden = true;
}

void Hud::show_hud_sprites() {
    heartSpriteInfo->entry->isHidden = false;
    dollarSpriteInfo->entry->isHidden = false;
    bombSpriteInfo->entry->isHidden = false;
    ropeSpriteInfo->entry->isHidden = false;
    holdingItemSpriteInfo->entry->isHidden = false;
}

void Hud::init_sprites() {

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

    set_hud_sprites_attributes();
}


void Hud::init() {
    consoleClear();
    init_sprites();
}

//call only when something changed (i.e hearts counter decremented and it needs to be updated on hud)
void Hud::draw() {

    consoleClear();

    if (!global::main_dude->dead) {
        printf("\n   %d    %d    %d    %d", hearts, bombs, ropes, money);
        if (dollars_buffer != 0)
            printf("\n                  +%d", dollars_buffer);

        if (thief) {
            printf("\n\n      COME BACK HERE, THIEF!");
        } else if (introduce_shop) {
            printf(shop_name);
        } else if (recently_bough_item) {
            printf("\n\n      YOU GOT A %s!", recently_bought_item_name);
        } else if (not_enough_money) {
            printf("\n\n   YOU HAVEN'T GOT ENOUGH MONEY!");
        } else if (holding_item_shopping) {
            printf("\n\n    %s FOR $%d.\n    PRESS L TO PURCHASE.\n", holding_item_name, *holding_item_cost);
        }

        //TODO Make use of debug flag
        //debug_oam();
    } else {

        hide_hud_sprites();
        printf("\n\n\n\n\n          GAME OVER");

        if (game_over_timer > 1750)
            printf("\n\n\n          FINAL SCORE:");

        if (game_over_timer > 2750) {
            printf("\n          $%d", money);
            printf("\n\n\n    PRESS Y FOR HIGH SCORES.");
        }

    }

}

void Hud::draw_on_level_done() {

    consoleClear();

    int seconds_on_level = (int) time_spent_on_level / 1000;
    int seconds_total = (int) total_time_spent / 1000;
    int minutes_on_level = seconds_on_level / 60;
    int minutes_total = seconds_total / 60;

    seconds_on_level = seconds_on_level - minutes_on_level * 60; //truncating to seconds of last minute
    seconds_total = seconds_total - minutes_total * 60;

    printf("\n\n\n\n\n     LEVEL %d COMPLETED!\n\n     ", level);
    printf("TIME : %02d:%02d - %02d:%02d", minutes_on_level, seconds_on_level, minutes_total, seconds_total);
    printf("\n\n     LOOT : \n\n     KILLS : \n\n     MONEY : %d - %d\n\n", money_on_this_level, money);

    std::sort(global::killed_npcs.begin(), global::killed_npcs.end());
    std::sort(global::collected_loot.begin(), global::collected_loot.end());

    draw_killed_npcs();
    draw_collected_loot();

}

void Hud::draw_scores() {
    hide_hud_sprites();
    printf("\n\n\n\n                TOP DEFILERS");
    printf("\n\n              MONEY: %d", money);
    printf("\n              KILLS: %d", 0);
    printf("\n              SAVES: %d", 0);
    printf("\n\n\n                 STATISTICS");
    printf("\n\n              PLAYS: %d", 0);
    printf("\n              DEATHS: %d", 0);
    printf("\n              WINS: %d", 0);
}

void Hud::update() {

    if (thief) {

        thief_timer += *global::timer;

        if (thief_timer > 4000) {
            thief_timer = 0;
            thief = false;
            draw();
        }
    }

    if (not_enough_money) {

        not_enough_money_timer += *global::timer;

        if (not_enough_money_timer > 4000) {
            not_enough_money_timer = 0;
            not_enough_money = false;
            draw();
        }

    }

    if (introduce_shop) {

        introduce_shop_timer += *global::timer;

        if (introduce_shop_timer > 4000) {
            introduce_shop_timer = 0;
            introduce_shop = false;
            draw();
        }
    }

    if (recently_bough_item) {

        recently_bough_item_timer += *global::timer;

        if (recently_bough_item_timer > 4000) {
            recently_bough_item = false;
            recently_bough_item_timer = 0;
            draw();
        }
    }


    if (collecting_timer > 0) {

        collecting_timer -= *global::timer;

        if (collecting_timer <= 0) {
            collecting_timer = 0;
            dollars_timer = 0;
        }
    }


    if (!global::game_state->in_main_menu && !global::game_state->scores_screen &&
        !global::game_state->levels_transition_screen) {
        set_hud_sprites_attributes();
    }

    if (collecting_timer == 0 && dollars_buffer != 0) {
        dollars_timer += *global::timer;
    }

    if (dollars_timer >= 50) {
        if (dollars_buffer >= 100) {
            money += 100;
            dollars_buffer -= 100;
        } else {
            money += dollars_buffer % 100;
            dollars_buffer -= dollars_buffer % 100;
        }
        dollars_timer = 0;
        draw();
    }

    if (!global::game_state->splash_screen)
        time_spent_on_level += *global::timer;

    if (global::main_dude->dead && global::main_dude->time_since_last_damage > MAIN_DUDE_DAMAGE_PROTECTION_TIME) {

        bool milestone_0 = game_over_timer > 0;
        bool milestone_1750 = game_over_timer > 1750;
        bool milestone_2750 = game_over_timer > 2750;

        game_over_timer += *global::timer;

        if ((!milestone_1750 && game_over_timer > 1750) ||
            (!milestone_2750 && game_over_timer > 2750) ||
            (!milestone_0 && game_over_timer > 0))
            draw();

    }

}

void Hud::add_moniez_on_collected_loot(int value) {
    collecting_timer += 500;
    dollars_buffer += value;
    money_on_this_level += value;
    draw();
}

void Hud::set_hud_sprites_attributes() {

    if (!global::main_dude->dead)
        show_hud_sprites();
    else
        hide_hud_sprites();

    heartSpriteInfo->entry->hFlip = false;
    heartSpriteInfo->entry->vFlip = false;

    dollarSpriteInfo->entry->hFlip = false;
    dollarSpriteInfo->entry->vFlip = false;

    bombSpriteInfo->entry->hFlip = false;
    bombSpriteInfo->entry->vFlip = false;

    ropeSpriteInfo->entry->hFlip = false;
    ropeSpriteInfo->entry->vFlip = false;

    holdingItemSpriteInfo->entry->hFlip = false;
    holdingItemSpriteInfo->entry->vFlip = false;

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


void Hud::increment_offset_on_grabbed_item() {
    items_offset_y += 18;
}

void Hud::debug_oam() {
    printf("\n \n \n  IPM: %d\n IPS: %d\n ITM: %d\n ITS: %d\n N: %d",
           global::main_oam_manager->current_oam_id_palette,
           global::sub_oam_manager->current_oam_id_palette,
           global::main_oam_manager->current_oam_id_tiles,
           global::sub_oam_manager->current_oam_id_tiles,
           global::sub_oam_manager->nextAvailableTileIdx);
}

void Hud::disable_all_prompts() {

    thief = false;
    thief_timer = 0;

    introduce_shop = false;
    introduce_shop_timer = 0;

    recently_bough_item = false;
    recently_bough_item_timer = 0;

    not_enough_money = false;
    not_enough_money_timer = 0;

    holding_item_shopping = false;
}

void Hud::draw_collected_loot() {

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

void Hud::draw_killed_npcs() {

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
        } else if (global::killed_npcs.at(a) == SpritesheetType::SKELETON_SPIDER) {
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
}
