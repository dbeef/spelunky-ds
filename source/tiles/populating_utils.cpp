//
// Created by xdbeef on 16.09.18.
//

#include <nds.h>
#include <cstdlib>

#include "populating_utils.hpp"
#include "../game_loop.hpp"

#include "../sprites/non_interactive/spelunky_title.h"
#include "../sprites/non_interactive/copyrights.h"
#include "../sprites/non_interactive/title_menu_sign.h"
#include "../sprites/items/rope.hpp"
#include "../sprites/enemies/bat.hpp"
#include "../sprites/enemies/spider.hpp"
#include "../sprites/traps/spikes.h"
#include "../sprites/collectibles/chest.hpp"
#include "../sprites/collectibles/crate.hpp"
#include "../sprites/enemies/caveman.hpp"
#include "../sprites/enemies/damsel.hpp"
#include "../sprites/non_interactive/lamp.h"
#include "../sprites/enemies/shopkeeper.hpp"
#include "../sprites/traps/arrow_trap.h"
#include "../sprites/collectibles/collectibles_utils.hpp"
#include "../sprites/animations/fake_skeleton.hpp"
#include "../sprites/enemies/skeleton.hpp"

#include "../rooms/shops.hpp"
#include "../rooms/closed_rooms.hpp"
#include "../rooms/left_right_rooms.hpp"
#include "../rooms/exit_rooms.hpp"
#include "../rooms/left_right_up_rooms.hpp"
#include "../rooms/left_right_down_rooms.hpp"
#include "../rooms/entrance_rooms.hpp"
#include "../sprites/collectibles/single_goldbar.hpp"
#include "../sprites/collectibles/triple_goldbar.h"
#include "../sprites/collectibles/ruby_small.h"
#include "../sprites/collectibles/ruby_big.h"

