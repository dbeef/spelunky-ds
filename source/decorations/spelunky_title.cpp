//
// Created by xdbeef on 29.04.18.
//

#include "spelunky_title.h"
#include "../globals_declarations.hpp"
#include "../../build/gfx_spelunky_title.h"
#include "../creatures/sprite_utils.hpp"

SpelunkyTitle::SpelunkyTitle(int x, int y) :
        BaseDecoration(x, y, spelunky_title_sprite_width, spelunky_title_sprite_height,
                       spelunky_title_spritesheet_type) {
    // Cleaning allocated memory from trash data.
    for (auto &mainSpriteInfo : _mainSpriteInfos) mainSpriteInfo = nullptr;
    init_sprites();
}

void SpelunkyTitle::delete_sprites() {
    for (auto &mainSpriteInfo : _mainSpriteInfos) {
        delete mainSpriteInfo;
        mainSpriteInfo = nullptr;
    }
}

void SpelunkyTitle::init_sprites() {

    delete_sprites();

    for (int a = 0; a < spelunky_title_graphics; a++) {
        _mainSpriteInfos[a] = global::main_oam_manager->initSprite(gfx_spelunky_titlePal, gfx_spelunky_titlePalLen,
                                                                   nullptr, _sprite_size, 64, _spritesheet_type,
                                                                   true, false, LAYER_LEVEL::MIDDLE_TOP);

        u8 *frame_gfx = sprite_utils::get_frame((u8 *) gfx_spelunky_titleTiles, _sprite_size, a);
        sprite_utils::update_frame(frame_gfx, _sprite_size, _mainSpriteInfos[a]);
    }

    for (auto &sprite_info : _mainSpriteInfos) {
        sprite_utils::set_horizontal_flip(false, sprite_info);
        sprite_utils::set_vertical_flip(false, sprite_info);
        sprite_utils::set_visibility(true, sprite_info);
    }

    update_sprites_position();
}

void SpelunkyTitle::update_sprites_position() {

    int dummy, main_x, main_y;
    get_x_y_viewported(&main_x, &main_y, &dummy, &dummy);

    for (int a = 0; a < spelunky_title_graphics; a++)
        sprite_utils::set_entry_xy(_mainSpriteInfos[a],
                                   static_cast<u16>(main_x + (_sprite_width * a)), static_cast<u16>(main_y));
}
