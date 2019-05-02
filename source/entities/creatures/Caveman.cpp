//
// Created by xdbeef on 31.05.18.
//


#include <maxmod9.h>
#include <cstdlib>
#include "Caveman.hpp"
#include "../../collisions/Collisions.hpp"
#include "../../../build/soundbank.h"
#include "../animations/Blood.hpp"
#include "../../../build/gfx_caveman_damsel.h"
#include "../../tiles/LevelRenderingUtils.hpp"
#include "../../tiles/TileOrientation.hpp"
#include "../../memory/SpriteUtils.hpp"
#include "../../GameState.hpp"
#include "../../time/Timer.h"

#define CAVEMAN_POS_INC_DELTA 20
#define CAVEMAN_TRIGGERED_SPEED 3
#define CAVEMAN_NORMAL_SPEED 0.75
#define CAVEMAN_ANIM_FRAME_DELTA 80
#define CAVEMAN_INVERT_SPEED_DELTA 400

void Caveman::update_creature_specific() {

    if (_ready_to_dispose)
        return;

    invert_speed_timer += Timer::getDeltaTime();
    blood_spawn_timer += Timer::getDeltaTime();

    sprite_utils::set_horizontal_flip(sprite_state == Orientation::RIGHT, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_visibility(true, mainSpriteInfo, subSpriteInfo);

    if (killed || stunned) {

        check_if_can_be_pickuped();
        set_pickuped_position(6, -4);

        if (hold_by_main_dude) {
            sprite_state = GameState::instance().main_dude->sprite_state;
            sprite_utils::set_priority(OBJPRIORITY_0, mainSpriteInfo, subSpriteInfo);
        } else
            sprite_utils::set_priority(OBJPRIORITY_2, mainSpriteInfo, subSpriteInfo);

        kill_creatures_if_have_speed(1);
    }

    update_sprites_position();

    if (!stunned && !killed) {
        if (hold_by_main_dude) {
            hold_by_main_dude = false;
            GameState::instance().main_dude->holding_item = false;
            goTimer = 2000;
            waitTimer = 0;
        }
    }

    animFrameTimer += Timer::getDeltaTime();

    if (animFrameTimer > CAVEMAN_ANIM_FRAME_DELTA) {
        animFrame++;
        animFrameTimer = 0;
        match_animation();
    }

    if (!triggered && !killed && !stunned) {
        check_if_can_be_triggered();
        if (triggered)
            mmEffect(SFX_XALERT);
    }

    make_some_movement();

    if (!stunned && !killed)
        kill_if_whip(1);
    if (!stunned && !killed) {
        if (GameState::instance().main_dude->carrying_spike_shoes)
            kill_if_main_dude_jumped_on_you(3);
        else
            kill_if_main_dude_jumped_on_you(1);
    }
    if (!stunned && !killed)
        deal_damage_main_dude_on_collision(1);

    if (stunned) {
        stunned_timer += Timer::getDeltaTime();
        if (stunned_timer > CAVEMAN_STUN_TIME) {
            stunned = false;
            stunned_timer = 0;
            _bouncing_factor_x = 0;
            _bouncing_factor_y = 0;
        }
    }

    if(_map_collisions_checked){

        if (!_bottom_collision) {

            if ((_neighboring_tiles[static_cast<uint16>(TileOrientation::RIGHT_MIDDLE)] == nullptr ||
                !_neighboring_tiles[static_cast<uint16>(TileOrientation::RIGHT_MIDDLE)]->collidable) &&
                (_neighboring_tiles[static_cast<uint16>(TileOrientation::RIGHT_UP)] != nullptr &&
                _neighboring_tiles[static_cast<uint16>(TileOrientation::RIGHT_DOWN)] != nullptr)) {
                //if there's no collidable tile on right-mid, but there are on right-up and right-down,
                //add extra x-pos to ease going through a hole
                if (_x_speed > 0)
                    _x += 2;
            }

            if ((_neighboring_tiles[static_cast<uint16>(TileOrientation::LEFT_MIDDLE)] == nullptr || !_neighboring_tiles[static_cast<uint16>(TileOrientation::LEFT_MIDDLE)]->collidable) &&
                (_neighboring_tiles[static_cast<uint16>(TileOrientation::LEFT_UP)] != nullptr && _neighboring_tiles[static_cast<uint16>(TileOrientation::LEFT_DOWN)] != nullptr)) {
                //same but for left side
                if (_x_speed < 0)
                    _x -= 2;
            }

        }

        bool bounce = (stunned || killed);
        
        standingOnLeftEdge = Collisions::isStandingOnLeftEdge(_neighboring_tiles, _x, _physical_width, _current_x_in_tiles);
        standingOnRightEdge = Collisions::isStandingOnRightEdge(_neighboring_tiles, _x, _physical_width, _current_x_in_tiles);

        if (bounce)
            return;

        if (_bottom_collision && _neighboring_tiles[static_cast<uint16>(TileOrientation::RIGHT_MIDDLE)] != nullptr &&
            _neighboring_tiles[static_cast<uint16>(TileOrientation::RIGHT_MIDDLE)]->collidable &&
            _neighboring_tiles[static_cast<uint16>(TileOrientation::LEFT_MIDDLE)] != nullptr &&
            _neighboring_tiles[static_cast<uint16>(TileOrientation::LEFT_MIDDLE)]->collidable) {
            //high jump if damsel's surrounded by tiles
            _y_speed = -3.6 - ((rand() % 10) / 5);
            landlocked = true;
        } else
            landlocked = false;


        _map_collisions_checked = false;
    }

}

void Caveman::randomizeMovement() {

    int r = rand() % 2;
    if (r == 0)
        sprite_state = Orientation::LEFT;
    //else keep the current side

    goTimer = (rand() % (1 * 2000)) + 1000;
    waitTimer = rand() % 1500;
}

void Caveman::apply_dmg(int dmg_to_apply) {

    if (dmg_to_apply == 1 && (stunned))
        return;

    //TODO Share this piece of code between damsel/shopkeeper/caveman
    hitpoints -= dmg_to_apply;
    if (blood_spawn_timer > 1000) {
        blood_spawn_timer = 0;
        _y_speed = -2.5; //jump a little bit on receiving dmg, even if dead
        if (!killed)
            spawn_blood();
    }

    if (killed)
        return;

    if (hitpoints <= 0) {
        killed = true;
        stunned = false;
        GameState::instance().killed_npcs.push_back(_creature_type);
        _bouncing_factor_x = ICollidable::default_bouncing_factor_x;
        _bouncing_factor_y = ICollidable::default_bouncing_factor_y;
    } else {
        _bouncing_factor_x = ICollidable::default_bouncing_factor_x;
        _bouncing_factor_y = ICollidable::default_bouncing_factor_y;
        stunned = true;
    }

}

void Caveman::init_sprites() {

    delete_sprites();

    subSpriteInfo = GameState::instance().sub_oam_manager->initSprite(gfx_caveman_damselPal, gfx_caveman_damselPalLen,
                                                        nullptr, _sprite_size, ObjSize::OBJSIZE_16, SpritesheetType::CAVEMAN_DAMSEL,
                                                        true, false, LAYER_LEVEL::MIDDLE_BOT);
    mainSpriteInfo = GameState::instance().main_oam_manager->initSprite(gfx_caveman_damselPal, gfx_caveman_damselPalLen,
                                                          nullptr, _sprite_size, ObjSize::OBJSIZE_16, SpritesheetType::CAVEMAN_DAMSEL,
                                                          true, false, LAYER_LEVEL::MIDDLE_BOT);
    match_animation();
    update_sprites_position();
    sprite_utils::set_horizontal_flip(sprite_state == Orientation::RIGHT, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_visibility(true, mainSpriteInfo, subSpriteInfo);

}

void Caveman::update_sprites_position() {
    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);
    sprite_utils::set_entry_xy(mainSpriteInfo, main_x, main_y);
    sprite_utils::set_entry_xy(subSpriteInfo, sub_x, sub_y);
}

void Caveman::make_some_movement() {

    if (killed || stunned)
        return;

    if (waitTimer > 0 && !triggered) {
        waitTimer -= Timer::getDeltaTime();
    } else {

        if (goTimer > 0)
            goTimer -= Timer::getDeltaTime();

        if (triggered) {
            if (sprite_state == Orientation::RIGHT)
                _x_speed = CAVEMAN_TRIGGERED_SPEED;
            else
                _x_speed = -CAVEMAN_TRIGGERED_SPEED;
        } else {
            if (sprite_state == Orientation::RIGHT)
                _x_speed = CAVEMAN_NORMAL_SPEED;
            else
                _x_speed = -CAVEMAN_NORMAL_SPEED;
        }


        if (goTimer <= 0 && !triggered) {
            randomizeMovement();
            _x_speed = 0;
        }

        if (invert_speed_timer < CAVEMAN_INVERT_SPEED_DELTA)
            return;

        invert_speed_timer = 0;

        if ((standingOnLeftEdge || standingOnRightEdge) && !triggered) {
            if (rand() % 2 == 0) {
                goTimer = 0;
            } else
                _x_speed *= -1;

        }

        if ((_left_collision || _right_collision) && !landlocked) {

            if (sprite_state == Orientation::LEFT)
                sprite_state = Orientation::RIGHT;
            else
                sprite_state = Orientation::LEFT;

            _x_speed *= -1;
            _right_collision = false;
            _left_collision = false;
        }

    }

}

void Caveman::check_if_can_be_triggered() {

    int diff = _x - GameState::instance().main_dude->_x;
    int diff_abs = abs(diff);

    if (!triggered && diff_abs < 9 * TILE_W && abs(_y - GameState::instance().main_dude->_y) < 0.8 * TILE_H) {

        int xx = floor_div(this->_x + 0.5 * _physical_width, TILE_W);
        int yy = floor_div(this->_y + 0.5 * _physical_height, TILE_H);

        int dude_xx = floor_div(GameState::instance().main_dude->_x + 0.5 * MainDude::main_dude_physical_width, TILE_W);

        triggered = true;

        MapTile *tiles[9] = {};
        if (sprite_state == Orientation::RIGHT && diff < 0)
            Collisions::getTilesOnRightFromXY(xx, yy, tiles);
        else if (sprite_state == Orientation::LEFT && diff > 0)
            Collisions::getTilesOnLeftFromXY(xx, yy, tiles);
        else triggered = false;

        for (int a = 0; a < 9; a++) {

            if (tiles[a]->exists && tiles[a]->collidable) {
                triggered = false;
                break;
            }

            if (abs(xx - dude_xx) - a == 0)
                break;

        }
    }

}

//!> after calling this function, call sprite_utils::update_frame to update OAM with current frameGfx
void Caveman::apply_stunned_carried_sprites() {

    if (animFrame >= 4)
        animFrame = 0;
    frameGfx = sprite_utils::get_frame((u8 *) gfx_caveman_damselTiles, _sprite_size, 6 + animFrame);
}

//!> after calling this function, call sprite_utils::update_frame to update OAM with current frameGfx
void Caveman::apply_dead_carried_sprites() {
    animFrame = 0;
    frameGfx = sprite_utils::get_frame((u8 *) gfx_caveman_damselTiles, _sprite_size, 0);
}

//!> after calling this function, call sprite_utils::update_frame to update OAM with current frameGfx
void Caveman::apply_dead_sprites() {
    animFrame = 0;
    if (_x_speed == 0)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_caveman_damselTiles, _sprite_size, 1);
    else if (_y_speed > 0)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_caveman_damselTiles, _sprite_size, 4);
    else
        frameGfx = sprite_utils::get_frame((u8 *) gfx_caveman_damselTiles, _sprite_size, 2);

}

