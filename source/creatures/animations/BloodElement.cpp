//
// Created by xdbeef on 28.04.18.
//

#include "BloodElement.hpp"
#include "../../../build/gfx_blood_rock_rope_poof.h"
#include "../../GlobalsDeclarations.hpp"
#include "Blood.hpp"
#include "../../collisions/Collisions.hpp"
#include "../../tiles/LevelRenderingUtils.hpp"
#include "../SpriteUtils.hpp"

void BloodElement::update_creature_specific() {

    if (_ready_to_dispose || finished)
        return;

    //won't change speed untill this timer reaches treshold
    inactive_delay += *global::timer;

    //animation not finished
    frameTimer += *global::timer;

    if (frameTimer > BLOOD_ANIM_FRAME_DELTA) {

        frameTimer = 0;
        currentFrame++;

        if (currentFrame >= 7) {
            currentFrame = 0;
            finished = true;
            _ready_to_dispose = true;
            mainSpriteInfo->entry->isHidden = true;
            subSpriteInfo->entry->isHidden = true;

        } else
            match_animation();

    }

    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);
    update_sprites_position();

}

void BloodElement::init_sprites() {

    delete_sprites();

    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_blood_rock_rope_poofPal, gfx_blood_rock_rope_poofPalLen,
                                                        nullptr, _sprite_size, ObjSize::OBJSIZE_8, BLOOD_ROCK_ROPE_POOF,
                                                        true, false, LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_blood_rock_rope_poofPal, gfx_blood_rock_rope_poofPalLen,
                                                          nullptr, _sprite_size, ObjSize::OBJSIZE_8, BLOOD_ROCK_ROPE_POOF,
                                                          true, false, LAYER_LEVEL::MIDDLE_TOP);
    match_animation();
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);
    update_sprites_position();
}

void BloodElement::update_sprites_position() {
    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);
    sprite_utils::set_entry_xy(mainSpriteInfo, main_x, main_y);
    sprite_utils::set_entry_xy(subSpriteInfo, sub_x, sub_y);
}

void BloodElement::match_animation() {
    frameGfx = sprite_utils::get_frame((u8 *) gfx_blood_rock_rope_poofTiles, _sprite_size, currentFrame);
    sprite_utils::update_frame(frameGfx, _sprite_size, mainSpriteInfo, subSpriteInfo);
}

void BloodElement::delete_sprites() {
    delete mainSpriteInfo;
    delete subSpriteInfo;
    mainSpriteInfo = nullptr;
    subSpriteInfo = nullptr;
}
