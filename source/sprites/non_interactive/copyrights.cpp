//
// Created by xdbeef on 29.04.18.
//

#include "spelunky_title.h"
#include "../../globals_declarations.h"
#include "../../../build/gfx_copyrights.h"
#include "copyrights.h"

void Copyrights::draw() {

    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);

    mainSpriteInfo_1->entry->x = main_x;
    mainSpriteInfo_1->entry->y = main_y;

    mainSpriteInfo_2->entry->x = main_x + COPYRIGHTS_SPRITE_WIDTH;
    mainSpriteInfo_2->entry->y = main_y;

    mainSpriteInfo_3->entry->x = main_x + COPYRIGHTS_SPRITE_WIDTH * 2;
    mainSpriteInfo_3->entry->y = main_y;

    mainSpriteInfo_4->entry->x = main_x + COPYRIGHTS_SPRITE_WIDTH * 3;
    mainSpriteInfo_4->entry->y = main_y;

    mainSpriteInfo_5->entry->x = main_x + COPYRIGHTS_SPRITE_WIDTH * 4;
    mainSpriteInfo_5->entry->y = main_y;

    subSpriteInfo_1->entry->x = sub_x;
    subSpriteInfo_1->entry->y = sub_y;

    subSpriteInfo_2->entry->x = sub_x + COPYRIGHTS_SPRITE_WIDTH;
    subSpriteInfo_2->entry->y = sub_y;

    subSpriteInfo_3->entry->x = sub_x + COPYRIGHTS_SPRITE_WIDTH * 2;
    subSpriteInfo_3->entry->y = sub_y;

    subSpriteInfo_4->entry->x = sub_x + COPYRIGHTS_SPRITE_WIDTH * 3;
    subSpriteInfo_4->entry->y = sub_y;

    subSpriteInfo_5->entry->x = sub_x + COPYRIGHTS_SPRITE_WIDTH * 4;
    subSpriteInfo_5->entry->y = sub_y;

}


void Copyrights::init() {
    initSprite();

    subSpriteInfo_1->entry->hFlip = false;
    subSpriteInfo_2->entry->hFlip = false;
    subSpriteInfo_3->entry->hFlip = false;
    subSpriteInfo_4->entry->hFlip = false;
    subSpriteInfo_5->entry->hFlip = false;

    mainSpriteInfo_1->entry->hFlip = false;
    mainSpriteInfo_2->entry->hFlip = false;
    mainSpriteInfo_3->entry->hFlip = false;
    mainSpriteInfo_4->entry->hFlip = false;
    mainSpriteInfo_5->entry->hFlip = false;

}

