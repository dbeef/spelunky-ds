//
// Created by xdbeef on 07.04.18.
// https://stackoverflow.com/questions/120876/what-are-the-rules-for-calling-the-superclass-constructor
//

#include <maxmod9.h>
#include <cmath>
#include "moving_object.h"
#include "../globals_declarations.h"
#include "../tiles/level_rendering_utils.h"
#include "../collisions/collisions.h"
#include "../../build/soundbank.h"
#include "../sound/sound_utils.h"
#include "animations/blood.h"

void MovingObject::spawn_blood() {
    for (int a = 0; a < 4; a++) {
        Blood *blood = new Blood();
        blood->init();
        blood->x = x;
        blood->y = y;

        if (rand() % 2 == 0)
            blood->xSpeed = (-0.8 - ((float) (rand() % 20) / 10)) / a;
        else
            blood->xSpeed = (0.8 + ((float) (rand() % 20) / 10)) / a;

        blood->ySpeed = (-1.5 - (((float) (rand() % 20) / 5))) / a;
        global::sprites_to_add.push_back(blood);
    }
}

void MovingObject::deal_damage_main_dude_on_collision(int dmg_to_apply) {
    if (!killed && !global::main_dude->dead && Collisions::checkCollisionWithMainDude(x, y, 16, 16) &&
        global::main_dude->time_since_last_damage > 1000 && !global::main_dude->exiting_level) {

        global::main_dude->time_since_last_damage = 0;
        global::hud->hearts -= dmg_to_apply;
        global::hud->draw_level_hud();

        if (global::hud->hearts <= 0) {
            global::main_dude->ySpeed = -MAIN_DUDE_JUMP_SPEED * 0.25;
            global::main_dude->dead = true;
            mmEffect(SFX_XDIE);
            consoleClear();

            sound::stop_cave_music();
        } else
            mmEffect(SFX_XHIT);
    }
}

void MovingObject::kill_if_whip(int dmg_to_apply) {
    if (global::main_dude->using_whip && !killed && global::main_dude->whip->whip_timer > 120) {
        if (Collisions::checkCollisionWithMainDudeWhip(x, y, sprite_width, sprite_height)) {
            apply_dmg(dmg_to_apply);
        }
    }
}

void MovingObject::kill_if_main_dude_jumped_on_you(int dmg_to_apply) {
    if (!killed && Collisions::checkCollisionWithMainDude(x, y, sprite_width, sprite_height) &&
        global::main_dude->ySpeed > 0 &&
        global::main_dude->y - 4 < y) {

        apply_dmg(dmg_to_apply);
        global::main_dude->ySpeed = -2;
        global::main_dude->jumping_timer = 0;

    }
}

//for opening chests and crates
bool MovingObject::check_if_can_be_opened() {
    if (!activated &&
        Collisions::checkCollisionWithMainDudeWidthBoundary(x, y, physical_width, physical_height, 8) &&
        global::input_handler->up_key_held && global::input_handler->y_key_down) {

        activated = true;
        global::input_handler->y_key_down = false;

        return true;
    } else
        return false;
}

//TODO Merge check_if_can_be_pickuped with set_pickuped_position
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
        global::input_handler->y_key_down = false;

    }

};

//check, if main dude can pickup to the inventory (not to the hands)
bool MovingObject::check_if_can_be_equipped() {

    bool q = (global::input_handler->y_key_down && global::input_handler->down_key_held &&
              !global::main_dude->holding_item) &&
             Collisions::checkCollisionWithMainDude(x, y, sprite_width, sprite_height);

    if (q) {
        global::input_handler->y_key_down = false;
    }

    return q;

};


//this should be applied, when item is being carried by main dude
void MovingObject::set_pickuped_position(int pickup_offset_x, int pickup_offset_y) {

    if (hold_by_main_dude) {

        y = global::main_dude->y + pickup_offset_y;

        if (global::main_dude->sprite_state == SpriteState::W_LEFT) {
            x = global::main_dude->x - pickup_offset_x;
        } else
            x = global::main_dude->x + pickup_offset_x;


        sprite_state = global::main_dude->sprite_state;
    }

}

//this should be applied, when item is being carried by main dude
void MovingObject::set_pickuped_position(int pickup_offset_x_left, int pickup_offset_x_right, int pickup_offset_y) {

    if (hold_by_main_dude) {

        y = global::main_dude->y + pickup_offset_y;

        if (global::main_dude->sprite_state == SpriteState::W_LEFT) {
            x = global::main_dude->x - pickup_offset_x_left;
        } else
            x = global::main_dude->x + pickup_offset_x_right;


        sprite_state = global::main_dude->sprite_state;
    }

}

//this should be applied, when item is being carried by another moving object
void
MovingObject::set_pickuped_position_on_another_moving_obj(int pickup_offset_x, int pickup_offset_y, MovingObject *m) {

    m->y = y + pickup_offset_y;

    if (sprite_state == SpriteState::W_LEFT) {
        m->x = x - pickup_offset_x;
    } else
        m->x = x + pickup_offset_x;

}


