//
// Created by xdbeef on 14.06.18.
//


#include "lamp.h"
#include "../../globals_declarations.h"
#include "../../../build/gfx_shopkeeper.h"

#define LAMP_OFFSET 25
#define ANIM_FRAME_DELTA 60

void Lamp::draw() {

    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);

    mainSpriteInfo->entry->hFlip = false;
    subSpriteInfo->entry->hFlip = false;
    mainSpriteInfo->entry->vFlip = false;
    subSpriteInfo->entry->vFlip = false;

    mainSpriteInfo->entry->x = main_x;
    mainSpriteInfo->entry->y = main_y;

    subSpriteInfo->entry->x = sub_x;
    subSpriteInfo->entry->y = sub_y;

    anim_frame_timer += *global::timer;

    if (anim_frame_timer > ANIM_FRAME_DELTA){
       anim_frame_timer = 0;
       anim_frame++;

       if(anim_frame >= 3)
           anim_frame = 0;

        u8 *frameGfx = (u8 *) gfx_shopkeeperTiles + ((LAMP_OFFSET + anim_frame) * LAMP_SPRITE_WIDTH * LAMP_SPRITE_HEIGHT / 2);
        mainSpriteInfo->updateFrame(frameGfx, LAMP_SPRITE_WIDTH * LAMP_SPRITE_HEIGHT);
        subSpriteInfo->updateFrame(frameGfx, LAMP_SPRITE_WIDTH * LAMP_SPRITE_HEIGHT);
    }

}

void Lamp::init() {
    initSprite();
    mainSpriteInfo->entry->hFlip = false;
    subSpriteInfo->entry->hFlip = false;
}

void Lamp::initSprite() {

        mainSpriteInfo = global::main_oam_manager->initSprite(gfx_shopkeeperPal, gfx_shopkeeperPalLen,
                                                                nullptr, LAMP_SPRITE_WIDTH *
                                                                         LAMP_SPRITE_HEIGHT, 16,
                                                                SpritesheetType::SHOPKEEPER,
                                                                true,
                                                                false, LAYER_LEVEL::MIDDLE_TOP);
        subSpriteInfo = global::sub_oam_manager->initSprite(gfx_shopkeeperPal, gfx_shopkeeperPalLen,
                                                                nullptr, LAMP_SPRITE_WIDTH *
                                                                         LAMP_SPRITE_HEIGHT, 16,
                                                                SpritesheetType::SHOPKEEPER,
                                                                true,
                                                                false, LAYER_LEVEL::MIDDLE_TOP);

        u8 *frameGfx = (u8 *) gfx_shopkeeperTiles + ((LAMP_OFFSET + anim_frame) * LAMP_SPRITE_WIDTH * LAMP_SPRITE_HEIGHT / 2);
        mainSpriteInfo->updateFrame(frameGfx, LAMP_SPRITE_WIDTH * LAMP_SPRITE_HEIGHT);
        subSpriteInfo->updateFrame(frameGfx, LAMP_SPRITE_WIDTH * LAMP_SPRITE_HEIGHT);

}


Lamp::Lamp(){
    physical_height = LAMP_PHYSICAL_HEIGHT;
    physical_width = LAMP_PHYSICAL_WIDTH;
    sprite_height = LAMP_SPRITE_HEIGHT;
    sprite_width = LAMP_SPRITE_WIDTH;
}
