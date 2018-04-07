#include <nds.h>
#include "Scrolling.h"
#include "MapUtils.h"
#include "../Consts.h"
#include "../animations/MainDude.h"
#include "../hud/Hud.h"

extern u16 map[4096];

static const int BOUNDARY_VALUE = 128; /* This is the default boundary value (can be set in REG_DISPCNT) */
static const int OFFSET_MULTIPLIER_MAIN = BOUNDARY_VALUE / sizeof(SPRITE_GFX[0]);
static const int OFFSET_MULTIPLIER_SUB = BOUNDARY_VALUE / sizeof(SPRITE_GFX_SUB[0]);

void spelunker::scroll(int bg_main, int bg_sub, LevelGenerator *l, u16 *fresh_map) {

    double timer = 0;

    InputHandler *inputHandler = new InputHandler();
    Camera *camera = new Camera();

    OAMManager *mainOamManager = new OAMManager();
    mainOamManager->initOAMTable(SPRITE_GFX, SPRITE_PALETTE, OAM, OFFSET_MULTIPLIER_MAIN);

    OAMManager *subOamManager = new OAMManager();
    subOamManager->initOAMTable(SPRITE_GFX_SUB, SPRITE_PALETTE_SUB, OAM_SUB, OFFSET_MULTIPLIER_SUB);

    Bomb *bomb = new Bomb();
    bomb->init(mainOamManager, subOamManager);
    bomb->xSpeed = 3;
    bomb->timer = &timer;
    bomb->levelGenerator = l;
    bomb->carried = true;

    MainDude *mainDude = new MainDude();
    mainDude->x = 100;
    mainDude->timer = &timer;
    mainDude->y = 100;
    mainDude->levelGenerator = l;
    mainDude->camera = camera;
    mainDude->inputHandler = inputHandler;
    mainDude->init(mainOamManager, subOamManager);
    mainDude->bomb = bomb;

    Hud *hud = new Hud();
    hud->init(mainOamManager);

    while (true) {

        timer = timerElapsed(0) / TICKS_PER_SECOND;

        inputHandler->updateInput();

        if ((inputHandler->b_key_held)) {
            dmaCopyHalfWords(DMA_CHANNEL, fresh_map, map, sizeof(map));
            l->newLayout(timer);
            l->mapBackground();
            l->mapFrame();
            l->generateRooms();
            l->tilesToMap();
            sectorize_map();
            dmaCopyHalfWords(DMA_CHANNEL, map, bgGetMapPtr(bg_main), sizeof(map));
            dmaCopyHalfWords(DMA_CHANNEL, map, bgGetMapPtr(bg_sub), sizeof(map));
            mainDude->bottomCollision = false;
            bomb->bottomCollision = false;
        }

        swiWaitForVBlank();
        mainDude->updateOther();
        mainDude->handleKeyInput();
        mainDude->updateSpeed();
        mainDude->updateTimers();
        mainDude->draw();
        bomb->update(camera);
        hud->draw();

        camera->updatePosition(mainDude->x, mainDude->y);
        camera->setScroll(bg_main, bg_sub);

        mainOamManager->updateOAM();
        subOamManager->updateOAM();

    }
}



