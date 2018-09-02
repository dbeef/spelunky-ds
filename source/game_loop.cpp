#include <nds.h>
#include "game_loop.h"
#include "sprites/non_interactive/spelunky_title.h"
#include "sprites/non_interactive/copyrights.h"
#include "sprites/non_interactive/title_menu_sign.h"
#include "sprites/items/rope.h"
#include "sprites/enemies/bat.h"
#include "sprites/enemies/spider.h"
#include "rooms/left_right_rooms.hpp"
#include "rooms/room_type.hpp"
#include "rooms/left_right_down_rooms.hpp"
#include "rooms/left_right_up_rooms.hpp"
#include "rooms/entrance_rooms.hpp"
#include "rooms/exit_rooms.hpp"
#include "rooms/closed_rooms.hpp"
#include <algorithm>
#include "time/time_utils.h"
#include "memory/oam_utils.h"
#include "sprites/traps/spikes.h"
#include "sprites/collectibles/chest.h"
#include "sprites/collectibles/crate.h"
#include "sprites/enemies/caveman.h"
#include "sprites/enemies/damsel.h"
#include "sprites/non_interactive/lamp.h"
#include "rooms/shops.hpp"
#include "sprites/enemies/shopkeeper.h"
#include "sprites/collectibles/collectibles_utils.h"
#include "sprites/items/arrow.h"
#include "sprites/traps/arrow_trap.h"
#include "sprites/enemies/skeleton.h"
#include "sprites/animations/fake_skeleton.h"

static const int BOUNDARY_VALUE = 64; /* This is the default boundary value (can be set in REG_DISPCNT) */
static const int OFFSET_MULTIPLIER_MAIN = BOUNDARY_VALUE / sizeof(SPRITE_GFX[0]);
static const int OFFSET_MULTIPLIER_SUB = BOUNDARY_VALUE / sizeof(SPRITE_GFX_SUB[0]);

void gameloop::scroll() {

    global::main_oam_manager->initOAMTable(SPRITE_GFX, SPRITE_PALETTE, OAM, OFFSET_MULTIPLIER_MAIN, OamType::MAIN);
    global::sub_oam_manager->initOAMTable(SPRITE_GFX_SUB, SPRITE_PALETTE_SUB, OAM_SUB, OFFSET_MULTIPLIER_SUB,
                                          OamType::SUB);
    global::main_dude->init();
    global::camera->x = 0;
    global::camera->y = 127;
    global::main_dude->x = 224;
    global::main_dude->y = 300;
    global::sprites.push_back(global::main_dude);

    global::hud->init();

    populate_main_menu();

    while (true) {

        time_utils::update_ms_since_last_frame();

        global::input_handler->updateInput();


        if (global::game_state->bombed) {
            global::level_generator->render_tiles_on_base_map();
            global::game_state->bombed = false;
            for (int a = 0; a < global::sprites.size(); a++)
                (*global::sprites.at(a)).bottomCollision = false;
        }

        global::camera->update_position();

        for (int a = 0; a < global::sprites.size(); a++) {
            if (global::sprites.at(
                    a) /*&& !global::sprites.at(a)->ready_to_dispose *//*&& !global::sprites.at(a)->killed*/) {
                (*global::sprites.at(a)).update();
                (*global::sprites.at(a)).draw();
            }
        }

        global::game_state->handle_transition_screen_smooch();

        global::main_dude->handle_key_input();
        global::hud->update();

        swiWaitForVBlank();

        //-> this should be done after Vblank (or crash!)
        if (global::game_state->just_started_game) {
            global::game_state->change_brightness_timer += *global::timer;

            if (global::game_state->change_brightness_timer > 100) {

                global::game_state->brightness_level--;

                if (global::game_state->brightness_level == 0)
                    global::game_state->just_started_game = false;
                else
                    setBrightness(3, global::game_state->brightness_level);

            }
        }

        if (global::game_state->in_main_menu && global::game_state->exiting_game) {

            global::game_state->change_brightness_timer += *global::timer;

            if (global::game_state->change_brightness_timer > 100) {

                global::game_state->brightness_level++;

                if (global::game_state->brightness_level > 16)
                    exit(0);

                setBrightness(3, global::game_state->brightness_level);

            }
        }
        //


        global::camera->set_scroll();

        global::main_oam_manager->updateOAM();
        global::sub_oam_manager->updateOAM();

        oam_utils::clean_unused_oam();

    }

}

