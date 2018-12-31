//
// Created by xdbeef on 16.09.18.
//

#include <nds.h>
#include <cstdlib>

#include "PopulatingUtils.hpp"
#include "../GameLoop.hpp"

#include "../decorations/SpelunkyTitle.h"
#include "../decorations/Copyrights.h"
#include "../creatures/items/Rope.hpp"
#include "../creatures/enemies/Bat.hpp"
#include "../creatures/enemies/Spider.hpp"
#include "../creatures/traps/Spikes.h"
#include "../creatures/collectibles/Chest.hpp"
#include "../creatures/collectibles/Crate.hpp"
#include "../creatures/enemies/Caveman.hpp"
#include "../creatures/enemies/Damsel.hpp"
#include "../decorations/Lamp.h"
#include "../creatures/enemies/Shopkeeper.hpp"
#include "../decorations/ArrowTrap.h"
#include "../creatures/collectibles/_CollectiblesUtils.hpp"
#include "../creatures/animations/FakeSkeleton.hpp"
#include "../creatures/enemies/Skeleton.hpp"

#include "../rooms/ShopRooms.hpp"
#include "../rooms/ClosedRooms.hpp"
#include "../rooms/LeftRightRooms.hpp"
#include "../rooms/ExitRooms.hpp"
#include "../rooms/LeftRightUpRooms.hpp"
#include "../rooms/LeftRightDownRooms.hpp"
#include "../rooms/EntranceRooms.hpp"
#include "../creatures/collectibles/SingleGoldbar.hpp"
#include "../creatures/collectibles/TripleGoldbar.h"
#include "../creatures/collectibles/RubySmall.h"
#include "../creatures/collectibles/RubyBig.h"
#include "../decorations/RockSign.h"
#include "../decorations/ArchSign.h"

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
                            auto *single_goldbar = new SingleGoldbar(pos_x * 16, pos_y * 16);
                            global::creatures.push_back(single_goldbar);
                        } else {
                            auto *triple_goldbar = new TripleGoldbar(pos_x * 16, pos_y * 16);
                            global::creatures.push_back(triple_goldbar);
                        }

                        gold_bars_left--;
                        last_placement = 0;
                    }

                    if (loot_type == 2 && rubies_left > 0 && r == 1) {

                        int ruby_type = rand() % 2;

                        if (ruby_type == 0) {
                            auto *ruby_small = new RubySmall(pos_x * 16, pos_y * 16);
                            global::creatures.push_back(ruby_small);
                        } else {
                            auto *ruby_big = new RubyBig(pos_x * 16, pos_y * 16);
                            global::creatures.push_back(ruby_big);
                        }

                        rubies_left--;
                        last_placement = 0;

                    }

                    if (loot_type == 3 && jars_left > 0 && r == 1) {
                        Jar *jar = new Jar(pos_x * 16, pos_y * 16);
                        global::creatures.push_back(jar);
                        jars_left--;
                        last_placement = 0;
                    }

