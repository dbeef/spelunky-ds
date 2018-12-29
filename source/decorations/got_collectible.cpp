//
// Created by xdbeef on 19.05.18.
//

#include "got_collectible.hpp"
#include "../globals_declarations.hpp"
#include "../../build/gfx_got_collectible.h"
#include "../creatures/sprite_utils.hpp"

void GotCollectible::update() {

    if (_ready_to_dispose) return;

    update_sprites_position();

    _anim_frame_timer += *global::timer;

    if (_anim_frame_timer > got_collectible_anim_frame_delta) {

        _anim_frame_timer = 0;
        _anim_frame_index++;
        _cycle_counter++;

        if (_anim_frame_index == 2)
            _anim_frame_index = 0;

        if (_cycle_counter > got_collectible_cycles_to_hide) {
            _ready_to_dispose = true;
            sprite_utils::set_visibility(false, _main_sprite_info, _sub_sprite_info);
        } else
            match_animation();
    }
}

GotCollectible::GotCollectible(int x, int y, GotCollectible::Type type) :
        BaseDecoration(x, y, got_collectible_sprite_width, got_collectible_sprite_height,
                       got_collectible_spritesheet_type), _type(type) {
    init_sprites();
}

void GotCollectible::init_sprites() {

    delete_sprites();

    _sub_sprite_info = global::sub_oam_manager->initSprite(gfx_got_collectiblePal, gfx_got_collectiblePalLen,
                                                           nullptr, _sprite_size, _sprite_width,
                                                           _spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
    _main_sprite_info = global::main_oam_manager->initSprite(gfx_got_collectiblePal, gfx_got_collectiblePalLen,
                                                             nullptr, _sprite_size, _sprite_width,
                                                             _spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);

    sprite_utils::set_vertical_flip(false, _main_sprite_info, _sub_sprite_info);
    sprite_utils::set_horizontal_flip(false, _main_sprite_info, _sub_sprite_info);

    update_sprites_position();
    match_animation();
}

void GotCollectible::match_animation() {

    //It will be two 32x32 chunks later every incrementation of _type.
    u8 offset = static_cast<u8>(_type) * static_cast<u8>(2);

    _frame_gfx = sprite_utils::get_frame((u8 *) gfx_got_collectibleTiles, _sprite_size,
                                         offset + _anim_frame_index);
    sprite_utils::update_frame(_frame_gfx, _sprite_size, _main_sprite_info, _sub_sprite_info);
}

void GotCollectible::delete_sprites() {
    delete _main_sprite_info;
    delete _sub_sprite_info;
    _main_sprite_info = nullptr;
    _sub_sprite_info = nullptr;
}

void GotCollectible::update_sprites_position() {
    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);
    sprite_utils::set_entry_xy(_main_sprite_info, static_cast<u16>(main_x), static_cast<u16>(main_y));
    sprite_utils::set_entry_xy(_sub_sprite_info, static_cast<u16>(sub_x), static_cast<u16>(sub_y));
}
