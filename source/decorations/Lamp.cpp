//
// Created by xdbeef on 14.06.18.
//

#include "../../build/gfx_shopkeeper.h"
#include "../GlobalsDeclarations.hpp"
#include "../creatures/SpriteUtils.hpp"
#include "Lamp.h"

Lamp::Lamp(int x, int y) :
        BaseDecoration(x, y, lamp_sprite_width, lamp_sprite_height, lamp_spritesheet_type) {
    init_sprites();
}

void Lamp::update_decoration_specific() {

    _anim_frame_timer += *global::timer;

    if (_anim_frame_timer > lamp_anim_frame_delta) {

        _anim_frame_timer = 0;
        _anim_frame_index++;

        if (_anim_frame_index >= 3)
            _anim_frame_index = 0;

        match_animation();
    }

}

void Lamp::init_sprites() {

    delete_sprites();

    _main_sprite_info = global::main_oam_manager->initSprite(gfx_shopkeeperPal, gfx_shopkeeperPalLen, nullptr,
                                                             _sprite_size, ObjSize::OBJSIZE_16, _spritesheet_type, true,
                                                             false, LAYER_LEVEL::MIDDLE_TOP);

    _sub_sprite_info = global::sub_oam_manager->initSprite(gfx_shopkeeperPal, gfx_shopkeeperPalLen, nullptr,
                                                           _sprite_size, ObjSize::OBJSIZE_16, _spritesheet_type, true,
                                                           false, LAYER_LEVEL::MIDDLE_TOP);

    sprite_utils::set_horizontal_flip(false, _main_sprite_info, _sub_sprite_info);
    sprite_utils::set_vertical_flip(false, _main_sprite_info, _sub_sprite_info);
    sprite_utils::set_visibility(true, _main_sprite_info, _sub_sprite_info);

    update_sprites_position();
    match_animation();
}

void Lamp::match_animation() {
    _frame_gfx = sprite_utils::get_frame((u8 *) gfx_shopkeeperTiles, _sprite_size, 25 + _anim_frame_index);
    sprite_utils::update_frame(_frame_gfx, _sprite_size, _main_sprite_info, _sub_sprite_info);
}

void Lamp::delete_sprites() {
    delete _main_sprite_info;
    delete _sub_sprite_info;
    _main_sprite_info = nullptr;
    _sub_sprite_info = nullptr;
}

void Lamp::update_sprites_position() {
    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);
    sprite_utils::set_entry_xy(_main_sprite_info, static_cast<u16>(main_x), static_cast<u16>(main_y));
    sprite_utils::set_entry_xy(_sub_sprite_info, static_cast<u16>(sub_x), static_cast<u16>(sub_y));
}

void Lamp::introduce_yourself() const {
    printf("LAMP\n");
}
