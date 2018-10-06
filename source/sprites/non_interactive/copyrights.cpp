//
// Created by xdbeef on 29.04.18.
//

#include "spelunky_title.h"
#include "../../globals_declarations.hpp"
#include "../../../build/gfx_copyrights.h"
#include "copyrights.h"
#include "../sprite_utils.hpp"

void Copyrights::draw() {

    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);
    
    sprite_utils::set_entry_xy(mainSpriteInfo_1, main_x, main_y);
    sprite_utils::set_entry_xy(mainSpriteInfo_2, main_x + COPYRIGHTS_SPRITE_WIDTH, main_y);
    sprite_utils::set_entry_xy(mainSpriteInfo_3, main_x + COPYRIGHTS_SPRITE_WIDTH * 2, main_y);
    sprite_utils::set_entry_xy(mainSpriteInfo_4, main_x + COPYRIGHTS_SPRITE_WIDTH * 3, main_y);
    sprite_utils::set_entry_xy(mainSpriteInfo_5, main_x + COPYRIGHTS_SPRITE_WIDTH * 4, main_y);
    
    sprite_utils::set_entry_xy(subSpriteInfo_1, sub_x, sub_y);
    sprite_utils::set_entry_xy(subSpriteInfo_2, sub_x + COPYRIGHTS_SPRITE_WIDTH, sub_y);
    sprite_utils::set_entry_xy(subSpriteInfo_3, sub_x + COPYRIGHTS_SPRITE_WIDTH * 2, sub_y);
    sprite_utils::set_entry_xy(subSpriteInfo_4, sub_x + COPYRIGHTS_SPRITE_WIDTH * 3, sub_y);
    sprite_utils::set_entry_xy(subSpriteInfo_5, sub_x + COPYRIGHTS_SPRITE_WIDTH * 4, sub_y);
}


void Copyrights::init() {
    
    initSprite();

    sprite_utils::set_horizontal_flip(false, mainSpriteInfo_1, mainSpriteInfo_2, mainSpriteInfo_3, 
            mainSpriteInfo_4, mainSpriteInfo_5, subSpriteInfo_1, subSpriteInfo_2, subSpriteInfo_3, 
            subSpriteInfo_4, subSpriteInfo_5);
    
    sprite_utils::set_vertical_flip(false, mainSpriteInfo_1, mainSpriteInfo_2, mainSpriteInfo_3, 
            mainSpriteInfo_4, mainSpriteInfo_5, subSpriteInfo_1, subSpriteInfo_2, subSpriteInfo_3, 
            subSpriteInfo_4, subSpriteInfo_5);

}

