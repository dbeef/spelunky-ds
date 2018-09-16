//
// Created by xdbeef on 31.05.18.
//


#include <maxmod9.h>
#include <cstdlib>
#include "caveman.hpp"
#include "../../globals_declarations.hpp"
#include "../../collisions/collisions.hpp"
#include "../../../build/soundbank.h"
#include "../animations/blood.hpp"
#include "../../../build/gfx_caveman_damsel.h"
#include "../../tiles/level_rendering_utils.hpp"
#include "../../tiles/tile_orientation.hpp"
#include "../sprite_utils.hpp"

#define CAVEMAN_POS_INC_DELTA 20
#define CAVEMAN_TRIGGERED_SPEED 3
#define CAVEMAN_NORMAL_SPEED 0.75
#define CAVEMAN_ANIM_FRAME_DELTA 80
#define CAVEMAN_INVERT_SPEED_DELTA 400

void Caveman::draw() {

    if (ready_to_dispose)
        return;

    invert_speed_timer += *global::timer;
    blood_spawn_timer += *global::timer;

    sprite_utils::set_horizontal_flip(sprite_state == SpriteState::W_RIGHT, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_visibility(true, mainSpriteInfo, subSpriteInfo);

    if (killed || stunned) {

        check_if_can_be_pickuped();
        set_pickuped_position(6, -4);

        if (hold_by_main_dude) {
            sprite_state = global::main_dude->sprite_state;
            sprite_utils::set_priority(OBJPRIORITY_0, mainSpriteInfo, subSpriteInfo);
        } else
            sprite_utils::set_priority(OBJPRIORITY_2, mainSpriteInfo, subSpriteInfo);

        kill_mobs_if_thrown(1);
    }

    if (bottomCollision && (stunned || killed)) {
        apply_friction(0.5f);
    }

    set_position();

    if (!stunned && !killed) {
        if (hold_by_main_dude) {
            hold_by_main_dude = false;
            global::main_dude->holding_item = false;
            goTimer = 2000;
            waitTimer = 0;
        }
    }

    animFrameTimer += *global::timer;

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
        if (global::main_dude->carrying_spike_shoes)
            kill_if_main_dude_jumped_on_you(3);
        else
            kill_if_main_dude_jumped_on_you(1);
    }
    if (!stunned && !killed)
        deal_damage_main_dude_on_collision(1);

    if (stunned) {
        stunned_timer += *global::timer;
        if (stunned_timer > CAVEMAN_STUN_TIME) {
            stunned = false;
            stunned_timer = 0;
        }
    }

}


void Caveman::init() {
    activated = true;
    initSprite();
    spritesheet_type = SpritesheetType::CAVEMAN_DAMSEL;
    randomizeMovement();
}

void Caveman::randomizeMovement() {

    int r = rand() % 2;
    if (r == 0)
        sprite_state = SpriteState::W_LEFT;
    //else keep the current side

    goTimer = (rand() % (1 * 2000)) + 1000;
    waitTimer = rand() % 1500;
}

void Caveman::updateSpeed() {

    limit_speed(MAX_X_SPEED_CAVEMAN, MAX_Y_SPEED_CAVEMAN);

    pos_inc_timer += *global::timer;

    if (pos_inc_timer > CAVEMAN_POS_INC_DELTA) {
        update_position();
        apply_gravity(GRAVITY_DELTA_SPEED * 1.5);
        pos_inc_timer = 0;
    }

}

