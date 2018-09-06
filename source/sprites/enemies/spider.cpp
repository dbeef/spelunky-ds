//
// Created by xdbeef on 03.05.18.
//

#include <maxmod9.h>
#include <cstdlib>
#include <cstdio>
#include "spider.h"
#include "../../globals_declarations.h"
#include "../animations/blood.h"
#include "../../../build/gfx_spider_skeleton.h"
#include "../items/rock.h"
#include "../../collisions/collisions.h"
#include "../../tiles/map_utils.h"
#include "../../../build/soundbank.h"
#include "../sprite_utils.h"

#define SPIDER_HITPOINTS 1
#define SPIDER_POS_INC_DELTA 30
#define SPIDER_HANGING_OFFSET 8
#define SPIDER_ANIM_FRAME_DELTA 75

/**
 * If hunting;
 * - if in air, have the same constant speed,
 * - if on the ground, check main-dude's position, set own speed to catch him up, bounce up
 * - bounce velocity should be random
 */
void Spider::draw() {

    if (ready_to_dispose)
        return;

    set_position();
    sprite_utils::set_visibility(true, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);

    if (!hunting)
        //Check if main dude is direcly under the spider - intentionally not checking for terrain obstacles,
        //like in the original game. Also check if main dude is in certain range
        hunting = abs(y - global::main_dude->y) < 9 * TILE_H && global::main_dude->x + MAIN_DUDE_PHYSICAL_WIDTH > x &&
                  global::main_dude->x < x + physical_width && global::main_dude->y > y;
    else {
        time_since_last_big_jump += *global::timer;
        time_since_last_jump += *global::timer;
        animFrameTimer += *global::timer;
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

    if (hunting && bottomCollision && !hanging)
        jump_to_main_dude();

    kill_if_whip(1);
    kill_if_main_dude_jumped_on_you(1);
    deal_damage_main_dude_on_collision(1);
}


void Spider::init() {
    hanging = true;
    random_speed = 0;
    initSprite();
}

void Spider::updateSpeed() {

    xSpeed = random_speed;

    limit_speed(MAX_X_SPEED_SPIDER, MAX_Y_SPEED_SPIDER);

    pos_inc_timer += *global::timer;

    if (pos_inc_timer > SPIDER_POS_INC_DELTA) {
        update_position();

        if (hunting)
            if (previously_collided)
                apply_gravity(GRAVITY_DELTA_SPEED * 1);
            else
                apply_gravity(GRAVITY_DELTA_SPEED * 0.8);

        pos_inc_timer = 0;
    }

}


void Spider::updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) {

    MapTile *t[9] = {};
    Collisions::getNeighboringTiles(global::level_generator->map_tiles,
                                    x_current_pos_in_tiles, y_current_pos_in_tiles, t);

    standingOnLeftEdge = Collisions::isStandingOnLeftEdge(t, x, physical_width, x_current_pos_in_tiles);
    standingOnRightEdge = Collisions::isStandingOnRightEdge(t, x, physical_width, x_current_pos_in_tiles);
    bottomCollision = Collisions::checkBottomCollision(t, &x, &y, &ySpeed, physical_width, physical_height, false, 0);
    leftCollision = Collisions::checkLeftCollision(t, &x, &y, &xSpeed, physical_width, physical_height, false, 0);
    rightCollision = Collisions::checkRightCollision(t, &x, &y, &xSpeed, physical_width, physical_height, false, 0);
    upperCollision = Collisions::checkUpperCollision(t, &x, &y, &ySpeed, physical_width, false, 0);

    if (bottomCollision)
        random_speed = 0;


    if (!previously_collided && (leftCollision || rightCollision)) {

        xSpeed = 0;
        previously_collided = true;

        if (leftCollision)
            previous_collision_side = SpriteState::W_LEFT;
        else
            previous_collision_side = SpriteState::W_RIGHT;

    }

}

