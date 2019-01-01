//
// Created by xdbeef on 16.09.18.
//

#include "../../../build/gfx_goldbars.h"
#include "../../GlobalsDeclarations.hpp"
#include "../SpriteUtils.hpp"
#include "TripleGoldbar.h"

void TripleGoldbar::init_sprites() {

    delete_sprites();

    u8 *frame_gfx = (u8 *) gfx_goldbarsTiles;

    _sub_sprite_info = global::sub_oam_manager->initSprite(gfx_goldbarsPal, gfx_goldbarsPalLen,
                                                        nullptr, _sprite_size, 16,
                                                        _spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
    _main_sprite_info = global::main_oam_manager->initSprite(gfx_goldbarsPal, gfx_goldbarsPalLen,
                                                          nullptr, _sprite_size, 16,
                                                          _spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);

    sprite_utils::update_frame(frame_gfx, _sprite_size, _main_sprite_info, _sub_sprite_info);

    update_sprites_position();
    sprite_utils::set_vertical_flip(false, _main_sprite_info, _sub_sprite_info);
    sprite_utils::set_horizontal_flip(false, _main_sprite_info, _sub_sprite_info);
    sprite_utils::set_priority(OBJPRIORITY_0, _main_sprite_info, _sub_sprite_info);

}