void gameloop::populate_cave_moniez() {


    int last_placement = 3;

    int gold_bars_left = 6;
    int rubies_left = 6;
    int rocks_left = 4;
    int jars_left = 4;
    int chests_left = 3;
    int crates_left = 2;

    for (int b = ROOMS_Y - 1; b >= 0; b--) {
        for (int a = 0; a < ROOMS_X; a++) {

            int room_type = global::level_generator->layout_room_types[a][b];
            int room_id = global::level_generator->layout_room_ids[a][b];


            if (room_id == -1)
                continue;


            for (int tab_y = 0; tab_y < ROOM_TILE_HEIGHT_GAME; tab_y++) {
                for (int tab_x = 0; tab_x < ROOM_TILE_WIDTH_GAME; tab_x++) {

                    last_placement++;
                    if (last_placement < 3)
                        continue;

                    int npc;

                    if (room_type == room_type::R_LEFT_RIGHT)
                        npc = left_right_loot[room_id][tab_y][tab_x];
                    else if (room_type == room_type::R_LEFT_RIGHT_UP)
                        npc = left_right_up_loot[room_id][tab_y][tab_x];
                    else if (room_type == room_type::R_LEFT_RIGHT_DOWN)
                        npc = left_right_down_loot[room_id][tab_y][tab_x];
                    else if (room_type == room_type::R_ENTRANCE)
                        npc = entrance_room_loot[room_id][tab_y][tab_x];
                    else if (room_type == room_type::R_EXIT)
                        npc = exit_room_loot[room_id][tab_y][tab_x];
                    else if (room_type == room_type::R_CLOSED)
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

                        Moniez *moniez = new Moniez();

                        moniez->spritesheet_type = MONIEZ_GOLDBARS;
                        moniez->value = 1000;

                        moniez->init();

                        global::sprites.push_back(moniez);

                        moniez->x = pos_x * 16;
                        moniez->y = pos_y * 16;

                        gold_bars_left--;
                        last_placement = 0;
                    }

                    if (loot_type == 2 && rubies_left > 0 && r == 1) {
                        Moniez *moniez = new Moniez();

                        moniez->spritesheet_type = MONIEZ_RUBY;
                        moniez->value = 1200;
                        moniez->init();
                        global::sprites.push_back(moniez);
                        moniez->x = pos_x * 16;
                        moniez->y = pos_y * 16;
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

//TODO Utils file for this
void gameloop::populate_cave_npcs() {

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

            int room_type = global::level_generator->layout_room_types[a][b];
            int room_id = global::level_generator->layout_room_ids[a][b];


            if (room_id == -1)
                continue;

            int shop_starting_item = -1;

            for (int tab_y = 0; tab_y < ROOM_TILE_HEIGHT_GAME; tab_y++) {
                for (int tab_x = 0; tab_x < ROOM_TILE_WIDTH_GAME; tab_x++) {


                    last_placement++;
                    if (last_placement < 3)
                        continue;

                    int npc;
                    if (room_type == room_type::R_LEFT_RIGHT)
                        npc = left_right_npcs[room_id][tab_y][tab_x];
                    else if (room_type == room_type::R_LEFT_RIGHT_DOWN)
                        npc = left_right_down_npcs[room_id][tab_y][tab_x];
                    else if (room_type == room_type::R_LEFT_RIGHT_UP)
                        npc = left_right_up_npcs[room_id][tab_y][tab_x];
                    else if (room_type == room_type::R_EXIT)
                        npc = exit_room_npcs[room_id][tab_y][tab_x];
                    else if (room_type == room_type::R_CLOSED)
                        npc = closed_rooms_npcs[room_id][tab_y][tab_x];
                    else if (room_type == room_type::R_SHOP_LEFT || room_type == room_type::R_SHOP_LEFT_MUGSHOT) {
                        npc = shops_npcs[0][tab_y][tab_x];
                        if (shop_starting_item == -1)
                            shop_starting_item = rand() % 9;
                    } else if (room_type == room_type::R_SHOP_RIGHT || room_type == room_type::R_SHOP_RIGHT_MUGSHOT) {
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

void gameloop::populate_main_menu() {

    global::hud->hide_hud_sprites();
    global::hud->bombs = 0;
    global::hud->ropes = 0;

    SpelunkyTitle *spelunkyTitle = new SpelunkyTitle();
    spelunkyTitle->oamType = OamType::MAIN;
    spelunkyTitle->init();
    spelunkyTitle->x = 60;
    spelunkyTitle->y = 175;


    global::sprites.push_back(spelunkyTitle);

    Copyrights *copyrights = new Copyrights();
    copyrights->oamType = OamType::SUB;
    copyrights->init();
    copyrights->x = 60;
    copyrights->y = 380;
    global::sprites.push_back(copyrights);

    TitleMenuSign *start = new TitleMenuSign();
    start->oamType = OamType::MAIN;
    start->menuSignType = MenuSignType::START;
    start->init();
    start->x = 50;
    start->y = 272;
    global::sprites.push_back(start);

    TitleMenuSign *scores = new TitleMenuSign();
    scores->oamType = OamType::MAIN;
    scores->menuSignType = MenuSignType::SCORES;
    scores->init();
    scores->x = 98;
    scores->y = 272;
    global::sprites.push_back(scores);

    TitleMenuSign *tutorial = new TitleMenuSign();
    tutorial->oamType = OamType::MAIN;
    tutorial->menuSignType = MenuSignType::TUTORIAL;
    tutorial->init();
    tutorial->x = 0;
    tutorial->y = 272;
    global::sprites.push_back(tutorial);

    TitleMenuSign *quit = new TitleMenuSign();
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