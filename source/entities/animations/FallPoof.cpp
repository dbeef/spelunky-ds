//
// Created by xdbeef on 01.06.18.
//

#include "../../entities/decorations/GotCollectible.hpp"
#include "../../GameState.hpp"
#include "../../../build/gfx_blood_rock_rope_poof.h"
#include "../creatures/_BaseCreature.h"
#include "FallPoof.hpp"
#include "../../collisions/Collisions.hpp"
#include "../../memory/SpriteUtils.hpp"
#include "../../time/Timer.h"


#define FALL_POOF_POS_INC_DELTA 35
#define FALL_POOF_ANIM_FRAME_DELTA  50
#define FALL_POOF_FRAMES 6

void FallPoof::update_creature_specific() {

    if (_ready_to_dispose)
        return;

    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_priority(OBJPRIORITY_0, mainSpriteInfo, subSpriteInfo);
    update_sprites_position();

    animFrameTimer += Timer::getDeltaTime();

    if (animFrameTimer > FALL_POOF_ANIM_FRAME_DELTA) {

        animFrameTimer = 0;
        animFrame++;

        if (animFrame >= FALL_POOF_FRAMES) {
            _ready_to_dispose = true;
            sprite_utils::set_visibility(false, mainSpriteInfo, subSpriteInfo);
        } else
            match_animation();

    }

}


void FallPoof::init_sprites() {

    delete_sprites();

    subSpriteInfo = GameState::instance().sub_oam_manager->initSprite(gfx_blood_rock_rope_poofPal, gfx_blood_rock_rope_poofPalLen,
                                                        nullptr, FALL_POOF_SPRITE_SIZE, ObjSize::OBJSIZE_16,
                                                        _spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = GameState::instance().main_oam_manager->initSprite(gfx_blood_rock_rope_poofPal, gfx_blood_rock_rope_poofPalLen,
                                                          nullptr, FALL_POOF_SPRITE_SIZE, ObjSize::OBJSIZE_16,
                                                          _spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);

    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_priority(OBJPRIORITY_0, mainSpriteInfo, subSpriteInfo);
    match_animation();
    update_sprites_position();

}

void FallPoof::update_sprites_position() {
    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);
    sprite_utils::set_entry_xy(mainSpriteInfo, main_x, main_y);
    sprite_utils::set_entry_xy(subSpriteInfo, sub_x, sub_y);
}

void FallPoof::match_animation() {
    frameGfx = sprite_utils::get_frame((u8 *) gfx_blood_rock_rope_poofTiles, FALL_POOF_SPRITE_SIZE, animFrame + 13);
    sprite_utils::update_frame(frameGfx, FALL_POOF_SPRITE_SIZE, mainSpriteInfo, subSpriteInfo);
}

void FallPoof::delete_sprites() {
    delete mainSpriteInfo;
    delete subSpriteInfo;
    mainSpriteInfo = nullptr;
    subSpriteInfo = nullptr;
}
