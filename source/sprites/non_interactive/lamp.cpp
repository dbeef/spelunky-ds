//
// Created by xdbeef on 14.06.18.
//


#include "lamp.h"
#include "../../globals_declarations.h"
#include "../../../build/gfx_shopkeeper.h"
#include "../sprite_utils.h"

#define LAMP_OFFSET 25
#define ANIM_FRAME_DELTA 60

void Lamp::draw() {

    set_position();
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);

    anim_frame_timer += *global::timer;

    if (anim_frame_timer > ANIM_FRAME_DELTA) {

        anim_frame_timer = 0;
        anim_frame++;

        if (anim_frame >= 3)
            anim_frame = 0;

        match_animation();
    }

}

void Lamp::init() {
    initSprite();
}


void Lamp::initSprite() {

    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_shopkeeperPal, gfx_shopkeeperPalLen, nullptr,
                                                          LAMP_SPRITE_WIDTH * LAMP_SPRITE_HEIGHT, 16,
                                                          SpritesheetType::SHOPKEEPER, true,
                                                          false, LAYER_LEVEL::MIDDLE_TOP);
    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_shopkeeperPal, gfx_shopkeeperPalLen, nullptr,
                                                        LAMP_SPRITE_WIDTH * LAMP_SPRITE_HEIGHT, 16,
                                                        SpritesheetType::SHOPKEEPER, true,
                                                        false, LAYER_LEVEL::MIDDLE_TOP);
    match_animation();
    set_position();
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
}


Lamp::Lamp() {
    physical_height = LAMP_PHYSICAL_HEIGHT;
    physical_width = LAMP_PHYSICAL_WIDTH;
    sprite_height = LAMP_SPRITE_HEIGHT;
    sprite_width = LAMP_SPRITE_WIDTH;
}

void Lamp::set_position() {
    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);
    sprite_utils::set_entry_xy(mainSpriteInfo, main_x, main_y);
    sprite_utils::set_entry_xy(subSpriteInfo, sub_x, sub_y);
}

void Lamp::match_animation() {
    frameGfx = sprite_utils::get_frame((u8 *) gfx_shopkeeperTiles, LAMP_SPRITE_SIZE, LAMP_OFFSET + anim_frame);
    sprite_utils::update_frame(frameGfx, LAMP_SPRITE_SIZE, mainSpriteInfo, subSpriteInfo);
    mainSpriteInfo->updateFrame(frameGfx, LAMP_SPRITE_SIZE);
    subSpriteInfo->updateFrame(frameGfx, LAMP_SPRITE_SIZE);

}
