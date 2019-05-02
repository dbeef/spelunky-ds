//
// Created by xdbeef on 22.07.18.
//

#include <cstdlib>
#include <maxmod9.h>
#include "Skeleton.hpp"

#include "../../../build/gfx_spider_skeleton.h"
#include "../../collisions/Collisions.hpp"
#include "../../tiles/LevelRenderingUtils.hpp"
#include "../../../build/soundbank.h"
#include "../animations/Bone.hpp"
#include "../items/Skull.hpp"

#include "../../GameState.hpp"
#include "../../time/Timer.h"
#include "../../graphics/SpriteUtils.hpp"

#define SKELETON_POS_INC_DELTA 19
#define SKELETON_ANIM_FRAME_DELTA 90
#define SKELETON_HITPOINTS 1

void Skeleton::update_creature_specific() {

    if (_ready_to_dispose)
        return;

    update_sprites_position();
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_visibility(true, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(!(sprite_state == Orientation::LEFT), mainSpriteInfo, subSpriteInfo);

    if (summoned || can_be_summoned) {
        if (summoning_delay_timer > 350) {
            anim_frame_timer += Timer::getDeltaTime();

            if (can_be_summoned && !set_up) {

                set_up = true;
                if (main_dude_orientation_at_summoning_moment == Orientation::LEFT) {
                    sprite_state = Orientation::RIGHT;
                } else {
                    sprite_state = Orientation::LEFT;
                }

            }

        } else
            summoning_delay_timer += Timer::getDeltaTime();

        change_turn_timer += Timer::getDeltaTime();
    }

    check_if_can_be_summoned();

    if (anim_frame_timer > SKELETON_ANIM_FRAME_DELTA) {

        anim_frame_counter++;
        anim_frame_timer = 0;
        match_animation();
    }

    if (summoned || (can_be_summoned && summoning_delay_timer > 350)) {
        kill_if_whip(1);
        kill_if_main_dude_jumped_on_you(1);
        deal_damage_main_dude_on_collision(1);
    }

    if(_map_collisions_checked) {
        
        standingOnLeftEdge = Collisions::isStandingOnLeftEdge(_neighboring_tiles, _x, _physical_width, _current_x_in_tiles);
        standingOnRightEdge = Collisions::isStandingOnRightEdge(_neighboring_tiles, _x, _physical_width, _current_x_in_tiles);

        if (_bottom_collision) {

            if (death_speed)
                apply_dmg(1);

            if (summoned && _x_speed == 0) {
                if (sprite_state == Orientation::RIGHT)
                    _x_speed = 1;
                else
                    _x_speed = -1;
            }

            if (change_turn_timer > 650) {
                if (_left_collision) {
                    _x_speed = -1;
                    sprite_state = Orientation::LEFT;
                    change_turn_timer = 0;

                } else if (_right_collision) {
                    _x_speed = 1;
                    sprite_state = Orientation::RIGHT;
                    change_turn_timer = 0;
                }
            }

        }
        _map_collisions_checked = false;
    }
    
    death_speed = _y_speed > 3.5f;

}

//!> skeleton has only 1 dmg point, always kill if any dmg_apply
void Skeleton::apply_dmg(int dmg_to_apply) {

    sprite_utils::set_visibility(false, mainSpriteInfo, subSpriteInfo);

    killed = true;
    _ready_to_dispose = true;
    GameState::instance().killed_npcs.push_back(_creature_type);
    mmEffect(SFX_XBREAK);
    spawn_remnants();
}

void Skeleton::init_sprites() {

    delete_sprites();

    subSpriteInfo = GameState::instance().sub_oam_manager->initSprite(gfx_spider_skeletonPal, gfx_spider_skeletonPalLen,
                                                        nullptr, _sprite_size, ObjSize::OBJSIZE_16, SpritesheetType::SKELETON_SPIDER,
                                                        true, false, LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = GameState::instance().main_oam_manager->initSprite(gfx_spider_skeletonPal, gfx_spider_skeletonPalLen,
                                                          nullptr, _sprite_size, ObjSize::OBJSIZE_16, SpritesheetType::SKELETON_SPIDER,
                                                          true, false, LAYER_LEVEL::MIDDLE_TOP);

    update_sprites_position();
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_visibility(true, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(!(sprite_state == Orientation::LEFT), mainSpriteInfo, subSpriteInfo);
    match_animation();

}

void Skeleton::update_sprites_position() {
    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);
    sprite_utils::set_entry_xy(mainSpriteInfo, main_x, main_y);
    sprite_utils::set_entry_xy(subSpriteInfo, sub_x, sub_y);
}

void Skeleton::check_if_can_be_summoned() {

    if (summoned || can_be_summoned)
        return;

    int diff = _x - GameState::instance().main_dude->_x; //distance between skeleton and main dude
    int diff_abs = abs(diff); //absolute distance

    if (diff_abs < 2 * TILE_W && abs(_y - GameState::instance().main_dude->_y) < 0.8 * TILE_H) {

        //TODO sprite_utils function, this xx, yy tile coordinates snippet is used widely
        int xx = floor_div(this->_x + 0.5 * _physical_width, TILE_W);
        int yy = floor_div(this->_y + 0.5 * _physical_height, TILE_H);

        int dude_xx = floor_div(GameState::instance().main_dude->_x + 0.5 * MainDude::main_dude_physical_width, TILE_W);

        MapTile *tiles[9] = {};

        if (diff > 0) {
            //must have been walking towards skeleton from skeleton's left side, so pointing rightwards
            main_dude_orientation_at_summoning_moment = Orientation::RIGHT;
            Collisions::getTilesOnLeftFromXY(xx, yy, tiles);
        } else {
            //must have been walking towards skeleton from skeleton's right side, so pointing leftwards
            main_dude_orientation_at_summoning_moment = Orientation::LEFT;
            Collisions::getTilesOnRightFromXY(xx, yy, tiles);
        }

        for (int a = 0; a < 9; a++) {

            if (tiles[a] != nullptr && tiles[a]->collidable) {
                //there's a collidable obstacle between skeleton and main dude, no summoning
                break;
            }

            if (abs(xx - dude_xx) - a == 0) {
                can_be_summoned = true;
                //if distance in tiles between skeleton and main dude - current tile index == 0, then break,
                // because we didn't find any obstacles between skeleton and main dude and we already are on its tile
                break;
            }

        }

    }
}

//!> after calling this function, call sprite_utils::update_frame to update OAM with current frameGfx
void Skeleton::set_sprite_pile_of_bones() {
    frameGfx = sprite_utils::get_frame((u8 *) gfx_spider_skeletonTiles, _sprite_size, 12);
}

//!> after calling this function, call sprite_utils::update_frame to update OAM with current frameGfx
void Skeleton::set_sprite_walking() {

    if (anim_frame_counter >= 5)
        anim_frame_counter = 0;

    frameGfx = sprite_utils::get_frame((u8 *) gfx_spider_skeletonTiles, _sprite_size, anim_frame_counter + 18);
}

//!> after calling this function, call sprite_utils::update_frame to update OAM with current frameGfx
void Skeleton::set_sprite_summoning() {

    if (anim_frame_counter >= 6) {

        //summoning animation is completed,
        anim_frame_counter = 0;
        summoned = true;

        //turn the skeleton upwards main dude
        if (main_dude_orientation_at_summoning_moment == Orientation::LEFT) {
            _x_speed = 1;
            sprite_state = Orientation::RIGHT;
        } else {
            _x_speed = -1;
            sprite_state = Orientation::LEFT;
        }

    } else {
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spider_skeletonTiles,
                                           _sprite_size, anim_frame_counter + 12);
    }
}

void Skeleton::match_animation() {

    if (summoned)
        set_sprite_walking();
    else if (can_be_summoned)
        set_sprite_summoning();
    else
        set_sprite_pile_of_bones();

    sprite_utils::update_frame(frameGfx, _sprite_size, mainSpriteInfo, subSpriteInfo);
}

void Skeleton::spawn_remnants() {

    auto *b_1 = new Bone(_x, _y - 5);
    b_1->_x_speed = 0;
    b_1->_y_speed = -1.4f;
    b_1->animFrame = 2;

    auto *b_2 = new Bone(_x + 5, _y - 2);
    b_2->_x_speed = -1.5f;
    b_2->_y_speed = -1.6f;
    b_1->animFrame = 6;


    auto *s = new Skull(_x - 6, _y - 4);
    s->_x_speed = -0.5f;
    s->_y_speed = -1.8f;

    GameState::instance().creatures.push_back(b_1);
    GameState::instance().creatures.push_back(b_2);
    GameState::instance().items.push_back(s);

}

void Skeleton::delete_sprites() {
    delete mainSpriteInfo;
    delete subSpriteInfo;
    mainSpriteInfo = nullptr;
    subSpriteInfo = nullptr;
}
