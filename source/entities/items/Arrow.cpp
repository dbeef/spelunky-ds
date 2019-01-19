//
// Created by xdbeef on 04.04.18.
//

#include <cmath>
#include <maxmod9.h>

#include "../../../build/gfx_arrow.h"
#include "../../../build/soundbank.h"
#include "../../sound/SoundUtils.hpp"
#include "../../GlobalsDeclarations.hpp"
#include "../../collisions/Collisions.hpp"
#include "../../tiles/LevelRenderingUtils.hpp"
#include "../main_dude/MainDude.hpp"
#include "../../memory/SpriteUtils.hpp"
#include "Arrow.hpp"

void Arrow::update_item_specific() {

    // TODO Isn't this checked in game loop when iterating? If so, remove because redundancy.
    if (_ready_to_dispose) return;

    if (_hold_by_main_dude) {

        _thrown = false;

        if (global::main_dude->sprite_state == Orientation::RIGHT)
            _angle = 90;
        else
            _angle = 270;

        update_frame((int) floor(_angle / 22.5f));

    } else if (_thrown) {
        _armed_timer += *global::timer;
        if (_armed_timer > 100) {
            _thrown = true;
            _armed_timer = 0;
        }
    }

    update_sprites_position();

    if (kill_creatures_if_have_speed(3)) {
        sprite_utils::set_visibility(false, _main_sprite_info, _sub_sprite_info);
        _ready_to_dispose = true;
    } else if (_thrown && (fabs(_x_speed) > 1.5f || fabs(_y_speed) > 1.5) && kill_main_dude_if_have_speed(3)) {
        sprite_utils::set_visibility(false, _main_sprite_info, _sub_sprite_info);
        _ready_to_dispose = true;

        // TODO Make an util function for this;
        // settings those flags causes main dude to fall off whatever he is hanging on.
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

        if (_orientation == Orientation::LEFT)
            global::main_dude->_x_speed = -3;
        else if (_orientation == Orientation::RIGHT)
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

        double temp_angle = _angle;

        if (_bottom_collision) {
            if (_angle > 90 && _angle < 270) {
                if (_angle > 90 && _angle < 180)
                    //right-bottom collision
                    _angle = 180 - _angle;
                else if (_angle > 180 && _angle < 270)
                    //left-bottom collision
                    _angle = 360 - (_angle - 180);
            }
        }

        if (_upper_collision) {
            if (_angle > 270 && _angle < 360)
                //left-upper collision
                _angle = 90 - (_angle - 270);
            else if (_angle > 0 && _angle < 90)
                //right-upper collision
                _angle = 270 + (90 - _angle);
        }

        if (_right_collision) {
            if (_angle > 0 && _angle < 90)
                _angle = 270 + (90 - _angle);
        }

        if (_left_collision) {
            if (_angle > 270 && _angle < 360)
                _angle = 360 - _angle;
        }

        if (_bottom_collision && fabs(_y_speed) < 0.22f) {
            if (_angle > 0 && _angle <= 180)
                _angle = 90;
            else
                _angle = 270;
        }

        if (temp_angle != _angle)
            update_frame((int) floor(_angle / 22.5f));

        if (!_bottom_collision && !_hold_by_main_dude) {


            int frame_before = (int) floor(_angle / 22.5f);

            //change angle from time to time, so arrow would point to ground eventually
            if (_angle > 0 && _angle < 180) {
                _angle += 8 * (1 / ((_x_speed)));
            } else if (_angle > 180 && _angle < 360) {
                _angle -= 8 * (1 / ((_x_speed)));
            }

            if (_angle < 0)
                _angle = 0;
            if (_angle > 360)
                _angle = 0;

            int frame_after = (int) floor(_angle / 22.5f);

            if (frame_before != frame_after) {
                update_frame(frame_after);
            }

        }
        _map_collisions_checked = false;
    }


}

void Arrow::init_sprites() {

    delete_sprites();

    _sub_sprite_info = global::sub_oam_manager->initSprite(gfx_arrowPal, gfx_arrowPalLen,
                                                        nullptr, _sprite_size, ObjSize::OBJSIZE_8, SpritesheetType::ARROW,
                                                        true, false, LAYER_LEVEL::MIDDLE_TOP);
    _main_sprite_info = global::main_oam_manager->initSprite(gfx_arrowPal, gfx_arrowPalLen, nullptr,
                                                          _sprite_size, ObjSize::OBJSIZE_8, SpritesheetType::ARROW,
                                                          true, false, LAYER_LEVEL::MIDDLE_TOP);

    update_frame(static_cast<u16>(floor(_angle / 22.5f)));
    update_sprites_position();

    sprite_utils::set_priority(OBJPRIORITY_0, _main_sprite_info, _sub_sprite_info);
    sprite_utils::set_horizontal_flip(false, _main_sprite_info, _sub_sprite_info);
    sprite_utils::set_vertical_flip(false, _main_sprite_info, _sub_sprite_info);
    sprite_utils::set_visibility(true, _main_sprite_info, _sub_sprite_info);

}

void Arrow::update_frame(int frame_num) {
    u8 *frame_gfx = sprite_utils::get_frame((u8 *) gfx_arrowTiles, _sprite_size, frame_num);
    sprite_utils::update_frame(frame_gfx, _sprite_size, _main_sprite_info, _sub_sprite_info);
}
