//
// Created by xdbeef on 07.04.18.
//

#include "moving_object.h"
#include "../globals_declarations.h"

void MovingObject::limit_speed(int max_x, int max_y) {

    if (ySpeed > max_y)
        ySpeed = max_y;
    else if (ySpeed < -max_y)
        ySpeed = -max_y;

    if (xSpeed > max_x)
        xSpeed = max_x;
    else if (xSpeed < -max_x)
        xSpeed = -max_x;

}

void MovingObject::apply_friction(int friction_delta_time_ms, int friction_delta_speed) {

    if (friction_timer > friction_delta_time_ms) {
        friction_timer = 0;
        if (xSpeed > 0) {
            xSpeed -= friction_delta_speed;
            if (xSpeed < 0)
                xSpeed = 0;
        }
        if (xSpeed < 0) {
            xSpeed += friction_delta_speed;
            if (xSpeed > 0)
                xSpeed = 0;
        }
    }

}

void MovingObject::get_x_y_viewported(int *out_main_x, int *out_main_y, int *out_sub_x, int *out_sub_y) {
    *out_main_x = x - global::camera->x;
    *out_main_y = y - global::camera->y;
    *out_sub_x = x - global::camera->x;
    *out_sub_y = y - global::camera->y - SCREEN_HEIGHT;

    if (global::camera->y + SCREEN_HEIGHT > this->y + sprite_height ||
        global::camera->y + SCREEN_HEIGHT + SCREEN_HEIGHT < this->y - sprite_height) {
        //Just an arbitrary position value, so the sprite would not be rendered.
        *out_sub_x = -SCREEN_WIDTH;
        *out_sub_y = -SCREEN_WIDTH;
    }
    if (global::camera->y > this->y + sprite_height ||
        global::camera->y + SCREEN_HEIGHT < this->y - sprite_height) {
        *out_main_x = -SCREEN_WIDTH;
        *out_main_y = -SCREEN_WIDTH;
    }
}

//https://stackoverflow.com/questions/120876/what-are-the-rules-for-calling-the-superclass-constructor
/*
MovingObject::MovingObject(int p_physical_width, int p_physical_height, int p_sprite_width, int p_sprite_height) {
    physical_width = p_physical_width;
    physical_height = p_physical_height;
    sprite_width = p_sprite_width;
    sprite_height = p_sprite_height;
}
*/
