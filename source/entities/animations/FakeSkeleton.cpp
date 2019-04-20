//
// Created by xdbeef on 29.07.18.
//

#include <cmath>
#include "../../entities/decorations/GotCollectible.hpp"
#include "../../GlobalsDeclarations.hpp"
#include "../../../build/gfx_spider_skeleton.h"
#include "../../collisions/Collisions.hpp"
#include "FakeSkeleton.hpp"
#include "../items/Skull.hpp"
#include "../../memory/SpriteUtils.hpp"

#define FAKE_SKELETON_POS_INC_DELTA 25
#define FRAME_OFFSET_FAKE_SKELETON_WITH_SKULL 33
#define FRAME_OFFSET_FAKE_SKELETON_WOUT_SKULL 23

//Skull object is spawned only when main dude attempts to pick it up,
//before that a single sprite is drawn that has both skeleton and skull. (saving some memory)

void FakeSkeleton::update_creature_specific() {

    if (_ready_to_dispose)
        return;

    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_priority(OBJPRIORITY_0, mainSpriteInfo, subSpriteInfo);
    update_sprites_position();

    if (!tried_to_pickup) {

        if (global::input_handler->y_key_down &&
            global::input_handler->down_key_held &&
            !global::main_dude->holding_item &&
            Collisions::checkCollisionWithMainDude(_x, _y, _physical_width, _physical_height)) {

            //pickup item from the ground

            global::main_dude->holding_item = true;
            global::input_handler->y_key_down = false;

            tried_to_pickup = true;
            match_animation();
            spawn_skull();
            _physical_height = FAKE_SKELETON_PHYSICAL_HEIGHT_WOUT_SKULL;
            _y += 2;
        }

    }

}

void FakeSkeleton::init_sprites() {

    delete_sprites();

    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_spider_skeletonPal, gfx_spider_skeletonPalLen,
                                                        nullptr, _sprite_size, ObjSize::OBJSIZE_16,
                                                        _spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_spider_skeletonPal, gfx_spider_skeletonPalLen,
                                                          nullptr, _sprite_size, ObjSize::OBJSIZE_16,
                                                          _spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_priority(OBJPRIORITY_0, mainSpriteInfo, subSpriteInfo);
    update_sprites_position();
    match_animation();
}

void FakeSkeleton::update_sprites_position() {
    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);
    sprite_utils::set_entry_xy(mainSpriteInfo, main_x, main_y);
    sprite_utils::set_entry_xy(subSpriteInfo, sub_x, sub_y);
}

void FakeSkeleton::spawn_skull() {
    auto s = new Skull(_x, _y);
    s->_hold_by_main_dude = true;
    global::main_dude->_currently_held_item = s;
    global::items.push_back(s);
}

void FakeSkeleton::match_animation() {

    if (tried_to_pickup)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spider_skeletonTiles, _sprite_size,
                                           animFrame + FRAME_OFFSET_FAKE_SKELETON_WOUT_SKULL);
    else
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spider_skeletonTiles, _sprite_size,
                                           animFrame + FRAME_OFFSET_FAKE_SKELETON_WITH_SKULL);

    sprite_utils::update_frame(frameGfx, _sprite_size, mainSpriteInfo, subSpriteInfo);

}

void FakeSkeleton::delete_sprites() {
    delete mainSpriteInfo;
    delete subSpriteInfo;
    mainSpriteInfo = nullptr;
    subSpriteInfo = nullptr;
}
