//
// Created by xdbeef on 29.04.18.
//

#include "spelunky_title.h"
#include "../../globals_declarations.h"
#include "../../../build/gfx_copyrights.h"
#include "copyrights.h"

void Copyrights::draw() {

    int main_x = x - global::camera->x;
    int main_y = y - global::camera->y;
    int sub_x = x - global::camera->x;
    int sub_y = y - global::camera->y - 192;

    if (global::camera->y + 192 > this->y + 32 || global::camera->y + 192 + 192 < this->y - 32) {
        sub_x = -128;
        sub_y = -128;
    }
    if (global::camera->y > this->y + 32 || global::camera->y + 192 < this->y - 32) {
        main_x = -128;
        main_y = -128;
    }


    if (sub_y + 32 < 0 || sub_x + 32 < 0) {
        sub_x = -128;
        sub_y = -128;
    }

    if (main_y + 32 < 0 || main_x + 32 < 0) {
        main_x = -128;
        main_y = -128;
    }


    mainSpriteInfo_1->entry->x = main_x;
    mainSpriteInfo_1->entry->y = main_y;

    mainSpriteInfo_2->entry->x = main_x + 32;
    mainSpriteInfo_2->entry->y = main_y;

    mainSpriteInfo_3->entry->x = main_x + 64;
    mainSpriteInfo_3->entry->y = main_y;

    mainSpriteInfo_4->entry->x = main_x + 96;
    mainSpriteInfo_4->entry->y = main_y;

    mainSpriteInfo_5->entry->x = main_x + 128;
    mainSpriteInfo_5->entry->y = main_y;

    subSpriteInfo_1->entry->x = sub_x;
    subSpriteInfo_1->entry->y = sub_y;

    subSpriteInfo_2->entry->x = sub_x + 32;
    subSpriteInfo_2->entry->y = sub_y;

    subSpriteInfo_3->entry->x = sub_x + 64;
    subSpriteInfo_3->entry->y = sub_y;

    subSpriteInfo_4->entry->x = sub_x + 96;
    subSpriteInfo_4->entry->y = sub_y;

    subSpriteInfo_5->entry->x = sub_x + 128;
    subSpriteInfo_5->entry->y = sub_y;

}


void Copyrights::init() {
 initSprite();

    subSpriteInfo_1->entry->hFlip = false;
    subSpriteInfo_2->entry->hFlip = false;
    subSpriteInfo_3->entry->hFlip = false;
    subSpriteInfo_4->entry->hFlip = false;
    subSpriteInfo_5->entry->hFlip = false;

}

void Copyrights::updateSpeed() {

}

void Copyrights::updatePosition() {
}

void Copyrights::updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) {
}

void Copyrights::initSprite() {

    if(oamType == OamType::MAIN) {
        mainSpriteInfo_1 = global::main_oam_manager->initSprite(gfx_copyrightsPal, gfx_copyrightsPalLen,
                                                                nullptr, 32 * 32, 32, SpritesheetType::SPELUNKY_COPYRIGHTS, true,
                                                                false);
        mainSpriteInfo_2 = global::main_oam_manager->initSprite(gfx_copyrightsPal, gfx_copyrightsPalLen,
                                                                nullptr, 32 * 32, 32, SpritesheetType::SPELUNKY_COPYRIGHTS, true,
                                                                false);
        mainSpriteInfo_3 = global::main_oam_manager->initSprite(gfx_copyrightsPal, gfx_copyrightsPalLen,
                                                                nullptr, 32 * 32, 32, SpritesheetType::SPELUNKY_COPYRIGHTS, true,
                                                                false);
        mainSpriteInfo_4 = global::main_oam_manager->initSprite(gfx_copyrightsPal, gfx_copyrightsPalLen,
                                                                nullptr, 32 * 32, 32, SpritesheetType::SPELUNKY_COPYRIGHTS, true,
                                                                false);
        mainSpriteInfo_4 = global::main_oam_manager->initSprite(gfx_copyrightsPal, gfx_copyrightsPalLen,
                                                                nullptr, 32 * 32, 32, SpritesheetType::SPELUNKY_COPYRIGHTS, true,
                                                                false);
    }

    if(oamType == OamType::SUB) {
        subSpriteInfo_1 = global::sub_oam_manager->initSprite(gfx_copyrightsPal, gfx_copyrightsPalLen,
                                                              nullptr, 32 * 32, 32, SpritesheetType::SPELUNKY_COPYRIGHTS, true,
                                                              false);
        subSpriteInfo_2 = global::sub_oam_manager->initSprite(gfx_copyrightsPal, gfx_copyrightsPalLen,
                                                              nullptr, 32 * 32, 32, SpritesheetType::SPELUNKY_COPYRIGHTS, true,
                                                              false);
        subSpriteInfo_3 = global::sub_oam_manager->initSprite(gfx_copyrightsPal, gfx_copyrightsPalLen,
                                                              nullptr, 32 * 32, 32, SpritesheetType::SPELUNKY_COPYRIGHTS, true,
                                                              false);
        subSpriteInfo_4 = global::sub_oam_manager->initSprite(gfx_copyrightsPal, gfx_copyrightsPalLen,
                                                              nullptr, 32 * 32, 32, SpritesheetType::SPELUNKY_COPYRIGHTS, true,
                                                              false);
        subSpriteInfo_5 = global::sub_oam_manager->initSprite(gfx_copyrightsPal, gfx_copyrightsPalLen,
                                                              nullptr, 32 * 32, 32, SpritesheetType::SPELUNKY_COPYRIGHTS, true,
                                                              false);
    }

    u8 *frameGfx1 = (u8 *) gfx_copyrightsTiles + 0 * 32 * 32 / 2;
    u8 *frameGfx2 = (u8 *) gfx_copyrightsTiles + 1 * 32 * 32 / 2;
    u8 *frameGfx3 = (u8 *) gfx_copyrightsTiles + 2 * 32 * 32 / 2;
    u8 *frameGfx4 = (u8 *) gfx_copyrightsTiles + 3 * 32 * 32 / 2;
    u8 *frameGfx5 = (u8 *) gfx_copyrightsTiles + 4 * 32 * 32 / 2;

    mainSpriteInfo_1->updateFrame(frameGfx1, 32 * 32);
    mainSpriteInfo_2->updateFrame(frameGfx2, 32 * 32);
    mainSpriteInfo_3->updateFrame(frameGfx3, 32 * 32);
    mainSpriteInfo_4->updateFrame(frameGfx4, 32 * 32);
    mainSpriteInfo_5->updateFrame(frameGfx5, 32 * 32);

    mainSpriteInfo_1->entry->hFlip = false;
    mainSpriteInfo_2->entry->hFlip = false;
    mainSpriteInfo_3->entry->hFlip = false;
    mainSpriteInfo_4->entry->hFlip = false;
    mainSpriteInfo_5->entry->hFlip = false;

    subSpriteInfo_1->updateFrame(frameGfx1, 32 * 32);
    subSpriteInfo_2->updateFrame(frameGfx2, 32 * 32);
    subSpriteInfo_3->updateFrame(frameGfx3, 32 * 32);
    subSpriteInfo_4->updateFrame(frameGfx4, 32 * 32);
    subSpriteInfo_5->updateFrame(frameGfx5, 32 * 32);
}

