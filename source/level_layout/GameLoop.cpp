#include <nds.h>
#include "GameLoop.h"
#include "MapUtils.h"
#include "../Globals.h"
#include "../animations/MainDude.h"
#include "../hud/Hud.h"
#include "../animations/Rock.h"
#include "../animations/Jar.h"
#include "../animations/Snake.h"
#include "../animations/Moniez.h"
#include "../../build/soundbank.h"
#include <vector>
//#include "../Globals.h";
#include <time.h>
#include <maxmod9.h>

static const int BOUNDARY_VALUE = 64; /* This is the default boundary value (can be set in REG_DISPCNT) */
static const int OFFSET_MULTIPLIER_MAIN = BOUNDARY_VALUE / sizeof(SPRITE_GFX[0]);
static const int OFFSET_MULTIPLIER_SUB = BOUNDARY_VALUE / sizeof(SPRITE_GFX_SUB[0]);

void gameloop::scroll() {

//    mmStart(SFX_MCAVE, MM_PLAY_LOOP);
    mmEffect(SFX_MCAVE);

    double timer = 0;
    int initTimer = 0;

    global::main_oam_manager->initOAMTable(SPRITE_GFX, SPRITE_PALETTE, OAM, OFFSET_MULTIPLIER_MAIN, OamType::MAIN);
    global::sub_oam_manager->initOAMTable(SPRITE_GFX_SUB, SPRITE_PALETTE_SUB, OAM_SUB, OFFSET_MULTIPLIER_SUB,
                                          OamType::SUB);

    global::main_dude->timer = &timer;
    global::main_dude->init();

    MapTile *entrance;
    global::level_generator->getEntranceTile(entrance);

    if (entrance == nullptr) {
        global::main_dude->x = 100;
        global::main_dude->y = 100;
    } else {
        global::main_dude->x = entrance->x * 16;
        global::main_dude->y = entrance->y * 16;
    }

    //fixme
    global::camera->followMainDude = true;
    for (int a = 0; a < 400; a++)
        global::camera->updatePosition(global::main_dude->x, global::main_dude->y);


    global::sprites.push_back(global::main_dude);

    //todo wycentrować bombe

    global::hud->init();


    bool initialised = false;


    while (true) {

        timer = timerElapsed(0) / TICKS_PER_SECOND;
        //fixme
        *global::timer = timer;

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

        swiWaitForVBlank();
        global::main_dude->handleKeyInput();

        for (int a = 0; a < global::sprites.size(); a++) {
            if (global::sprites.at(a)) {
                (*global::sprites.at(a)).update();
                (*global::sprites.at(a)).draw();
            }
        }

        global::camera->updatePosition(global::main_dude->x, global::main_dude->y);
        global::camera->setScroll();

        global::main_oam_manager->updateOAM();
        global::sub_oam_manager->updateOAM();

        initTimer += timer;

        global::hud->updateMoniez();

        if (!initialised /*&& initTimer > 5000*/) {

            srand(global::input_handler->seed + global::main_dude->x + global::main_dude->y);

//            populateCaveItems();
            populateCaveNpcs();

            initialised = true;
        }

    }

}

void gameloop::populateCaveItems() {

    for (int a = 0; a < 4; a++) {
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

        int curr_x = rand() % 400 + 48 + a * 16;
        int curr_y = rand() % 400 + 48;

        moniez->x = curr_x;
        moniez->y = curr_y;

    }
    for (int a = 0; a < 4; a++) {
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

        int curr_x = rand() % 400 + 48 + a * 16;
        int curr_y = rand() % 400 + 48;

        moniez->x = curr_x;
        moniez->y = curr_y;

    }

    for (int a = 0; a < 2; a++) {
        Jar *jar = new Jar();
        jar->init();
        jar->timer = global::timer;
        global::sprites.push_back(jar);

        int curr_x = rand() % 400 + 48;
        int curr_y = rand() % 400 + 48;

        jar->x = curr_x;
        jar->y = curr_y;
    }
    for (int a = 0; a < 2; a++) {
        Rock *rock = new Rock();
        rock->init();
        rock->timer = global::timer;
        global::sprites.push_back(rock);

        int curr_x = rand() % 400 + 48;
        int curr_y = rand() % 400 + 48;

        rock->x = curr_x;
        rock->y = curr_y;
    }

}


void gameloop::populateCaveNpcs() {

    for (int a = 0; a < 2; a++) {
        Snake *snake = new Snake();
        snake->init();
        snake->timer = global::timer;
        global::sprites.push_back(snake);

        int curr_x = rand() % 400 + 48 + a * 16;
        int curr_y = rand() % 400 + 48;

        snake->x = curr_x;
        snake->y = curr_y;

    }

}