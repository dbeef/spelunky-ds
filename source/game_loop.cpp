#include <nds.h>
#include "game_loop.h"
#include "tiles/map_utils.h"
#include "globals_declarations.h"
#include "sprites/main_dude/main_dude.h"
#include "hud/hud.h"
#include "sprites/items/rock.h"
#include "sprites/items/jar.h"
#include "sprites/enemies/snake.h"
#include "sprites/collectibles/moniez.h"
#include "../build/soundbank.h"
#include "sprites/non_interactive/spelunky_title.h"
#include "sprites/non_interactive/copyrights.h"
#include "sprites/non_interactive/title_menu_sign.h"
#include "sprites/items/rope.h"
#include "sprites/enemies/bat.h"
#include "sprites/enemies/spider.h"
#include "rooms/left_right_rooms.h"
#include "rooms/room_type.h"
#include "rooms/left_right_down_rooms.h"
#include "rooms/left_right_up_rooms.h"
#include "rooms/entrance_rooms.h"
#include "rooms/exit_rooms.h"
#include "rooms/closed_rooms.h"
#include <vector>
#include <time.h>
#include <maxmod9.h>
#include <nds/arm9/console.h>
#include<algorithm>


static const int BOUNDARY_VALUE = 64; /* This is the default boundary value (can be set in REG_DISPCNT) */
static const int OFFSET_MULTIPLIER_MAIN = BOUNDARY_VALUE / sizeof(SPRITE_GFX[0]);
static const int OFFSET_MULTIPLIER_SUB = BOUNDARY_VALUE / sizeof(SPRITE_GFX_SUB[0]);

void gameloop::scroll() {

    int garbage_timer = 0;

    double timer = 0;
    int initTimer = 0;

    global::main_oam_manager->initOAMTable(SPRITE_GFX, SPRITE_PALETTE, OAM, OFFSET_MULTIPLIER_MAIN, oam_type::MAIN);
    global::sub_oam_manager->initOAMTable(SPRITE_GFX_SUB, SPRITE_PALETTE_SUB, OAM_SUB, OFFSET_MULTIPLIER_SUB,
                                          oam_type::SUB);
    global::timer = &timer;

    global::main_dude->timer = &timer;
    global::main_dude->init();

    global::camera->x = 0;
    global::camera->y = 127;


    global::main_dude->x = 224;
    global::main_dude->y = 300;

    global::sprites.push_back(global::main_dude);

    //todo wycentrować bombe



    global::hud->init();

    populate_main_menu();

    while (true) {

        timer = timerElapsed(0) / TICKS_PER_SECOND;
        garbage_timer += timer;
        //fixme

        global::input_handler->updateInput();

        if ((global::input_handler->b_key_held) || global::bombed) {
            dmaCopyHalfWords(DMA_CHANNEL, global::base_map, global::current_map, sizeof(global::current_map));
            global::level_generator->tilesToMap();
            sectorize_map();
            dmaCopyHalfWords(DMA_CHANNEL, global::current_map, bgGetMapPtr(global::bg_main_address),
                             sizeof(global::current_map));
            dmaCopyHalfWords(DMA_CHANNEL, global::current_map, bgGetMapPtr(global::bg_sub_address),
                             sizeof(global::current_map));
            global::main_dude->bottomCollision = false;
            global::bombed = false;
        }

        global::main_dude->handleKeyInput();

        for (int a = 0; a < global::sprites.size(); a++) {
            if (global::sprites.at(a) && !global::sprites.at(a)->ready_to_dispose && !global::sprites.at(a)->killed) {
                (*global::sprites.at(a)).update();
                (*global::sprites.at(a)).draw();
            }
        }

        global::camera->updatePosition(global::main_dude->x, global::main_dude->y);
        global::camera->setScroll();
        global::hud->update();


        swiWaitForVBlank();

        global::main_oam_manager->updateOAM();
        global::sub_oam_manager->updateOAM();

        initTimer += timer;

        if (garbage_timer > 2500 && global::main_oam_manager->current_oam_id_tiles >= 108) {

            global::main_oam_manager->clearAllSprites();
            global::sub_oam_manager->clearAllSprites();



//            global::Hud->ropes++;
//            std::cout << "            "<< "MEM CLEAN";


            for (int a = 0; a < global::sprites.size(); a++) {
                //fixme - being killed not always means it's ready for disposing!
                if (!global::sprites.at(a)->ready_to_dispose && !global::sprites.at(a)->killed)
                    global::sprites.at(a)->initSprite();
                else {
//                    delete (global::sprites.at(a));
                }
            }

            global::hud->initSprites();


            garbage_timer = 0;
        }

    }

}

