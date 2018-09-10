//
// Created by xdbeef on 04.04.18.
//

#include "../../globals_declarations.h"
#include "../../../build/gfx_blood_rock_rope_poof.h"
#include "rope_element.h"
#include "rope.h"

void RopeElement::draw() {

    if (!active) {
        mainSpriteInfo->entry->isHidden = true;
        mainSpriteInfo_2->entry->isHidden = true;
        subSpriteInfo->entry->isHidden = true;
        subSpriteInfo_2->entry->isHidden = true;
        return;
    }


    if (ready_to_dispose) {
        mainSpriteInfo->entry->isHidden = true;
        subSpriteInfo->entry->isHidden = true;
        return;
    } else {
        mainSpriteInfo->entry->isHidden = false;
        subSpriteInfo->entry->isHidden = false;
    }

    if (active_timer < 50)
        active_timer += *global::timer;

    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);

    mainSpriteInfo->entry->x = main_x;
    mainSpriteInfo->entry->y = main_y;

    subSpriteInfo->entry->x = sub_x;
    subSpriteInfo->entry->y = sub_y;

    mainSpriteInfo_2->entry->x = main_x;
    mainSpriteInfo_2->entry->y = main_y + sprite_height;

    subSpriteInfo_2->entry->x = sub_x;
    subSpriteInfo_2->entry->y = sub_y + sprite_height;

    if (active_timer > 20) {
        mainSpriteInfo_2->entry->isHidden = false;
        subSpriteInfo_2->entry->isHidden = false;
    }

    mainSpriteInfo->entry->isHidden = false;
    subSpriteInfo->entry->isHidden = false;

}


void RopeElement::init() {
    initSprite();
}

void RopeElement::initSprite() {

    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_blood_rock_rope_poofPal, gfx_blood_rock_rope_poofPalLen,
                                                        nullptr, sprite_width * sprite_height, 8, BLOOD_ROCK_ROPE_POOF, true,
                                                        false,
                                                        LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_blood_rock_rope_poofPal, gfx_blood_rock_rope_poofPalLen,
                                                          nullptr, sprite_width * sprite_height, 8, BLOOD_ROCK_ROPE_POOF,
                                                          true, false,
                                                          LAYER_LEVEL::MIDDLE_TOP);

    subSpriteInfo_2 = global::sub_oam_manager->initSprite(gfx_blood_rock_rope_poofPal, gfx_blood_rock_rope_poofPalLen,
                                                          nullptr, sprite_width * sprite_height, 8, BLOOD_ROCK_ROPE_POOF,
                                                          true, false,
                                                          LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo_2 = global::main_oam_manager->initSprite(gfx_blood_rock_rope_poofPal, gfx_blood_rock_rope_poofPalLen,
                                                            nullptr, sprite_width * sprite_height, 8, BLOOD_ROCK_ROPE_POOF,
                                                            true, false,
                                                            LAYER_LEVEL::MIDDLE_TOP);

    frameGfx = (u8 *) gfx_blood_rock_rope_poofTiles + 8 * sprite_width * sprite_height / 2;

    subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
    mainSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
    subSpriteInfo_2->updateFrame(frameGfx, sprite_width * sprite_height);
    mainSpriteInfo_2->updateFrame(frameGfx, sprite_width * sprite_height);

}

RopeElement::RopeElement() {
    physical_height = ROPE_PHYSICAL_HEIGHT;
    physical_width = ROPE_PHYSICAL_WIDTH;
    sprite_height = ROPE_SPRITE_HEIGHT;
    sprite_width = ROPE_SPRITE_WIDTH;
}