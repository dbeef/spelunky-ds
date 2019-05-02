//
// Created by xdbeef on 01.04.18.
//

#include <cstdlib>
#include <nds/arm9/background.h>

#include "Camera.hpp"
#include "../GameState.hpp"
#include "../time/Timer.h"

constexpr u16 MAP_WIDTH = 512;
constexpr u16 MAP_HEIGHT = 512;

// boundaries, that are used when it comes to follow main dude.
// camera does not stick to the main dude - main dude can make some little movement
// inside this BOUNDARY_X/Y box and the camera won't move untill he crosses the box.
constexpr u16 BOUNDARY_X = 32;
constexpr u16 BOUNDARY_Y = 16;

constexpr u8 shaking_max_delta = 3;
constexpr u8 shaking_max_same_direction = 3;

Camera* Camera::_instance = nullptr;

void Camera::init() {
    SPELUNKYDS_BREAKING_ASSERT(!_instance);
    _instance = new Camera();
    SPELUNKYDS_BREAKING_ASSERT(_instance);
}

void Camera::dispose() {
    SPELUNKYDS_BREAKING_ASSERT(_instance);
    delete _instance;
}

void Camera::apply_shaking() {

    if (shakescreen_duration_timer > 0) {

        shakescreen_duration_timer -= Timer::getDeltaTime();

        if (shakescreen_duration_timer < 0)
            shakescreen_duration_timer = 0;

        int x_r = (rand() % 2) == 0 ? (rand() % shaking_max_delta) : -(rand() % shaking_max_delta);
        int y_r = (rand() % 2) == 0 ? (rand() % shaking_max_delta) : -(rand() % shaking_max_delta);

        if (x_r < 0)
            x_shake_direction--;
        else
            x_shake_direction++;

        if (y_r < 0)
            y_shake_direction--;
        else
            y_shake_direction++;

        if (x_shake_direction < -shaking_max_same_direction) {
            x_r = 1;
            x_shake_direction++;
        } else if (x_shake_direction > shaking_max_same_direction) {
            x_r = -1;
            x_shake_direction--;
        }

        if (y_shake_direction < -shaking_max_same_direction) {
            y_r = 1;
            y_shake_direction++;
        } else if (y_shake_direction > shaking_max_same_direction) {
            y_r = -1;
            y_shake_direction--;
        }

        x -= x_r;
        y -= y_r;
    }

}

// Instant camera focus with main dude in center.
void Camera::instant_focus() {
    x = MainDude::instance()._x - (SCREEN_WIDTH / 2);
    y = MainDude::instance()._y - (SCREEN_HEIGHT / 2);
    apply_map_boundaries();
}

// Camera focus with main dude in center, applied incrementally every function call
void Camera::incremental_focus(int camera_speed) {

    int center_x = MainDude::instance()._x - (SCREEN_WIDTH / 2);
    int center_y = MainDude::instance()._y - (SCREEN_HEIGHT / 2);

    u16 distance_x = static_cast<u16>(abs(center_x - this->x));
    u16 distance_y = static_cast<u16>(abs(center_y - this->y));

    if (distance_x > BOUNDARY_X) {
        if (center_x > this->x)
            this->x += camera_speed;
        else
            this->x -= camera_speed;
    }

    if (distance_y > BOUNDARY_Y) {
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

    position_update_timer += Timer::getDeltaTime();

    if (position_update_timer > 15) {

        //main dude's spriting, scroll faster
        if (InputHandler::instance().r_bumper_held)
            incremental_focus(3);
        else
            incremental_focus(2);

        position_update_timer = 0;
    }

    apply_map_boundaries();

    apply_shaking();
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
    bgSetScroll(GameState::instance().bg_main_address, this->x, this->y);
    bgSetScroll(GameState::instance().bg_sub_address, this->x, this->y + SCREEN_HEIGHT);
    bgUpdate();
}
