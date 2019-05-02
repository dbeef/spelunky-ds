//
// Created by xdbeef on 04.04.18.
//

#include "../items/Rope.hpp"
#include "RopeElement.hpp"
#include "../../GameState.hpp"
#include "../../../build/gfx_blood_rock_rope_poof.h"
#include "../../memory/SpriteUtils.hpp"
#include "RopeElement.hpp"
#include "../../time/Timer.h"

void RopeElement::update_decoration_specific() {

    if (_ready_to_dispose) return;

    if (_active) {
        if (_active_timer < 50)
            _active_timer += Timer::getDeltaTime();
        else {
            update_visibility();
        }
    }

    update_sprites_position();

}

void RopeElement::init_sprites() {

    delete_sprites();

    _sub_sprite_info = GameState::instance().sub_oam_manager->initSprite(gfx_blood_rock_rope_poofPal, gfx_blood_rock_rope_poofPalLen,
                                                           nullptr, _sprite_size, ObjSize::OBJSIZE_8, SpritesheetType::BLOOD_ROCK_ROPE_POOF, true,
                                                           false, LAYER_LEVEL::MIDDLE_TOP);
    _main_sprite_info = GameState::instance().main_oam_manager->initSprite(gfx_blood_rock_rope_poofPal,
                                                             gfx_blood_rock_rope_poofPalLen,
                                                             nullptr, _sprite_size, ObjSize::OBJSIZE_8, SpritesheetType::BLOOD_ROCK_ROPE_POOF,
                                                             true, false, LAYER_LEVEL::MIDDLE_TOP);
    _sub_sprite_info_2 = GameState::instance().sub_oam_manager->initSprite(gfx_blood_rock_rope_poofPal,
                                                             gfx_blood_rock_rope_poofPalLen,
                                                             nullptr, _sprite_size, ObjSize::OBJSIZE_8, SpritesheetType::BLOOD_ROCK_ROPE_POOF,
                                                             true, false, LAYER_LEVEL::MIDDLE_TOP);
    _main_sprite_info_2 = GameState::instance().main_oam_manager->initSprite(gfx_blood_rock_rope_poofPal,
                                                               gfx_blood_rock_rope_poofPalLen,
                                                               nullptr, _sprite_size, ObjSize::OBJSIZE_8, SpritesheetType::BLOOD_ROCK_ROPE_POOF,
                                                               true, false, LAYER_LEVEL::MIDDLE_TOP);

    u8 *frame_gfx = sprite_utils::get_frame((u8 *) gfx_blood_rock_rope_poofTiles, _sprite_size, 8);
    sprite_utils::update_frame(frame_gfx, _sprite_size, _main_sprite_info,
                               _main_sprite_info_2, _sub_sprite_info, _sub_sprite_info_2);

    sprite_utils::set_horizontal_flip(false, _main_sprite_info, _main_sprite_info_2, _sub_sprite_info,
                                      _sub_sprite_info_2);
    sprite_utils::set_vertical_flip(false, _main_sprite_info, _main_sprite_info_2, _sub_sprite_info,
                                    _sub_sprite_info_2);

    update_visibility();

}

void RopeElement::update_sprites_position() {
    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);
    sprite_utils::set_entry_xy(_main_sprite_info, static_cast<u16>(main_x), static_cast<u16>(main_y));
    sprite_utils::set_entry_xy(_sub_sprite_info, static_cast<u16>(sub_x), static_cast<u16>(sub_y));
    sprite_utils::set_entry_xy(_main_sprite_info_2, static_cast<u16>(main_x), static_cast<u16>(main_y + _sprite_height));
    sprite_utils::set_entry_xy(_sub_sprite_info_2, static_cast<u16>(sub_x), static_cast<u16>(sub_y + _sprite_height));
}

void RopeElement::delete_sprites() {
    delete _main_sprite_info;
    delete _sub_sprite_info;
    _main_sprite_info = nullptr;
    _sub_sprite_info = nullptr;

    delete _main_sprite_info_2;
    delete _sub_sprite_info_2;
    _main_sprite_info_2 = nullptr;
    _sub_sprite_info_2 = nullptr;
}

void RopeElement::update_visibility() {
    if (_active_timer > 20)
        sprite_utils::set_visibility(true, _main_sprite_info_2, _sub_sprite_info_2);
    sprite_utils::set_visibility(true, _main_sprite_info, _sub_sprite_info);
}

void RopeElement::print_classname() const {
    printf("ROPE ELEMENT\n");
}
