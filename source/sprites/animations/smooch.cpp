//
// Created by xdbeef on 10.06.18.
//


#include "smooch.hpp"
#include "got_collectible.hpp"
#include "../../../build/gfx_caveman_damsel.h"
#include "../../globals_declarations.hpp"
#include "../sprite_utils.hpp"

#define SMOOCH_CYCLES_TO_HIDE 25
#define SMOOCH_ANIM_FRAME_DELTA 70

void Smooch::draw() {
    
    if (ready_to_dispose) 
        return;

    set_position();
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_visibility(true, mainSpriteInfo, subSpriteInfo);

    animFrameTimer += *global::timer;

    if (animFrameTimer > SMOOCH_ANIM_FRAME_DELTA) {

        animFrameTimer = 0;
        animFrame++;
        cycle_counter++;

        y--;

        if (animFrame == 2)
            animFrame = 0;

        if (cycle_counter > SMOOCH_CYCLES_TO_HIDE) {
            ready_to_dispose = true;
            sprite_utils::set_visibility(false, mainSpriteInfo, subSpriteInfo);
        }
            
        match_animation();
    }

}


void Smooch::init() {
    initSprite();
}

void Smooch::initSprite() {

    delete mainSpriteInfo;
    delete subSpriteInfo;

    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_caveman_damselPal, gfx_caveman_damselPalLen,
                                                        nullptr, SMOOCH_SPRITE_SIZE, sprite_width,
                                                        spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_caveman_damselPal, gfx_caveman_damselPalLen,
                                                          nullptr, SMOOCH_SPRITE_SIZE, sprite_width,
                                                          spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
    match_animation();
    set_position();
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_visibility(true, mainSpriteInfo, subSpriteInfo);
}

void Smooch::set_position() {
    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);
    sprite_utils::set_entry_xy(mainSpriteInfo, main_x, main_y);
    sprite_utils::set_entry_xy(subSpriteInfo, sub_x, sub_y);
}

Smooch::Smooch() {
    sprite_height = SMOOCH_SPRITE_HEIGHT;
    sprite_width = SMOOCH_SPRITE_WIDTH;
    spritesheet_type = SpritesheetType::CAVEMAN_DAMSEL;
}

void Smooch::match_animation() {
    frameGfx = sprite_utils::get_frame((u8 *) gfx_caveman_damselTiles, SMOOCH_SPRITE_SIZE, animFrame + 67);
    sprite_utils::update_frame(frameGfx, SMOOCH_SPRITE_SIZE, mainSpriteInfo, subSpriteInfo);
}

