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

void spelunker::scroll(int bg_main, int bg_sub, u16 *fresh_map) {

    double timer = 0;
    std::vector<MovingObject *> sprites(SPRITE_COUNT * 2, nullptr);

    global::input_handler = new InputHandler();
    global::camera = new Camera();
    global::main_oam_manager = new OAMManager();
    global::main_oam_manager->initOAMTable(SPRITE_GFX, SPRITE_PALETTE, OAM, OFFSET_MULTIPLIER_MAIN);
    global::sub_oam_manager = new OAMManager();
    global::sub_oam_manager->initOAMTable(SPRITE_GFX_SUB, SPRITE_PALETTE_SUB, OAM_SUB, OFFSET_MULTIPLIER_SUB);
    global::level_generator;

    Bomb *bomb = new Bomb();
    bomb->init(global::main_oam_manager, global::sub_oam_manager);
    bomb->xSpeed = 3;
    bomb->timer = &timer;
    bomb->levelGenerator = global::level_generator;
    bomb->carried = true;
    bomb->camera = global::camera;

    MainDude *mainDude = new MainDude();
    mainDude->x = 100;
    mainDude->timer = &timer;
    mainDude->y = 100;
    mainDude->levelGenerator = global::level_generator;
    mainDude->camera = global::camera;
    mainDude->inputHandler = global::input_handler;
   
    mainDude->init(global::main_oam_manager, global::sub_oam_manager);
    mainDude->bomb = bomb;
    mainDude->sprites = sprites;

    sprites.push_back(bomb);
    sprites.push_back(mainDude);

    global::hud = new Hud();
    global::hud->init(global::main_oam_manager);

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
            dmaCopyHalfWords(DMA_CHANNEL, map, bgGetMapPtr(bg_main), sizeof(map));
            dmaCopyHalfWords(DMA_CHANNEL, map, bgGetMapPtr(bg_sub), sizeof(map));
            mainDude->bottomCollision = false;
            bomb->bottomCollision = false;
        }

        swiWaitForVBlank();
        mainDude->handleKeyInput();
        global::hud->draw();

        for (int a = 0; a < sprites.size(); a++) {
            if (sprites.at(a)) {
                (*sprites.at(a)).update();
                (*sprites.at(a)).draw();
            }
        }

        global::camera->updatePosition(mainDude->x, mainDude->y);
        global::camera->setScroll(bg_main, bg_sub);

        global::main_oam_manager->updateOAM();
        global::sub_oam_manager->updateOAM();

    }
}



