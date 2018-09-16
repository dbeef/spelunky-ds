//
// Created by xdbeef on 04.04.18.
//

#include <cmath>
#include <maxmod9.h>
#include "../../globals_declarations.hpp"
#include "../main_dude/main_dude.hpp"
#include "../../../build/gfx_arrow.h"
#include "rock.hpp"
#include "../../collisions/collisions.hpp"
#include "../../tiles/level_rendering_utils.hpp"
#include "arrow.hpp"
#include "../../../build/soundbank.h"
#include "../../sound/sound_utils.hpp"
#include "../sprite_utils.hpp"

#define ARROW_POS_INC_DELTA 15

void Arrow::draw() {

    if (ready_to_dispose) return;

    check_if_can_be_pickuped();

    if (hold_by_main_dude) {

        thrown = false;

        set_pickuped_position(1, 10, 6);

        if (global::main_dude->sprite_state == SpriteState::W_RIGHT)
            angle = 90;
        else
            angle = 270;

        update_frame((int) floor(angle / 22.5f));

    } else if (thrown) {
        armed_timer += *global::timer;
        if (armed_timer > 100) {
            thrown = true;
            armed_timer = 0;
        }
    }

    set_position();
    sprite_utils::set_priority(OBJPRIORITY_0, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);

    if (kill_mobs_if_thrown(3)) {
        sprite_utils::set_visibility(false, mainSpriteInfo, subSpriteInfo);
        ready_to_dispose = true;
    } else if (thrown && (fabs(xSpeed) > 1.5f || fabs(ySpeed) > 1.5) && kill_main_dude_if_thrown(3)) {
        sprite_utils::set_visibility(false, mainSpriteInfo, subSpriteInfo);
        ready_to_dispose = true;

        spawn_blood();

        //TODO Make an util function for this
        global::main_dude->can_climb_rope = false;
        global::main_dude->started_climbing_rope = false;
        global::main_dude->can_climb_ladder = false;
        global::main_dude->started_climbing_ladder = false;
        global::main_dude->climbing = false;
        global::main_dude->hanging_on_tile_left = false;
        global::main_dude->hanging_on_tile_right = false;
        global::main_dude->using_whip = false;
        global::main_dude->stunned_timer = 0;
        global::main_dude->stunned = true;

        if (sprite_state == SpriteState::W_LEFT)
            global::main_dude->xSpeed = -3;
        else if (sprite_state == SpriteState::W_RIGHT)
            global::main_dude->xSpeed = 3;

        global::main_dude->time_since_last_damage = 0;
        global::hud->hearts -= 2;
        global::hud->draw_level_hud();

        if (global::hud->hearts <= 0) {
            global::main_dude->ySpeed = -MAIN_DUDE_JUMP_SPEED * 0.25;
            global::main_dude->dead = true;
            mmEffect(SFX_XDIE);
            sound::stop_cave_music();
            consoleClear();

        } else
            mmEffect(SFX_XHIT);

    }

}


void Arrow::init() {
    thrown = true;
    activated = true;
    angle = 90;
    initSprite();
}

void Arrow::updateSpeed() {

    limit_speed(MAX_X_SPEED_ARROW, MAX_Y_SPEED_ARROW);

    pos_inc_timer += *global::timer;

    bool change_pos = (pos_inc_timer > ARROW_POS_INC_DELTA) && !hold_by_main_dude;

    if (change_pos) {
        apply_friction(0.085);
        update_position();
        apply_gravity(GRAVITY_DELTA_SPEED * 0.8);
        pos_inc_timer = 0;


        if (!bottomCollision) {

            int frame_before = (int) floor(angle / 22.5f);

            //change angle from time to time, so arrow would point to ground eventually
            if (angle > 0 && angle < 180) {
                angle += 8 * (1 / ((xSpeed)));
            } else if (angle > 180 && angle < 360) {
                angle -= 8 * (1 / ((xSpeed)));
            }

            if (angle < 0)
                angle = 0;
            if (angle > 360)
                angle = 0;

            int frame_after = (int) floor(angle / 22.5f);

            if (frame_before != frame_after) {
                update_frame(frame_after);
            }
        }

    }


}


void Arrow::updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) {

    MapTile *t[9];
    Collisions::getNeighboringTiles(global::current_level->map_tiles,
                                    x_current_pos_in_tiles, y_current_pos_in_tiles, t);

    upperCollision = Collisions::checkUpperCollision(t, &x, &y, &ySpeed, physical_width, true, 0.7);
    bottomCollision = Collisions::checkBottomCollision(t, &x, &y, &ySpeed, physical_width, physical_height, true, 0.42);
    leftCollision = Collisions::checkLeftCollision(t, &x, &y, &xSpeed, physical_width, physical_height, true, 0.15);
    rightCollision = Collisions::checkRightCollision(t, &x, &y, &xSpeed, physical_width, physical_height, true, 0.15);

    //now handle bouncing off map tiles:

    double temp_angle = angle;

    if (bottomCollision) {
        if (angle > 90 && angle < 270) {
            if (angle > 90 && angle < 180)
                //right-bottom collision
                angle = 180 - angle;
            else if (angle > 180 && angle < 270)
                //left-bottom collision
                angle = 360 - (angle - 180);
        }
    }

    if (upperCollision) {
        if (angle > 270 && angle < 360)
            //left-upper collision
            angle = 90 - (angle - 270);
        else if (angle > 0 && angle < 90)
            //right-upper collision
            angle = 270 + (90 - angle);
    }

    if (rightCollision) {
        if (angle > 0 && angle < 90)
            angle = 270 + (90 - angle);
    }

    if (leftCollision) {
        if (angle > 270 && angle < 360)
            angle = 360 - angle;
    }

    if (bottomCollision && fabs(ySpeed) < 0.22f) {
        if (angle > 0 && angle <= 180)
            angle = 90;
        else
            angle = 270;
    }

    if (temp_angle != angle)
        update_frame((int) floor(angle / 22.5f));

}

void Arrow::initSprite() {

    delete mainSpriteInfo;
    delete subSpriteInfo;

    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_arrowPal, gfx_arrowPalLen,
                                                        nullptr, ARROW_SPRITE_SIZE, 8, SpritesheetType::ARROW,
                                                        true, false, LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_arrowPal, gfx_arrowPalLen, nullptr,
                                                          ARROW_SPRITE_SIZE, 8, SpritesheetType::ARROW,
                                                          true, false, LAYER_LEVEL::MIDDLE_TOP);
    update_frame((int) floor(angle / 22.5f));
    set_position();
    sprite_utils::set_priority(OBJPRIORITY_0, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
}

Arrow::Arrow() {
    physical_height = ARROW_PHYSICAL_HEIGHT;
    physical_width = ARROW_PHYSICAL_WIDTH;
    sprite_height = ARROW_SPRITE_HEIGHT;
    sprite_width = ARROW_SPRITE_WIDTH;
    spritesheet_type = SpritesheetType::ARROW;
    sprite_type = SpriteType::S_ARROW;
}

void Arrow::update_frame(int frame_num) {
    frameGfx = sprite_utils::get_frame((u8 *) gfx_arrowTiles, ARROW_SPRITE_SIZE, frame_num);
    sprite_utils::update_frame(frameGfx, ARROW_SPRITE_SIZE, mainSpriteInfo, subSpriteInfo);
}

void Arrow::set_position() {
    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);
    sprite_utils::set_entry_xy(mainSpriteInfo, main_x, main_y);
    sprite_utils::set_entry_xy(subSpriteInfo, sub_x, sub_y);
}