//!> after calling this function, call sprite_utils::update_frame to update OAM with current frameGfx
void Caveman::apply_walking_sprites() {

    if (animFrame >= 4)
        animFrame = 0;

    if (_x_speed == 0 && _y_speed == 0)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_caveman_damselTiles, _sprite_size, 10);
    else
        frameGfx = sprite_utils::get_frame((u8 *) gfx_caveman_damselTiles, _sprite_size, 11 + animFrame);

}

//!> after calling this function, call sprite_utils::update_frame to update OAM with current frameGfx
void Caveman::apply_stunned_sprites() {
    if (animFrame >= 5)
        animFrame = 0;

    if (_y_speed == 0)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_caveman_damselTiles, _sprite_size, 15 + animFrame);
    else if (_x_speed > 0)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_caveman_damselTiles, _sprite_size, 4);
    else
        frameGfx = sprite_utils::get_frame((u8 *) gfx_caveman_damselTiles, _sprite_size, 2);
}

void Caveman::match_animation() {

    if (stunned && hold_by_main_dude)
        apply_stunned_carried_sprites();
    else if (stunned)
        apply_stunned_sprites();
    else if (killed && hold_by_main_dude)
        apply_dead_carried_sprites();
    else if (killed)
        apply_dead_sprites();
    else
        apply_walking_sprites();

    sprite_utils::update_frame(frameGfx, _sprite_size, mainSpriteInfo, subSpriteInfo);
}

void Caveman::delete_sprites() {
    delete mainSpriteInfo;
    delete subSpriteInfo;
    mainSpriteInfo = nullptr;
    subSpriteInfo = nullptr;
}