void gameloop::populate_cave_moniez() {


    int last_placement = 3;

    int gold_bars_left = 8;
    int rubies_left = 8;
    int rocks_left = 8;
    int jars_left = 8;

    std::cout << '\n' << '\n';

    for (int b = ROOMS_Y - 1; b >= 0; b--) {
        for (int a = 0; a < ROOMS_X; a++) {

            int room_type = global::level_generator->layout_room_types[a][b];
            int room_id = global::level_generator->layout_room_ids[a][b];


            if (room_id == -1)
                continue;


            for (int tab_y = 0; tab_y < ROOM_TILE_HEIGHT_GAME; tab_y++) {
                for (int tab_x = 0; tab_x < ROOM_TILE_WIDTH_GAME; tab_x++) {

                    last_placement++;
                    if (last_placement < 4)
                        continue;

                    int npc;

                    if (room_type == room_type::ROOM_LEFT_RIGHT)
                        npc = left_right_loot[room_id][tab_y][tab_x];
                    else if (room_type == room_type::ROOM_LEFT_RIGHT_UP)
                        npc = left_right_up_loot[room_id][tab_y][tab_x];
                    else if (room_type == room_type::ROOM_LEFT_RIGHT_DOWN)
                        npc = left_right_down_loot[room_id][tab_y][tab_x];
                    else if (room_type == room_type::ROOM_ENTRANCE)
                        npc = entrance_room_loot[room_id][tab_y][tab_x];
                    else if (room_type == room_type::ROOM_EXIT)
                        npc = exit_room_loot[room_id][tab_y][tab_x];
                    else if (room_type == room_type::ROOM_CLOSED)
                        npc = closed_rooms_loot[room_id][tab_y][tab_x];
                    else
                        continue;

                    if (npc == 0)
                        continue;

                    int r = rand() % 3;
                    int loot_type = rand() % 4;

                    u16 pos_x = (OFFSET_X + tab_x * 2 + 2 * ROOM_TILE_WIDTH_GAME * a) / 2;
                    u16 pos_y = (OFFSET_X + tab_y * 2 + 2 * ROOM_TILE_HEIGHT_GAME * ((ROOMS_Y - b) - 1)) / 2;


                    if (loot_type == 1 && gold_bars_left > 0 && r == 1) {

                        Moniez *moniez = new Moniez();

                        moniez->sprite_height = 16;
                        moniez->sprite_width = 16;
                        moniez->physics_height = 8;
                        moniez->physics_width = 16;

                        moniez->spriteType = MONIEZ_TRIPLE_GOLD_BARS;
                        moniez->value = 1000;

                        moniez->init();

                        moniez->timer = global::timer;
                        global::sprites.push_back(moniez);

                        moniez->x = pos_x * 16;
                        moniez->y = pos_y * 16;

                        gold_bars_left--;
                        last_placement = 0;
                    }

                    if (loot_type == 2 && rubies_left > 0 && r == 1) {
                        Moniez *moniez = new Moniez();
                        moniez->sprite_height = 8;
                        moniez->sprite_width = 8;
                        moniez->physics_height = 8;
                        moniez->physics_width = 8;
                        moniez->spriteType = MONIEZ_RUBY_BIG;
                        moniez->value = 1200;
                        moniez->init();
                        moniez->timer = global::timer;
                        global::sprites.push_back(moniez);
                        moniez->x = pos_x * 16;
                        moniez->y = pos_y * 16;
                        rubies_left--;
                        last_placement = 0;
                    }

                    if (loot_type == 3 && jars_left > 0 && r == 1) {
                        Jar *jar = new Jar();
                        jar->init();
                        jar->timer = global::timer;
                        global::sprites.push_back(jar);
                        jar->x = pos_x * 16;
                        jar->y = pos_y * 16;
                        jars_left--;
                        last_placement = 0;
                    }

                    if (loot_type == 0 && rocks_left > 0 && r == 1) {
                        Rock *rock = new Rock();
                        rock->init();
                        rock->timer = global::timer;
                        global::sprites.push_back(rock);
                        rock->x = pos_x * 16;
                        rock->y = pos_y * 16;
                        rocks_left--;
                        last_placement = 0;
                    }
                }
            }
        }
    }


}