//TODO Make main menu signs globals
void populate_cave_moniez() {

    int last_placement = 3;

    int gold_bars_left = 6;
    int rubies_left = 6;
    int rocks_left = 4;
    int jars_left = 4;
    int chests_left = 3;
    int crates_left = 2;

    for (int b = ROOMS_Y - 1; b >= 0; b--) {
        for (int a = 0; a < ROOMS_X; a++) {

            int room_type = global::current_level->layout[a][b];
            int room_id = global::current_level->layout_room_ids[a][b];


            if (room_id == -1)
                continue;


            for (int tab_y = 0; tab_y < ROOM_TILE_HEIGHT_GAME; tab_y++) {
                for (int tab_x = 0; tab_x < ROOM_TILE_WIDTH_GAME; tab_x++) {

                    last_placement++;
                    if (last_placement < 3)
                        continue;

                    int npc;

                    if (room_type == RoomType::R_LEFT_RIGHT)
                        npc = left_right_loot[room_id][tab_y][tab_x];
                    else if (room_type == RoomType::R_LEFT_RIGHT_UP)
                        npc = left_right_up_loot[room_id][tab_y][tab_x];
                    else if (room_type == RoomType::R_LEFT_RIGHT_DOWN)
                        npc = left_right_down_loot[room_id][tab_y][tab_x];
                    else if (room_type == RoomType::R_ENTRANCE)
                        npc = entrance_room_loot[room_id][tab_y][tab_x];
                    else if (room_type == RoomType::R_EXIT)
                        npc = exit_room_loot[room_id][tab_y][tab_x];
                    else if (room_type == RoomType::R_CLOSED)
                        npc = closed_rooms_loot[room_id][tab_y][tab_x];
                    else
                        continue;

                    if (npc == 0)
                        continue;

                    int r = rand() % 3;
                    int loot_type = rand() % 6;

                    u16 pos_x = (OFFSET_X + tab_x * 2 + 2 * ROOM_TILE_WIDTH_GAME * a) / 2;
                    u16 pos_y = (OFFSET_X + tab_y * 2 + 2 * ROOM_TILE_HEIGHT_GAME * ((ROOMS_Y - b) - 1)) / 2;

                    if (loot_type == 1 && gold_bars_left > 0 && r == 1) {

                        int goldbar_type = rand() % 2;

                        if (goldbar_type == 0) {
                            auto *single_goldbar = new SingleGoldbar();
                            single_goldbar->init();
                            single_goldbar->x = pos_x * 16;
                            single_goldbar->y = pos_y * 16;
                            global::sprites.push_back(single_goldbar);
                        } else {
                            auto *triple_goldbar = new TripleGoldbar();
                            triple_goldbar->init();
                            triple_goldbar->x = pos_x * 16;
                            triple_goldbar->y = pos_y * 16;
                            global::sprites.push_back(triple_goldbar);
                        }

                        gold_bars_left--;
                        last_placement = 0;
                    }

                    if (loot_type == 2 && rubies_left > 0 && r == 1) {

                        int ruby_type = rand() % 2;

                        if (ruby_type == 0) {
                            auto *ruby_small = new RubySmall();
                            ruby_small->spritesheet_type = MONIEZ_RUBY;
                            ruby_small->value = 1200;
                            ruby_small->init();
                            ruby_small->x = pos_x * 16;
                            ruby_small->y = pos_y * 16;
                            global::sprites.push_back(ruby_small);
                        } else {
                            auto *ruby_big = new RubyBig();
                            ruby_big->spritesheet_type = MONIEZ_RUBY;
                            ruby_big->value = 1200;
                            ruby_big->init();
                            ruby_big->x = pos_x * 16;
                            ruby_big->y = pos_y * 16;
                            global::sprites.push_back(ruby_big);
                        }

                        rubies_left--;
                        last_placement = 0;

                    }

                    if (loot_type == 3 && jars_left > 0 && r == 1) {
                        Jar *jar = new Jar();
                        jar->init();
                        global::sprites.push_back(jar);
                        jar->x = pos_x * 16;
                        jar->y = pos_y * 16;
                        jars_left--;
                        last_placement = 0;
                    }

/*                    if (loot_type == 0 && rocks_left > 0 && r == 1) {
                        Arrow *arrow = new Arrow();
                        arrow->init();
                        global::sprites.push_back(arrow);
                        arrow->x = pos_x * 16;
                        arrow->y = pos_y * 16;
                        rocks_left--;
                        last_placement = 0;
                    }*/

                    if (loot_type == 0 && rocks_left > 0 && r == 1) {
                        Rock *rock = new Rock();
                        rock->init();
                        global::sprites.push_back(rock);
                        rock->x = pos_x * 16;
                        rock->y = pos_y * 16;
                        rocks_left--;
                        last_placement = 0;
                    }


                    if (loot_type == 4 && chests_left > 0 && r == 1) {
                        Chest *chest = new Chest();
                        chest->init();
                        global::sprites.push_back(chest);
                        chest->x = pos_x * 16;
                        chest->y = pos_y * 16;
                        chests_left--;
                        last_placement = 0;
                    }


                    if (loot_type == 5 && crates_left > 0 && r == 1) {
                        Crate *crate = new Crate();
                        crate->init();
                        global::sprites.push_back(crate);
                        crate->x = pos_x * 16;
                        crate->y = pos_y * 16;
                        crates_left--;
                        last_placement = 0;
                    }


                }
            }
        }
    }


}

