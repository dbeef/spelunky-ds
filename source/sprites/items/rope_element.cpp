//
// Created by xdbeef on 04.04.18.
//

#include "../../globals_declarations.h"
#include "../../../build/gfx_blood_rock_rope.h"
#include "rope_element.h";


void RopeElement::draw() {

    int main_x = x - global::camera->x;
    int main_y = y - global::camera->y;
    int sub_x = x - global::camera->x;
    int sub_y = y - global::camera->y - 192;


    if (global::camera->y + 192 > this->y + 8 || global::camera->y + 192 + 192 < this->y - 8) {
        sub_x = -128;
        sub_y = -128;
    }
    if (global::camera->y > this->y + 8 || global::camera->y + 192 < this->y - 8) {
        main_x = -128;
        main_y = -128;
    }

    mainSpriteInfo->entry->x = main_x;
    mainSpriteInfo->entry->y = main_y;

    subSpriteInfo->entry->x = sub_x;
    subSpriteInfo->entry->y = sub_y;

    mainSpriteInfo_2->entry->x = main_x;
    mainSpriteInfo_2->entry->y = main_y + 8;

    subSpriteInfo_2->entry->x = sub_x;
    subSpriteInfo_2->entry->y = sub_y + 8;

    mainSpriteInfo->entry->isHidden = false;
    mainSpriteInfo_2->entry->isHidden = false;

    subSpriteInfo->entry->isHidden = false;
    subSpriteInfo_2->entry->isHidden = false;


}


void RopeElement::init() {

    initSprite();
}

void RopeElement::updateSpeed() {
}

void RopeElement::update_position() {
}

void RopeElement::updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) {
}

void RopeElement::initSprite() {

    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_blood_rock_ropePal, gfx_blood_rock_ropePalLen,
                                                        nullptr, 8 * 8, 8, BLOOD_ROCK_ROPE, true, false,
                                                        LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_blood_rock_ropePal, gfx_blood_rock_ropePalLen,
                                                          nullptr, 8 * 8, 8, BLOOD_ROCK_ROPE, true, false,
                                                          LAYER_LEVEL::MIDDLE_TOP);

    subSpriteInfo_2 = global::sub_oam_manager->initSprite(gfx_blood_rock_ropePal, gfx_blood_rock_ropePalLen,
                                                          nullptr, 8 * 8, 8, BLOOD_ROCK_ROPE, true, false,
                                                          LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo_2 = global::main_oam_manager->initSprite(gfx_blood_rock_ropePal, gfx_blood_rock_ropePalLen,
                                                            nullptr, 8 * 8, 8, BLOOD_ROCK_ROPE, true, false,
                                                            LAYER_LEVEL::MIDDLE_TOP);

    frameGfx = (u8 *) gfx_blood_rock_ropeTiles + 8 * 8 * 8 / 2;

    subSpriteInfo->updateFrame(frameGfx, 8 * 8);
    mainSpriteInfo->updateFrame(frameGfx, 8 * 8);
    subSpriteInfo_2->updateFrame(frameGfx, 8 * 8);
    mainSpriteInfo_2->updateFrame(frameGfx, 8 * 8);


}

