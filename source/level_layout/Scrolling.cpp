#include <nds.h>
#include "Scrolling.h"
#include "MapUtils.h"
#include "../Globals.h"
#include "../animations/MainDude.h"
#include "../hud/Hud.h"
#include <vector>
//#include "../Globals.h";

extern u16 map[4096];

static const int BOUNDARY_VALUE = 128; /* This is the default boundary value (can be set in REG_DISPCNT) */
static const int OFFSET_MULTIPLIER_MAIN = BOUNDARY_VALUE / sizeof(SPRITE_GFX[0]);
static const int OFFSET_MULTIPLIER_SUB = BOUNDARY_VALUE / sizeof(SPRITE_GFX_SUB[0]);

void spelunker::scroll(u16 *fresh_map) {

    double timer = 0;

    global::main_oam_manager->initOAMTable(SPRITE_GFX, SPRITE_PALETTE, OAM, OFFSET_MULTIPLIER_MAIN);
    global::sub_oam_manager->initOAMTable(SPRITE_GFX_SUB, SPRITE_PALETTE_SUB, OAM_SUB, OFFSET_MULTIPLIER_SUB);

    Bomb *bomb = new Bomb();
    bomb->init();
    bomb->xSpeed = 3;
    bomb->timer = &timer;
    bomb->carried = true;

    global::main_dude->x = 100;
    global::main_dude->timer = &timer;
    global::main_dude->y = 100;

    global::main_dude->init();
    global::main_dude->bomb = bomb;
    global::main_dude->sprites = global::sprites;

    global::sprites.push_back(bomb);
    global::sprites.push_back(global::main_dude);

    global::hud->init();
    
    while (true) {

        timer = timerElapsed(0) / TICKS_PER_SECOND;

        global::input_handler->updateInput();

        if ((global::input_handler->b_key_held)) {
            dmaCopyHalfWords(DMA_CHANNEL, fresh_map, map, sizeof(map));
            global::level_generator->newLayout(timer);
            global::level_generator->mapBackground();
            global::level_generator->mapFrame();
            global::level_generator->generateRooms();
            global::level_generator->tilesToMap();
            sectorize_map();
            dmaCopyHalfWords(DMA_CHANNEL, map, bgGetMapPtr(global::bg_main_address), sizeof(map));
            dmaCopyHalfWords(DMA_CHANNEL, map, bgGetMapPtr(global::bg_sub_address), sizeof(map));
            global::main_dude->bottomCollision = false;
            bomb->bottomCollision = false;
        }

        swiWaitForVBlank();
        global::main_dude->handleKeyInput();
        global::hud->draw();

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

    }
}



