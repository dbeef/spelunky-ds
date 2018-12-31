//
// Created by xdbeef on 07.04.18.
// https://stackoverflow.com/questions/120876/what-are-the-rules-for-calling-the-superclass-constructor
//

#include <maxmod9.h>
#include <cmath>
#include <cstdio>

#include "../../build/soundbank.h"
#include "../memory/OamManager.hpp"
#include "../camera/Camera.hpp"
#include "../GlobalsDeclarations.hpp"
#include "../tiles/LevelRenderingUtils.hpp"
#include "../collisions/Collisions.hpp"
#include "../sound/SoundUtils.hpp"
#include "_BaseCreature.h"
#include "animations/Blood.hpp"

void BaseCreature::spawn_blood() const {
    for (int a = 0; a < 4; a++) {
        auto *blood = new Blood(_x, _y);

        if (rand() % 2 == 0)
            blood->_x_speed = (-0.8 - ((float) (rand() % 20) / 10)) / a;
        else
            blood->_x_speed = (0.8 + ((float) (rand() % 20) / 10)) / a;

        blood->_y_speed = (-1.5 - (((float) (rand() % 20) / 5))) / a;
        global::creatures_to_add.push_back(blood);
    }
}

void BaseCreature::deal_damage_main_dude_on_collision(int dmg_to_apply) const {
    if (!killed && !global::main_dude->dead && Collisions::checkCollisionWithMainDude(_x, _y, 16, 16) &&
        global::main_dude->time_since_last_damage > 1000 && !global::main_dude->exiting_level) {

        global::main_dude->time_since_last_damage = 0;
        global::hud->hearts -= dmg_to_apply;
        global::hud->draw_level_hud();

        if (global::hud->hearts <= 0) {
            global::main_dude->set_dead();
        } else
            mmEffect(SFX_XHIT);
    }
}

void BaseCreature::kill_if_whip(int dmg_to_apply) {
    if (global::main_dude->using_whip && !killed && global::main_dude->whip->_whiping_timer > 120) {
        if (Collisions::checkCollisionWithMainDudeWhip(_x, _y, _sprite_width, _sprite_height)) {
            apply_dmg(dmg_to_apply);
        }
    }
}

void BaseCreature::kill_if_main_dude_jumped_on_you(int dmg_to_apply) {
    if (!killed && Collisions::checkCollisionWithMainDude(_x, _y, _sprite_width, _sprite_height) &&
        global::main_dude->_y_speed > 0 &&
        global::main_dude->_y_speed - 4 < _y) {

        apply_dmg(dmg_to_apply);
        global::main_dude->_y_speed = -2;
        global::main_dude->jumping_timer = 0;

    }
}

//for opening chests and crates
bool BaseCreature::check_if_can_be_opened() {
    if (!activated &&
        Collisions::checkCollisionWithMainDudeWidthBoundary(_x, _y, _physical_width, _physical_height, 8) &&
        global::input_handler->up_key_held && global::input_handler->y_key_down) {

        activated = true;
        global::input_handler->y_key_down = false;

        return true;
    } else
        return false;
}

//TODO Merge check_if_can_be_pickuped with set_pickuped_position
//check, if main dude can pickup this item (to hands, not to inventory)
void BaseCreature::check_if_can_be_pickuped() {

    if (hold_by_main_dude &&
        global::input_handler->y_key_down &&
        global::input_handler->down_key_held &&
        global::main_dude->_bottom_collision) {

        //leave item on ground

        hold_by_main_dude = false;
        global::main_dude->holding_item = false;
        global::input_handler->y_key_down = false;
        _bottom_collision = false;

    } else if (global::input_handler->y_key_down &&
               global::input_handler->down_key_held &&
               !global::main_dude->holding_item &&
               Collisions::checkCollisionWithMainDude(_x, _y, _physical_width, _physical_height)) {

        //pickup item from the ground

        global::main_dude->holding_item = true;
        hold_by_main_dude = true;
        global::input_handler->y_key_down = false;

    }

};

//check, if main dude can pickup to the inventory (not to the hands)
bool BaseCreature::check_if_can_be_equipped() {

    bool q = (global::input_handler->y_key_down && global::input_handler->down_key_held &&
              !global::main_dude->holding_item) &&
             Collisions::checkCollisionWithMainDude(_x, _y, _sprite_width, _sprite_height);

    if (q) {
        global::input_handler->y_key_down = false;
    }

    return q;

};


//this should be applied, when item is being carried by main dude
void BaseCreature::set_pickuped_position(int pickup_offset_x, int pickup_offset_y) {

    if (hold_by_main_dude) {

        _y = global::main_dude->_y + pickup_offset_y;

        if (global::main_dude->sprite_state == SpriteState::W_LEFT) {
            _x = global::main_dude->_x - pickup_offset_x;
        } else
            _x = global::main_dude->_x + pickup_offset_x;


        sprite_state = global::main_dude->sprite_state;
    }

}

