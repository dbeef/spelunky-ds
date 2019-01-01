//
// Created by xdbeef on 29.07.18.
//

#include <cmath>
#include "../../decorations/GotCollectible.hpp"
#include "../../GlobalsDeclarations.hpp"
#include "../../../build/gfx_spider_skeleton.h"
#include "../../collisions/Collisions.hpp"
#include "Skull.hpp"
#include "Bone.hpp"
#include "../SpriteUtils.hpp"

#define SKULL_POS_INC_DELTA 22
#define SKULL_ANIM_FRAME_DELTA  45
#define COLLIDED_FRAMES 7
#define FRAME_OFFSET_SKULL 24
#define FRAME_OFFSET_COLLIDED 33

void Skull::update_creature_specific() {

    if (_ready_to_dispose) return;

    sprite_utils::set_visibility(true, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_priority(OBJPRIORITY_0, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    update_sprites_position();

    if (hold_by_main_dude)
        sprite_utils::set_horizontal_flip(global::main_dude->sprite_state == Orientation::RIGHT,
                                          mainSpriteInfo, subSpriteInfo);

    set_pickuped_position(0, 3);

    if (collided)
        animFrameTimer += *global::timer;

    if (animFrameTimer > SKULL_ANIM_FRAME_DELTA) {

        animFrameTimer = 0;
        animFrame++;

        match_animation();
    }
    
    if(_map_collisions_checked) {


        if (((_upper_collision || _bottom_collision || _left_collision || _right_collision)
             && (fabs(_x_speed) > 0 || fabs(_y_speed) > 0)) && !collided) {

            collided = true;
            animFrame = 0;

            auto b = new Bone(_x, _y - 5);
            b->_x_speed = 0;
            b->_y_speed = -1.4f;
            b->animFrame = 2;
            global::creatures_to_add.push_back(b);
        }

        _map_collisions_checked = false;
    }
}

void Skull::init_sprites() {

    delete mainSpriteInfo;
    delete subSpriteInfo;

    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_spider_skeletonPal, gfx_spider_skeletonPalLen,
                                                        nullptr, _sprite_size, ObjSize::OBJSIZE_16,
                                                        _spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_spider_skeletonPal, gfx_spider_skeletonPalLen,
                                                          nullptr, _sprite_size, ObjSize::OBJSIZE_16,
                                                          _spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
    match_animation();
    sprite_utils::set_visibility(true, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_priority(OBJPRIORITY_0, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    update_sprites_position();

    if (hold_by_main_dude)
        sprite_utils::set_horizontal_flip(global::main_dude->sprite_state == Orientation::RIGHT,
                                          mainSpriteInfo, subSpriteInfo);

}

void Skull::update_sprites_position() {
    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);
    sprite_utils::set_entry_xy(mainSpriteInfo, main_x, main_y);
    sprite_utils::set_entry_xy(subSpriteInfo, sub_x, sub_y);
}

void Skull::match_animation() {

    if (collided) {

        if (animFrame >= COLLIDED_FRAMES) {
            _ready_to_dispose = true;
            sprite_utils::set_visibility(false, mainSpriteInfo, subSpriteInfo);
        }

        frameGfx = sprite_utils::get_frame((u8 *) gfx_spider_skeletonTiles, _sprite_size,
                                           animFrame + FRAME_OFFSET_COLLIDED);

    } else
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spider_skeletonTiles, _sprite_size,
                                           FRAME_OFFSET_SKULL);

    sprite_utils::update_frame(frameGfx, _sprite_size, mainSpriteInfo, subSpriteInfo);
}

void Skull::delete_sprites() {
    delete mainSpriteInfo;
    delete subSpriteInfo;
    mainSpriteInfo = nullptr;
    subSpriteInfo = nullptr;
}