void Caveman::updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) {

    MapTile *t[9] = {};
    Collisions::getNeighboringTiles(global::current_level->map_tiles, x_current_pos_in_tiles,
                                    y_current_pos_in_tiles, t);

    bool bounce = (stunned || killed);


    standingOnLeftEdge = Collisions::isStandingOnLeftEdge(t, x, physical_width, x_current_pos_in_tiles);
    standingOnRightEdge = Collisions::isStandingOnRightEdge(t, x, physical_width, x_current_pos_in_tiles);

    if (bounce) {
        //if bounce then reversing sequence of collision detection calls
        upperCollision = Collisions::checkUpperCollision(t, &x, &y, &ySpeed, physical_width, false, 0);
        bottomCollision = Collisions::checkBottomCollision(t, &x, &y, &ySpeed, physical_width, physical_height,
                                                           bounce, BOUNCING_FACTOR_Y);
    } else {
        bottomCollision = Collisions::checkBottomCollision(t, &x, &y, &ySpeed, physical_width, physical_height,
                                                           bounce, BOUNCING_FACTOR_Y);
        leftCollision = Collisions::checkLeftCollision(t, &x, &y, &xSpeed, physical_width, physical_height, bounce,
                                                       BOUNCING_FACTOR_X);
    }

    rightCollision = Collisions::checkRightCollision(t, &x, &y, &xSpeed, physical_width, physical_height,
                                                     bounce, BOUNCING_FACTOR_X);
    upperCollision = Collisions::checkUpperCollision(t, &x, &y, &ySpeed, physical_width, false, 0);

    if (bounce)
        return;

    if (bottomCollision && t[TileOrientation::RIGHT_MIDDLE] != nullptr &&
        t[TileOrientation::RIGHT_MIDDLE]->collidable &&
        t[TileOrientation::LEFT_MIDDLE] != nullptr && t[TileOrientation::LEFT_MIDDLE]->collidable) {
        //high jump if damsel's surrounded by tiles
        ySpeed = -3.6 - ((rand() % 10) / 5);
        landlocked = true;
    } else
        landlocked = false;

    if (!bottomCollision) {

        if ((t[TileOrientation::RIGHT_MIDDLE] == nullptr || !t[TileOrientation::RIGHT_MIDDLE]->collidable) &&
            (t[TileOrientation::RIGHT_UP] != nullptr && t[TileOrientation::RIGHT_DOWN] != nullptr)) {
            //if there's no collidable tile on right-mid, but there are on right-up and right-down,
            //add extra x-pos to ease going through a hole
            if (xSpeed > 0)
                x += 2;
        }

        if ((t[TileOrientation::LEFT_MIDDLE] == nullptr || !t[TileOrientation::LEFT_MIDDLE]->collidable) &&
            (t[TileOrientation::LEFT_UP] != nullptr && t[TileOrientation::LEFT_DOWN] != nullptr)) {
            //same but for left side
            if (xSpeed < 0)
                x -= 2;
        }

    }

}

void Caveman::apply_dmg(int dmg_to_apply) {

    if (dmg_to_apply == 1 && (stunned))
        return;

    //TODO Share this piece of code between damsel/shopkeeper/caveman
    hitpoints -= dmg_to_apply;
    if (blood_spawn_timer > 1000) {
        blood_spawn_timer = 0;
        ySpeed = -2.5; //jump a little bit on receiving dmg, even if dead
        if (!killed)
            spawn_blood();
    }

    if (killed)
        return;

    if (hitpoints <= 0) {
        killed = true;
        stunned = false;
        global::killed_npcs.push_back(SpriteType::S_CAVEMAN);
    } else {
        stunned = true;
    }

}

void Caveman::initSprite() {

    delete subSpriteInfo;
    delete mainSpriteInfo;

    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_caveman_damselPal, gfx_caveman_damselPalLen,
                                                        nullptr, CAVEMAN_SPRITE_SIZE, 16, CAVEMAN_DAMSEL,
                                                        true, false, LAYER_LEVEL::MIDDLE_BOT);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_caveman_damselPal, gfx_caveman_damselPalLen,
                                                          nullptr, CAVEMAN_SPRITE_SIZE, 16, CAVEMAN_DAMSEL,
                                                          true, false, LAYER_LEVEL::MIDDLE_BOT);
    match_animation();
    set_position();
    sprite_utils::set_horizontal_flip(sprite_state == SpriteState::W_RIGHT, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_visibility(true, mainSpriteInfo, subSpriteInfo);

}

void Caveman::set_position() {
    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);
    sprite_utils::set_entry_xy(mainSpriteInfo, main_x, main_y);
    sprite_utils::set_entry_xy(subSpriteInfo, sub_x, sub_y);
}

Caveman::Caveman() {
    physical_height = CAVEMAN_PHYSICAL_HEIGHT;
    physical_width = CAVEMAN_PHYSICAL_WIDTH;
    sprite_height = CAVEMAN_SPRITE_HEIGHT;
    sprite_width = CAVEMAN_SPRITE_WIDTH;
    hitpoints = CAVEMAN_HITPOINTS;
}


