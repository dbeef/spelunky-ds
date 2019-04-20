//
// Created by dbeef on 19.01.19.
//

#include <cstdio>

#include "Explosion.h"
#include "../../memory/SpriteUtils.hpp"
#include "../../GlobalsDeclarations.hpp"
#include "../../../build/gfx_explosion.h"

Explosion::Explosion(int x, int y) :
        BaseDecoration(x, y,
                       explosion_sprite_width,
                       explosion_sprite_height,
                       explosion_spritesheet_type) {
    init_sprites();
}

void Explosion::update_decoration_specific() {

    _explosion_timer += *global::timer;
    if (_explosion_timer > 50 && _current_frame_index < 10) {

        _explosion_timer = 0;
        _current_frame_index++;

        if (_current_frame_index >= 10) {
            _ready_to_dispose = true;
            sprite_utils::set_visibility(false, _main_sprite_info, _sub_sprite_info);
        } else {
            u8 *frame_gfx = sprite_utils::get_frame((u8 *) gfx_explosionTiles, _sprite_size, _current_frame_index);
            sprite_utils::update_frame(frame_gfx, _sprite_size, _main_sprite_info, _sub_sprite_info);
        }
    }
    
}

void Explosion::print_classname() const {
    printf("EXPLOSION\n");
}

void Explosion::init_sprites() {

    delete_sprites();

    _sub_sprite_info = global::sub_oam_manager->initSprite(gfx_explosionPal, gfx_explosionPalLen,
                                                        nullptr, _sprite_size, ObjSize::OBJSIZE_64, BOMB, true, false,
                                                        LAYER_LEVEL::MIDDLE_TOP);
    _main_sprite_info = global::main_oam_manager->initSprite(gfx_explosionPal, gfx_explosionPalLen,
                                                          nullptr, _sprite_size, ObjSize::OBJSIZE_64, BOMB, true, false,
                                                          LAYER_LEVEL::MIDDLE_TOP);

    update_sprites_position();

    sprite_utils::set_vertical_flip(false, _main_sprite_info, _sub_sprite_info);
    sprite_utils::set_horizontal_flip(false, _main_sprite_info, _sub_sprite_info);
    sprite_utils::set_priority(OBJPRIORITY_0, _main_sprite_info, _sub_sprite_info);
    sprite_utils::set_visibility(true, _main_sprite_info, _sub_sprite_info);
}

void Explosion::delete_sprites() {
    delete _main_sprite_info;
    delete _sub_sprite_info;
    _main_sprite_info = nullptr;
    _sub_sprite_info = nullptr;
}

void Explosion::update_sprites_position() {
    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);
    sprite_utils::set_entry_xy(_main_sprite_info, static_cast<u16>(main_x), static_cast<u16>(main_y));
    sprite_utils::set_entry_xy(_sub_sprite_info, static_cast<u16>(sub_x), static_cast<u16>(sub_y));
}
