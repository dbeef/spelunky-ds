//
// Created by xdbeef on 17.06.18.
//

#include <cstdlib>
#include "../../globals_declarations.h"
#include "../../../build/gfx_goldbars.h"
#include "shopping_icon.h"

#define SHOPPING_ICON_OFFSET 12
#define ANIM_FRAME_DELTA 25

void ShoppingIcon::draw() {

    if (ready_to_dispose) {
        mainSpriteInfo->entry->isHidden = true;
        subSpriteInfo->entry->isHidden = true;
        return;
    }

    mainSpriteInfo->entry->isHidden = false;
    subSpriteInfo->entry->isHidden = false;
    mainSpriteInfo->entry->hFlip = false;
    subSpriteInfo->entry->hFlip = false;
    mainSpriteInfo->entry->vFlip = false;
    subSpriteInfo->entry->vFlip = false;

    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);

    mainSpriteInfo->entry->x = main_x;
    mainSpriteInfo->entry->y = main_y;

    subSpriteInfo->entry->x = sub_x;
    subSpriteInfo->entry->y = sub_y;

    trigger_timer += *global::timer;

    if (trigger_timer > 200)
        anim_frame_timer += *global::timer;

    if (anim_frame_timer > ANIM_FRAME_DELTA) {
        anim_frame_timer = 0;
        anim_frame++;

        if (anim_frame >= 20) {
            anim_frame = 0;
            trigger_timer = 0;
        }

        u8 *frameGfx = (u8 *) gfx_goldbarsTiles +
                       ((SHOPPING_ICON_OFFSET + anim_frame) * SHOPPING_ICON_SPRITE_WIDTH * SHOPPING_ICON_SPRITE_HEIGHT /
                        2);
        mainSpriteInfo->updateFrame(frameGfx, SHOPPING_ICON_SPRITE_WIDTH * SHOPPING_ICON_SPRITE_HEIGHT);
        subSpriteInfo->updateFrame(frameGfx, SHOPPING_ICON_SPRITE_WIDTH * SHOPPING_ICON_SPRITE_HEIGHT);
    }

}

void ShoppingIcon::init() {
    initSprite();
    mainSpriteInfo->entry->hFlip = false;
    subSpriteInfo->entry->hFlip = false;
    trigger_timer = rand() % 300;
}

void ShoppingIcon::initSprite() {

    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_goldbarsPal, gfx_goldbarsPalLen,
                                                          nullptr, SHOPPING_ICON_SPRITE_WIDTH *
                                                                   SHOPPING_ICON_SPRITE_HEIGHT, 16,
                                                          SpritesheetType::MONIEZ_GOLDBARS,
                                                          true,
                                                          false, LAYER_LEVEL::MIDDLE_TOP);
    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_goldbarsPal, gfx_goldbarsPalLen,
                                                        nullptr, SHOPPING_ICON_SPRITE_WIDTH *
                                                                 SHOPPING_ICON_SPRITE_HEIGHT, 16,
                                                        SpritesheetType::MONIEZ_GOLDBARS,
                                                        true,
                                                        false, LAYER_LEVEL::MIDDLE_TOP);

    u8 *frameGfx = (u8 *) gfx_goldbarsTiles +
                   ((SHOPPING_ICON_OFFSET + anim_frame) * SHOPPING_ICON_SPRITE_WIDTH * SHOPPING_ICON_SPRITE_HEIGHT / 2);
    mainSpriteInfo->updateFrame(frameGfx, SHOPPING_ICON_SPRITE_WIDTH * SHOPPING_ICON_SPRITE_HEIGHT);
    subSpriteInfo->updateFrame(frameGfx, SHOPPING_ICON_SPRITE_WIDTH * SHOPPING_ICON_SPRITE_HEIGHT);

}


ShoppingIcon::ShoppingIcon() {
    physical_height = SHOPPING_ICON_PHYSICAL_HEIGHT;
    physical_width = SHOPPING_ICON_PHYSICAL_WIDTH;
    sprite_height = SHOPPING_ICON_SPRITE_HEIGHT;
    sprite_width = SHOPPING_ICON_SPRITE_WIDTH;
}