void Caveman::make_some_movement() {

    if (killed || stunned)
        return;

    if (waitTimer > 0 && !triggered) {
        waitTimer -= *global::timer;
    } else {

        if (goTimer > 0)
            goTimer -= *global::timer;

        if (triggered) {
            if (sprite_state == SpriteState::W_RIGHT)
                xSpeed = CAVEMAN_TRIGGERED_SPEED;
            else
                xSpeed = -CAVEMAN_TRIGGERED_SPEED;
        } else {
            if (sprite_state == SpriteState::W_RIGHT)
                xSpeed = CAVEMAN_NORMAL_SPEED;
            else
                xSpeed = -CAVEMAN_NORMAL_SPEED;
        }


        if (goTimer <= 0 && !triggered) {
            randomizeMovement();
            xSpeed = 0;
        }

        if (invert_speed_timer < CAVEMAN_INVERT_SPEED_DELTA)
            return;

        invert_speed_timer = 0;

        if ((standingOnLeftEdge || standingOnRightEdge) && !triggered) {
            if (rand() % 2 == 0) {
                goTimer = 0;
            } else
                xSpeed *= -1;

        }

        if ((leftCollision || rightCollision) && !landlocked) {

            if (sprite_state == SpriteState::W_LEFT)
                sprite_state = SpriteState::W_RIGHT;
            else
                sprite_state = SpriteState::W_LEFT;

            xSpeed *= -1;
            rightCollision = false;
            leftCollision = false;
        }

    }

}

void Caveman::check_if_can_be_triggered() {

    int diff = x - global::main_dude->x;
    int diff_abs = abs(diff);

    if (!triggered && diff_abs < 9 * TILE_W && abs(y - global::main_dude->y) < 0.8 * TILE_H) {

        int xx = floor_div(this->x + 0.5 * physical_width, TILE_W);
        int yy = floor_div(this->y + 0.5 * physical_height, TILE_H);

        int dude_xx = floor_div(global::main_dude->x + 0.5 * MAIN_DUDE_PHYSICAL_WIDTH, TILE_W);

        triggered = true;

        MapTile *tiles[9] = {};
        if (sprite_state == SpriteState::W_RIGHT && diff < 0)
            Collisions::getTilesOnRightFromXY(global::current_level->map_tiles, xx, yy, tiles);
        else if (sprite_state == SpriteState::W_LEFT && diff > 0)
            Collisions::getTilesOnLeftFromXY(global::current_level->map_tiles, xx, yy, tiles);
        else triggered = false;


        for (int a = 0; a < 9; a++) {

            if (tiles[a] != nullptr && tiles[a]->collidable) {
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
    frameGfx = sprite_utils::get_frame((u8 *) gfx_caveman_damselTiles, CAVEMAN_SPRITE_SIZE, 6 + animFrame);
}

//!> after calling this function, call sprite_utils::update_frame to update OAM with current frameGfx
void Caveman::apply_dead_carried_sprites() {
    animFrame = 0;
    frameGfx = sprite_utils::get_frame((u8 *) gfx_caveman_damselTiles, CAVEMAN_SPRITE_SIZE, 0);
}

//!> after calling this function, call sprite_utils::update_frame to update OAM with current frameGfx
void Caveman::apply_dead_sprites() {
    animFrame = 0;
    if (ySpeed == 0)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_caveman_damselTiles, CAVEMAN_SPRITE_SIZE, 1);
    else if (ySpeed > 0)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_caveman_damselTiles, CAVEMAN_SPRITE_SIZE, 4);
    else
        frameGfx = sprite_utils::get_frame((u8 *) gfx_caveman_damselTiles, CAVEMAN_SPRITE_SIZE, 2);

}

//!> after calling this function, call sprite_utils::update_frame to update OAM with current frameGfx
void Caveman::apply_walking_sprites() {

    if (animFrame >= 4)
        animFrame = 0;

    if (xSpeed == 0 && ySpeed == 0)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_caveman_damselTiles, CAVEMAN_SPRITE_SIZE, 10);
    else
        frameGfx = sprite_utils::get_frame((u8 *) gfx_caveman_damselTiles, CAVEMAN_SPRITE_SIZE, 11 + animFrame);

}

//!> after calling this function, call sprite_utils::update_frame to update OAM with current frameGfx
void Caveman::apply_stunned_sprites() {
    if (animFrame >= 5)
        animFrame = 0;

    if (ySpeed == 0)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_caveman_damselTiles, CAVEMAN_SPRITE_SIZE, 15 + animFrame);
    else if (ySpeed > 0)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_caveman_damselTiles, CAVEMAN_SPRITE_SIZE, 4);
    else
        frameGfx = sprite_utils::get_frame((u8 *) gfx_caveman_damselTiles, CAVEMAN_SPRITE_SIZE, 2);
}


Caveman::Caveman(int x, int y) : Caveman() {
    this->x = x;
    this->y = y;
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

    sprite_utils::update_frame(frameGfx, CAVEMAN_SPRITE_SIZE, mainSpriteInfo, subSpriteInfo);
}
