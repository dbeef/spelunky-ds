//
// Created by xdbeef on 16.09.18.
//

#include <cstdlib>

#include "../../../build/gfx_rubies.h"
#include "../../GlobalsDeclarations.hpp"
#include "../SpriteUtils.hpp"
#include "RubyBig.h"

void RubyBig::init_sprites() {

    delete_sprites();

    _main_sprite_info = global::main_oam_manager->initSprite(gfx_rubiesPal, gfx_rubiesPalLen,
                                                            nullptr, _sprite_size, ObjSize::OBJSIZE_8,
                                                            _spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
    _sub_sprite_info = global::sub_oam_manager->initSprite(gfx_rubiesPal, gfx_rubiesPalLen, nullptr,
                                                            _sprite_size, ObjSize::OBJSIZE_8, _spritesheet_type, true,
                                                            false, LAYER_LEVEL::MIDDLE_TOP);

    u8 offset;

    switch (_ruby_color) {
        case RubyColor::RED:
            offset = 0;
            break;
        case RubyColor::GREEN:
            offset = 1;
            break;
        case RubyColor::BLUE:
            offset = 2;
            break;
        default:
            offset = 0;
            break;
    }

    u8 *frame_gfx = sprite_utils::get_frame((u8 *) gfx_rubiesTiles, _sprite_size, offset);

    sprite_utils::update_frame(frame_gfx, _sprite_size, _main_sprite_info, _sub_sprite_info);

    update_sprites_position();
    sprite_utils::set_vertical_flip(false, _main_sprite_info, _sub_sprite_info);
    sprite_utils::set_horizontal_flip(false, _main_sprite_info, _sub_sprite_info);
    sprite_utils::set_priority(OBJPRIORITY_0, _main_sprite_info, _sub_sprite_info);
}

RubyBig::RubyBig(int x, int y) : BaseTreasure(
        x,
        y,
        ruby_big_sprite_width,
        ruby_big_sprite_height,
        ruby_big_spritesheet_type,
        ruby_big_physical_width,
        ruby_big_physical_height,
        TreasureType::RUBY_BIG
) {
    _ruby_color = static_cast<RubyColor>(rand() % 3);
    init_sprites();
}
