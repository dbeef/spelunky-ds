//
// Created by xdbeef on 19.05.18.
//

#include "got_collectible.h"
#include "../../globals_declarations.h"
#include "../../../build/gfx_got_collectible.h"

#define GOT_COLLECTIBLE_CYCLES_TO_HIDE 10
#define GOT_COLLECTIBLE_ANIM_FRAME_DELTA 90

void GotCollectible::draw() {


    if (ready_to_dispose) {
        mainSpriteInfo->entry->isHidden = true;
        subSpriteInfo->entry->isHidden = true;
        return;
    } else {
        mainSpriteInfo->entry->isHidden = false;
        subSpriteInfo->entry->isHidden = false;
    }


    animFrameTimer += *global::timer;

    if (animFrameTimer > GOT_COLLECTIBLE_ANIM_FRAME_DELTA) {

        animFrameTimer = 0;
        animFrame++;
        cycle_counter++;

        if (animFrame == 2)
            animFrame = 0;

        if (cycle_counter > GOT_COLLECTIBLE_CYCLES_TO_HIDE)
            ready_to_dispose = true;

        frameGfx = (u8 *) gfx_got_collectibleTiles +
                   (sprite_width * sprite_height * ((collectible_type * 2) + animFrame) / 2);
        subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
        mainSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);

    }

    set_position();
}


void GotCollectible::init() {
    initSprite();
}

void GotCollectible::initSprite() {

    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_got_collectiblePal, gfx_got_collectiblePalLen,
                                                        nullptr, sprite_width * sprite_height, sprite_width,
                                                        spriteType, true, false,LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_got_collectiblePal, gfx_got_collectiblePalLen,
                                                          nullptr, sprite_width * sprite_height, sprite_width,
                                                          spriteType, true, false,LAYER_LEVEL::MIDDLE_TOP);

    if (collectible_type == 0) {
        frameGfx = (u8 *) gfx_got_collectibleTiles + (sprite_width * sprite_height * (0) / 2);
    } else if (collectible_type == 1) {
        frameGfx = (u8 *) gfx_got_collectibleTiles + (sprite_width * sprite_height * (2) / 2);
    } else if (collectible_type == 2) {
        frameGfx = (u8 *) gfx_got_collectibleTiles + (sprite_width * sprite_height * (4) / 2);
    }

    subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
    mainSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);


}

void GotCollectible::set_position() {

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

}

GotCollectible::GotCollectible() {
    sprite_height = GOT_COLLECTIBLE_SPRITE_HEIGHT;
    sprite_width = GOT_COLLECTIBLE_SPRITE_WIDTH;
    spriteType = SpritesheetType::GOT_COLLECTIBLE;
}

