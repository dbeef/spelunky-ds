//
// Created by dbeef on 2/17/19.
//

#include "Boulder.h"
#include "../../memory/SpriteUtils.hpp"
#include "../../../build/gfx_got_collectible.h"
#include "../../GlobalsDeclarations.hpp"

void Boulder::init_sprites() {

    delete_sprites();

    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_got_collectiblePal, gfx_got_collectiblePalLen,
                                                        nullptr, _sprite_size, ObjSize::OBJSIZE_32, _spritesheet_type,
                                                        true, false, LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_got_collectiblePal, gfx_got_collectiblePalLen,
                                                          nullptr, _sprite_size, ObjSize::OBJSIZE_32, _spritesheet_type,
                                                          true, false, LAYER_LEVEL::MIDDLE_TOP);

    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_visibility(true, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(true, mainSpriteInfo, subSpriteInfo);
    update_sprites_position();
    match_animation();
    
}

void Boulder::match_animation() {
    // Match frame basing on frame counter
    frameGfx = sprite_utils::get_frame((u8 *) gfx_got_collectibleTiles, _sprite_size, animFrame + 6);
    sprite_utils::update_frame(frameGfx, _sprite_size, mainSpriteInfo, subSpriteInfo);
}

void Boulder::delete_sprites() {
    delete mainSpriteInfo;
    delete subSpriteInfo;
    mainSpriteInfo = nullptr;
    subSpriteInfo = nullptr;
}

void Boulder::update_sprites_position() {
    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);
    sprite_utils::set_entry_xy(mainSpriteInfo, main_x, main_y);
    sprite_utils::set_entry_xy(subSpriteInfo, sub_x, sub_y);
}

void Boulder::apply_dmg(int dmg_to_apply) {
    // nothing can't kill boulder, but only a bomb
}

void Boulder::update_creature_specific() {
    if (_ready_to_dispose) return;

    update_sprites_position();
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_visibility(true, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(true, mainSpriteInfo, subSpriteInfo);
}
