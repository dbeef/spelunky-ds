//
// Created by xdbeef on 28.12.18.
//

#include "ArchSign.h"

#include "../../GameState.hpp"
#include "../../../build/gfx_menu_signs_start_scores.h"
#include "../../graphics/SpriteUtils.hpp"


ArchSign::ArchSign(int x, int y, ArchSign::Type type) :
        BaseDecoration(x, y, rock_sign_sprite_width, rock_sign_sprite_height,
                       rock_sign_spritesheet_type), _type(type) {
    // Cleaning allocated graphics from trash data.
    for (auto &mainSpriteInfo : _mainSpriteInfos) mainSpriteInfo = nullptr;
    init_sprites();
}

void ArchSign::delete_sprites() {
    for (auto &mainSpriteInfo : _mainSpriteInfos) {
        delete mainSpriteInfo;
        mainSpriteInfo = nullptr;
    }
}

void ArchSign::init_sprites() {

    delete_sprites();

    for (int a = 0; a < rock_sign_graphics; a++) {
        _mainSpriteInfos[a] = GameState::instance().main_oam_manager->initSprite(gfx_menu_signs_start_scoresPal,
                                                                   gfx_menu_signs_start_scoresPalLen,
                                                                   nullptr, _sprite_size, ObjSize::OBJSIZE_32, _spritesheet_type,
                                                                   true, false, LAYER_LEVEL::MIDDLE_TOP);

        //will start two 32x32 chunks further if type == 1 (SCORES)
        int offset = static_cast<u8>(_type) * 2;

        u8 *frame_gfx = sprite_utils::get_frame((u8 *) gfx_menu_signs_start_scoresTiles, _sprite_size, a + offset);
        sprite_utils::update_frame(frame_gfx, _sprite_size, _mainSpriteInfos[a]);
    }

    for (auto &sprite_info : _mainSpriteInfos) {
        sprite_utils::set_horizontal_flip(false, sprite_info);
        sprite_utils::set_vertical_flip(false, sprite_info);
        sprite_utils::set_visibility(true, sprite_info);
    }

    update_sprites_position();
}

void ArchSign::update_sprites_position() {

    int dummy, main_x, main_y;
    get_x_y_viewported(&main_x, &main_y, &dummy, &dummy);

    for (int a = 0; a < rock_sign_graphics; a++)
        sprite_utils::set_entry_xy(_mainSpriteInfos[a],
                                   static_cast<u16>(main_x + (_sprite_width * a)), static_cast<u16>(main_y));
}

void ArchSign::print_classname() const {
    printf("ARCH SIGN\n");
}

void ArchSign::update_decoration_specific() {
    // Does nothing besides displaying sprites.
}


