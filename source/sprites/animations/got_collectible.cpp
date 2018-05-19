//
// Created by xdbeef on 19.05.18.
//


#include "got_collectible.h"
#include "../../globals_declarations.h"
#include "../../../build/gfx_got_collectible.h"

void GotCollectible::draw() {

    if (ready_to_dispose){
        mainSpriteInfo->entry->isHidden = true;
        mainSpriteInfo->entry->isHidden = true;
        return;
    }

    animFrameTimer += *global::timer;

    if (animFrameTimer > 125) {
        animFrameTimer = 0;
        animFrame++;
        cycle_counter++;

        if (animFrame == 2)
            animFrame = 0;

        if (cycle_counter > 10) {
            ready_to_dispose = true;
            mainSpriteInfo->entry->isHidden = true;
            mainSpriteInfo->entry->isHidden = true;
        }

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

void GotCollectible::updateSpeed() {

}

void GotCollectible::updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) {
}

void GotCollectible::initSprite() {


    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_got_collectiblePal, gfx_got_collectiblePalLen,
                                                        nullptr, sprite_width * sprite_height, sprite_width,
                                                        spriteType, true, false);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_got_collectiblePal, gfx_got_collectiblePalLen,
                                                          nullptr, sprite_width * sprite_height, sprite_width,
                                                          spriteType, true, false);

    if (collectible_type == 0) {
        frameGfx = (u8 *) gfx_got_collectibleTiles + (sprite_width * sprite_height * (0) / 2);
    } else if (collectible_type == 1) {
        frameGfx = (u8 *) gfx_got_collectibleTiles + (sprite_width * sprite_height * (2) / 2);
    } else if (collectible_type == 2) {
        frameGfx = (u8 *) gfx_got_collectibleTiles + (sprite_width * sprite_height * (4) / 2);
    }

    mainSpriteInfo->entry->x = x;
    subSpriteInfo->entry->y = y;

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

