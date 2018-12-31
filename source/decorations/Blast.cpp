//
// Created by xdbeef on 23.06.18.
//

#include "../../build/gfx_spike_collectibles_flame.h"
#include "../GlobalsDeclarations.hpp"
#include "../creatures/SpriteUtils.hpp"
#include "Blast.hpp"

Blast::Blast(int x, int y) :
        BaseDecoration(x, y, blast_sprite_width, blast_sprite_height, blast_spritesheet_type) {
    init_sprites();
}

void Blast::update_decoration_specific() {

    if (_ready_to_dispose) return;

    //'_firing' property is set by the owner of a blast instance when firing, a pistol or shotgun.
    if (_firing) {

        _anim_frame_timer += *global::timer;

        if (_anim_frame_timer > blast_anim_frame_delta) {
            _anim_frame_timer = 0;
            _anim_frame_index++;
        }

        match_animation();

        sprite_utils::set_visibility(true, _main_sprite_info, _sub_sprite_info);

    } else {
        sprite_utils::set_visibility(false, _main_sprite_info, _sub_sprite_info);
        sprite_utils::set_horizontal_flip(_sprite_state == SpriteState::W_LEFT, _main_sprite_info, _sub_sprite_info);
    }

    update_sprites_position();

}

void Blast::init_sprites() {

    delete_sprites();

    _sub_sprite_info = global::sub_oam_manager->initSprite(gfx_spike_collectibles_flamePal,
                                                           gfx_spike_collectibles_flamePalLen,
                                                           nullptr, _sprite_size, _sprite_width,
                                                           _spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
    _main_sprite_info = global::main_oam_manager->initSprite(gfx_spike_collectibles_flamePal,
                                                             gfx_spike_collectibles_flamePalLen,
                                                             nullptr, _sprite_size, _sprite_width,
                                                             _spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);

    sprite_utils::set_visibility(true, _main_sprite_info, _sub_sprite_info);
    sprite_utils::set_vertical_flip(false, _main_sprite_info, _sub_sprite_info);
    sprite_utils::set_horizontal_flip(_sprite_state == SpriteState::W_LEFT, _main_sprite_info, _sub_sprite_info);
    sprite_utils::set_priority(OBJPRIORITY_0, _main_sprite_info, _sub_sprite_info);

    match_animation();
    update_sprites_position();

}

void Blast::update_sprites_position() {

    int temp_y = _y, temp_x = _x;

    _y -= blast_firing_offset_y;

    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);

    if (_sprite_state == SpriteState::W_LEFT)
        _x -= blast_firing_offset_x;
    else
        _x += blast_firing_offset_x;

    sprite_utils::set_entry_xy(_main_sprite_info, static_cast<u16>(main_x), static_cast<u16>(main_y));
    sprite_utils::set_entry_xy(_sub_sprite_info, static_cast<u16>(sub_x), static_cast<u16>(sub_y));

    _y = temp_y;
    _x = temp_x;
}

void Blast::match_animation() {
    _frame_gfx = sprite_utils::get_frame((u8 *) gfx_spike_collectibles_flameTiles, _sprite_size,
                                         13 + _anim_frame_index);
    sprite_utils::update_frame(_frame_gfx, _sprite_size, _main_sprite_info, _sub_sprite_info);
}

void Blast::delete_sprites() {
    delete _main_sprite_info;
    delete _sub_sprite_info;
    _main_sprite_info = nullptr;
    _sub_sprite_info = nullptr;
}

void Blast::introduce_yourself() const {
    printf("BLAST\n");
}

