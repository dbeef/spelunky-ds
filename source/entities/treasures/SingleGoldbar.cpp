//
// Created by xdbeef on 16.09.18.
//

#include "../../../build/gfx_goldbars.h"

#include "SingleGoldbar.hpp"
#include "../../GameState.hpp"
#include "../../graphics/SpriteUtils.hpp"

void SingleGoldbar::init_sprites() {

    delete_sprites();

    u8 *frame_gfx = sprite_utils::get_frame((u8 *) gfx_goldbarsTiles, _sprite_size, 1);

    _sub_sprite_info = GameState::instance().sub_oam_manager->initSprite(gfx_goldbarsPal, gfx_goldbarsPalLen,
                                                        nullptr, _sprite_size, ObjSize::OBJSIZE_16,
                                                        _spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
    _main_sprite_info = GameState::instance().main_oam_manager->initSprite(gfx_goldbarsPal, gfx_goldbarsPalLen,
                                                          nullptr, _sprite_size, ObjSize::OBJSIZE_16,
                                                          _spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);

    sprite_utils::update_frame(frame_gfx, _sprite_size, _main_sprite_info, _sub_sprite_info);

    update_sprites_position();
    sprite_utils::set_vertical_flip(false, _main_sprite_info, _sub_sprite_info);
    sprite_utils::set_horizontal_flip(false, _main_sprite_info, _sub_sprite_info);
    sprite_utils::set_visibility(true, _main_sprite_info, _sub_sprite_info);
    sprite_utils::set_priority(OBJPRIORITY_0, _main_sprite_info, _sub_sprite_info);
}

SingleGoldbar::SingleGoldbar(int x, int y) : BaseTreasure(
        x,
        y,
        single_goldbar_sprite_width,
        single_goldbar_sprite_height,
        single_goldbar_spritesheet_type,
        single_goldbar_physical_width,
        single_goldbar_physical_height,
        TreasureType::SINGLE_GOLDBAR
) {
    init_sprites();
}

