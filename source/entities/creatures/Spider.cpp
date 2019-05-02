//
// Created by xdbeef on 03.05.18.
//


#include <cstdlib>
#include <cstdio>
#include "Spider.hpp"
#include "../animations/Blood.hpp"
#include "../../../build/gfx_spider_skeleton.h"
#include "../items/Rock.hpp"
#include "../../collisions/Collisions.hpp"
#include "../../tiles/LevelRenderingUtils.hpp"
#include "../../../build/soundbank.h"

#include "../../GameState.hpp"
#include "../../time/Timer.h"
#include "../../graphics/SpriteUtils.hpp"

#define SPIDER_HANGING_OFFSET 8
#define SPIDER_ANIM_FRAME_DELTA 75

/**
 * If hunting;
 * - if in air, have the same constant speed,
 * - if on the ground, check main-dude's position, set own speed to catch him up, bounce up
 * - bounce velocity should be random
 */
void Spider::update_creature_specific() {

    if (_ready_to_dispose)
        return;

    update_sprites_position();
    sprite_utils::set_visibility(true, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);

    if (!hunting)
        //Check if main dude is direcly under the spider - intentionally not checking for terrain obstacles,
        //like in the original game. Also check if main dude is in certain range
        hunting = abs(_y - GameState::instance().main_dude->_y) < 9 * TILE_H && GameState::instance().main_dude->_x + MainDude::main_dude_physical_width > _x &&
                  GameState::instance().main_dude->_x < _x + _physical_width && GameState::instance().main_dude->_y > _y;
    else {
        time_since_last_big_jump += Timer::getDeltaTime();
        time_since_last_jump += Timer::getDeltaTime();
        animFrameTimer += Timer::getDeltaTime();
    }

    if (animFrameTimer > SPIDER_ANIM_FRAME_DELTA) {

        animFrameTimer = 0;
        animFrame++;

        if (jumping && animFrame >= 4) {
            //done jumping, now still
            animFrame = 0;
            jumping = false;
        }

        if (hanging && hunting && animFrame >= 7) {
            //done rotating animation, now still
            animFrame = 0;
            hanging = false;
        }

        match_animation();
    }

    if (hunting && _bottom_collision && !hanging)
        jump_to_main_dude();

    kill_if_whip(1);
    kill_if_main_dude_jumped_on_you(1);
    deal_damage_main_dude_on_collision(1);

    if(_map_collisions_checked){

        standingOnLeftEdge = Collisions::isStandingOnLeftEdge(_neighboring_tiles, _x, _physical_width, _current_x_in_tiles);
        standingOnRightEdge = Collisions::isStandingOnRightEdge(_neighboring_tiles, _x, _physical_width, _current_y_in_tiles);

        if (_bottom_collision)
            random_speed = 0;

        if (!previously_collided && (_left_collision || _right_collision)) {

            _x_speed = 0;
            previously_collided = true;

            if (_left_collision)
                previous_collision_side = Orientation::LEFT;
            else
                previous_collision_side = Orientation::RIGHT;

        }


        _map_collisions_checked = false;
    }
}

//!>spider has only 1 dmg point, always kill if any dmg_apply
void Spider::apply_dmg(int dmg_to_apply) {
    sprite_utils::set_visibility(false, mainSpriteInfo, subSpriteInfo);
    GameState::instance().killed_npcs.push_back(_creature_type);
    spawn_blood();
    killed = true;
    _ready_to_dispose = true;
}