//this should be applied, when item is being carried by main dude
void MovingObject::set_pickuped_position_not_checking(int pickup_offset_x, int pickup_offset_y) {

    y = global::main_dude->y + pickup_offset_y;

    if (global::main_dude->sprite_state == SpriteState::W_LEFT) {
        x = global::main_dude->x - pickup_offset_x;
    } else
        x = global::main_dude->x + pickup_offset_x;

}


//when applied, item kills mobs if it boths travels and collides them
bool MovingObject::kill_mobs_if_thrown(int dmg_to_apply) {

    bool killed = false;

    if (abs(xSpeed) > 0 || abs(ySpeed) > 0) {
        for (int a = 0; a < global::sprites.size(); a++) {

            if ((global::sprites.at(a)->spritesheet_type == SpritesheetType::SNAKE ||
                 global::sprites.at(a)->spritesheet_type == SpritesheetType::BAT_JETPACK ||
                 global::sprites.at(a)->spritesheet_type == SpritesheetType::CAVEMAN_DAMSEL ||
                 global::sprites.at(a)->spritesheet_type == SpritesheetType::SKELETON_SPIDER)
                && !global::sprites.at(a)->killed) {
                if (Collisions::checkCollisionBodies(x, y, 16, 16, global::sprites.at(a)->x,
                                                     global::sprites.at(a)->y, physical_width, physical_height)) {
                    global::sprites.at(a)->apply_dmg(dmg_to_apply);
                    killed = true;
                }
            }
        }
    }

    return killed;

}

bool MovingObject::kill_main_dude_if_thrown(int dmg_to_apply) {

    if (abs(xSpeed) > 0 || abs(ySpeed) > 0) {

        if (Collisions::checkCollisionBodies(x, y, physical_width, physical_height, global::main_dude->x,
                                             global::main_dude->y,
                                             global::main_dude->physical_width,
                                             global::main_dude->physical_height)) {
            global::main_dude->xSpeed += this->xSpeed * 0.3f;
            global::main_dude->apply_dmg(dmg_to_apply);
            return true;
        }

    }
    return false;
}

//when applied, item kills mobs and destroys items (like jars), if it both travels and collides them
bool MovingObject::kill_mobs_items_if_thrown(int dmg_to_apply) {

    bool killed = false;

    if (abs(xSpeed) > 0 || abs(ySpeed) > 0) {
        for (int a = 0; a < global::sprites.size(); a++) {
            //FIXME Change spritesheet type to sprite_type, so jetpack wouldn't be affected
            if ((global::sprites.at(a)->spritesheet_type == SpritesheetType::SNAKE ||
                 global::sprites.at(a)->spritesheet_type == SpritesheetType::BAT_JETPACK ||
                 global::sprites.at(a)->spritesheet_type == SpritesheetType::SHOPKEEPER ||
                 global::sprites.at(a)->spritesheet_type == SpritesheetType::SKELETON_SPIDER ||
                 global::sprites.at(a)->spritesheet_type == SpritesheetType::CAVEMAN_DAMSEL ||
                 global::sprites.at(a)->spritesheet_type == SpritesheetType::JAR)
                && !global::sprites.at(a)->ready_to_dispose) {

                if (Collisions::checkCollisionBodies(x, y, physical_width, physical_height,
                                                     global::sprites.at(a)->x,
                                                     global::sprites.at(a)->y,
                                                     global::sprites.at(a)->physical_width,
                                                     global::sprites.at(a)->physical_height)) {
                    global::sprites.at(a)->xSpeed += this->xSpeed * 0.3f;

                    global::sprites.at(a)->apply_dmg(dmg_to_apply);
                    killed = true;
                }
            }
        }
    }

    return killed;
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

    if (*out_sub_y + sprite_height < 0 || *out_sub_x + sprite_width < 0) {
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

    double tempXspeed = fabs(xSpeed);
    double tempYspeed = fabs(ySpeed);

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

        xx = floor_div(this->x + 0.5 * physical_width, TILE_W);
        yy = floor_div(this->y + 0.5 * physical_height, TILE_H);

        if (old_xx != xx || old_yy != yy || physical_width <= 8 || physical_height <= 8 || killed) {
            if (xx < 31 && yy < 31)
                updateCollisionsMap(xx, yy);
        }

        old_xx = xx;
        old_yy = yy;

        tempXspeed--;
        tempYspeed--;

    }
}

void MovingObject::set_sprite_state_basing_on_speed() {
    if (xSpeed > 0)
        sprite_state = SpriteState::W_RIGHT;
    else
        sprite_state = SpriteState::W_LEFT;
}

//https://stackoverflow.com/questions/120876/what-are-the-rules-for-calling-the-superclass-constructor