//
// Created by xdbeef on 23.04.18.
//

#include <cmath>
#include "Jar.hpp"
#include "../../GlobalsDeclarations.hpp"
#include "../../../build/gfx_spike_collectibles_flame.h"
#include "../../collisions/Collisions.hpp"
#include "../../memory/SpriteUtils.hpp"

void Jar::destroy() {
    _destroyed = true;
    match_animation();
}

void Jar::update_item_specific() {

    if (_ready_to_dispose) return;

    if (_anim_frame_index >= 7) {
        //last frame of 'poof' animation that is played on destroying jar
        sprite_utils::set_visibility(false, _main_sprite_info, _sub_sprite_info);
        _ready_to_dispose = true;
        return;
    }

    if (global::main_dude->using_whip) {
        if(check_collision(global::main_dude->whip)){
            destroy();
        }
    }

    if (_destroyed && _anim_frame_index < 7) {
        _anim_frame_timer += *global::timer;
        if (_anim_frame_timer > 50) {
            _anim_frame_index++;
            match_animation();
            _anim_frame_timer = 0;
        }
    }

    if(_hold_by_main_dude)
        sprite_utils::set_priority(OBJPRIORITY_0, _main_sprite_info, _sub_sprite_info);
    else
        sprite_utils::set_priority(OBJPRIORITY_1, _main_sprite_info, _sub_sprite_info);

    if (kill_creatures_if_have_speed(1)) { _destroyed = true; }

    if ((fabs(_x_speed) > 0.5 || fabs(_y_speed) > 0.5) &&
        (_bottom_collision || _left_collision || _right_collision || _upper_collision)) {
        //destroy jar on colliding with map tiles with enough speed
        destroy();
        _x_speed = 0;
        _y_speed = 0;
    }

    update_sprites_position();

}

void Jar::init_sprites() {

    delete_sprites();

    _sub_sprite_info = global::sub_oam_manager->initSprite(gfx_spike_collectibles_flamePal,
                                                           gfx_spike_collectibles_flamePalLen, nullptr, _sprite_size,
                                                           ObjSize::OBJSIZE_16, JAR, true, false,
                                                           LAYER_LEVEL::MIDDLE_TOP);
    _main_sprite_info = global::main_oam_manager->initSprite(gfx_spike_collectibles_flamePal,
                                                             gfx_spike_collectibles_flamePalLen, nullptr, _sprite_size,
                                                             ObjSize::OBJSIZE_16, JAR, true, false,
                                                             LAYER_LEVEL::MIDDLE_TOP);
    match_animation();
    update_sprites_position();

    sprite_utils::set_visibility(true, _main_sprite_info, _sub_sprite_info);
    sprite_utils::set_horizontal_flip(false, _main_sprite_info, _sub_sprite_info);
    sprite_utils::set_vertical_flip(false, _main_sprite_info, _sub_sprite_info);

}

void Jar::match_animation() {

    u8 *frame_gfx;

    if (_destroyed)
        frame_gfx = sprite_utils::get_frame((u8 *) gfx_spike_collectibles_flameTiles, _sprite_size,
                                            _anim_frame_index + 24);
    else
        frame_gfx = sprite_utils::get_frame((u8 *) gfx_spike_collectibles_flameTiles, _sprite_size, 24);

    sprite_utils::update_frame(frame_gfx, _sprite_size, _main_sprite_info, _sub_sprite_info);
}