//!>spider has only 1 dmg point, always kill if any dmg_apply
void Spider::apply_dmg(int dmg_to_apply) {

    sprite_utils::set_visibility(false, mainSpriteInfo, subSpriteInfo);
    global::killed_npcs.push_back(SpriteType::S_SPIDER);
    spawn_blood();
    killed = true;
    ready_to_dispose = true;

}

void Spider::initSprite() {

    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_spider_skeletonPal, gfx_spider_skeletonPalLen,
                                                        nullptr, SPIDER_SPRITE_SIZE, 16, SKELETON_SPIDER, true,
                                                        false, LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_spider_skeletonPal, gfx_spider_skeletonPalLen,
                                                          nullptr, SPIDER_SPRITE_SIZE, 16, SKELETON_SPIDER, true,
                                                          false, LAYER_LEVEL::MIDDLE_TOP);
    match_animation();
    set_position();
    sprite_utils::set_visibility(true, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);

}

//!> after calling this function, call sprite_utils::update_frame to update OAM with current frameGfx
void Spider::set_sprite_hanging() {
    frameGfx = sprite_utils::get_frame((u8 *) gfx_spider_skeletonTiles, SPIDER_SPRITE_SIZE, 4);
}

//!> after calling this function, call sprite_utils::update_frame to update OAM with current frameGfx
void Spider::set_sprite_flipping() {
    frameGfx = sprite_utils::get_frame((u8 *) gfx_spider_skeletonTiles, SPIDER_SPRITE_SIZE, animFrame + 4);
}

//!> after calling this function, call sprite_utils::update_frame to update OAM with current frameGfx
void Spider::set_sprite_jumping() {
    frameGfx = sprite_utils::get_frame((u8 *) gfx_spider_skeletonTiles, SPIDER_SPRITE_SIZE, animFrame);
}

//!> after calling this function, call sprite_utils::update_frame to update OAM with current frameGfx
void Spider::set_sprite_falling() {
    frameGfx = sprite_utils::get_frame((u8 *) gfx_spider_skeletonTiles, SPIDER_SPRITE_SIZE, 0);
}

void Spider::set_position() {

    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);

    if (!hanging) {
        main_y -= SPIDER_HANGING_OFFSET;
        sub_y -= SPIDER_HANGING_OFFSET;
    }

    sprite_utils::set_entry_xy(mainSpriteInfo, main_x, main_y);
    sprite_utils::set_entry_xy(subSpriteInfo, sub_x, sub_y);
}

Spider::Spider() {
    physical_height = SPIDER_PHYSICAL_HEIGHT;
    physical_width = SPIDER_PHYSICAL_WIDTH;
    sprite_height = SPIDER_SPRITE_HEIGHT;
    sprite_width = SPIDER_SPRITE_WIDTH;
    hitpoints = SPIDER_HITPOINTS;
    spritesheet_type = SpritesheetType::SKELETON_SPIDER;
    sprite_type = SpriteType::S_SPIDER;
}

void Spider::jump_to_main_dude() {

    if (!bottomCollision || time_since_last_jump < (200 + jump_delay))
        return;

    time_since_last_jump = 0;

    int diff = global::main_dude->x - x > 0;

    bool additional_jump_speed = false;

    if (diff) {

        if (previously_collided && time_since_last_big_jump > 2500) {

            if (previous_collision_side == SpriteState::W_RIGHT) {
                random_speed = 0.3f;
                xSpeed = 0.3f;
            } else {
                random_speed = -0.3f;
                xSpeed = -0.3f;
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

            if (previous_collision_side == SpriteState::W_RIGHT) {
                random_speed = 0.3f;
                xSpeed = 0.3f;
            } else {
                random_speed = -0.3f;
                xSpeed = -0.3f;
            }

            xSpeed = -0.3f;
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
        ySpeed = -3.5;
    } else
        ySpeed = -1.6 - ((rand() % 3) / 10);

}

Spider::Spider(int x, int y) : Spider() {
    this->x = x;
    this->y = y;
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

    sprite_utils::update_frame(frameGfx, SPIDER_SPRITE_SIZE, mainSpriteInfo, subSpriteInfo);
}
