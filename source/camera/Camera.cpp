//
// Created by xdbeef on 01.04.18.
//

#include <cstdlib>
#include <nds/arm9/background.h>

#include "Camera.hpp"
#include "../GameState.hpp"
#include "../time/Timer.h"
#include "../tiles/LevelRenderer.hpp"

static const u16 CAMERA_MENU_START_X = 0;
static const u16 CAMERA_MENU_START_Y = 127;

constexpr u16 MAP_WIDTH = 32 * TILE_WIDTH;
constexpr u16 MAP_HEIGHT = 32 * TILE_HEIGHT;

// Boundaries, that are used when it comes to follow main dude.
// Camera does not stick to the main dude - main dude can make some little movement
// inside this BOUNDARY_X/Y box and the camera won't move untill he crosses the box.
constexpr u16 BOUNDARY_X = 32;
constexpr u16 BOUNDARY_Y = 16;

constexpr u8 CAMERA_SPEED_MAIN_DUDE_RUNNING = 3;
constexpr u8 CAMERA_SPEED_MAIN_DUDE_WALKING = 2;

constexpr u16 CAMERA_UPDATE_DELTA = 15;

constexpr u8 SHAKING_MAX_DELTA_POS = 3;
constexpr u8 SHAKING_MAX_SHAKES_IN_SAME_DIRECTION = 3;

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

    if (_shakescreen_duration_timer > 0) {

        _shakescreen_duration_timer -= Timer::getDeltaTime();

        if (_shakescreen_duration_timer < 0)
            _shakescreen_duration_timer = 0;

        int x_r = (rand() % 2) == 0 ? (rand() % SHAKING_MAX_DELTA_POS) : -(rand() % SHAKING_MAX_DELTA_POS);
        int y_r = (rand() % 2) == 0 ? (rand() % SHAKING_MAX_DELTA_POS) : -(rand() % SHAKING_MAX_DELTA_POS);

        if (x_r < 0)
            _x_shake_direction--;
        else
            _x_shake_direction++;

        if (y_r < 0)
            _y_shake_direction--;
        else
            _y_shake_direction++;

        if (_x_shake_direction < -SHAKING_MAX_SHAKES_IN_SAME_DIRECTION) {
            x_r = 1;
            _x_shake_direction++;
        } else if (_x_shake_direction > SHAKING_MAX_SHAKES_IN_SAME_DIRECTION) {
            x_r = -1;
            _x_shake_direction--;
        }

        if (_y_shake_direction < -SHAKING_MAX_SHAKES_IN_SAME_DIRECTION) {
            y_r = 1;
            _y_shake_direction++;
        } else if (_y_shake_direction > SHAKING_MAX_SHAKES_IN_SAME_DIRECTION) {
            y_r = -1;
            _y_shake_direction--;
        }

        _x -= x_r;
        _y -= y_r;
    }

}

// Instant camera focus with main dude in center.
void Camera::instant_focus() {
    _x = MainDude::instance()._x - (SCREEN_WIDTH / 2);
    _y = MainDude::instance()._y - (SCREEN_HEIGHT / 2);
    apply_map_boundaries();
}

// Camera focus with main dude in center, applied incrementally every function call
void Camera::incremental_focus(int camera_speed) {

    int center_x = MainDude::instance()._x - (SCREEN_WIDTH / 2);
    int center_y = MainDude::instance()._y - (SCREEN_HEIGHT / 2);

    u16 distance_x = static_cast<u16>(abs(center_x - this->_x));
    u16 distance_y = static_cast<u16>(abs(center_y - this->_y));

    if (distance_x > BOUNDARY_X) {
        if (center_x > this->_x)
            this->_x += camera_speed;
        else
            this->_x -= camera_speed;
    }

    if (distance_y > BOUNDARY_Y) {
        if (center_y > this->_y)
            this->_y += camera_speed;
        else
            this->_y -= camera_speed;
    }

}

// General update function.
// Camera will attempt to focus incrementally on main dude every call.
void Camera::update() {

    if (!_follow_main_dude) return;

    _position_update_timer += Timer::getDeltaTime();

    if (_position_update_timer > CAMERA_UPDATE_DELTA) {

        if (InputHandler::instance().keys.r_bumper_held)
            incremental_focus(CAMERA_SPEED_MAIN_DUDE_RUNNING);
        else
            incremental_focus(CAMERA_SPEED_MAIN_DUDE_WALKING);

        _position_update_timer = 0;
    }

    apply_map_boundaries();

    apply_shaking();

    write_current_position_to_graphics_engines();
}

// Keeps camera inside 512x512 px boundries.
// 512 is 2 x NDS screen width and ~2.6 x NDS screen height.
void Camera::apply_map_boundaries() {
    if (this->_x < 0) this->_x = 0;
    if (this->_x >= MAP_WIDTH - SCREEN_WIDTH) this->_x = MAP_WIDTH - 1 - SCREEN_WIDTH;
    if (this->_y < 0) this->_y = 0;
    if (this->_y >= MAP_HEIGHT - (2 * SCREEN_HEIGHT)) this->_y = MAP_HEIGHT - 1 - (2 * SCREEN_HEIGHT);
}

// Tells the main and sub screen graphics engine to update camera position with current camera x/y
void Camera::write_current_position_to_graphics_engines() {
    bgSetScroll(LevelRenderer::instance()._main_background_id, this->_x, this->_y);
    bgSetScroll(LevelRenderer::instance()._sub_background_id, this->_x, this->_y + SCREEN_HEIGHT);
    bgUpdate();
}

void Camera::set_main_menu_position() {
    _x = CAMERA_MENU_START_X;
    _y = CAMERA_MENU_START_Y;
}

void Camera::detach_from_main_dude() {
    _follow_main_dude = false;
    apply_map_boundaries();
    write_current_position_to_graphics_engines();
}

void Camera::follow_main_dude() {
    _follow_main_dude = true;
}
