//
// Created by xdbeef on 04.04.18.
//

#include "../../globals_declarations.hpp"
#include "../../../build/gfx_blood_rock_rope_poof.h"
#include "rope_element.hpp"
#include "rope.hpp"
#include "../sprite_utils.hpp"

void RopeElement::draw() {

    if (ready_to_dispose) return;

    if (!active)
        sprite_utils::set_visibility(false, mainSpriteInfo, mainSpriteInfo_2, subSpriteInfo, subSpriteInfo_2);
    else {
        if (active_timer < 50)
            active_timer += *global::timer;
        set_sprite_attributes();
    }
}


void RopeElement::init() {
    initSprite();
}

void RopeElement::initSprite() {

    delete mainSpriteInfo;
    delete subSpriteInfo;
    delete mainSpriteInfo_2;
    delete subSpriteInfo_2;

    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_blood_rock_rope_poofPal, gfx_blood_rock_rope_poofPalLen,
                                                        nullptr, ROPE_SPRITE_SIZE, 8, BLOOD_ROCK_ROPE_POOF, true,
                                                        false, LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_blood_rock_rope_poofPal, gfx_blood_rock_rope_poofPalLen,
                                                          nullptr, ROPE_SPRITE_SIZE, 8, BLOOD_ROCK_ROPE_POOF,
                                                          true, false, LAYER_LEVEL::MIDDLE_TOP);
    subSpriteInfo_2 = global::sub_oam_manager->initSprite(gfx_blood_rock_rope_poofPal, gfx_blood_rock_rope_poofPalLen,
                                                          nullptr, ROPE_SPRITE_SIZE, 8, BLOOD_ROCK_ROPE_POOF,
                                                          true, false, LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo_2 = global::main_oam_manager->initSprite(gfx_blood_rock_rope_poofPal, gfx_blood_rock_rope_poofPalLen,
                                                            nullptr, ROPE_SPRITE_SIZE, 8, BLOOD_ROCK_ROPE_POOF,
                                                            true, false, LAYER_LEVEL::MIDDLE_TOP);

    frameGfx = sprite_utils::get_frame((u8 *) gfx_blood_rock_rope_poofTiles, ROPE_SPRITE_SIZE, 8);
    sprite_utils::update_frame(frameGfx, ROPE_SPRITE_SIZE, mainSpriteInfo,
                               mainSpriteInfo_2, subSpriteInfo, subSpriteInfo_2);
    set_sprite_attributes();
}

RopeElement::RopeElement() {
    physical_height = ROPE_PHYSICAL_HEIGHT;
    physical_width = ROPE_PHYSICAL_WIDTH;
    sprite_height = ROPE_SPRITE_HEIGHT;
    sprite_width = ROPE_SPRITE_WIDTH;
}

void RopeElement::set_position() {
    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);
    sprite_utils::set_entry_xy(mainSpriteInfo, main_x, main_y);
    sprite_utils::set_entry_xy(subSpriteInfo, sub_x, sub_y);
    sprite_utils::set_entry_xy(mainSpriteInfo_2, main_x, main_y + ROPE_SPRITE_HEIGHT);
    sprite_utils::set_entry_xy(subSpriteInfo_2, sub_x, sub_y + ROPE_SPRITE_HEIGHT);
}

void RopeElement::set_visibility() {

    if (active_timer > 20)
        sprite_utils::set_visibility(true, mainSpriteInfo_2, subSpriteInfo_2);

    sprite_utils::set_visibility(true, mainSpriteInfo, subSpriteInfo);
}

void RopeElement::set_sprite_attributes() {
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, mainSpriteInfo_2, subSpriteInfo, subSpriteInfo_2);
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, mainSpriteInfo_2, subSpriteInfo, subSpriteInfo_2);
    set_position();
    set_visibility();
}
