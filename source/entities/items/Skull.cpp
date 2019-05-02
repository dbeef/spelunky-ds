//
// Created by xdbeef on 29.07.18.
//

#include <cmath>
#include "../../../build/gfx_spider_skeleton.h"
#include "../../GameState.hpp"
#include "../../collisions/Collisions.hpp"

#include "../animations/Bone.hpp"
#include "Skull.hpp"
#include "../../time/Timer.h"
#include "../../graphics/SpriteUtils.hpp"

#define SKULL_ANIM_FRAME_DELTA  45
#define COLLIDED_FRAMES 7
#define FRAME_OFFSET_SKULL 24
#define FRAME_OFFSET_COLLIDED 33

void Skull::update_item_specific() {

    if (_ready_to_dispose) return;

    update_sprites_position();

    if (_collided) {
        _anim_frame_timer += Timer::getDeltaTime();
        if (_anim_frame_timer > SKULL_ANIM_FRAME_DELTA) {
            _anim_frame_timer = 0;
            _anim_frame++;
            match_animation();
        }
        return;
    }

    kill_creatures_if_have_speed(1);

    if (_hold_by_main_dude) {
        sprite_utils::set_horizontal_flip(GameState::instance().main_dude->sprite_state == Orientation::RIGHT,
                                          _main_sprite_info, _sub_sprite_info);
        return;
    }

    if (_map_collisions_checked) {
        if (_upper_collision || _bottom_collision || _left_collision || _right_collision) {

            _collided = true;
            _anim_frame = 0;

            auto b = new Bone(_x, _y - 5);
            b->_x_speed = 0;
            b->_y_speed = -1.4f;
            b->animFrame = 2;
            GameState::instance().creatures.push_back(b);

        } else _map_collisions_checked = false;
    }
}

void Skull::init_sprites() {

    delete_sprites();

    _sub_sprite_info = OAMManager::sub().initSprite(gfx_spider_skeletonPal, gfx_spider_skeletonPalLen,
                                                           nullptr, _sprite_size, ObjSize::OBJSIZE_16,
                                                           _spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
    _main_sprite_info = OAMManager::main().initSprite(gfx_spider_skeletonPal, gfx_spider_skeletonPalLen,
                                                             nullptr, _sprite_size, ObjSize::OBJSIZE_16,
                                                             _spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
    match_animation();
    sprite_utils::set_visibility(true, _main_sprite_info, _sub_sprite_info);
    sprite_utils::set_priority(OBJPRIORITY_0, _main_sprite_info, _sub_sprite_info);
    sprite_utils::set_vertical_flip(false, _main_sprite_info, _sub_sprite_info);
    update_sprites_position();

    if (_hold_by_main_dude)
        sprite_utils::set_horizontal_flip(GameState::instance().main_dude->sprite_state == Orientation::RIGHT,
                                          _main_sprite_info, _sub_sprite_info);

}

void Skull::match_animation() {

    u8 *frame_gfx;

    if (_collided) {

        if (_anim_frame >= COLLIDED_FRAMES) {
            _ready_to_dispose = true;
            sprite_utils::set_visibility(false, _main_sprite_info, _sub_sprite_info);
        }

        frame_gfx = sprite_utils::get_frame((u8 *) gfx_spider_skeletonTiles, _sprite_size,
                                            _anim_frame + FRAME_OFFSET_COLLIDED);

    } else
        frame_gfx = sprite_utils::get_frame((u8 *) gfx_spider_skeletonTiles, _sprite_size,
                                            FRAME_OFFSET_SKULL);

    sprite_utils::update_frame(frame_gfx, _sprite_size, _main_sprite_info, _sub_sprite_info);
}