void populate_cave_npcs() {

    Shopkeeper *shopkeeper = nullptr;
    ShoppingObject *shop_items[4]{};
    int shop_items_index = 0;

    int last_placement = 3;

    int skeletons_left = 3;
    int bats_left = 4;
    int damsels_left = 1;
    int spiders_left = 4;
    int snakes_left = 4;
    int spikes_left = 4;
    int cavemen_left = 3;

    for (int b = ROOMS_Y - 1; b >= 0; b--) {
        for (int a = 0; a < ROOMS_X; a++) {

            int room_type = global::current_level->layout[a][b];
            int room_id = global::current_level->layout_room_ids[a][b];


            if (room_id == -1)
                continue;

            int shop_starting_item = -1;

            for (int tab_y = 0; tab_y < ROOM_TILE_HEIGHT_GAME; tab_y++) {
                for (int tab_x = 0; tab_x < ROOM_TILE_WIDTH_GAME; tab_x++) {


                    last_placement++;
                    if (last_placement < 3)
                        continue;

                    int npc;
                    if (room_type == RoomType::R_LEFT_RIGHT)
                        npc = left_right_npcs[room_id][tab_y][tab_x];
                    else if (room_type == RoomType::R_LEFT_RIGHT_DOWN)
                        npc = left_right_down_npcs[room_id][tab_y][tab_x];
                    else if (room_type == RoomType::R_LEFT_RIGHT_UP)
                        npc = left_right_up_npcs[room_id][tab_y][tab_x];
                    else if (room_type == RoomType::R_EXIT)
                        npc = exit_room_npcs[room_id][tab_y][tab_x];
                    else if (room_type == RoomType::R_CLOSED)
                        npc = closed_rooms_npcs[room_id][tab_y][tab_x];
                    else if (room_type == RoomType::R_SHOP_LEFT || room_type == RoomType::R_SHOP_LEFT_MUGSHOT) {
                        npc = shops_npcs[0][tab_y][tab_x];
                        if (shop_starting_item == -1)
                            shop_starting_item = rand() % 9;
                    } else if (room_type == RoomType::R_SHOP_RIGHT || room_type == RoomType::R_SHOP_RIGHT_MUGSHOT) {
                        npc = shops_npcs[1][tab_y][tab_x];
                        if (shop_starting_item == -1)
                            shop_starting_item = rand() % 9;
                    } else
                        continue;

                    int r = rand() % 3;

                    u16 pos_x = (OFFSET_X + tab_x * 2 + 2 * ROOM_TILE_WIDTH_GAME * a) / 2;
                    u16 pos_y = (OFFSET_X + tab_y * 2 + 2 * ROOM_TILE_HEIGHT_GAME * ((ROOMS_Y - b) - 1)) / 2;

                    if (npc == 9 || npc == 10) {
                        ArrowTrap *arrowTrap = new ArrowTrap();
                        arrowTrap->init();
                        arrowTrap->x = pos_x * 16;
                        arrowTrap->y = pos_y * 16;
                        if (npc == 9)
                            arrowTrap->sprite_state = SpriteState::W_LEFT;
                        else if (npc == 10)
                            arrowTrap->sprite_state = SpriteState::W_RIGHT;
                        global::sprites.push_back(arrowTrap);
                    }

                    if (npc == 13 && skeletons_left > 0 && r == 1) {

                        if (rand() % 2 == 0) {
                            auto *skeleton = new Skeleton();
                            skeleton->init();
                            global::sprites.push_back(skeleton);
                            skeleton->x = pos_x * 16;
                            skeleton->y = pos_y * 16;
                            skeletons_left--;
                            last_placement = 0;
                        } else {

                            auto *fakeSkeleton = new FakeSkeleton();
                            fakeSkeleton->init();
                            global::sprites.push_back(fakeSkeleton);
                            fakeSkeleton->x = pos_x * 16;
                            fakeSkeleton->y = pos_y * 16;
                            skeletons_left--;
                            last_placement = 0;

                        }
                    }


                    if (npc == 1 && snakes_left > 0 && r == 1) {
                        Snake *snake = new Snake();
                        snake->init();
                        global::sprites.push_back(snake);
                        snake->x = pos_x * 16;
                        snake->y = pos_y * 16;
                        snakes_left--;
                        last_placement = 0;
                    }

                    if (npc == 2 && bats_left > 0 && r == 1) {
                        Bat *bat = new Bat();
                        bat->init();
                        global::sprites.push_back(bat);
                        bats_left--;
                        bat->x = pos_x * 16;
                        bat->y = pos_y * 16;
                        last_placement = 0;
                    }

                    if (npc == 3 && spiders_left > 0 && r == 1) {
                        Spider *spider = new Spider();
                        spider->init();
                        global::sprites.push_back(spider);
                        spider->x = pos_x * 16;
                        spider->y = pos_y * 16;
                        spiders_left--;
                        last_placement = 0;
                    }

                    if (npc == 4 && spikes_left > 0 && r == 1) {
                        Spikes *spikes = new Spikes();
                        spikes->init();
                        global::sprites.push_back(spikes);
                        spikes->x = pos_x * 16;
                        spikes->y = (pos_y * 16) + 3;
                        spikes_left--;
                        last_placement = 0;
                    }


                    if (npc == 5 && cavemen_left > 0 && r == 1) {
                        Caveman *caveman = new Caveman();
                        caveman->init();
                        global::sprites.push_back(caveman);
                        caveman->x = pos_x * 16;
                        caveman->y = pos_y * 16;
                        cavemen_left--;
                        last_placement = 0;
                    }

                    if (npc == 6 && damsels_left > 0) {
                        Damsel *damsel = new Damsel();
                        damsel->init();
                        global::sprites.push_back(damsel);
                        damsel->x = pos_x * 16;
                        damsel->y = pos_y * 16;
                        damsels_left--;
                        last_placement = 0;
                    }

                    if (npc == 7) {
                        Lamp *lamp = new Lamp();
                        lamp->init();
                        global::sprites.push_back(lamp);
                        lamp->x = pos_x * 16;
                        lamp->y = pos_y * 16;
                    }

                    if (npc == 8) {
                        shopkeeper = new Shopkeeper();
                        shopkeeper->init();
                        global::sprites.push_back(shopkeeper);
                        shopkeeper->x = pos_x * 16;
                        shopkeeper->y = pos_y * 16;
                        shopkeeper->set_shop_bounds();
                    }

                    if (npc == 12) {
                        if (shop_starting_item >= 9)
                            shop_starting_item = 0;

                        ShoppingObject *m = collectibles_utils::spawn_item(pos_x * 16, pos_y * 16, shop_starting_item,
                                                                           false);

                        shop_items[shop_items_index] = m;
                        shop_items_index++;

                        shop_starting_item++;
                    }

                }
            }
        }
    }

    if (shopkeeper != nullptr)
        for (int a = 0; a < 4; a++)
            shopkeeper->shop_items[a] = shop_items[a];

}

