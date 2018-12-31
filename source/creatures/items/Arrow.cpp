//
// Created by xdbeef on 04.04.18.
//

#include <cmath>
#include <maxmod9.h>
#include "../../GlobalsDeclarations.hpp"
#include "../main_dude/MainDude.hpp"
#include "../../../build/gfx_arrow.h"
#include "Rock.hpp"
#include "../../collisions/Collisions.hpp"
#include "../../tiles/LevelRenderingUtils.hpp"
#include "Arrow.hpp"
#include "../../../build/soundbank.h"
#include "../../sound/SoundUtils.hpp"
#include "../SpriteUtils.hpp"

void Arrow::update_creature_specific() {

    if (_ready_to_dispose) return;

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

    update_sprites_position();
    sprite_utils::set_priority(OBJPRIORITY_0, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);

    if (kill_mobs_if_thrown(3)) {
        sprite_utils::set_visibility(false, mainSpriteInfo, subSpriteInfo);
        _ready_to_dispose = true;
    } else if (thrown && (fabs(_x_speed) > 1.5f || fabs(_y_speed) > 1.5) && kill_main_dude_if_thrown(3)) {
        sprite_utils::set_visibility(false, mainSpriteInfo, subSpriteInfo);
        _ready_to_dispose = true;

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
            global::main_dude->_x_speed = -3;
        else if (sprite_state == SpriteState::W_RIGHT)
            global::main_dude->_x_speed = 3;

        global::main_dude->time_since_last_damage = 0;
        global::hud->hearts -= 2;
        global::hud->draw_level_hud();

        if (global::hud->hearts <= 0) {
            global::main_dude->set_dead();
        } else
            mmEffect(SFX_XHIT);

    }


    if (_map_collisions_checked) {
        //now handle bouncing off map tiles:

        double temp_angle = angle;

        if (_bottom_collision) {
            if (angle > 90 && angle < 270) {
                if (angle > 90 && angle < 180)
                    //right-bottom collision
                    angle = 180 - angle;
                else if (angle > 180 && angle < 270)
                    //left-bottom collision
                    angle = 360 - (angle - 180);
            }
        }

        if (_upper_collision) {
            if (angle > 270 && angle < 360)
                //left-upper collision
                angle = 90 - (angle - 270);
            else if (angle > 0 && angle < 90)
                //right-upper collision
                angle = 270 + (90 - angle);
        }

        if (_right_collision) {
            if (angle > 0 && angle < 90)
                angle = 270 + (90 - angle);
        }

        if (_left_collision) {
            if (angle > 270 && angle < 360)
                angle = 360 - angle;
        }

        if (_bottom_collision && fabs(_y_speed) < 0.22f) {
            if (angle > 0 && angle <= 180)
                angle = 90;
            else
                angle = 270;
        }

        if (temp_angle != angle)
            update_frame((int) floor(angle / 22.5f));

        if (!_bottom_collision && !hold_by_main_dude) {


            int frame_before = (int) floor(angle / 22.5f);

            //change angle from time to time, so arrow would point to ground eventually
            if (angle > 0 && angle < 180) {
                angle += 8 * (1 / ((_x_speed)));
            } else if (angle > 180 && angle < 360) {
                angle -= 8 * (1 / ((_x_speed)));
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


    _map_collisions_checked = false;
}

void Arrow::init_sprites() {

    delete_sprites();

    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_arrowPal, gfx_arrowPalLen,
                                                        nullptr, ARROW_SPRITE_SIZE, 8, SpritesheetType::ARROW,
                                                        true, false, LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_arrowPal, gfx_arrowPalLen, nullptr,
                                                          ARROW_SPRITE_SIZE, 8, SpritesheetType::ARROW,
                                                          true, false, LAYER_LEVEL::MIDDLE_TOP);
    update_frame((int) floor(angle / 22.5f));
    update_sprites_position();
    sprite_utils::set_priority(OBJPRIORITY_0, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
}

void Arrow::update_frame(int frame_num) {
    frameGfx = sprite_utils::get_frame((u8 *) gfx_arrowTiles, ARROW_SPRITE_SIZE, frame_num);
    sprite_utils::update_frame(frameGfx, ARROW_SPRITE_SIZE, mainSpriteInfo, subSpriteInfo);
}

void Arrow::update_sprites_position() {
    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);
    sprite_utils::set_entry_xy(mainSpriteInfo, main_x, main_y);
    sprite_utils::set_entry_xy(subSpriteInfo, sub_x, sub_y);
}

void Arrow::delete_sprites() {
    delete mainSpriteInfo;
    delete subSpriteInfo;
    mainSpriteInfo = nullptr;
    subSpriteInfo = nullptr;
}
