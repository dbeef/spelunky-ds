//
// Created by xdbeef on 10.06.18.
//


#include "smooch.h"
#include "got_collectible.h"
#include "../../../build/gfx_caveman_damsel.h"
#include "../../globals_declarations.h"

#define SMOOCH_CYCLES_TO_HIDE 25
#define SMOOCH_ANIM_FRAME_DELTA 70

void Smooch::draw() {


    if (ready_to_dispose) {
        mainSpriteInfo->entry->isHidden = true;
        subSpriteInfo->entry->isHidden = true;
        return;
    } else {
        mainSpriteInfo->entry->isHidden = false;
        subSpriteInfo->entry->isHidden = false;
    }


    animFrameTimer += *global::timer;

    if (animFrameTimer > SMOOCH_ANIM_FRAME_DELTA) {

        animFrameTimer = 0;
        animFrame++;
        cycle_counter++;

        y--;

        if (animFrame == 2)
            animFrame = 0;

        if (cycle_counter > SMOOCH_CYCLES_TO_HIDE)
            ready_to_dispose = true;

        frameGfx = (u8 *) gfx_caveman_damselTiles +
                   (sprite_width * sprite_height * (animFrame + 67) / 2);
        subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
        mainSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);

    }

    set_position();
}


void Smooch::init() {
    initSprite();
}

void Smooch::initSprite() {

    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_caveman_damselPal, gfx_caveman_damselPalLen,
                                                        nullptr, sprite_width * sprite_height, sprite_width,
                                                        spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_caveman_damselPal, gfx_caveman_damselPalLen,
                                                          nullptr, sprite_width * sprite_height, sprite_width,
                                                          spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);


    frameGfx = (u8 *) gfx_caveman_damselTiles + (sprite_width * sprite_height * (67) / 2);
    subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
    mainSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);


}

void Smooch::set_position() {

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

Smooch::Smooch() {
    sprite_height = SMOOCH_SPRITE_HEIGHT;
    sprite_width = SMOOCH_SPRITE_WIDTH;
    spritesheet_type = SpritesheetType::CAVEMAN_DAMSEL;
}