/*                    if (loot_type == 0 && rocks_left > 0 && r == 1) {
                        Arrow *arrow = new Arrow();
                        arrow->init();
                        global::creatures.push_back(arrow);
                        arrow->x = pos_x * 16;
                        arrow->y = pos_y * 16;
                        rocks_left--;
                        last_placement = 0;
                    }*/

                    if (loot_type == 0 && rocks_left > 0 && r == 1) {
                        Rock *rock = new Rock(pos_x * 16, pos_y * 16);
                        global::creatures.push_back(rock);
                        rocks_left--;
                        last_placement = 0;
                    }


                    if (loot_type == 4 && chests_left > 0 && r == 1) {
                        Chest *chest = new Chest(pos_x * 16, pos_y * 16);
                        global::creatures.push_back(chest);
                        chests_left--;
                        last_placement = 0;
                    }


                    if (loot_type == 5 && crates_left > 0 && r == 1) {
                        Crate *crate = new Crate(pos_x * 16, pos_y * 16);
                        global::creatures.push_back(crate);
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
                        ArrowTrap *arrowTrap = nullptr;
                        if (npc == 9)
                            arrowTrap = new ArrowTrap(pos_x * 16, pos_y * 16, SpriteState::W_LEFT);
                        else if (npc == 10)
                            arrowTrap = new ArrowTrap(pos_x * 16, pos_y * 16, SpriteState::W_RIGHT);
                        global::decorations_to_add.push_back(arrowTrap);
                    }

                    if (npc == 13 && skeletons_left > 0 && r == 1) {

                        if (rand() % 2 == 0) {
                            auto *skeleton = new Skeleton(pos_x * 16, pos_y * 16);
                            global::creatures.push_back(skeleton);
                            skeletons_left--;
                            last_placement = 0;
                        } else {

                            auto *fakeSkeleton = new FakeSkeleton(pos_x * 16, pos_y * 16);
                            global::creatures.push_back(fakeSkeleton);
                            skeletons_left--;
                            last_placement = 0;

                        }
                    }


                    if (npc == 1 && snakes_left > 0 && r == 1) {
                        Snake *snake = new Snake(pos_x * 16, pos_y * 16);
                        global::creatures.push_back(snake);
                        snakes_left--;
                        last_placement = 0;
                    }

                    if (npc == 2 && bats_left > 0 && r == 1) {
                        Bat *bat = new Bat(pos_x * 16, pos_y * 16);
                        global::creatures.push_back(bat);
                        bats_left--;
                        last_placement = 0;
                    }

                    if (npc == 3 && spiders_left > 0 && r == 1) {
                        Spider *spider = new Spider(pos_x * 16, pos_y * 16);
                        global::creatures.push_back(spider);
                        spiders_left--;
                        last_placement = 0;
                    }

                    if (npc == 4 && spikes_left > 0 && r == 1) {
                        Spikes *spikes = new Spikes(pos_x * 16, (pos_y * 16) + 3);
                        global::creatures.push_back(spikes);
                        spikes_left--;
                        last_placement = 0;
                    }


                    if (npc == 5 && cavemen_left > 0 && r == 1) {
                        Caveman *caveman = new Caveman(pos_x * 16, pos_y * 16);
                        global::creatures.push_back(caveman);
                        cavemen_left--;
                        last_placement = 0;
                    }

                    if (npc == 6 && damsels_left > 0) {
                        Damsel *damsel = new Damsel(pos_x * 16, pos_y * 16);
                        global::creatures.push_back(damsel);
                        damsels_left--;
                        last_placement = 0;
                    }

                    if (npc == 7) {
                        Lamp *lamp = new Lamp(pos_x * 16, pos_y * 16);
                        global::decorations_to_add.push_back(lamp);
                    }

                    if (npc == 8) {
                        shopkeeper = new Shopkeeper(pos_x * 16, pos_y * 16);
                        global::creatures.push_back(shopkeeper);
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

    auto *spelunkyTitle = new SpelunkyTitle(60, 175);
    global::decorations_to_add.push_back(spelunkyTitle);

    auto *copyrights = new Copyrights(60, 380);
    global::decorations_to_add.push_back(copyrights);

    auto *start = new ArchSign(50, 272, ArchSign::Type::START);
    global::decorations_to_add.push_back(start);

    auto *scores = new ArchSign(98, 272, ArchSign::Type::SCORES);
    global::decorations_to_add.push_back(scores);

    auto *tutorial = new RockSign(0, 272, RockSign::Type::TUTORIAL);
    global::decorations_to_add.push_back(tutorial);

    auto *quit = new RockSign(192, 143, RockSign::Type::QUIT);
    global::decorations_to_add.push_back(quit);

    Rope *rope = new Rope(227, 272);
    rope->_y_speed = -4;
    rope->_extended_rope = true;
    rope->hold_by_main_dude = false;
    rope->activated = false;
    rope->_thrown = true;

    global::creatures.push_back(rope);

}