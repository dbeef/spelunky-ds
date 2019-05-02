//
// Created by xdbeef on 29.07.18.
//

#include "../../entities/decorations/GotCollectible.hpp"
#include "../../GameState.hpp"
#include "../../../build/gfx_spider_skeleton.h"
#include "../../collisions/Collisions.hpp"
#include "Bone.hpp"

#include "../../time/Timer.h"
#include "../../graphics/SpriteUtils.hpp"

#define BONE_POS_INC_DELTA 35
#define BONE_ANIM_FRAME_DELTA  45
#define BONE_FRAMES 8
#define COLLIDED_FRAMES 7
#define FRAME_OFFSET_BONE 25
#define FRAME_OFFSET_COLLIDED 33

void Bone::update_creature_specific() {

    if (_ready_to_dispose) return;

    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_priority(OBJPRIORITY_0, mainSpriteInfo, subSpriteInfo);
    update_sprites_position();

    animFrameTimer += Timer::getDeltaTime();

    if (animFrameTimer > BONE_ANIM_FRAME_DELTA) {
        animFrameTimer = 0;
        animFrame++;
        match_animation();
    }
    
    if(_map_collisions_checked){
        
        if (_upper_collision || _bottom_collision || _left_collision || _right_collision) {
            collided = true;
            animFrame = 0;
        }
        
        _map_collisions_checked = false;
    }

}

void Bone::init_sprites() {

    delete_sprites();

    subSpriteInfo = GameState::instance().sub_oam_manager->initSprite(gfx_spider_skeletonPal, gfx_spider_skeletonPalLen,
                                                        nullptr, _sprite_size, ObjSize::OBJSIZE_16,
                                                        _spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = GameState::instance().main_oam_manager->initSprite(gfx_spider_skeletonPal, gfx_spider_skeletonPalLen,
                                                          nullptr, _sprite_size, ObjSize::OBJSIZE_16,
                                                          _spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_priority(OBJPRIORITY_0, mainSpriteInfo, subSpriteInfo);
    match_animation();
    update_sprites_position();
}

void Bone::update_sprites_position() {
    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);
    sprite_utils::set_entry_xy(mainSpriteInfo, main_x, main_y);
    sprite_utils::set_entry_xy(subSpriteInfo, sub_x, sub_y);
}

void Bone::match_animation() {

    if (collided) {

        if (animFrame >= COLLIDED_FRAMES) {
            //poof animation done, bone is ready to dispose
            sprite_utils::set_visibility(false, mainSpriteInfo, subSpriteInfo);
            _ready_to_dispose = true;
        } else
            //continue poof animation
            frameGfx = sprite_utils::get_frame((u8 *) gfx_spider_skeletonTiles, _sprite_size,
                                               animFrame + FRAME_OFFSET_COLLIDED);

    } else {
        //rotating bone animation
        if (animFrame >= BONE_FRAMES)
            animFrame = 0;

        frameGfx = sprite_utils::get_frame((u8 *) gfx_spider_skeletonTiles, _sprite_size,
                                           animFrame + FRAME_OFFSET_BONE);
    }

    sprite_utils::update_frame(frameGfx, _sprite_size, mainSpriteInfo, subSpriteInfo);
}

void Bone::delete_sprites() {
    delete mainSpriteInfo;
    delete subSpriteInfo;
    mainSpriteInfo = nullptr;
    subSpriteInfo = nullptr;
}
