//
// Created by xdbeef on 29.04.18.
//

#include "SpelunkyTitle.h"
#include "../Globals.h"
#include "../../build/spelunky_title.h"

void SpelunkyTitle::draw() {

    int main_x = x - global::camera->x;
    int main_y = y - global::camera->y;
    int sub_x = x - global::camera->x;
    int sub_y = y - global::camera->y - 192;

    if (global::camera->y + 192 > this->y + 64 || global::camera->y + 192 + 192 < this->y - 64) {
        sub_x = -128;
        sub_y = -128;
    }
    if (global::camera->y > this->y + 64 || global::camera->y + 192 < this->y - 64) {
        main_x = -128;
        main_y = -128;
    }


    if (sub_y + 64 < 0 || sub_x + 64 < 0) {
        sub_x = -128;
        sub_y = -128;
    }

    if (main_y + 64 < 0 || main_x + 64 < 0) {
        main_x = -128;
        main_y = -128;
    }


    mainSpriteInfo_1->entry->x = main_x;
    mainSpriteInfo_1->entry->y = main_y;

    mainSpriteInfo_2->entry->x = main_x + 64;
    mainSpriteInfo_2->entry->y = main_y;

    mainSpriteInfo_3->entry->x = main_x + 128;
    mainSpriteInfo_3->entry->y = main_y;

    subSpriteInfo_1->entry->x = sub_x;
    subSpriteInfo_1->entry->y = sub_y;

    subSpriteInfo_2->entry->x = sub_x + 64;
    subSpriteInfo_2->entry->y = sub_y;

    subSpriteInfo_3->entry->x = sub_x + 128;
    subSpriteInfo_3->entry->y = sub_y;

}


void SpelunkyTitle::init() {
 
    if(oamType == OamType::MAIN) {
        mainSpriteInfo_1 = global::main_oam_manager->initSprite(spelunky_titlePal, spelunky_titlePalLen,
                                                                nullptr, 64 * 64, 64, SpriteType::SPELUNKY_TITLE, true,
                                                                false);
        mainSpriteInfo_2 = global::main_oam_manager->initSprite(spelunky_titlePal, spelunky_titlePalLen,
                                                                nullptr, 64 * 64, 64, SpriteType::SPELUNKY_TITLE, true,
                                                                false);
        mainSpriteInfo_3 = global::main_oam_manager->initSprite(spelunky_titlePal, spelunky_titlePalLen,
                                                                nullptr, 64 * 64, 64, SpriteType::SPELUNKY_TITLE, true,
                                                                false);

        u8 *frameGfx1 = (u8 *) spelunky_titleTiles + 0 * 64 * 64 / 2;
        u8 *frameGfx2 = (u8 *) spelunky_titleTiles + 1 * 64 * 64 / 2;
        u8 *frameGfx3 = (u8 *) spelunky_titleTiles + 2 * 64 * 64 / 2;

        mainSpriteInfo_1->updateFrame(frameGfx1, 64 * 64);
        mainSpriteInfo_2->updateFrame(frameGfx2, 64 * 64);
        mainSpriteInfo_3->updateFrame(frameGfx3, 64 * 64);

        mainSpriteInfo_1->entry->hFlip = false;
        mainSpriteInfo_2->entry->hFlip = false;
        mainSpriteInfo_3->entry->hFlip = false;
    }
    
    if(oamType == OamType::SUB) {
        subSpriteInfo_1 = global::sub_oam_manager->initSprite(spelunky_titlePal, spelunky_titlePalLen,
                                                                nullptr, 64 * 64, 64, SpriteType::SPELUNKY_TITLE, true,
                                                                false);
        subSpriteInfo_2 = global::sub_oam_manager->initSprite(spelunky_titlePal, spelunky_titlePalLen,
                                                                nullptr, 64 * 64, 64, SpriteType::SPELUNKY_TITLE, true,
                                                                false);
        subSpriteInfo_3 = global::sub_oam_manager->initSprite(spelunky_titlePal, spelunky_titlePalLen,
                                                                nullptr, 64 * 64, 64, SpriteType::SPELUNKY_TITLE, true,
                                                                false);

        u8 *frameGfx1 = (u8 *) spelunky_titleTiles + 0 * 64 * 64 / 2;
        u8 *frameGfx2 = (u8 *) spelunky_titleTiles + 1 * 64 * 64 / 2;
        u8 *frameGfx3 = (u8 *) spelunky_titleTiles + 2 * 64 * 64 / 2;

        subSpriteInfo_1->updateFrame(frameGfx1, 64 * 64);
        subSpriteInfo_2->updateFrame(frameGfx2, 64 * 64);
        subSpriteInfo_3->updateFrame(frameGfx3, 64 * 64);

        subSpriteInfo_1->entry->hFlip = false;
        subSpriteInfo_2->entry->hFlip = false;
        subSpriteInfo_3->entry->hFlip = false;
    }
    
}

void SpelunkyTitle::updateSpeed() {

}

void SpelunkyTitle::updatePosition() {
}

void SpelunkyTitle::updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) {
}

