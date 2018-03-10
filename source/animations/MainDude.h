//
// Created by xdbeef on 24.02.18.
//

#ifndef SPELUNKYDS_MAINDUDE_H
#define SPELUNKYDS_MAINDUDE_H

#include <nds.h>
#include "SpriteState.h"

struct MainDude {

public:

     int key_pressed_time;

     SpriteState state;
     int x;
     int y;
     int anim_frame;
     u16 *sprite_gfx_mem;
     u8 *frame_gfx;

    void animateMan();
    void changePos(int keys, int timeElapsed);
    void initMan();
};


#endif //SPELUNKYDS_MAINDUDE_H