void Spider::init_sprites() {

    delete_sprites();

    subSpriteInfo = OAMManager::sub().initSprite(gfx_spider_skeletonPal, gfx_spider_skeletonPalLen,
                                                        nullptr, _sprite_size, ObjSize::OBJSIZE_16, SpritesheetType::SKELETON_SPIDER, true,
                                                        false, LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = OAMManager::main().initSprite(gfx_spider_skeletonPal, gfx_spider_skeletonPalLen,
                                                          nullptr, _sprite_size, ObjSize::OBJSIZE_16, SpritesheetType::SKELETON_SPIDER, true,
                                                          false, LAYER_LEVEL::MIDDLE_TOP);
    update_sprites_position();
    sprite_utils::set_visibility(true, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);
    match_animation();

}

//!> after calling this function, call sprite_utils::update_frame to update OAM with current frameGfx
void Spider::set_sprite_hanging() {
    frameGfx = sprite_utils::get_frame((u8 *) gfx_spider_skeletonTiles, _sprite_size, 4);
}

//!> after calling this function, call sprite_utils::update_frame to update OAM with current frameGfx
void Spider::set_sprite_flipping() {
    frameGfx = sprite_utils::get_frame((u8 *) gfx_spider_skeletonTiles, _sprite_size, animFrame + 4);
}

//!> after calling this function, call sprite_utils::update_frame to update OAM with current frameGfx
void Spider::set_sprite_jumping() {
    frameGfx = sprite_utils::get_frame((u8 *) gfx_spider_skeletonTiles, _sprite_size, animFrame);
}

//!> after calling this function, call sprite_utils::update_frame to update OAM with current frameGfx
void Spider::set_sprite_falling() {
    frameGfx = sprite_utils::get_frame((u8 *) gfx_spider_skeletonTiles, _sprite_size, 0);
}

void Spider::update_sprites_position() {

    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);

    if (!hanging) {
        main_y -= SPIDER_HANGING_OFFSET;
        sub_y -= SPIDER_HANGING_OFFSET;
    }

    sprite_utils::set_entry_xy(mainSpriteInfo, main_x, main_y);
    sprite_utils::set_entry_xy(subSpriteInfo, sub_x, sub_y);
}

void Spider::jump_to_main_dude() {

    if (!_bottom_collision || time_since_last_jump < (200 + jump_delay))
        return;

    time_since_last_jump = 0;

    int diff = GameState::instance().main_dude->_x - _x > 0;

    bool additional_jump_speed = false;

    if (diff) {

        if (previously_collided && time_since_last_big_jump > 2500) {

            if (previous_collision_side == Orientation::RIGHT) {
                random_speed = 0.3f;
                _x_speed = 0.3f;
            } else {
                random_speed = -0.3f;
                _x_speed = -0.3f;
            }

            additional_jump_speed = true;
            time_since_last_big_jump = 0;

        } else {
            if (jump_delay >= 400)
                random_speed = 1.5 + ((rand() % diff) / 10);
            else
                random_speed = 1.1 + ((rand() % diff) / 10);
        }

    } else {
        if (previously_collided && time_since_last_big_jump > 2500) {

            if (previous_collision_side == Orientation::RIGHT) {
                random_speed = 0.3f;
                _x_speed = 0.3f;
            } else {
                random_speed = -0.3f;
                _x_speed = -0.3f;
            }

            _x_speed = -0.3f;
            random_speed = -0.3f;
            additional_jump_speed = true;
            time_since_last_big_jump = 0;
        } else {
            if (jump_delay >= 400)
                random_speed = -1.5 + ((rand() % 3) / 10);
            else
                random_speed = -1.1 + ((rand() % 3) / 10);
        }
    }

    if (additional_jump_speed)
        jump_delay = rand() % 700;
    else
        jump_delay = rand() % 500;

    jumping = true;
    animFrame = 0;
    previously_collided = false;

    if (additional_jump_speed) {
        _y_speed = -3.5;
    } else
        _y_speed = -1.6 - ((rand() % 3) / 10);

}

void Spider::match_animation() {

    if (hanging && !hunting)
        set_sprite_hanging();
    else if (hanging && hunting)
        set_sprite_flipping();
    else if (!hanging && hunting && jumping)
        set_sprite_jumping();
    else
        set_sprite_falling();

    sprite_utils::update_frame(frameGfx, _sprite_size, mainSpriteInfo, subSpriteInfo);
}

void Spider::delete_sprites() {
    delete mainSpriteInfo;
    delete subSpriteInfo;
    mainSpriteInfo = nullptr;
    subSpriteInfo = nullptr;
}
