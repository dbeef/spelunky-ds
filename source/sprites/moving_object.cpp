//
// Created by xdbeef on 07.04.18.
//

#include "moving_object.h"
#include "../globals_declarations.h"
#include "../tiles/map_utils.h"

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
        *out_sub_x = -SCREEN_HEIGHT;
        *out_sub_y = -SCREEN_HEIGHT;
    }
    if (global::camera->y > this->y + sprite_height ||
        global::camera->y + SCREEN_HEIGHT < this->y - sprite_height) {
        *out_main_x = -SCREEN_HEIGHT;
        *out_main_y = -SCREEN_HEIGHT;
    }

    if (*out_sub_y + sprite_height < 0 || out_sub_x + sprite_width < 0) {
        *out_sub_x = -SCREEN_HEIGHT;
        *out_sub_y = -SCREEN_HEIGHT;
    }

    if (*out_main_y + sprite_height < 0 || *out_main_x + sprite_width< 0) {
        *out_main_x = -SCREEN_HEIGHT;
        *out_main_y = -SCREEN_HEIGHT;
    }

}

void MovingObject::updatePosition() {

    double tempXspeed = abs(xSpeed);
    double tempYspeed = abs(ySpeed);

    int old_xx = -1;
    int old_yy = -1;
    int xx;
    int yy;

    while (tempXspeed > 0 || tempYspeed > 0) {
        if (tempXspeed > 0) {
            if (xSpeed > 0) {
                x += 1;
            } else if (xSpeed < 0) {
                x -= 1;
            }
        }
        if (tempYspeed > 0) {
            if (ySpeed > 0)
                y += 1;
            else if (ySpeed < 0)
                y -= 1;
        }

        xx = floor_div(this->x + 0.5 * physical_width, 16);
        yy = floor_div(this->y + 0.5 * physical_height, 16);

        if (old_xx != xx || old_yy != yy || physical_width < 8 || physical_height < 8) {
            if (xx < 31 && yy < 31)
                updateCollisionsMap(xx, yy);
        }

        old_xx = xx;
        old_yy = yy;

        tempXspeed--;
        tempYspeed--;

    }
}

MovingObject::MovingObject() {
    hold_by_main_dude = false;
    activated_by_main_dude = false;
    killed = false;
    ready_to_dispose = false;
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