/** Creates all Sprites that are exlusively needed in the main menu scene */
void populate_main_menu() {

    global::hud->bombs = 0;
    global::hud->ropes = 0;

    auto *spelunkyTitle = new SpelunkyTitle();
    spelunkyTitle->oamType = OamType::MAIN;
    spelunkyTitle->init();
    spelunkyTitle->x = 60;
    spelunkyTitle->y = 175;
    global::sprites.push_back(spelunkyTitle);

    auto *copyrights = new Copyrights();
    copyrights->oamType = OamType::SUB;
    copyrights->init();
    copyrights->x = 60;
    copyrights->y = 380;
    global::sprites.push_back(copyrights);

    auto *start = new TitleMenuSign();
    start->oamType = OamType::MAIN;
    start->menuSignType = MenuSignType::START;
    start->init();
    start->x = 50;
    start->y = 272;
    global::sprites.push_back(start);

    auto *scores = new TitleMenuSign();
    scores->oamType = OamType::MAIN;
    scores->menuSignType = MenuSignType::SCORES;
    scores->init();
    scores->x = 98;
    scores->y = 272;
    global::sprites.push_back(scores);

    auto *tutorial = new TitleMenuSign();
    tutorial->oamType = OamType::MAIN;
    tutorial->menuSignType = MenuSignType::TUTORIAL;
    tutorial->init();
    tutorial->x = 0;
    tutorial->y = 272;
    global::sprites.push_back(tutorial);

    auto *quit = new TitleMenuSign();
    quit->oamType = OamType::MAIN;
    quit->menuSignType = MenuSignType::QUIT;
    quit->init();
    quit->x = 192;
    quit->y = 143;
    global::sprites.push_back(quit);

    Rope *rope = new Rope();
    rope->init();
    rope->x = 227;
    rope->y = 272;
    rope->ySpeed = -4;

    rope->instant_rope = true;
    rope->extended_rope = true;
    rope->hold_by_main_dude = false;
    rope->activated = false;
    rope->thrown = true;

    global::sprites.push_back(rope);

}