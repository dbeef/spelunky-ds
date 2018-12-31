//
// Created by xdbeef on 01.04.18.
//

#include <cstdlib>
#include <nds/arm9/background.h>
#include "camera.hpp"
#include "../globals_declarations.hpp"

constexpr u16 MAP_WIDTH = 512;
constexpr u16 MAP_HEIGHT = 512;

//boundaries, that are used when it comes to follow main dude.
//camera does not stick to the main dude - main dude can make some little movement
//inside this BOUNDARY_X/Y box and the camera won't move untill he crosses the box.
constexpr u16 BOUNDARY_X = 32;
constexpr u16 BOUNDARY_Y = 16;

//Instant camera focus with main dude in center.
void Camera::instant_focus() {
    x = global::main_dude->_x - (SCREEN_WIDTH / 2);
    y = global::main_dude->_y - (SCREEN_HEIGHT / 2);
    apply_map_boundaries();
}

//Camera focus with main dude in center, applied incrementally every function call
void Camera::incremental_focus(int camera_speed) {

    int center_x = global::main_dude->_x - (SCREEN_WIDTH / 2);
    int center_y = global::main_dude->_y - (SCREEN_HEIGHT / 2);

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

/**
 * General update function.
 * Camera will attempt to focus incrementally on main dude every call.
 */
void Camera::update() {

    if (!follow_main_dude)
        return;

    position_update_timer += *global::timer;

    //FIXME Not using position_update_timer

    //main dude's spriting, scroll faster
    if (global::input_handler->r_bumper_held)
        incremental_focus(3);
    else
        incremental_focus(2);

    position_update_timer = 0;

    apply_map_boundaries();
}

/**
 * Keeps camera inside 512x512 px boundries.
 * 512 is 2 x NDS screen width and ~2.6 x NDS screen height.
 */
void Camera::apply_map_boundaries() {
    if (this->x < 0) this->x = 0;
    if (this->x >= MAP_WIDTH - SCREEN_WIDTH) this->x = MAP_WIDTH - 1 - SCREEN_WIDTH;
    if (this->y < 0) this->y = 0;
    if (this->y >= MAP_HEIGHT - (2 * SCREEN_HEIGHT)) this->y = MAP_HEIGHT - 1 - (2 * SCREEN_HEIGHT);
}

//tells the main and sub screen graphics engine to update camera position with current camera x/y
void Camera::write_current_position_to_graphics_engines() {
    bgSetScroll(global::bg_main_address, this->x, this->y);
    bgSetScroll(global::bg_sub_address, this->x, this->y + SCREEN_HEIGHT);
    bgUpdate();
}