void Copyrights::initSprite() {

    deleteSprite();

    if (oamType == OamType::MAIN) {
        mainSpriteInfo_1 = global::main_oam_manager->initSprite(gfx_copyrightsPal, gfx_copyrightsPalLen, nullptr,
                                                                COPYRIGHTS_SPRITE_SIZE, 32,
                                                                SpritesheetType::SPELUNKY_COPYRIGHTS, true,
                                                                false, LAYER_LEVEL::MIDDLE_TOP);
        mainSpriteInfo_2 = global::main_oam_manager->initSprite(gfx_copyrightsPal, gfx_copyrightsPalLen, nullptr,
                                                                COPYRIGHTS_SPRITE_SIZE, 32,
                                                                SpritesheetType::SPELUNKY_COPYRIGHTS, true,
                                                                false, LAYER_LEVEL::MIDDLE_TOP);
        mainSpriteInfo_3 = global::main_oam_manager->initSprite(gfx_copyrightsPal, gfx_copyrightsPalLen, nullptr,
                                                                COPYRIGHTS_SPRITE_SIZE, 32,
                                                                SpritesheetType::SPELUNKY_COPYRIGHTS, true,
                                                                false, LAYER_LEVEL::MIDDLE_TOP);
        mainSpriteInfo_4 = global::main_oam_manager->initSprite(gfx_copyrightsPal, gfx_copyrightsPalLen, nullptr,
                                                                COPYRIGHTS_SPRITE_SIZE, 32,
                                                                SpritesheetType::SPELUNKY_COPYRIGHTS, true,
                                                                false, LAYER_LEVEL::MIDDLE_TOP);
        mainSpriteInfo_4 = global::main_oam_manager->initSprite(gfx_copyrightsPal, gfx_copyrightsPalLen, nullptr,
                                                                COPYRIGHTS_SPRITE_SIZE, 32,
                                                                SpritesheetType::SPELUNKY_COPYRIGHTS, true,
                                                                false, LAYER_LEVEL::MIDDLE_TOP);
    }

    if (oamType == OamType::SUB) {
        subSpriteInfo_1 = global::sub_oam_manager->initSprite(gfx_copyrightsPal, gfx_copyrightsPalLen,
                                                              nullptr,
                                                              COPYRIGHTS_SPRITE_SIZE, 32,
                                                              SpritesheetType::SPELUNKY_COPYRIGHTS, true,
                                                              false, LAYER_LEVEL::MIDDLE_TOP);
        subSpriteInfo_2 = global::sub_oam_manager->initSprite(gfx_copyrightsPal, gfx_copyrightsPalLen,
                                                              nullptr,
                                                              COPYRIGHTS_SPRITE_SIZE, 32,
                                                              SpritesheetType::SPELUNKY_COPYRIGHTS, true,
                                                              false, LAYER_LEVEL::MIDDLE_TOP);
        subSpriteInfo_3 = global::sub_oam_manager->initSprite(gfx_copyrightsPal, gfx_copyrightsPalLen,
                                                              nullptr,
                                                              COPYRIGHTS_SPRITE_SIZE, 32,
                                                              SpritesheetType::SPELUNKY_COPYRIGHTS, true,
                                                              false, LAYER_LEVEL::MIDDLE_TOP);
        subSpriteInfo_4 = global::sub_oam_manager->initSprite(gfx_copyrightsPal, gfx_copyrightsPalLen,
                                                              nullptr,
                                                              COPYRIGHTS_SPRITE_SIZE, 32,
                                                              SpritesheetType::SPELUNKY_COPYRIGHTS, true,
                                                              false, LAYER_LEVEL::MIDDLE_TOP);
        subSpriteInfo_5 = global::sub_oam_manager->initSprite(gfx_copyrightsPal, gfx_copyrightsPalLen,
                                                              nullptr,
                                                              COPYRIGHTS_SPRITE_SIZE, 32,
                                                              SpritesheetType::SPELUNKY_COPYRIGHTS, true,
                                                              false, LAYER_LEVEL::MIDDLE_TOP);
    }

    u8 *frameGfx1 = sprite_utils::get_frame((u8 *) gfx_copyrightsTiles, COPYRIGHTS_SPRITE_SIZE, 0);
    u8 *frameGfx2 = sprite_utils::get_frame((u8 *) gfx_copyrightsTiles, COPYRIGHTS_SPRITE_SIZE, 1);
    u8 *frameGfx3 = sprite_utils::get_frame((u8 *) gfx_copyrightsTiles, COPYRIGHTS_SPRITE_SIZE, 2);
    u8 *frameGfx4 = sprite_utils::get_frame((u8 *) gfx_copyrightsTiles, COPYRIGHTS_SPRITE_SIZE, 3);
    u8 *frameGfx5 = sprite_utils::get_frame((u8 *) gfx_copyrightsTiles, COPYRIGHTS_SPRITE_SIZE, 4);

    sprite_utils::update_frame(frameGfx1, COPYRIGHTS_SPRITE_SIZE, mainSpriteInfo_1, subSpriteInfo_1);
    sprite_utils::update_frame(frameGfx2, COPYRIGHTS_SPRITE_SIZE, mainSpriteInfo_2, subSpriteInfo_2);
    sprite_utils::update_frame(frameGfx3, COPYRIGHTS_SPRITE_SIZE, mainSpriteInfo_3, subSpriteInfo_3);
    sprite_utils::update_frame(frameGfx4, COPYRIGHTS_SPRITE_SIZE, mainSpriteInfo_4, subSpriteInfo_4);
    sprite_utils::update_frame(frameGfx5, COPYRIGHTS_SPRITE_SIZE, mainSpriteInfo_5, subSpriteInfo_5);
}


Copyrights::Copyrights() {
    physical_height = COPYRIGHTS_PHYSICAL_HEIGHT;
    physical_width = COPYRIGHTS_PHYSICAL_WIDTH;
    sprite_height = COPYRIGHTS_SPRITE_HEIGHT;
    sprite_width = COPYRIGHTS_SPRITE_WIDTH;
}

void Copyrights::deleteSprite() {

    delete mainSpriteInfo_1;
    delete mainSpriteInfo_2;
    delete mainSpriteInfo_3;
    delete mainSpriteInfo_4;
    delete mainSpriteInfo_5;
    delete subSpriteInfo_1;
    delete subSpriteInfo_2;
    delete subSpriteInfo_3;
    delete subSpriteInfo_4;
    delete subSpriteInfo_5;

    mainSpriteInfo_1 = nullptr;
    mainSpriteInfo_2 = nullptr;
    mainSpriteInfo_3 = nullptr;
    mainSpriteInfo_4 = nullptr;
    mainSpriteInfo_5 = nullptr;
    subSpriteInfo_1 = nullptr;
    subSpriteInfo_2 = nullptr;
    subSpriteInfo_3 = nullptr;
    subSpriteInfo_4 = nullptr;
    subSpriteInfo_5 = nullptr;
}
