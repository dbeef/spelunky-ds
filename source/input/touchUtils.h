//
// Created by xdbeef on 01.04.18.
//

#ifndef SPELUNKYDS_TOUCHUTILS_H
#define SPELUNKYDS_TOUCHUTILS_H


/*
        //For clearing touched tile.
        touchPosition input;
        touchRead(&input);
        if (Timer > 500) {
            if (input.px != 0 && input.py != 0) {
                dmaCopyHalfWords(DMA_CHANNEL, fresh_map, map, sizeof(map));
                levelGenerator->clearCollidedTile(input.px, input.py, sx, sy, bg_main);
                sectorize_map();
                dmaCopyHalfWords(DMA_CHANNEL, map, bgGetMapPtr(bg_main), sizeof(map));
            }
            Timer = 0;
        }*/


#endif //SPELUNKYDS_TOUCHUTILS_H
