//
// Created by xdbeef on 24.03.18.
//

#include <nds/arm9/sprite.h>
#include <cstdio>
#include <algorithm>

#include "../../build/gfx_hud.h"
#include "Hud.hpp"
#include "../GlobalsDeclarations.hpp"
#include "../creatures/enemies/Snake.hpp"
#include "../creatures/enemies/Bat.hpp"
#include "../creatures/enemies/Spider.hpp"
#include "../creatures/SpriteUtils.hpp"
#include "../creatures/enemies/Damsel.hpp"
#include "../creatures/enemies/Skeleton.hpp"
#include "../creatures/enemies/Caveman.hpp"
#include "../creatures/enemies/Shopkeeper.hpp"
#include "../creatures/treasures/RubyBig.h"
#include "../creatures/treasures/RubySmall.h"
#include "../creatures/treasures/TripleGoldbar.h"
#include "../creatures/treasures/SingleGoldbar.hpp"

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

#define HUD_ICON_WIDTH 16
#define HUD_ICON_HEIGHT 16
#define HUD_ICON_SIZE HUD_ICON_WIDTH * HUD_ICON_HEIGHT

void Hud::delete_sprites(){

    delete heartSpriteInfo;
    delete dollarSpriteInfo;
    delete bombSpriteInfo;
    delete ropeSpriteInfo;
    delete holdingItemSpriteInfo;

    heartSpriteInfo = nullptr;
    dollarSpriteInfo = nullptr;
    bombSpriteInfo = nullptr;
    ropeSpriteInfo = nullptr;
    holdingItemSpriteInfo = nullptr;
}

void Hud::init_sprites() {

    delete_sprites();

    heartSpriteInfo = global::main_oam_manager->initSprite(gfx_hudPal, gfx_hudPalLen,
                                                           nullptr, HUD_ICON_SIZE,
                                                           ObjSize::OBJSIZE_16, HUD, true, false, LAYER_LEVEL::TOP);
    dollarSpriteInfo = global::main_oam_manager->initSprite(gfx_hudPal, gfx_hudPalLen,
                                                            nullptr, HUD_ICON_SIZE,
                                                            ObjSize::OBJSIZE_16, HUD, true, false, LAYER_LEVEL::TOP);
    bombSpriteInfo = global::main_oam_manager->initSprite(gfx_hudPal, gfx_hudPalLen,
                                                          nullptr, HUD_ICON_SIZE,
                                                          ObjSize::OBJSIZE_16, HUD, true, false, LAYER_LEVEL::TOP);
    ropeSpriteInfo = global::main_oam_manager->initSprite(gfx_hudPal, gfx_hudPalLen,
                                                          nullptr, HUD_ICON_SIZE,
                                                          ObjSize::OBJSIZE_16, HUD, true, false, LAYER_LEVEL::TOP);
    holdingItemSpriteInfo = global::main_oam_manager->initSprite(gfx_hudPal, gfx_hudPalLen,
                                                                 nullptr, HUD_ICON_SIZE,
                                                                 ObjSize::OBJSIZE_16, HUD, true, false, LAYER_LEVEL::TOP);

    u8 *frameGfxHeart = sprite_utils::get_frame((u8 *) gfx_hudTiles, HUD_ICON_SIZE, 0);
    u8 *frameGfxHoldingItem = sprite_utils::get_frame((u8 *) gfx_hudTiles, HUD_ICON_SIZE, 1);
    u8 *frameGfxRope = sprite_utils::get_frame((u8 *) gfx_hudTiles, HUD_ICON_SIZE, 2);
    u8 *frameGfxBomb = sprite_utils::get_frame((u8 *) gfx_hudTiles, HUD_ICON_SIZE, 3);
    u8 *frameGfxDollar = sprite_utils::get_frame((u8 *) gfx_hudTiles, HUD_ICON_SIZE, 4);

    heartSpriteInfo->updateFrame(frameGfxHeart, HUD_ICON_SIZE);
    holdingItemSpriteInfo->updateFrame(frameGfxHoldingItem, HUD_ICON_SIZE);
    ropeSpriteInfo->updateFrame(frameGfxRope, HUD_ICON_SIZE);
    bombSpriteInfo->updateFrame(frameGfxBomb, HUD_ICON_SIZE);
    dollarSpriteInfo->updateFrame(frameGfxDollar, HUD_ICON_SIZE);

    set_hud_sprites_attributes();
}


void Hud::init() {
    consoleClear();
    init_sprites();
}

