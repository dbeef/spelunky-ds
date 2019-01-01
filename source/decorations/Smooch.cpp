//
// Created by xdbeef on 10.06.18.
//

#include "../../build/gfx_caveman_damsel.h"
#include "../GlobalsDeclarations.hpp"
#include "../creatures/SpriteUtils.hpp"
#include "Smooch.hpp"

void Smooch::update_decoration_specific() {

    if (_ready_to_dispose) return;

    update_sprites_position();

    _anim_frame_timer += *global::timer;

    // Animate and go upwards gradually
    if (_anim_frame_timer > smooch_anim_frame_delta) {

        _anim_frame_timer = 0;
        _anim_frame_index++;
        _cycle_counter++;

        _y--;

        if (_anim_frame_index == 2)
            _anim_frame_index = 0;

        if (_cycle_counter > smooch_cycles_to_hide) {
            _ready_to_dispose = true;
            sprite_utils::set_visibility(false, _main_sprite_info, _sub_sprite_info);
        }

        match_animation();
    }

}

Smooch::Smooch(int x, int y) :
        BaseDecoration(x, y, smooch_sprite_width, smooch_sprite_height, smooch_spritesheet_type) {
    init_sprites();
}

void Smooch::init_sprites() {

    delete_sprites();

    _main_sprite_info = global::main_oam_manager->initSprite(gfx_caveman_damselPal, gfx_caveman_damselPalLen, nullptr,
                                                             _sprite_size, ObjSize::OBJSIZE_16, _spritesheet_type, true,
                                                             false, LAYER_LEVEL::MIDDLE_TOP);

    _sub_sprite_info = global::sub_oam_manager->initSprite(gfx_caveman_damselPal, gfx_caveman_damselPalLen, nullptr,
                                                           _sprite_size, ObjSize::OBJSIZE_16, _spritesheet_type, true,
                                                           false, LAYER_LEVEL::MIDDLE_TOP);

    sprite_utils::set_horizontal_flip(false, _main_sprite_info, _sub_sprite_info);
    sprite_utils::set_vertical_flip(false, _main_sprite_info, _sub_sprite_info);
    sprite_utils::set_visibility(true, _main_sprite_info, _sub_sprite_info);

    update_sprites_position();
    match_animation();
}

void Smooch::match_animation() {
    _frame_gfx = sprite_utils::get_frame((u8 *) gfx_caveman_damselTiles, _sprite_size, _anim_frame_index + 67);
    sprite_utils::update_frame(_frame_gfx, _sprite_size, _main_sprite_info, _sub_sprite_info);
}

void Smooch::delete_sprites() {
    delete _main_sprite_info;
    delete _sub_sprite_info;
    _main_sprite_info = nullptr;
    _sub_sprite_info = nullptr;
}

void Smooch::update_sprites_position() {
    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);
    sprite_utils::set_entry_xy(_main_sprite_info, static_cast<u16>(main_x), static_cast<u16>(main_y));
    sprite_utils::set_entry_xy(_sub_sprite_info, static_cast<u16>(sub_x), static_cast<u16>(sub_y));
}

void Smooch::introduce_yourself() const {
    printf("SMOOCH\n");
}
