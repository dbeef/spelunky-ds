//
// Created by xdbeef on 29.04.18.
//

#include "spelunky_title.h"
#include "../../globals_declarations.h"
#include "../../../build/gfx_spelunky_title.h"

void SpelunkyTitle::draw() {

    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);

    mainSpriteInfo_1->entry->x = main_x;
    mainSpriteInfo_1->entry->y = main_y;

    mainSpriteInfo_2->entry->x = main_x + SPELUNKY_TITLE_SPRITE_WIDTH;
    mainSpriteInfo_2->entry->y = main_y;

    mainSpriteInfo_3->entry->x = main_x + SPELUNKY_TITLE_SPRITE_WIDTH * 2;
    mainSpriteInfo_3->entry->y = main_y;

    subSpriteInfo_1->entry->x = sub_x;
    subSpriteInfo_1->entry->y = sub_y;

    subSpriteInfo_2->entry->x = sub_x + SPELUNKY_TITLE_SPRITE_WIDTH;
    subSpriteInfo_2->entry->y = sub_y;

    subSpriteInfo_3->entry->x = sub_x + SPELUNKY_TITLE_SPRITE_WIDTH * 2;
    subSpriteInfo_3->entry->y = sub_y;

}

void SpelunkyTitle::init() {
    initSprite();

    mainSpriteInfo_1->entry->hFlip = false;
    mainSpriteInfo_2->entry->hFlip = false;
    mainSpriteInfo_3->entry->hFlip = false;


    subSpriteInfo_1->entry->hFlip = false;
    subSpriteInfo_2->entry->hFlip = false;
    subSpriteInfo_3->entry->hFlip = false;

}

void SpelunkyTitle::initSprite() {

    if (oamType == OamType::MAIN) {
        mainSpriteInfo_1 = global::main_oam_manager->initSprite(gfx_spelunky_titlePal, gfx_spelunky_titlePalLen,
                                                                nullptr, SPELUNKY_TITLE_SPRITE_WIDTH *
                                                                         SPELUNKY_TITLE_SPRITE_HEIGHT, 64,
                                                                SpritesheetType::SPELUNKY_TITLE,
                                                                true,
                                                                false, LAYER_LEVEL::MIDDLE_TOP);
        mainSpriteInfo_2 = global::main_oam_manager->initSprite(gfx_spelunky_titlePal, gfx_spelunky_titlePalLen,
                                                                nullptr, SPELUNKY_TITLE_SPRITE_WIDTH *
                                                                         SPELUNKY_TITLE_SPRITE_HEIGHT, 64,
                                                                SpritesheetType::SPELUNKY_TITLE,
                                                                true,
                                                                false, LAYER_LEVEL::MIDDLE_TOP);
        mainSpriteInfo_3 = global::main_oam_manager->initSprite(gfx_spelunky_titlePal, gfx_spelunky_titlePalLen,
                                                                nullptr, SPELUNKY_TITLE_SPRITE_WIDTH *
                                                                         SPELUNKY_TITLE_SPRITE_HEIGHT, 64,
                                                                SpritesheetType::SPELUNKY_TITLE,
                                                                true,
                                                                false, LAYER_LEVEL::MIDDLE_TOP);

        u8 *frameGfx1 =
                (u8 *) gfx_spelunky_titleTiles + 0 * SPELUNKY_TITLE_SPRITE_WIDTH * SPELUNKY_TITLE_SPRITE_HEIGHT / 2;
        u8 *frameGfx2 =
                (u8 *) gfx_spelunky_titleTiles + 1 * SPELUNKY_TITLE_SPRITE_WIDTH * SPELUNKY_TITLE_SPRITE_HEIGHT / 2;
        u8 *frameGfx3 =
                (u8 *) gfx_spelunky_titleTiles + 2 * SPELUNKY_TITLE_SPRITE_WIDTH * SPELUNKY_TITLE_SPRITE_HEIGHT / 2;

        mainSpriteInfo_1->updateFrame(frameGfx1, SPELUNKY_TITLE_SPRITE_WIDTH * SPELUNKY_TITLE_SPRITE_HEIGHT);
        mainSpriteInfo_2->updateFrame(frameGfx2, SPELUNKY_TITLE_SPRITE_WIDTH * SPELUNKY_TITLE_SPRITE_HEIGHT);
        mainSpriteInfo_3->updateFrame(frameGfx3, SPELUNKY_TITLE_SPRITE_WIDTH * SPELUNKY_TITLE_SPRITE_HEIGHT);

    }

    if (oamType == OamType::SUB) {
        subSpriteInfo_1 = global::sub_oam_manager->initSprite(gfx_spelunky_titlePal, gfx_spelunky_titlePalLen,
                                                              nullptr, SPELUNKY_TITLE_SPRITE_WIDTH *
                                                                       SPELUNKY_TITLE_SPRITE_HEIGHT, 64,
                                                              SpritesheetType::SPELUNKY_TITLE,
                                                              true,
                                                              false, LAYER_LEVEL::MIDDLE_TOP);
        subSpriteInfo_2 = global::sub_oam_manager->initSprite(gfx_spelunky_titlePal, gfx_spelunky_titlePalLen,
                                                              nullptr, SPELUNKY_TITLE_SPRITE_WIDTH *
                                                                       SPELUNKY_TITLE_SPRITE_HEIGHT, 64,
                                                              SpritesheetType::SPELUNKY_TITLE,
                                                              true,
                                                              false, LAYER_LEVEL::MIDDLE_TOP);
        subSpriteInfo_3 = global::sub_oam_manager->initSprite(gfx_spelunky_titlePal, gfx_spelunky_titlePalLen,
                                                              nullptr, SPELUNKY_TITLE_SPRITE_WIDTH *
                                                                       SPELUNKY_TITLE_SPRITE_HEIGHT, 64,
                                                              SpritesheetType::SPELUNKY_TITLE,
                                                              true,
                                                              false, LAYER_LEVEL::MIDDLE_TOP);

        u8 *frameGfx1 =
                (u8 *) gfx_spelunky_titleTiles + 0 * SPELUNKY_TITLE_SPRITE_WIDTH * SPELUNKY_TITLE_SPRITE_HEIGHT / 2;
        u8 *frameGfx2 =
                (u8 *) gfx_spelunky_titleTiles + 1 * SPELUNKY_TITLE_SPRITE_WIDTH * SPELUNKY_TITLE_SPRITE_HEIGHT / 2;
        u8 *frameGfx3 =
                (u8 *) gfx_spelunky_titleTiles + 2 * SPELUNKY_TITLE_SPRITE_WIDTH * SPELUNKY_TITLE_SPRITE_HEIGHT / 2;

        subSpriteInfo_1->updateFrame(frameGfx1, SPELUNKY_TITLE_SPRITE_WIDTH * SPELUNKY_TITLE_SPRITE_HEIGHT);
        subSpriteInfo_2->updateFrame(frameGfx2, SPELUNKY_TITLE_SPRITE_WIDTH * SPELUNKY_TITLE_SPRITE_HEIGHT);
        subSpriteInfo_3->updateFrame(frameGfx3, SPELUNKY_TITLE_SPRITE_WIDTH * SPELUNKY_TITLE_SPRITE_HEIGHT);
    }
}


SpelunkyTitle::SpelunkyTitle(){
    physical_height = SPELUNKY_TITLE_PHYSICAL_HEIGHT;
    physical_width = SPELUNKY_TITLE_PHYSICAL_WIDTH;
    sprite_height = SPELUNKY_TITLE_SPRITE_HEIGHT;
    sprite_width = SPELUNKY_TITLE_SPRITE_WIDTH;
}

