//
// Created by xdbeef on 23.04.18.
//

#include <cmath>
#include "Jar.hpp"
#include "../../GlobalsDeclarations.hpp"
#include "../../../build/gfx_spike_collectibles_flame.h"
#include "../../collisions/Collisions.hpp"
#include "../SpriteUtils.hpp"

#define JAR_POS_INC_DELTA 15
#define JAR_HITPOINTS 1

void Jar::update_creature_specific() {

    if (_ready_to_dispose) return;

    if (_anim_frame_index >= 7) {
        //last frame of 'poof' animation that is played on destroying jar
        sprite_utils::set_visibility(false, _main_sprite_info, _sub_sprite_info);
        _ready_to_dispose = true;
        return;
    }

    check_if_can_be_pickuped();
    set_pickuped_position(2, 8, 6);

    if (killed && _anim_frame_index < 7) {

        _anim_frame_timer += *global::timer;
        if (_anim_frame_timer > 50) {
            _anim_frame_index++;
            match_animation();
            _anim_frame_timer = 0;
        }

    }

    if (kill_mobs_if_thrown(1))
        apply_dmg(1);

    set_sprite_attributes();
    kill_if_whip(1);

    if ((fabs(_x_speed) > 0.5 || fabs(_y_speed) > 0.5) &&
        (_bottom_collision || _left_collision || _right_collision || _upper_collision)) {
        //destroy jar on colliding with map tiles with enough speed
        apply_dmg(1);
        _x_speed = 0;
        _y_speed = 0;
    }

}

void Jar::init_sprites() {

    delete_sprites();

    _sub_sprite_info = global::sub_oam_manager->initSprite(gfx_spike_collectibles_flamePal,
                                                        gfx_spike_collectibles_flamePalLen, nullptr, _sprite_size,
                                                           ObjSize::OBJSIZE_16, JAR, true, false, LAYER_LEVEL::MIDDLE_TOP);
    _main_sprite_info = global::main_oam_manager->initSprite(gfx_spike_collectibles_flamePal,
                                                          gfx_spike_collectibles_flamePalLen, nullptr, _sprite_size,
                                                             ObjSize::OBJSIZE_16, JAR, true, false, LAYER_LEVEL::MIDDLE_TOP);
    match_animation();
    set_sprite_attributes();
}

void Jar::apply_dmg(int dmg_to_apply) {
    //jar has only 1 dmg point, always kill if any dmg_apply
    killed = true;
}

void Jar::match_animation() {

    if (killed)
        _frame_gfx = sprite_utils::get_frame((u8 *) gfx_spike_collectibles_flameTiles, _sprite_size, _anim_frame_index + 24);
    else
        _frame_gfx = sprite_utils::get_frame((u8 *) gfx_spike_collectibles_flameTiles, _sprite_size, 24);

    sprite_utils::update_frame(_frame_gfx, _sprite_size, _main_sprite_info, _sub_sprite_info);
}

void Jar::update_sprites_position() {
    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);
    sprite_utils::set_entry_xy(_main_sprite_info, static_cast<u16>(main_x), static_cast<u16>(main_y));
    sprite_utils::set_entry_xy(_sub_sprite_info, static_cast<u16>(sub_x), static_cast<u16>(sub_y));
}

void Jar::set_sprite_attributes() {
    update_sprites_position();
    sprite_utils::set_visibility(true, _main_sprite_info, _sub_sprite_info);
    sprite_utils::set_vertical_flip(false, _main_sprite_info, _sub_sprite_info);
    sprite_utils::set_horizontal_flip(false, _main_sprite_info, _sub_sprite_info);
    sprite_utils::set_priority(OBJPRIORITY_0, _main_sprite_info, _sub_sprite_info);
}

void Jar::delete_sprites() {
    delete _main_sprite_info;
    delete _sub_sprite_info;
    _main_sprite_info = nullptr;
    _sub_sprite_info = nullptr;
}
