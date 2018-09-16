//
// Created by xdbeef on 29.04.18.
//

#include "spelunky_title.h"
#include "../../globals_declarations.hpp"
#include "../../../build/gfx_spelunky_title.h"
#include "../sprite_utils.hpp"

void SpelunkyTitle::draw() {

    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);

    sprite_utils::set_entry_xy(mainSpriteInfo_1, main_x, main_y);
    sprite_utils::set_entry_xy(mainSpriteInfo_2, main_x + SPELUNKY_TITLE_SPRITE_WIDTH, main_y);
    sprite_utils::set_entry_xy(mainSpriteInfo_3, main_x + SPELUNKY_TITLE_SPRITE_WIDTH * 2, main_y);

    sprite_utils::set_entry_xy(subSpriteInfo_1, sub_x, sub_y);
    sprite_utils::set_entry_xy(subSpriteInfo_2, sub_x + SPELUNKY_TITLE_SPRITE_WIDTH, sub_y);
    sprite_utils::set_entry_xy(subSpriteInfo_3, sub_x + SPELUNKY_TITLE_SPRITE_WIDTH * 2, sub_y);

}

void SpelunkyTitle::init() {
    initSprite();
}

void SpelunkyTitle::initSprite() {
    
    delete mainSpriteInfo_1;
    delete mainSpriteInfo_2;
    delete mainSpriteInfo_3;
    
    delete subSpriteInfo_1;
    delete subSpriteInfo_2;
    delete subSpriteInfo_3;
    
    if (oamType == OamType::MAIN) {
        mainSpriteInfo_1 = global::main_oam_manager->initSprite(gfx_spelunky_titlePal, gfx_spelunky_titlePalLen,
                                                                nullptr, SPELUNKY_TITLE_SPRITE_SIZE, 64,
                                                                SpritesheetType::SPELUNKY_TITLE, true,
                                                                false, LAYER_LEVEL::MIDDLE_TOP);
        mainSpriteInfo_2 = global::main_oam_manager->initSprite(gfx_spelunky_titlePal, gfx_spelunky_titlePalLen,
                                                                nullptr, SPELUNKY_TITLE_SPRITE_SIZE, 64,
                                                                SpritesheetType::SPELUNKY_TITLE, true,
                                                                false, LAYER_LEVEL::MIDDLE_TOP);
        mainSpriteInfo_3 = global::main_oam_manager->initSprite(gfx_spelunky_titlePal, gfx_spelunky_titlePalLen,
                                                                nullptr, SPELUNKY_TITLE_SPRITE_SIZE, 64,
                                                                SpritesheetType::SPELUNKY_TITLE, true,
                                                                false, LAYER_LEVEL::MIDDLE_TOP);


        sprite_utils::set_visibility(true, mainSpriteInfo_1, mainSpriteInfo_2, mainSpriteInfo_3);
        sprite_utils::set_horizontal_flip(false, mainSpriteInfo_1, mainSpriteInfo_2, mainSpriteInfo_3);
        sprite_utils::set_vertical_flip(false, mainSpriteInfo_1, mainSpriteInfo_2, mainSpriteInfo_3);
        
        u8 *frameGfx1 = sprite_utils::get_frame((u8 *) gfx_spelunky_titleTiles, SPELUNKY_TITLE_SPRITE_SIZE, 0);
        u8 *frameGfx2 = sprite_utils::get_frame((u8 *) gfx_spelunky_titleTiles, SPELUNKY_TITLE_SPRITE_SIZE, 1);
        u8 *frameGfx3 = sprite_utils::get_frame((u8 *) gfx_spelunky_titleTiles, SPELUNKY_TITLE_SPRITE_SIZE, 2);

        sprite_utils::update_frame(frameGfx1, SPELUNKY_TITLE_SPRITE_SIZE, mainSpriteInfo_1);
        sprite_utils::update_frame(frameGfx2, SPELUNKY_TITLE_SPRITE_SIZE, mainSpriteInfo_2);
        sprite_utils::update_frame(frameGfx3, SPELUNKY_TITLE_SPRITE_SIZE, mainSpriteInfo_3);
    }

    if (oamType == OamType::SUB) {
        subSpriteInfo_1 = global::sub_oam_manager->initSprite(gfx_spelunky_titlePal, gfx_spelunky_titlePalLen,
                                                              nullptr, SPELUNKY_TITLE_SPRITE_SIZE, 64,
                                                              SpritesheetType::SPELUNKY_TITLE, true,
                                                              false, LAYER_LEVEL::MIDDLE_TOP);
        subSpriteInfo_2 = global::sub_oam_manager->initSprite(gfx_spelunky_titlePal, gfx_spelunky_titlePalLen,
                                                              nullptr, SPELUNKY_TITLE_SPRITE_SIZE, 64,
                                                              SpritesheetType::SPELUNKY_TITLE, true,
                                                              false, LAYER_LEVEL::MIDDLE_TOP);
        subSpriteInfo_3 = global::sub_oam_manager->initSprite(gfx_spelunky_titlePal, gfx_spelunky_titlePalLen,
                                                              nullptr, SPELUNKY_TITLE_SPRITE_SIZE, 64,
                                                              SpritesheetType::SPELUNKY_TITLE, true,
                                                              false, LAYER_LEVEL::MIDDLE_TOP);

        sprite_utils::set_visibility(true, subSpriteInfo_1, subSpriteInfo_2, subSpriteInfo_3);
        sprite_utils::set_horizontal_flip(false, subSpriteInfo_1, subSpriteInfo_2, subSpriteInfo_3);
        sprite_utils::set_vertical_flip(false, subSpriteInfo_1, subSpriteInfo_2, subSpriteInfo_3);

        u8 *frameGfx1 = sprite_utils::get_frame((u8 *) gfx_spelunky_titleTiles, SPELUNKY_TITLE_SPRITE_SIZE, 0);
        u8 *frameGfx2 = sprite_utils::get_frame((u8 *) gfx_spelunky_titleTiles, SPELUNKY_TITLE_SPRITE_SIZE, 1);
        u8 *frameGfx3 = sprite_utils::get_frame((u8 *) gfx_spelunky_titleTiles, SPELUNKY_TITLE_SPRITE_SIZE, 2);

        sprite_utils::update_frame(frameGfx1, SPELUNKY_TITLE_SPRITE_SIZE, subSpriteInfo_1);
        sprite_utils::update_frame(frameGfx2, SPELUNKY_TITLE_SPRITE_SIZE, subSpriteInfo_2);
        sprite_utils::update_frame(frameGfx3, SPELUNKY_TITLE_SPRITE_SIZE, subSpriteInfo_3);
    }
    
}


SpelunkyTitle::SpelunkyTitle() {
    physical_height = SPELUNKY_TITLE_PHYSICAL_HEIGHT;
    physical_width = SPELUNKY_TITLE_PHYSICAL_WIDTH;
    sprite_height = SPELUNKY_TITLE_SPRITE_HEIGHT;
    sprite_width = SPELUNKY_TITLE_SPRITE_WIDTH;
}

