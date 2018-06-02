//
// Created by xdbeef on 01.04.18.
//

#include <cstdlib>
#include <nds/arm9/background.h>
#include "camera.h"
#include "../globals_declarations.h"

#define  MAP_WIDTH 512
#define  MAP_HEIGHT 512

static int BOUNDARY_X = 32;
static int BOUNDARY_Y = 16;

//Instant camera focus with main dude in center.
void Camera::instant_focus() {

    x = global::main_dude->x - 128;
    y = global::main_dude->y - 96;

    apply_map_boundaries();
}

//Camera focus with main dude in center, applied as the time progresses.
void Camera::update_position() {

    if (!follow_main_dude)
        return;

    position_update_timer += *global::timer;

    //main dude's spriting, scroll faster
    if (global::input_handler->r_bumper_held)
        center_camera(3);
    else
        center_camera(2);

    position_update_timer = 0;

    apply_map_boundaries();
}

void Camera::apply_map_boundaries() {
    if (this->x < 0) this->x = 0;
    if (this->x >= MAP_WIDTH - 256) this->x = MAP_WIDTH - 1 - 256;
    if (this->y < 0) this->y = 0;
    if (this->y >= MAP_HEIGHT - 192 - 192) this->y = MAP_HEIGHT - 1 - 192 - 192;
}

void Camera::set_scroll() {
    bgSetScroll(global::bg_main_address, this->x, this->y);
    bgSetScroll(global::bg_sub_address, this->x, this->y + 192);
    bgUpdate();
}

void Camera::center_camera(int camera_speed) {

    int center_x = global::main_dude->x - 128;
    int center_y = global::main_dude->y - 96;


    if (abs(center_x - this->x) > BOUNDARY_X) {
        if (center_x > this->x)
            this->x += camera_speed;
        else
            this->x -= camera_speed;
    }

    if (abs(center_y - this->y) > BOUNDARY_Y) {
        if (center_y > this->y)
            this->y += camera_speed;
        else
            this->y -= camera_speed;
    }
}
