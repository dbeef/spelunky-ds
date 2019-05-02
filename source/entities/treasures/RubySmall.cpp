//
// Created by xdbeef on 16.09.18.
//

#include <cstdlib>

#include "../../../build/gfx_rubies.h"

#include "RubySmall.h"
#include "../../GameState.hpp"
#include "../../graphics/SpriteUtils.hpp"

void RubySmall::init_sprites() {

    delete_sprites();

    _sub_sprite_info = GameState::instance().sub_oam_manager->initSprite(gfx_rubiesPal, gfx_rubiesPalLen,
                                                           nullptr, _sprite_size, ObjSize::OBJSIZE_8,
                                                           _spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
    _main_sprite_info = GameState::instance().main_oam_manager->initSprite(gfx_rubiesPal, gfx_rubiesPalLen, nullptr,
                                                             _sprite_size, ObjSize::OBJSIZE_8, _spritesheet_type, true,
                                                             false, LAYER_LEVEL::MIDDLE_TOP);

    u8 offset;

    switch (_ruby_color) {
        case RubyColor::RED:
            offset = 3;
            break;
        case RubyColor::GREEN:
            offset = 4;
            break;
        case RubyColor::BLUE:
            offset = 5;
            break;
        default:
            offset = 3;
            break;
    }

    u8 *frame_gfx = sprite_utils::get_frame((u8 *) gfx_rubiesTiles, _sprite_size, offset);

    sprite_utils::update_frame(frame_gfx, _sprite_size, _main_sprite_info, _sub_sprite_info);

    update_sprites_position();
    sprite_utils::set_vertical_flip(false, _main_sprite_info, _sub_sprite_info);
    sprite_utils::set_horizontal_flip(false, _main_sprite_info, _sub_sprite_info);
    sprite_utils::set_priority(OBJPRIORITY_0, _main_sprite_info, _sub_sprite_info);
}

RubySmall::RubySmall(int x, int y) : BaseTreasure(
        x,
        y,
        ruby_small_sprite_width,
        ruby_small_sprite_height,
        ruby_small_spritesheet_type,
        ruby_small_physical_width,
        ruby_small_physical_height,
        TreasureType::RUBY_SMALL
) {
    _ruby_color = static_cast<RubyColor>(rand() % 3);
    init_sprites();
}