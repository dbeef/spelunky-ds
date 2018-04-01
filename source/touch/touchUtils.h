//
// Created by xdbeef on 01.04.18.
//

#ifndef SPELUNKYDS_TOUCHUTILS_H
#define SPELUNKYDS_TOUCHUTILS_H


/*
        //For clearing touched tile.
        touchPosition touch;
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


#endif //SPELUNKYDS_TOUCHUTILS_H
