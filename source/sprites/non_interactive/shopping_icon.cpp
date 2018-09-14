//
// Created by xdbeef on 17.06.18.
//

#include <cstdlib>
#include "../../globals_declarations.h"
#include "../../../build/gfx_goldbars.h"
#include "shopping_icon.h"
#include "../sprite_utils.h"

#define SHOPPING_ICON_OFFSET 12
#define ANIM_FRAME_DELTA 25

void ShoppingIcon::draw() {

    if (ready_to_dispose)
        return;

    sprite_utils::set_visibility(true, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    set_position();

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

        match_animation();
    }

}

void ShoppingIcon::init() {
    initSprite();
    mainSpriteInfo->entry->hFlip = false;
    subSpriteInfo->entry->hFlip = false;
    trigger_timer = rand() % 300;
}

void ShoppingIcon::initSprite() {

    delete mainSpriteInfo;
    delete subSpriteInfo;

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

    match_animation();
    sprite_utils::set_visibility(true, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    set_position();
}


ShoppingIcon::ShoppingIcon() {
    physical_height = SHOPPING_ICON_PHYSICAL_HEIGHT;
    physical_width = SHOPPING_ICON_PHYSICAL_WIDTH;
    sprite_height = SHOPPING_ICON_SPRITE_HEIGHT;
    sprite_width = SHOPPING_ICON_SPRITE_WIDTH;
}

void ShoppingIcon::set_position() {
    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);
    sprite_utils::set_entry_xy(mainSpriteInfo, main_x, main_y);
    sprite_utils::set_entry_xy(subSpriteInfo, sub_x, sub_y);
}

void ShoppingIcon::match_animation() {
    u8 *frameGfx = sprite_utils::get_frame((u8 *) gfx_goldbarsTiles, SHOPPING_ICON_SPRITE_SIZE,
                                           SHOPPING_ICON_OFFSET + anim_frame);
    sprite_utils::update_frame(frameGfx, SHOPPING_ICON_SPRITE_SIZE, mainSpriteInfo, subSpriteInfo);
}

void ShoppingIcon::set_ready_to_dispose() {
    ready_to_dispose = true;
    if (mainSpriteInfo != nullptr && subSpriteInfo != nullptr)
        sprite_utils::set_visibility(false, mainSpriteInfo, subSpriteInfo);
}
