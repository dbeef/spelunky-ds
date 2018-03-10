//
// Created by xdbeef on 24.02.18.
//

#include <nds.h>
#include <nds/arm9/sprite.h>
#include "MainDude.h"
#include "../level_layout/ScreenPositions.h"
#include "../Consts.h"
#include "../../build/spelunker.h"

void MainDude::changePos(int keys, int timeElapsed) {

    if (keys) {
        if (keys & KEY_UP) {
//            if (y >= SCREEN_TOP) y--;
//            state = W_UP;
        }
        if (keys & KEY_LEFT) {
            if (x >= SCREEN_LEFT) x-=2;
            state = W_LEFT;
        }
        if (keys & KEY_RIGHT) {
            if (x <= SCREEN_RIGHT) x+=2;
            state = W_RIGHT;
        }
        if (keys & KEY_DOWN) {
//            if (y <= SCREEN_BOTTOM) y++;
//            state = W_DOWN;
        }

        key_pressed_time += timeElapsed;
        if(key_pressed_time > 70) {
            key_pressed_time = 0;
            anim_frame++;
        }
        if (anim_frame >= FRAMES_PER_ANIMATION) anim_frame = 0;

    }
}

void MainDude::animateMan() {
    int frame = anim_frame + state * FRAMES_PER_ANIMATION;
    u8 *offset = frame_gfx + frame * 32 * 32;
    dmaCopy(offset, sprite_gfx_mem, 32 * 32);
    oamSet(&oamMain, 0, x, y, 0, 0, SpriteSize_32x32, SpriteColorFormat_256Color,
           sprite_gfx_mem, -1, false, false, false, false, false);
}

void MainDude::initMan() {
    sprite_gfx_mem = oamAllocateGfx(&oamMain, SpriteSize_32x32, SpriteColorFormat_256Color);
    frame_gfx = (u8 *) spelunkerTiles;
    dmaCopy(spelunkerPal, SPRITE_PALETTE, 512);
}
