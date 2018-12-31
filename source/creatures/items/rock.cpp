//
// Created by xdbeef on 04.04.18.
//

#include "rock.hpp"
#include "../../globals_declarations.hpp"
#include "../main_dude/main_dude.hpp"
#include "../../../build/gfx_blood_rock_rope_poof.h"
#include "../../collisions/collisions.hpp"
#include "../../tiles/level_rendering_utils.hpp"
#include "../sprite_utils.hpp"

void Rock::update_creature_specific() {

    if (_ready_to_dispose) return;

    check_if_can_be_pickuped();
    set_pickuped_position(0, 10, 6);
    kill_mobs_if_thrown(1);
    update_sprites_position();
}

void Rock::init_sprites() {

    delete_sprites();

    _sub_sprite_info = global::sub_oam_manager->initSprite(gfx_blood_rock_rope_poofPal, gfx_blood_rock_rope_poofPalLen,
                                                        nullptr, _sprite_size, 8, BLOOD_ROCK_ROPE_POOF, true, false,
                                                        LAYER_LEVEL::MIDDLE_TOP);
    _main_sprite_info = global::main_oam_manager->initSprite(gfx_blood_rock_rope_poofPal, gfx_blood_rock_rope_poofPalLen,
                                                          nullptr, _sprite_size, 8, BLOOD_ROCK_ROPE_POOF, true,
                                                          false, LAYER_LEVEL::MIDDLE_TOP);

    _frame_gfx = sprite_utils::get_frame((u8 *) gfx_blood_rock_rope_poofTiles, _sprite_size, 7);
    sprite_utils::update_frame(_frame_gfx, _sprite_size, _main_sprite_info, _sub_sprite_info);

    update_sprites_position();
    sprite_utils::set_priority(OBJPRIORITY_0, _main_sprite_info, _sub_sprite_info);
    sprite_utils::set_horizontal_flip(false, _main_sprite_info, _sub_sprite_info);
    sprite_utils::set_vertical_flip(false, _main_sprite_info, _sub_sprite_info);
    sprite_utils::set_visibility(true, _main_sprite_info, _sub_sprite_info);
}

void Rock::update_sprites_position() {
    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);
    sprite_utils::set_entry_xy(_main_sprite_info, static_cast<u16>(main_x), static_cast<u16>(main_y));
    sprite_utils::set_entry_xy(_sub_sprite_info, static_cast<u16>(sub_x), static_cast<u16>(sub_y));
}

void Rock::delete_sprites() {
    delete _main_sprite_info;
    delete _sub_sprite_info;
    _main_sprite_info = nullptr;
    _sub_sprite_info = nullptr;
}