//Call only when something changed (i.e hearts counter decremented and it needs to be updated on hud)
//otherwise, if called frequently, font blinking will occur.
//TODO Statically sized array - printing to the buffer, then to the console, to avoid screen flickering.
//^ other way to avoid flickering is OAM sprite for every digit (but then it would take some from the 128 creatures pool).
//^ btw, flickering is very noticable on an emulator, but not that much on a real NDS
void Hud::draw_level_hud() {

    consoleClear();

    if (!global::main_dude->dead) {

        if (dollars_buffer != 0) {
            printf("\n   %d    %d    %d    %d\n                  +%d",
                   hearts, bombs, ropes, money, dollars_buffer);
        } else
            printf("\n   %d    %d    %d    %d", hearts, bombs, ropes, money);

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

        sprite_utils::set_visibility(false, heartSpriteInfo, bombSpriteInfo,
                                     ropeSpriteInfo, dollarSpriteInfo, holdingItemSpriteInfo);

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
    std::sort(global::collected_treasures.begin(), global::collected_treasures.end());

    draw_killed_npcs();
    draw_collected_loot();

}

void Hud::draw_scores() {
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

    set_hud_sprites_attributes();

    if (thief) {

        thief_timer += *global::timer;

        if (thief_timer > 4000) {
            thief_timer = 0;
            thief = false;
            draw_level_hud();
        }
    }

    if (not_enough_money) {

        not_enough_money_timer += *global::timer;

        if (not_enough_money_timer > 4000) {
            not_enough_money_timer = 0;
            not_enough_money = false;
            draw_level_hud();
        }

    }

    if (introduce_shop) {

        introduce_shop_timer += *global::timer;

        if (introduce_shop_timer > 4000) {
            introduce_shop_timer = 0;
            introduce_shop = false;
            draw_level_hud();
        }
    }

    if (recently_bough_item) {

        recently_bough_item_timer += *global::timer;

        if (recently_bough_item_timer > 4000) {
            recently_bough_item = false;
            recently_bough_item_timer = 0;
            draw_level_hud();
        }
    }


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
            money += 100;
            dollars_buffer -= 100;
        } else {
            money += dollars_buffer % 100;
            dollars_buffer -= dollars_buffer % 100;
        }
        dollars_timer = 0;
        draw_level_hud();
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
            draw_level_hud();

    }

}

void Hud::add_moniez_on_collected_loot(int value) {
    collecting_timer += 500;
    dollars_buffer += value;
    money_on_this_level += value;
    draw_level_hud();
}

void Hud::set_hud_sprites_attributes() {

    bool playing_level =
            !global::game_state->in_main_menu &&
            !global::game_state->scores_screen &&
            !global::game_state->levels_transition_screen;

    sprite_utils::set_visibility(!global::main_dude->dead & playing_level,
                                 heartSpriteInfo, bombSpriteInfo, ropeSpriteInfo, dollarSpriteInfo,
                                 holdingItemSpriteInfo);

    sprite_utils::set_horizontal_flip(false, heartSpriteInfo, dollarSpriteInfo,
                                      bombSpriteInfo, ropeSpriteInfo, holdingItemSpriteInfo);
    sprite_utils::set_vertical_flip(false, heartSpriteInfo, dollarSpriteInfo,
                                    bombSpriteInfo, ropeSpriteInfo, holdingItemSpriteInfo);

    sprite_utils::set_entry_xy(heartSpriteInfo, HEART_POSITION_X, HEART_POSITION_Y);
    sprite_utils::set_entry_xy(bombSpriteInfo, BOMB_POSITION_X, BOMB_POSITION_Y);
    sprite_utils::set_entry_xy(dollarSpriteInfo, DOLLAR_POSITION_X, DOLLAR_POSITION_Y);
    sprite_utils::set_entry_xy(ropeSpriteInfo, ROPE_POSITION_X, ROPE_POSITION_Y);
    sprite_utils::set_entry_xy(holdingItemSpriteInfo, HOLDING_ITEM_FRAME_POSITION_X, HOLDING_ITEM_FRAME_POSITION_Y);

}

Hud::Hud() {
    hearts = MAIN_DUDE_HITPOINTS;
    items_offset_y = 0;
}


void Hud::increment_offset_on_grabbed_item() {
    items_offset_y += 18;
}

void Hud::debug_oam() {
    printf("\n \n \n  IPM: %lu\n IPS: %lu\n ITM: %lu\n ITS: %lu\n NS: %d NM: %d",
           static_cast<unsigned long>(global::main_oam_manager->current_oam_id_palette),
           static_cast<unsigned long>(global::sub_oam_manager->current_oam_id_palette),
           static_cast<unsigned long>(global::main_oam_manager->current_oam_id_tiles),
           static_cast<unsigned long>(global::sub_oam_manager->current_oam_id_tiles),
           global::sub_oam_manager->nextAvailableTileIdx,
           global::main_oam_manager->nextAvailableTileIdx);
}

void Hud::disable_all_prompts() {
    thief = introduce_shop = recently_bough_item = not_enough_money = holding_item_shopping = false;
    thief_timer = recently_bough_item_timer = not_enough_money_timer = introduce_shop_timer = 0;
}

