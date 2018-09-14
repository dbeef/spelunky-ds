//
// Created by xdbeef on 19.05.18.
//

#include "got_collectible.h"
#include "../../globals_declarations.h"
#include "../../../build/gfx_got_collectible.h"
#include "../sprite_utils.h"

#define GOT_COLLECTIBLE_CYCLES_TO_HIDE 10
#define GOT_COLLECTIBLE_ANIM_FRAME_DELTA 90

void GotCollectible::draw() {

    if (ready_to_dispose)
        return;

    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);
    set_position();

    animFrameTimer += *global::timer;

    if (animFrameTimer > GOT_COLLECTIBLE_ANIM_FRAME_DELTA) {

        animFrameTimer = 0;
        animFrame++;
        cycle_counter++;

        if (animFrame == 2)
            animFrame = 0;

        if (cycle_counter > GOT_COLLECTIBLE_CYCLES_TO_HIDE) {
            ready_to_dispose = true;
            sprite_utils::set_visibility(false, mainSpriteInfo, subSpriteInfo);
        } else
            match_animation();

    }

}


void GotCollectible::init() {
    initSprite();
}

void GotCollectible::initSprite() {

    delete mainSpriteInfo;
    delete subSpriteInfo;

    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_got_collectiblePal, gfx_got_collectiblePalLen,
                                                        nullptr, GOT_COLLECTIBLE_SPRITE_SIZE, sprite_width,
                                                        spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_got_collectiblePal, gfx_got_collectiblePalLen,
                                                          nullptr, GOT_COLLECTIBLE_SPRITE_SIZE, sprite_width,
                                                          spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);

    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);
    set_position();
    match_animation();

}

void GotCollectible::set_position() {
    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);
    sprite_utils::set_entry_xy(mainSpriteInfo, main_x, main_y);
    sprite_utils::set_entry_xy(subSpriteInfo, sub_x, sub_y);
}

GotCollectible::GotCollectible() {
    sprite_height = GOT_COLLECTIBLE_SPRITE_HEIGHT;
    sprite_width = GOT_COLLECTIBLE_SPRITE_WIDTH;
    spritesheet_type = SpritesheetType::GOT_COLLECTIBLE;
}

void GotCollectible::match_animation() {
    frameGfx = sprite_utils::get_frame((u8 *) gfx_got_collectibleTiles, GOT_COLLECTIBLE_SPRITE_SIZE,
                                       (collectible_type * 2) + animFrame);
    sprite_utils::update_frame(frameGfx, GOT_COLLECTIBLE_SPRITE_SIZE, mainSpriteInfo, subSpriteInfo);
}