void Copyrights::initSprite() {

    if (oamType == OamType::MAIN) {
        mainSpriteInfo_1 = global::main_oam_manager->initSprite(gfx_copyrightsPal, gfx_copyrightsPalLen,
                                                                nullptr,
                                                                COPYRIGHTS_SPRITE_WIDTH * COPYRIGHTS_SPRITE_HEIGHT, 32,
                                                                SpritesheetType::SPELUNKY_COPYRIGHTS, true,
                                                                false, LAYER_LEVEL::MIDDLE_TOP);
        mainSpriteInfo_2 = global::main_oam_manager->initSprite(gfx_copyrightsPal, gfx_copyrightsPalLen,
                                                                nullptr,
                                                                COPYRIGHTS_SPRITE_WIDTH * COPYRIGHTS_SPRITE_HEIGHT, 32,
                                                                SpritesheetType::SPELUNKY_COPYRIGHTS, true,
                                                                false, LAYER_LEVEL::MIDDLE_TOP);
        mainSpriteInfo_3 = global::main_oam_manager->initSprite(gfx_copyrightsPal, gfx_copyrightsPalLen,
                                                                nullptr,
                                                                COPYRIGHTS_SPRITE_WIDTH * COPYRIGHTS_SPRITE_HEIGHT, 32,
                                                                SpritesheetType::SPELUNKY_COPYRIGHTS, true,
                                                                false, LAYER_LEVEL::MIDDLE_TOP);
        mainSpriteInfo_4 = global::main_oam_manager->initSprite(gfx_copyrightsPal, gfx_copyrightsPalLen,
                                                                nullptr,
                                                                COPYRIGHTS_SPRITE_WIDTH * COPYRIGHTS_SPRITE_HEIGHT, 32,
                                                                SpritesheetType::SPELUNKY_COPYRIGHTS, true,
                                                                false, LAYER_LEVEL::MIDDLE_TOP);
        mainSpriteInfo_4 = global::main_oam_manager->initSprite(gfx_copyrightsPal, gfx_copyrightsPalLen,
                                                                nullptr,
                                                                COPYRIGHTS_SPRITE_WIDTH * COPYRIGHTS_SPRITE_HEIGHT, 32,
                                                                SpritesheetType::SPELUNKY_COPYRIGHTS, true,
                                                                false, LAYER_LEVEL::MIDDLE_TOP);
    }

    if (oamType == OamType::SUB) {
        subSpriteInfo_1 = global::sub_oam_manager->initSprite(gfx_copyrightsPal, gfx_copyrightsPalLen,
                                                              nullptr,
                                                              COPYRIGHTS_SPRITE_WIDTH * COPYRIGHTS_SPRITE_HEIGHT, 32,
                                                              SpritesheetType::SPELUNKY_COPYRIGHTS, true,
                                                              false, LAYER_LEVEL::MIDDLE_TOP);
        subSpriteInfo_2 = global::sub_oam_manager->initSprite(gfx_copyrightsPal, gfx_copyrightsPalLen,
                                                              nullptr,
                                                              COPYRIGHTS_SPRITE_WIDTH * COPYRIGHTS_SPRITE_HEIGHT, 32,
                                                              SpritesheetType::SPELUNKY_COPYRIGHTS, true,
                                                              false, LAYER_LEVEL::MIDDLE_TOP);
        subSpriteInfo_3 = global::sub_oam_manager->initSprite(gfx_copyrightsPal, gfx_copyrightsPalLen,
                                                              nullptr,
                                                              COPYRIGHTS_SPRITE_WIDTH * COPYRIGHTS_SPRITE_HEIGHT, 32,
                                                              SpritesheetType::SPELUNKY_COPYRIGHTS, true,
                                                              false, LAYER_LEVEL::MIDDLE_TOP);
        subSpriteInfo_4 = global::sub_oam_manager->initSprite(gfx_copyrightsPal, gfx_copyrightsPalLen,
                                                              nullptr,
                                                              COPYRIGHTS_SPRITE_WIDTH * COPYRIGHTS_SPRITE_HEIGHT, 32,
                                                              SpritesheetType::SPELUNKY_COPYRIGHTS, true,
                                                              false, LAYER_LEVEL::MIDDLE_TOP);
        subSpriteInfo_5 = global::sub_oam_manager->initSprite(gfx_copyrightsPal, gfx_copyrightsPalLen,
                                                              nullptr,
                                                              COPYRIGHTS_SPRITE_WIDTH * COPYRIGHTS_SPRITE_HEIGHT, 32,
                                                              SpritesheetType::SPELUNKY_COPYRIGHTS, true,
                                                              false, LAYER_LEVEL::MIDDLE_TOP);
    }

    u8 *frameGfx1 = (u8 *) gfx_copyrightsTiles + 0 * COPYRIGHTS_SPRITE_WIDTH * COPYRIGHTS_SPRITE_HEIGHT / 2;
    u8 *frameGfx2 = (u8 *) gfx_copyrightsTiles + 1 * COPYRIGHTS_SPRITE_WIDTH * COPYRIGHTS_SPRITE_HEIGHT / 2;
    u8 *frameGfx3 = (u8 *) gfx_copyrightsTiles + 2 * COPYRIGHTS_SPRITE_WIDTH * COPYRIGHTS_SPRITE_HEIGHT / 2;
    u8 *frameGfx4 = (u8 *) gfx_copyrightsTiles + 3 * COPYRIGHTS_SPRITE_WIDTH * COPYRIGHTS_SPRITE_HEIGHT / 2;
    u8 *frameGfx5 = (u8 *) gfx_copyrightsTiles + 4 * COPYRIGHTS_SPRITE_WIDTH * COPYRIGHTS_SPRITE_HEIGHT / 2;

    mainSpriteInfo_1->updateFrame(frameGfx1, COPYRIGHTS_SPRITE_WIDTH * COPYRIGHTS_SPRITE_HEIGHT);
    mainSpriteInfo_2->updateFrame(frameGfx2, COPYRIGHTS_SPRITE_WIDTH * COPYRIGHTS_SPRITE_HEIGHT);
    mainSpriteInfo_3->updateFrame(frameGfx3, COPYRIGHTS_SPRITE_WIDTH * COPYRIGHTS_SPRITE_HEIGHT);
    mainSpriteInfo_4->updateFrame(frameGfx4, COPYRIGHTS_SPRITE_WIDTH * COPYRIGHTS_SPRITE_HEIGHT);
    mainSpriteInfo_5->updateFrame(frameGfx5, COPYRIGHTS_SPRITE_WIDTH * COPYRIGHTS_SPRITE_HEIGHT);

    subSpriteInfo_1->updateFrame(frameGfx1, COPYRIGHTS_SPRITE_WIDTH * COPYRIGHTS_SPRITE_HEIGHT);
    subSpriteInfo_2->updateFrame(frameGfx2, COPYRIGHTS_SPRITE_WIDTH * COPYRIGHTS_SPRITE_HEIGHT);
    subSpriteInfo_3->updateFrame(frameGfx3, COPYRIGHTS_SPRITE_WIDTH * COPYRIGHTS_SPRITE_HEIGHT);
    subSpriteInfo_4->updateFrame(frameGfx4, COPYRIGHTS_SPRITE_WIDTH * COPYRIGHTS_SPRITE_HEIGHT);
    subSpriteInfo_5->updateFrame(frameGfx5, COPYRIGHTS_SPRITE_WIDTH * COPYRIGHTS_SPRITE_HEIGHT);
}


Copyrights::Copyrights() {
    physical_height = COPYRIGHTS_PHYSICAL_HEIGHT;
    physical_width = COPYRIGHTS_PHYSICAL_WIDTH;
    sprite_height = COPYRIGHTS_SPRITE_HEIGHT;
    sprite_width = COPYRIGHTS_SPRITE_WIDTH;
}
