#include <nds.h>
#include "Scrolling.h"
#include "MapUtils.h"
#include "../Consts.h"
#include "../animations/MainDude.h"
#include <nds/arm9/input.h>
#include <nds/arm9/sprite.h>

extern u16 map[4096];

void spelunker::scroll(int id, int width, int height, LevelGenerator *l, int bg, u16 *fresh_map) {
    int keys_held = 0;
    int keys_up = 0;
    int sx = 0;
    int sy = 0;

    int timer = 0;

    MainDude *mainDude = new MainDude();
    mainDude->x = 100;
    mainDude->y = 100;

    mainDude->initMan();


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
            l->mapFrame();
            l->generateRooms();
            l->tilesToMap();
            sectorizeMap();
            dmaCopyHalfWords(DMA_CHANNEL, map, bgGetMapPtr(bg), sizeof(map));
        }

        if (keys_held & KEY_X) sy -= 3;
        if (keys_held & KEY_B) sy += 3;
        if (keys_held & KEY_Y) sx -= 3;
        if (keys_held & KEY_A) sx += 3;

        if (sx < 0) sx = 0;
        if (sx >= width - 256) sx = width - 1 - 256;
        if (sy < 0) sy = 0;
        if (sy >= height - 192) sy = height - 1 - 192;

        swiWaitForVBlank();

        mainDude->updateTimers( timerElapsed(0) / TICKS_PER_SECOND);
        mainDude->animateMan(sx, sy);
        mainDude->checkCollisionWitMapTiles(l->mapTiles);
        mainDude->changePos(keys_held, keys_up);
        oamUpdate(&oamSub);
        oamUpdate(&oamMain);

        bgSetScroll(id, sx, sy);

        bgUpdate();


        touchRead(&touch);
        if (timer > 500) {
            if (touch.px != 0 && touch.py != 0) {
                dmaCopyHalfWords(DMA_CHANNEL, fresh_map, map, sizeof(map));
                l->clearCollidedTile(touch.px, touch.py, sx, sy, bg);
                sectorizeMap();
                dmaCopyHalfWords(DMA_CHANNEL, map, bgGetMapPtr(bg), sizeof(map));
            }
            timer = 0;
        }
    }
}
