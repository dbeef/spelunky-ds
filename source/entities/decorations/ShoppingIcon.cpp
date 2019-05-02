//
// Created by xdbeef on 17.06.18.
//

#include <cstdlib>

#include "../../../build/gfx_goldbars.h"
#include "../../GameState.hpp"
#include "../../memory/SpriteUtils.hpp"
#include "ShoppingIcon.h"
#include "../../time/Timer.h"

ShoppingIcon::ShoppingIcon(int x, int y) :
        BaseDecoration(x, y, shopping_icon_sprite_width, shopping_icon_sprite_height, shopping_icon_spritesheet_type) {
    init_sprites();
}

void ShoppingIcon::update_decoration_specific() {

    if (_ready_to_dispose) return;

    update_sprites_position();

    _trigger_timer += Timer::getDeltaTime();

    // Launch animation every time _trigger_timer overflows
    if (_trigger_timer > shopping_icon_trigger_delta)
        _anim_frame_timer += Timer::getDeltaTime();

    if (_anim_frame_timer > shopping_icon_anim_frame_delta) {
        _anim_frame_timer = 0;
        _anim_frame_index++;

        if (_anim_frame_index >= 20) {
            _anim_frame_index = 0;
            _trigger_timer = 0;
        }

        match_animation();
    }

}

void ShoppingIcon::init_sprites() {

    delete_sprites();

    _main_sprite_info = GameState::instance().main_oam_manager->initSprite(gfx_goldbarsPal, gfx_goldbarsPalLen, nullptr,
                                                             _sprite_size, ObjSize::OBJSIZE_16, SpritesheetType::MONIEZ_GOLDBARS, true,
                                                             false, LAYER_LEVEL::MIDDLE_TOP);

    _sub_sprite_info = GameState::instance().sub_oam_manager->initSprite(gfx_goldbarsPal, gfx_goldbarsPalLen, nullptr,
                                                           _sprite_size, ObjSize::OBJSIZE_16, SpritesheetType::MONIEZ_GOLDBARS, true,
                                                           false, LAYER_LEVEL::MIDDLE_TOP);
    match_animation();
    sprite_utils::set_visibility(true, _main_sprite_info, _sub_sprite_info);
    sprite_utils::set_horizontal_flip(false, _main_sprite_info, _sub_sprite_info);
    sprite_utils::set_vertical_flip(false, _main_sprite_info, _sub_sprite_info);
    update_sprites_position();
}

void ShoppingIcon::update_sprites_position() {
    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);
    sprite_utils::set_entry_xy(_main_sprite_info, static_cast<u16>(main_x), static_cast<u16>(main_y));
    sprite_utils::set_entry_xy(_sub_sprite_info, static_cast<u16>(sub_x), static_cast<u16>(sub_y));
}

void ShoppingIcon::match_animation() {
    u8 *frameGfx = sprite_utils::get_frame((u8 *) gfx_goldbarsTiles, _sprite_size, 12 + _anim_frame_index);
    sprite_utils::update_frame(frameGfx, _sprite_size, _main_sprite_info, _sub_sprite_info);
}

void ShoppingIcon::set_ready_to_dispose() {
    sprite_utils::set_visibility(false, _main_sprite_info, _sub_sprite_info);
    _ready_to_dispose = true;
}

void ShoppingIcon::delete_sprites() {
    delete _main_sprite_info;
    delete _sub_sprite_info;
    _main_sprite_info = nullptr;
    _sub_sprite_info = nullptr;
}

void ShoppingIcon::print_classname() const {
    printf("SHOPPING ICON\n");
}