//this should be applied, when item is being carried by main dude
void BaseCreature::set_pickuped_position(int pickup_offset_x_left, int pickup_offset_x_right, int pickup_offset_y) {

    if (hold_by_main_dude) {

        _y = global::main_dude->_y + pickup_offset_y;

        if (global::main_dude->sprite_state == SpriteState::W_LEFT) {
            _x = global::main_dude->_x - pickup_offset_x_left;
        } else
            _x = global::main_dude->_x + pickup_offset_x_right;


        sprite_state = global::main_dude->sprite_state;
    }

}

//this should be applied, when item is being carried by another moving object
void
BaseCreature::set_pickuped_position_on_another_moving_obj(int pickup_offset_x, int pickup_offset_y, BaseCreature *m) const {

    m->_y = _y + pickup_offset_y;

    if (sprite_state == SpriteState::W_LEFT) {
        m->_x = _x - pickup_offset_x;
    } else
        m->_x = _x + pickup_offset_x;

}


//this should be applied, when item is being carried by main dude
void BaseCreature::set_pickuped_position_not_checking(int pickup_offset_x, int pickup_offset_y) {

    _y = global::main_dude->_y + pickup_offset_y;

    if (global::main_dude->sprite_state == SpriteState::W_LEFT) {
        _x = global::main_dude->_x - pickup_offset_x;
    } else
        _x = global::main_dude->_x + pickup_offset_x;

}

//when applied, item kills mobs if it boths travels and collides them
bool BaseCreature::kill_mobs_if_thrown(int dmg_to_apply) const {

    bool killed = false;

    if (abs(_x_speed) > 0 || abs(_y_speed) > 0) {
        for (unsigned long a = 0; a < global::creatures.size(); a++) {

            if ((global::creatures.at(a)->_spritesheet_type == SpritesheetType::SNAKE ||
                 global::creatures.at(a)->_spritesheet_type == SpritesheetType::BAT_JETPACK ||
                 global::creatures.at(a)->_spritesheet_type == SpritesheetType::CAVEMAN_DAMSEL ||
                 global::creatures.at(a)->_spritesheet_type == SpritesheetType::SKELETON_SPIDER)
                && !global::creatures.at(a)->killed) {
                if (Collisions::checkCollisionBodies(_x, _y, 16, 16, global::creatures.at(a)->_x,
                                                     global::creatures.at(a)->_y, _physical_width, _physical_height)) {

                    global::creatures.at(a)->apply_dmg(dmg_to_apply);
                    killed = true;

                }
            }
        }
    }

    return killed;

}

bool BaseCreature::kill_main_dude_if_thrown(int dmg_to_apply) const {

    if (abs(_x_speed) > 0 || abs(_y_speed) > 0) {

        if (Collisions::checkCollisionBodies(_x, _y, _physical_width, _physical_height, global::main_dude->_x,
                                             global::main_dude->_y,
                                             global::main_dude->_physical_width,
                                             global::main_dude->_physical_height)) {
            global::main_dude->_x_speed += this->_x_speed * 0.3f;
            global::main_dude->apply_dmg(dmg_to_apply);
            return true;
        }

    }
    return false;
}

//when applied, item kills mobs and destroys items (like jars), if it both travels and collides them
bool BaseCreature::kill_mobs_items_if_thrown(int dmg_to_apply) {

    bool killed = false;

    if (abs(_x_speed) > 0 || abs(_y_speed) > 0) {
        for (unsigned long a = 0; a < global::creatures.size(); a++) {
            //FIXME Change spritesheet type to sprite_type, so jetpack wouldn't be affected
            if ((global::creatures.at(a)->_spritesheet_type == SpritesheetType::SNAKE ||
                 global::creatures.at(a)->_spritesheet_type == SpritesheetType::BAT_JETPACK ||
                 global::creatures.at(a)->_spritesheet_type == SpritesheetType::SHOPKEEPER ||
                 global::creatures.at(a)->_spritesheet_type == SpritesheetType::SKELETON_SPIDER ||
                 global::creatures.at(a)->_spritesheet_type == SpritesheetType::CAVEMAN_DAMSEL ||
                 global::creatures.at(a)->_spritesheet_type == SpritesheetType::JAR)
                && !global::creatures.at(a)->_ready_to_dispose) {

                if (Collisions::checkCollisionBodies(_x, _y, _physical_width, _physical_height,
                                                     global::creatures.at(a)->_x,
                                                     global::creatures.at(a)->_y,
                                                     global::creatures.at(a)->_physical_width,
                                                     global::creatures.at(a)->_physical_height)) {
                    global::creatures.at(a)->_x_speed += this->_x_speed * 0.3f;

                    global::creatures.at(a)->apply_dmg(dmg_to_apply);
                    killed = true;
                }
            }
        }
    }

    return killed;
}


//https://stackoverflow.com/questions/120876/what-are-the-rules-for-calling-the-superclass-constructor