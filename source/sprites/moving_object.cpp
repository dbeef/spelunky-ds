//
// Created by xdbeef on 07.04.18.
// https://stackoverflow.com/questions/120876/what-are-the-rules-for-calling-the-superclass-constructor
//

#include "moving_object.h"
#include "../globals_declarations.h"
#include "../tiles/map_utils.h"
#include "../collisions/collisions.h"

//for opening chests and crates
bool MovingObject::check_if_can_be_opened() {
    if (!activated_by_main_dude &&
        Collisions::checkCollisionWithMainDudeWidthBoundary(x, y, physical_width, physical_height, 8) &&
        global::input_handler->up_key_held && global::input_handler->y_key_down) {

        activated_by_main_dude = true;
        global::input_handler->y_key_down = false;

        return true;
    } else
        return false;
}

//check, if main dude can pickup this item (to hands, not to inventory)
void MovingObject::check_if_can_be_pickuped() {

    if (hold_by_main_dude &&
        global::input_handler->y_key_down &&
        global::input_handler->down_key_held &&
        global::main_dude->bottomCollision) {

        //leave item on ground

        hold_by_main_dude = false;
        global::main_dude->holding_item = false;
        global::input_handler->y_key_down = false;
        bottomCollision = false;

    } else if (global::input_handler->y_key_down &&
               global::input_handler->down_key_held &&
               !global::main_dude->holding_item &&
               Collisions::checkCollisionWithMainDude(x, y, physical_width, physical_height)) {

        //pickup item from the ground

        global::main_dude->holding_item = true;
        hold_by_main_dude = true;
        global::main_dude->holding_item = true;
        global::input_handler->y_key_down = false;

    }

};

//check, if main dude can pickup to the inventory (not to the hands)
bool MovingObject::check_if_can_be_equipped() {

    return (global::input_handler->y_key_down && global::input_handler->down_key_held &&
            !global::main_dude->holding_item) &&
           Collisions::checkCollisionWithMainDude(x, y, sprite_width, sprite_height);

};


//this should be applied, when item is being carried by main dude
void MovingObject::set_pickuped_position(int pickup_offset_x, int pickup_offset_y) {

    if (hold_by_main_dude) {

        y = global::main_dude->y + pickup_offset_y;

        if (global::main_dude->state == 1) {
            x = global::main_dude->x - pickup_offset_x;
        } else
            x = global::main_dude->x + pickup_offset_x;

    }

}

//when applied, item kills mobs if it boths travels and collides them
void MovingObject::kill_mobs_if_thrown() {
    if (abs(xSpeed) > 0 || abs(ySpeed) > 0) {
        for (int a = 0; a < global::sprites.size(); a++) {

            if ((global::sprites.at(a)->spriteType == SpritesheetType::SNAKE ||
                 global::sprites.at(a)->spriteType == SpritesheetType::BAT ||
                 global::sprites.at(a)->spriteType == SpritesheetType::SPIDER)
                && !global::sprites.at(a)->killed) {
                if (Collisions::checkCollisionBodies(x, y, 16, 16, global::sprites.at(a)->x,
                                                     global::sprites.at(a)->y, physical_width, physical_height)) {
                    global::sprites.at(a)->kill();
                }
            }
        }
    }
}

//when applied, item kills mobs and destroys items (like jars), if it both travels and collides them
void MovingObject::kill_mobs_items_if_thrown() {

    if (abs(xSpeed) > 0 || abs(ySpeed) > 0) {
        for (int a = 0; a < global::sprites.size(); a++) {
            if ((global::sprites.at(a)->spriteType == SpritesheetType::SNAKE ||
                 global::sprites.at(a)->spriteType == SpritesheetType::BAT ||
                 global::sprites.at(a)->spriteType == SpritesheetType::SPIDER ||
                 global::sprites.at(a)->spriteType == SpritesheetType::JAR)
                && !global::sprites.at(a)->killed) {

                if (Collisions::checkCollisionBodies(x, y, physical_width, physical_height, global::sprites.at(a)->x,
                                                     global::sprites.at(a)->y,
                                                     global::sprites.at(a)->physical_width,
                                                     global::sprites.at(a)->physical_height)) {
                    global::sprites.at(a)->kill();
                }
            }
        }
    }

}

void MovingObject::apply_gravity(double acceleration) {
    if (!bottomCollision)
        ySpeed += acceleration;
}

//limit speed to its maximum
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

void MovingObject::apply_friction(float friction_delta_speed) {

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

//get x,y position according to current camera position
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

    if (*out_main_y + sprite_height < 0 || *out_main_x + sprite_width < 0) {
        *out_main_x = -SCREEN_HEIGHT;
        *out_main_y = -SCREEN_HEIGHT;
    }

}

//moves this object according to current x/y speed, also check collisions with map
void MovingObject::update_position() {

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

        if (old_xx != xx || old_yy != yy || physical_width <= 8 || physical_height <= 8) {
            if (xx < 31 && yy < 31)
                updateCollisionsMap(xx, yy);
        }

        old_xx = xx;
        old_yy = yy;

        tempXspeed--;
        tempYspeed--;

    }
}
