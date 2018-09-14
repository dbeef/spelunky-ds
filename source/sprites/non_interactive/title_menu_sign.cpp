//
// Created by xdbeef on 30.04.18.
//

#include "title_menu_sign.h"
#include "../../globals_declarations.h"
#include "../../../build/gfx_menu_signs_start_scores.h"
#include "../../../build/gfx_menu_signs_tutorial_quit.h"
#include "../sprite_utils.h"

void TitleMenuSign::draw() {

    int offset;

    if (menuSignType == MenuSignType::TUTORIAL || menuSignType == MenuSignType::QUIT)
        offset = 16;
    else
        offset = 32;

    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);

    sprite_utils::set_entry_xy(mainSpriteInfo_1, main_x, main_y);
    sprite_utils::set_entry_xy(mainSpriteInfo_2, main_x + offset, main_y);

    //FIXME Why subspriteinfo anyway? It's only on the uppper screen
    sprite_utils::set_entry_xy(subSpriteInfo_1, sub_x, sub_y);
    sprite_utils::set_entry_xy(subSpriteInfo_2, sub_x + offset, sub_y);

}


void TitleMenuSign::init() {
    initSprite();
    sprite_utils::set_vertical_flip(false, mainSpriteInfo_1, mainSpriteInfo_2);
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo_1, mainSpriteInfo_2);
    sprite_utils::set_visibility(true, mainSpriteInfo_1, mainSpriteInfo_2);
}


void TitleMenuSign::initSprite() {

    delete mainSpriteInfo_1;
    delete mainSpriteInfo_2;
    
    delete subSpriteInfo_1;
    delete subSpriteInfo_2;
    
    OAMManager *used_oam_manager = nullptr;
    u8 *frameGfx1 = nullptr;
    u8 *frameGfx2 = nullptr;

    if (oamType == OamType::MAIN)
        used_oam_manager = global::main_oam_manager;
    else if (oamType == OamType::SUB)
        used_oam_manager = global::sub_oam_manager;

    switch (menuSignType) {

        case MenuSignType::QUIT:
            mainSpriteInfo_1 = used_oam_manager->initSprite(gfx_menu_signs_tutorial_quitPal,
                                                            gfx_menu_signs_tutorial_quitPalLen,
                                                            nullptr, 16 * 16, 16,
                                                            SpritesheetType::MENU_SIGNS_TUTORIAL_QUIT, true, false,
                                                            LAYER_LEVEL::MIDDLE_TOP);
            mainSpriteInfo_2 = used_oam_manager->initSprite(gfx_menu_signs_tutorial_quitPal,
                                                            gfx_menu_signs_tutorial_quitPalLen,
                                                            nullptr, 16 * 16, 16,
                                                            SpritesheetType::MENU_SIGNS_TUTORIAL_QUIT, true, false,
                                                            LAYER_LEVEL::MIDDLE_TOP);
            frameGfx1 = (u8 *) gfx_menu_signs_tutorial_quitTiles + 2 * 16 * 16 / 2;
            frameGfx2 = (u8 *) gfx_menu_signs_tutorial_quitTiles + 3 * 16 * 16 / 2;

            break;
        case MenuSignType::SCORES:
            mainSpriteInfo_1 = used_oam_manager->initSprite(gfx_menu_signs_start_scoresPal,
                                                            gfx_menu_signs_start_scoresPalLen,
                                                            nullptr, 32 * 32, 32,
                                                            SpritesheetType::MENU_SIGNS_START_SCORES, true, false,
                                                            LAYER_LEVEL::MIDDLE_TOP);
            mainSpriteInfo_2 = used_oam_manager->initSprite(gfx_menu_signs_start_scoresPal,
                                                            gfx_menu_signs_start_scoresPalLen,
                                                            nullptr, 32 * 32, 32,
                                                            SpritesheetType::MENU_SIGNS_START_SCORES, true, false,
                                                            LAYER_LEVEL::MIDDLE_TOP);
            frameGfx1 = (u8 *) gfx_menu_signs_start_scoresTiles + 2 * 32 * 32 / 2;
            frameGfx2 = (u8 *) gfx_menu_signs_start_scoresTiles + 3 * 32 * 32 / 2;
            break;
        case MenuSignType::START:
            mainSpriteInfo_1 = used_oam_manager->initSprite(gfx_menu_signs_start_scoresPal,
                                                            gfx_menu_signs_start_scoresPalLen,
                                                            nullptr, 32 * 32, 32,
                                                            SpritesheetType::MENU_SIGNS_START_SCORES, true, false,
                                                            LAYER_LEVEL::MIDDLE_TOP);
            mainSpriteInfo_2 = used_oam_manager->initSprite(gfx_menu_signs_start_scoresPal,
                                                            gfx_menu_signs_start_scoresPalLen,
                                                            nullptr, 32 * 32, 32,
                                                            SpritesheetType::MENU_SIGNS_START_SCORES, true, false,
                                                            LAYER_LEVEL::MIDDLE_TOP);
            frameGfx1 = (u8 *) gfx_menu_signs_start_scoresTiles + 0 * 32 * 32 / 2;
            frameGfx2 = (u8 *) gfx_menu_signs_start_scoresTiles + 1 * 32 * 32 / 2;
            break;
        case MenuSignType::TUTORIAL:
            mainSpriteInfo_1 = used_oam_manager->initSprite(gfx_menu_signs_tutorial_quitPal,
                                                            gfx_menu_signs_tutorial_quitPalLen,
                                                            nullptr, 16 * 16, 16,
                                                            SpritesheetType::MENU_SIGNS_TUTORIAL_QUIT, true, false,
                                                            LAYER_LEVEL::MIDDLE_TOP);
            mainSpriteInfo_2 = used_oam_manager->initSprite(gfx_menu_signs_tutorial_quitPal,
                                                            gfx_menu_signs_tutorial_quitPalLen,
                                                            nullptr, 16 * 16, 16,
                                                            SpritesheetType::MENU_SIGNS_TUTORIAL_QUIT, true, false,
                                                            LAYER_LEVEL::MIDDLE_TOP);
            frameGfx1 = (u8 *) gfx_menu_signs_tutorial_quitTiles + 0 * 16 * 16 / 2;
            frameGfx2 = (u8 *) gfx_menu_signs_tutorial_quitTiles + 1 * 16 * 16 / 2;
            break;
        default:
            break;

    }

    if (menuSignType == MenuSignType::TUTORIAL || menuSignType == MenuSignType::QUIT) {
        mainSpriteInfo_1->updateFrame(frameGfx1, 16 * 16);
        mainSpriteInfo_2->updateFrame(frameGfx2, 16 * 16);
    } else {
        mainSpriteInfo_1->updateFrame(frameGfx1, 32 * 32);
        mainSpriteInfo_2->updateFrame(frameGfx2, 32 * 32);
    }


}