void Hud::draw_collected_loot() {

    for (unsigned long a = 0; a < global::collected_treasures.size(); a++) {

        //FIXME Pass specific moniez type

        if (global::collected_treasures.at(a) == TreasureType::RUBY_BIG) {

            auto *ruby_big = new RubyBig(90 + (a * 4), 190);
            ruby_big->_y += (16 - ruby_big->_physical_height); //aligning to same level
            global::treasures_to_add.push_back(ruby_big);
            ruby_big->update_sprites_position();
            ruby_big->_ready_to_dispose = true;

        } else if (global::collected_treasures.at(a) == TreasureType::RUBY_SMALL) {

            auto *ruby_small = new RubySmall(90 + (a * 4), 190);
            ruby_small->_y += (16 - ruby_small->_physical_height); //aligning to same level
            global::treasures_to_add.push_back(ruby_small);
            ruby_small->update_sprites_position();
            ruby_small->_ready_to_dispose = true;

        } else if (global::collected_treasures.at(a) == TreasureType::TRIPLE_GOLDBAR) {

            auto *triple_goldbars = new TripleGoldbar(90 + (a * 4), 190);
            triple_goldbars->_y += (16 - triple_goldbars->_physical_height); //aligning to same level
            global::treasures_to_add.push_back(triple_goldbars);
            triple_goldbars->update_sprites_position();
            triple_goldbars->_ready_to_dispose = true;

        } else if (global::collected_treasures.at(a) == TreasureType::SINGLE_GOLDBAR) {

            auto *single_goldbar = new SingleGoldbar(90 + (a * 4), 190);
            single_goldbar->_y += (16 - single_goldbar->_physical_height); //aligning to same level
            global::treasures_to_add.push_back(single_goldbar);
            single_goldbar->update_sprites_position();
            single_goldbar->_ready_to_dispose = true;

        }

    }

}

void Hud::draw_killed_npcs() {

    for (unsigned long a = 0; a < global::killed_npcs.size(); a++) {

        switch (global::killed_npcs.at(a)) {

            case CreatureType::SPIDER: {
                auto *spider = new Spider(95 + (a * 8), 208);
                //deliberately not aligning to same level - already aligned
                global::creatures.push_back(spider);
                spider->_ready_to_dispose = true;
                spider->set_sprite_falling();
                spider->update_sprites_position();
                break;
            }
            case CreatureType::BAT: {
                auto *bat = new Bat(95 + (a * 8), 208);
                bat->_x_speed = -1; //so it would face leftwards
                bat->_y += (16 - bat->_physical_height); //aligning to same level
                bat->set_sprite_flying_left();
                bat->update_sprites_position();
                bat->_ready_to_dispose = true;
                global::creatures.push_back(bat);
                break;
            }
            case CreatureType::SNAKE: {
                auto *snake = new Snake(95 + (a * 8), 208);
                snake->sprite_state = SpriteState::W_LEFT;
                snake->_y += (16 - snake->_physical_height); //aligning to same level
                global::creatures.push_back(snake);
                snake->_ready_to_dispose = true;
                snake->update_sprites_position();
                snake->set_sprite_left();
                break;
            }

            case CreatureType::DAMSEL: {
                auto *damsel = new Damsel(95 + (a * 8), 208);
                damsel->match_animation();
                damsel->update();
                damsel->_y += (16 - damsel->_physical_height); //aligning to same level
                global::creatures.push_back(damsel);
                damsel->_ready_to_dispose = true;
                damsel->update_sprites_position();
                break;
            }

            case CreatureType::SKELETON: {
                auto *skeleton = new Skeleton(95 + (a * 8), 208);
                skeleton->summoned = true;
                skeleton->main_dude_orientation_at_summoning_moment = SpriteState::W_RIGHT;
                skeleton->set_sprite_walking();
                skeleton->_y += (16 - skeleton->_physical_height); //aligning to same level
                global::creatures.push_back(skeleton);
                skeleton->_ready_to_dispose = true;
                skeleton->update_sprites_position();
                break;
            }

            case CreatureType::CAVEMAN: {
                auto *caveman = new Caveman(95 + (a * 8), 208);
                caveman->sprite_state = SpriteState::W_LEFT;
                caveman->animFrameTimer = 1000;
                caveman->update();
                caveman->_y += (16 - caveman->_physical_height); //aligning to same level
                global::creatures.push_back(caveman);
                caveman->_ready_to_dispose = true;
                caveman->update_sprites_position();
                break;
            }

            case CreatureType::SHOPKEEPER: {
                auto *shopkeeper = new Shopkeeper(95 + (a * 8), 208);
                shopkeeper->no_shotgun = true;
                shopkeeper->anim_frame_timer = 1000;
                shopkeeper->sprite_state = SpriteState::W_LEFT;
                shopkeeper->_y += (16 - shopkeeper->_physical_height); //aligning to same level
                shopkeeper->_ready_to_dispose = true;
                shopkeeper->update();
                global::creatures.push_back(shopkeeper);
                break;
            }

            default:
                break;
        }

    }

}

