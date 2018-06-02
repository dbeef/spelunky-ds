//
// Created by xdbeef on 01.06.18.
//

#include "got_collectible.h"
#include "../../globals_declarations.h"
#include "../../../build/gfx_blood_rock_rope_poof.h"
#include "../moving_object.h"
#include "fall_poof.h"

#define FALL_POOF_ANIM_FRAME_DELTA  50
#define FALL_POOF_FRAMES 6

void FallPoof::draw() {

    if (ready_to_dispose) {
        mainSpriteInfo->entry->isHidden = true;
        subSpriteInfo->entry->isHidden = true;
        return;
    }

    animFrameTimer += *global::timer;

    if (animFrameTimer > FALL_POOF_ANIM_FRAME_DELTA) {

        animFrameTimer = 0;
        animFrame++;

        if (animFrame >= FALL_POOF_FRAMES) {
            ready_to_dispose = true;
        } else {
            frameGfx = (u8 *) gfx_blood_rock_rope_poofTiles +
                       (sprite_width * sprite_height * (animFrame  + 13)/ 2);
            subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
            mainSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
        }
    }

    set_position();
}


void FallPoof::init() {
    initSprite();
}

void FallPoof::initSprite() {

    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_blood_rock_rope_poofPal, gfx_blood_rock_rope_poofPalLen,
                                                        nullptr, sprite_width * sprite_height, sprite_width,
                                                        spriteType, true, false, LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_blood_rock_rope_poofPal, gfx_blood_rock_rope_poofPalLen,
                                                          nullptr, sprite_width * sprite_height, sprite_width,
                                                          spriteType, true, false, LAYER_LEVEL::MIDDLE_TOP);


    frameGfx = (u8 *) gfx_blood_rock_rope_poofTiles +
               (sprite_width * sprite_height * (animFrame  + 13 )/ 2);

    subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
    mainSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);


}

void FallPoof::set_position() {

    if (ready_to_dispose)
        return;

    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);

    mainSpriteInfo->entry->x = main_x;
    mainSpriteInfo->entry->y = main_y;

    subSpriteInfo->entry->x = sub_x;
    subSpriteInfo->entry->y = sub_y;

    mainSpriteInfo->entry->vFlip = false;
    mainSpriteInfo->entry->hFlip = false;

    subSpriteInfo->entry->vFlip = false;
    subSpriteInfo->entry->hFlip = false;

    mainSpriteInfo->entry->priority = OBJPRIORITY_0;
    subSpriteInfo->entry->priority = OBJPRIORITY_0;

}

FallPoof::FallPoof() {
    sprite_height = FALL_POOF_SPRITE_HEIGHT;
    sprite_width = FALL_POOF_SPRITE_WIDTH;
    spriteType = SpritesheetType::BLOOD_ROCK_ROPE_POOF;
}