void gameloop::populate_cave_npcs() {

    int last_placement = 3;

    int bats_left = 8;
    int spiders_left = 8;
    int snakes_left = 8;

    std::cout << '\n' << '\n';

    for (int b = ROOMS_Y - 1; b >= 0; b--) {
        for (int a = 0; a < ROOMS_X; a++) {

            int room_type = global::level_generator->layout_room_types[a][b];
            int room_id = global::level_generator->layout_room_ids[a][b];


            if (room_id == -1)
                continue;


            for (int tab_y = 0; tab_y < ROOM_TILE_HEIGHT_GAME; tab_y++) {
                for (int tab_x = 0; tab_x < ROOM_TILE_WIDTH_GAME; tab_x++) {

                    last_placement++;
                    if (last_placement < 4)
                        continue;

                    int npc;
                    if (room_type == room_type::ROOM_LEFT_RIGHT)
                        npc = left_right_npcs[room_id][tab_y][tab_x];
                    else if (room_type == room_type::ROOM_LEFT_RIGHT_DOWN)
                        npc = left_right_down_npcs[room_id][tab_y][tab_x];
                    else if (room_type == room_type::ROOM_LEFT_RIGHT_UP)
                        npc = left_right_up_npcs[room_id][tab_y][tab_x];
                    else if (room_type == room_type::ROOM_EXIT)
                        npc = exit_room_npcs[room_id][tab_y][tab_x];
                    else if (room_type == room_type::ROOM_CLOSED)
                        npc = closed_rooms_npcs[room_id][tab_y][tab_x];
                    else
                        continue;


                    int r = rand() % 3;

                    u16 pos_x = (OFFSET_X + tab_x * 2 + 2 * ROOM_TILE_WIDTH_GAME * a) / 2;
                    u16 pos_y = (OFFSET_X + tab_y * 2 + 2 * ROOM_TILE_HEIGHT_GAME * ((ROOMS_Y - b) - 1)) / 2;


                    if (npc == 1 && snakes_left > 0 && r == 1) {
                        Snake *snake = new Snake();
                        snake->init();
                        snake->timer = global::timer;
                        global::sprites.push_back(snake);
                        snake->x = pos_x * 16;
                        snake->y = pos_y * 16;
                        snakes_left--;
                        last_placement = 0;
                    }

                    if (npc == 2 && bats_left > 0 && r == 1) {
                        Bat *bat = new Bat();
                        bat->init();
                        bat->timer = global::timer;
                        global::sprites.push_back(bat);
                        bats_left--;
                        bat->x = pos_x * 16;
                        bat->y = pos_y * 16;
                        last_placement = 0;
                    }

                    if (npc == 3 && spiders_left > 0 && r == 1) {
                        Spider *spider = new Spider();
                        spider->init();
                        spider->timer = global::timer;
                        global::sprites.push_back(spider);
                        spider->x = pos_x * 16;
                        spider->y = pos_y * 16;
                        spiders_left--;
                        last_placement = 0;
                    }
                }
            }
        }
    }

}

void gameloop::populate_main_menu() {

    global::hud->hide();
    global::hud->bombs = 0;
    global::hud->ropes = 0;

    SpelunkyTitle *spelunkyTitle = new SpelunkyTitle();
    spelunkyTitle->oamType = oam_type::MAIN;
    spelunkyTitle->init();
    spelunkyTitle->x = 60;
    spelunkyTitle->y = 175;


    global::sprites.push_back(spelunkyTitle);

    Copyrights *copyrights = new Copyrights();
    copyrights->oamType = oam_type::SUB;
    copyrights->init();
    copyrights->x = 60;
    copyrights->y = 380;
    global::sprites.push_back(copyrights);

    TitleMenuSign *start = new TitleMenuSign();
    start->oamType = oam_type::MAIN;
    start->menuSignType = menu_sign_type::START;
    start->init();
    start->x = 50;
    start->y = 272;
    global::sprites.push_back(start);

    TitleMenuSign *scores = new TitleMenuSign();
    scores->oamType = oam_type::MAIN;
    scores->menuSignType = menu_sign_type::SCORES;
    scores->init();
    scores->x = 98;
    scores->y = 272;
    global::sprites.push_back(scores);

    TitleMenuSign *tutorial = new TitleMenuSign();
    tutorial->oamType = oam_type::MAIN;
    tutorial->menuSignType = menu_sign_type::TUTORIAL;
    tutorial->init();
    tutorial->x = 0;
    tutorial->y = 272;
    global::sprites.push_back(tutorial);

    TitleMenuSign *quit = new TitleMenuSign();
    quit->oamType = oam_type::MAIN;
    quit->menuSignType = menu_sign_type::QUIT;
    quit->init();
    quit->x = 192;
    quit->y = 143;
    global::sprites.push_back(quit);

    Rope *rope = new Rope();
    rope->timer = global::timer;
    rope->init();
    rope->x = 227;
    rope->y = 260;
    rope->ySpeed = -4;

    rope->hold_by_main_dude = false;
    rope->activated_by_main_dude = false;
    rope->thrown = true;

    global::sprites.push_back(rope);

}