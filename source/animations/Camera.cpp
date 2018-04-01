//
// Created by xdbeef on 01.04.18.
//

#include <cstdlib>
#include <nds/arm9/background.h>
#include "Camera.h"

#define  MAP_WIDTH 512
#define  MAP_HEIGHT 512

void Camera::updatePosition(int main_dude_x, int main_dude_y) {

    static int BOUNDARY_X = 32;
    static int BOUNDARY_Y = 16;

    int center_x = main_dude_x - 128;
    int center_y = main_dude_y - 96;

    //todo Bound this->x/this->y delta to mainDude.xSpeed/mainDude.ySpeed, or to value of (center-x/y - this->x/y)
    //todo timer
//        if(camera_timer > 10) {

    if (abs(center_x - this->x) > BOUNDARY_X) {
        if (center_x > this->x)
            this->x += 1;
        else
            this->x -= 1;
    }

    if (abs(center_y - this->y) > BOUNDARY_Y) {
        if (center_y > this->y)
            this->y += 1;
        else
            this->y -= 1;
    }

//            camera_timer = 0;
//        }

    if (this->x < 0) this->x = 0;
    if (this->x >= MAP_WIDTH - 256) this->x = MAP_WIDTH - 1 - 256;
    if (this->y < 0) this->y = 0;
    if (this->y >= MAP_HEIGHT - 192 - 192) this->y = MAP_HEIGHT - 1 - 192 - 192;
/*
        if (this->x < 0) this->x = 0;
        if (this->x >= width - 256) this->x = width - 1 - 256;
        if (this->y < 0) this->y = 0;
        if (this->y >= height - 192) this->y = height - 1 - 192;
*/

}

void Camera::setScroll(int bg_main_index, int bg_sub_index) {
    bgSetScroll(bg_main_index, this->x, this->y);
    bgSetScroll(bg_sub_index, this->x, this->y + 192);
    bgUpdate();
}