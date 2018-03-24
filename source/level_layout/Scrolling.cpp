#include <nds.h>
#include "Scrolling.h"
#include "MapUtils.h"
#include "../Consts.h"
#include "../animations/MainDude.h"
#include "../hud/Hud.h"
#include <nds/arm9/input.h>
#include <nds/arm9/sprite.h>

extern u16 map[4096];

void spelunker::scroll(int bg_main, int bg_sub, int width, int height, LevelGenerator *l, u16 *fresh_map) {
    int keys_held = 0;
    int keys_up = 0;
    int sx = 0;
    int sy = 0;

    int timer = 0;

    MainDude *mainDude = new MainDude();
    mainDude->x = 100;
    mainDude->y = 100;

    mainDude->init();

    Hud *hud = new Hud();
    hud->initHud();

    touchPosition touch;
    while (true) {
        scanKeys();

        timer += timerElapsed(0) / TICKS_PER_SECOND;
        keys_held = keysHeld();
        keys_up = keysUp();

        if ((keys_held & KEY_B) && timer > 1000) {

            timer = 0;
            dmaCopyHalfWords(DMA_CHANNEL, fresh_map, map, sizeof(map));
            l->newLayout(timerElapsed(0));
            l->mapBackground();
            l->mapFrame();
            l->generateRooms();
            l->tilesToMap();
            sectorize_map();
            dmaCopyHalfWords(DMA_CHANNEL, map, bgGetMapPtr(bg_main), sizeof(map));
            dmaCopyHalfWords(DMA_CHANNEL, map, bgGetMapPtr(bg_sub), sizeof(map));
        }

//        if (keys_held & KEY_X) sy -= 3;
//        if (keys_held & KEY_B) sy += 3;
//        if (keys_held & KEY_Y) sx -= 3;
//        if (keys_held & KEY_A) sx += 3;

//        sx = mainDude->x - 128;
//        sy = mainDude->y - 96;

//        hud->drawHud();

        static int BOUNDARY_X = 32;
        static int BOUNDARY_Y = 16;

        int center_x = mainDude->x - 128;
        int center_y = mainDude->y - 96;


        //todo Bound sx/sy delta to mainDude.xSpeed/mainDude.ySpeed, or to value of (center-x/y - sx/y)
        if (abs(center_x - sx) > BOUNDARY_X) {
            if (center_x > sx)
                sx += 2;
            else
                sx -= 2;
        }

        if (abs(center_y - sy) > BOUNDARY_Y) {
            if (center_y > sy)
                sy += 2;
            else
                sy -= 2;
        }


        if (sx < 0) sx = 0;
        if (sx >= width - 256) sx = width - 1 - 256;
        if (sy < 0) sy = 0;
        if (sy >= height - 192 - 192) sy = height - 1 - 192 - 192;
/*

        if (sx < 0) sx = 0;
        if (sx >= width - 256) sx = width - 1 - 256;
        if (sy < 0) sy = 0;
        if (sy >= height - 192) sy = height - 1 - 192;
*/

        swiWaitForVBlank();

        mainDude->update(sx, sy, keys_held, keys_up, l);

        oamUpdate(&oamSub);
        oamUpdate(&oamMain);

        bgSetScroll(bg_main, sx, sy);
        bgSetScroll(bg_sub, sx, sy + 192);

        bgUpdate();

/*
        touchRead(&touch);
        if (Timer > 500) {
            if (touch.px != 0 && touch.py != 0) {
                dmaCopyHalfWords(DMA_CHANNEL, fresh_map, map, sizeof(map));
                l->clearCollidedTile(touch.px, touch.py, sx, sy, bg_main);
                sectorize_map();
                dmaCopyHalfWords(DMA_CHANNEL, map, bgGetMapPtr(bg_main), sizeof(map));
            }
            Timer = 0;
        }*/
    }
}
