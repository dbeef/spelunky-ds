//
// Created by xdbeef on 30.04.18.
//

#include "title_menu_sign.h"
#include "../../globals_declarations.h"
#include "../../../build/gfx_menu_signs_start_scores.h"
#include "../../../build/gfx_menu_signs_tutorial_quit.h"

void TitleMenuSign::draw() {

    int offset;
    if(menuSignType == MenuSignType::TUTORIAL || menuSignType == MenuSignType::QUIT)
        offset = 16;
    else
        offset = 32;
    
    int main_x = x - global::camera->x;
    int main_y = y - global::camera->y;
    int sub_x = x - global::camera->x;
    int sub_y = y - global::camera->y - 192;

    if (global::camera->y + 192 > this->y + offset || global::camera->y + 192 + 192 < this->y - offset) {
        sub_x = -128;
        sub_y = -128;
    }
    if (global::camera->y > this->y + offset || global::camera->y + 192 < this->y - offset) {
        main_x = -128;
        main_y = -128;
    }


    if (sub_y + offset < 0 || sub_x + offset < 0) {
        sub_x = -128;
        sub_y = -128;
    }

    if (main_y + offset < 0 || main_x + offset < 0) {
        main_x = -128;
        main_y = -128;
    }


    mainSpriteInfo_1->entry->x = main_x;
    mainSpriteInfo_1->entry->y = main_y;

    mainSpriteInfo_2->entry->x = main_x + offset;
    mainSpriteInfo_2->entry->y = main_y;


    subSpriteInfo_1->entry->x = sub_x;
    subSpriteInfo_1->entry->y = sub_y;

    subSpriteInfo_2->entry->x = sub_x + offset;
    subSpriteInfo_2->entry->y = sub_y;

    mainSpriteInfo_1->entry->hFlip = false;
    mainSpriteInfo_2->entry->hFlip = false;
    mainSpriteInfo_1->entry->vFlip = false;
    mainSpriteInfo_2->entry->vFlip = false;

}


void TitleMenuSign::init() {
    initSprite();
    mainSpriteInfo_1->entry->hFlip = false;
    mainSpriteInfo_2->entry->hFlip = false;
}

void TitleMenuSign::updateSpeed() {

}

void TitleMenuSign::update_position() {
}

void TitleMenuSign::updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) {
}

void TitleMenuSign::initSprite() {

    OAMManager *used_oam_manager;
    if (oamType == OamType::MAIN) {
        used_oam_manager = global::main_oam_manager;
    } else if (oamType == OamType::SUB) {
        used_oam_manager = global::sub_oam_manager;
    }

    u8 *frameGfx1;
    u8 *frameGfx2;

    switch (menuSignType) {

        case MenuSignType::QUIT:
            mainSpriteInfo_1 = used_oam_manager->initSprite(gfx_menu_signs_tutorial_quitPal, gfx_menu_signs_tutorial_quitPalLen,
                                                            nullptr, 16 * 16, 16, SpritesheetType::MENU_SIGNS_TUTORIAL_QUIT, true, false,LAYER_LEVEL::MIDDLE_TOP);
            mainSpriteInfo_2 = used_oam_manager->initSprite(gfx_menu_signs_tutorial_quitPal, gfx_menu_signs_tutorial_quitPalLen,
                                                            nullptr, 16 * 16, 16, SpritesheetType::MENU_SIGNS_TUTORIAL_QUIT, true, false,LAYER_LEVEL::MIDDLE_TOP);
            frameGfx1 = (u8 *) gfx_menu_signs_tutorial_quitTiles + 2 * 16 * 16 / 2;
            frameGfx2 = (u8 *) gfx_menu_signs_tutorial_quitTiles + 3 * 16 * 16 / 2;

            break;
        case MenuSignType::SCORES:
            mainSpriteInfo_1 = used_oam_manager->initSprite(gfx_menu_signs_start_scoresPal, gfx_menu_signs_start_scoresPalLen,
                                                            nullptr, 32 * 32, 32, SpritesheetType::MENU_SIGNS_START_SCORES, true, false,LAYER_LEVEL::MIDDLE_TOP);
            mainSpriteInfo_2 = used_oam_manager->initSprite(gfx_menu_signs_start_scoresPal, gfx_menu_signs_start_scoresPalLen,
                                                            nullptr, 32 * 32, 32, SpritesheetType::MENU_SIGNS_START_SCORES, true, false,LAYER_LEVEL::MIDDLE_TOP);
            frameGfx1 = (u8 *) gfx_menu_signs_start_scoresTiles + 2 * 32 * 32 / 2;
            frameGfx2 = (u8 *) gfx_menu_signs_start_scoresTiles + 3 * 32 * 32 / 2;
            break;
        case MenuSignType::START:
            mainSpriteInfo_1 = used_oam_manager->initSprite(gfx_menu_signs_start_scoresPal, gfx_menu_signs_start_scoresPalLen,
                                                            nullptr, 32 * 32, 32, SpritesheetType::MENU_SIGNS_START_SCORES, true, false,LAYER_LEVEL::MIDDLE_TOP);
            mainSpriteInfo_2 = used_oam_manager->initSprite(gfx_menu_signs_start_scoresPal, gfx_menu_signs_start_scoresPalLen,
                                                            nullptr, 32 * 32, 32, SpritesheetType::MENU_SIGNS_START_SCORES, true, false,LAYER_LEVEL::MIDDLE_TOP);
            frameGfx1 = (u8 *) gfx_menu_signs_start_scoresTiles + 0 * 32 * 32 / 2;
            frameGfx2 = (u8 *) gfx_menu_signs_start_scoresTiles + 1 * 32 * 32 / 2;
            break;
        case MenuSignType::TUTORIAL:
            mainSpriteInfo_1 = used_oam_manager->initSprite(gfx_menu_signs_tutorial_quitPal, gfx_menu_signs_tutorial_quitPalLen,
                                                            nullptr, 16 * 16, 16, SpritesheetType::MENU_SIGNS_TUTORIAL_QUIT, true, false,LAYER_LEVEL::MIDDLE_TOP);
            mainSpriteInfo_2 = used_oam_manager->initSprite(gfx_menu_signs_tutorial_quitPal, gfx_menu_signs_tutorial_quitPalLen,
                                                            nullptr, 16 * 16, 16, SpritesheetType::MENU_SIGNS_TUTORIAL_QUIT, true, false,LAYER_LEVEL::MIDDLE_TOP);
            frameGfx1 = (u8 *) gfx_menu_signs_tutorial_quitTiles + 0 * 16 * 16 / 2;
            frameGfx2 = (u8 *) gfx_menu_signs_tutorial_quitTiles + 1 * 16 * 16 / 2;
            break;
        default:
            break;

    }

    if(menuSignType == MenuSignType::TUTORIAL || menuSignType == MenuSignType::QUIT) {
        mainSpriteInfo_1->updateFrame(frameGfx1, 16 * 16);
        mainSpriteInfo_2->updateFrame(frameGfx2, 16 * 16);
    }
    else
    {
        mainSpriteInfo_1->updateFrame(frameGfx1, 32 * 32);
        mainSpriteInfo_2->updateFrame(frameGfx2, 32 * 32);
    }


}

