//
// Created by xdbeef on 02.05.18.
//

#include <maxmod9.h>
#include <cstdlib>
#include "bat.h"
#include "../../globals_declarations.h"
#include "../../../build/gfx_bat_snake_jetpack.h"
#include "../items/rock.h"
#include "../animations/blood.h"
#include "../../collisions/collisions.h"
#include "../../tiles/level_rendering_utils.h"
#include "../../../build/soundbank.h"
#include "../sprite_utils.h"

#define BAT_ANIM_FRAME_DELTA 100
#define BAT_POS_INC_DELTA 30
#define BAT_HITPOINTS 1

void Bat::draw() {

    if (ready_to_dispose)
        return;

//    printf("1");
    set_position();
//    printf("2");

    sprite_utils::set_horizontal_flip(true, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_visibility(true, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
//    printf("3");

    if (!hunting) {
        //check if main dude is in bat's triggering scope
        hunting = abs(x - global::main_dude->x) < 7 * 16 &&
                  abs(y - global::main_dude->y) < 7 * 16 &&
                  global::main_dude->y > y;

        if (hunting)
            mmEffect(SFX_XBAT);

    } else
        //checking if main dude is still in bat's triggering scope
        hunting = abs(x - global::main_dude->x) < 9 * 16 && abs(y - global::main_dude->y) < 9 * 16;

    animFrameTimer += *global::timer;

    if (animFrameTimer > BAT_ANIM_FRAME_DELTA) {

        animFrameTimer = 0;
        animFrame++;

        if (animFrame >= 3)
            animFrame = 0;

//        printf("4");
        match_animation();
//        printf("5");
    }

    if (hunting) {
        follow_main_dude();
    } else {
        //fly upwards, to seek something to hang on
        xSpeed = 0;
        ySpeed = -1;
    }

//    printf("6");
//here
    kill_if_whip(1);
    kill_if_main_dude_jumped_on_you(1);
    deal_damage_main_dude_on_collision(1);

}


void Bat::init() {
    initSprite();
    spritesheet_type = SpritesheetType::BAT_JETPACK;
}

void Bat::updateSpeed() {

    limit_speed(MAX_X_SPEED_BAT, MAX_Y_SPEED_BAT);

    pos_inc_timer += *global::timer;

    if (pos_inc_timer > BAT_POS_INC_DELTA) {
        update_position();
        pos_inc_timer = 0;
    }

}


void Bat::updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) {

    MapTile *t[9] = {};
    Collisions::getNeighboringTiles(global::current_level->map_tiles,
                                    x_current_pos_in_tiles, y_current_pos_in_tiles, t);

    standingOnLeftEdge = Collisions::isStandingOnLeftEdge(t, x, physical_width, x_current_pos_in_tiles);
    standingOnRightEdge = Collisions::isStandingOnRightEdge(t, x, 16, x_current_pos_in_tiles);
    bottomCollision = Collisions::checkBottomCollision(t, &x, &y, &ySpeed, physical_width, physical_height, false, 0);
    leftCollision = Collisions::checkLeftCollision(t, &x, &y, &xSpeed, physical_width, physical_height, false, 0);
    rightCollision = Collisions::checkRightCollision(t, &x, &y, &xSpeed, physical_width, physical_height, false, 0);
    upperCollision = Collisions::checkUpperCollision(t, &x, &y, &ySpeed, physical_width, false, 0);

    hanging = upperCollision && !hunting;
}

//!>bat has only 1 dmg point, always kill if any dmg_apply
void Bat::apply_dmg(int dmg_to_apply) {

    sprite_utils::set_visibility(false, mainSpriteInfo, subSpriteInfo);
    killed = true;
    ready_to_dispose = true;
    spawn_blood();
    global::killed_npcs.push_back(SpriteType::S_BAT);

}

void Bat::initSprite() {

    delete subSpriteInfo;
    delete mainSpriteInfo;

    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_bat_snake_jetpackPal, gfx_bat_snake_jetpackPalLen,
                                                        nullptr, BAT_SPRITE_SIZE, 16, BAT_JETPACK, true,
                                                        false, LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_bat_snake_jetpackPal, gfx_bat_snake_jetpackPalLen,
                                                          nullptr, BAT_SPRITE_SIZE, 16, BAT_JETPACK, true,
                                                          false, LAYER_LEVEL::MIDDLE_TOP);

    match_animation();

    sprite_utils::set_horizontal_flip(true, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_visibility(true, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);

}

//!> after calling this function, call sprite_utils::update_frame to update OAM with current frameGfx
void Bat::set_sprite_hanging() {
    frameGfx = sprite_utils::get_frame((u8 *) gfx_bat_snake_jetpackTiles, BAT_SPRITE_SIZE, 0);
}

//!> after calling this function, call sprite_utils::update_frame to update OAM with current frameGfx
void Bat::set_sprite_flying_right() {
    frameGfx = sprite_utils::get_frame((u8 *) gfx_bat_snake_jetpackTiles, BAT_SPRITE_SIZE, animFrame + 1);
}

//!> after calling this function, call sprite_utils::update_frame to update OAM with current frameGfx
void Bat::set_sprite_flying_left() {
    frameGfx = sprite_utils::get_frame((u8 *) gfx_bat_snake_jetpackTiles, BAT_SPRITE_SIZE, animFrame + 4);
}

void Bat::set_position() {
    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);
    sprite_utils::set_entry_xy(mainSpriteInfo, main_x, main_y);
    sprite_utils::set_entry_xy(subSpriteInfo, sub_x, sub_y);
}

Bat::Bat() {
    physical_height = BAT_PHYSICAL_HEIGHT;
    physical_width = BAT_PHYSICAL_WIDTH;
    sprite_height = BAT_SPRITE_HEIGHT;
    sprite_width = BAT_SPRITE_WIDTH;
    hitpoints = BAT_HITPOINTS;
}

//!> naive approach to following main dude, disregarding obstacles just like in the original game
void Bat::follow_main_dude() {
    if (global::main_dude->x > x)
        xSpeed = 1;
    else
        xSpeed = -1;

    if (global::main_dude->y == y)
        ySpeed = 0;
    else if (global::main_dude->y > y)
        ySpeed = 1;
    else
        ySpeed = -1;
}


Bat::Bat(int x, int y) : Bat() {
    this->x = x;
    this->y = y;
}

void Bat::match_animation() {
    if (hanging)
        set_sprite_hanging();
    else if (xSpeed >= 0)
        set_sprite_flying_right();
    else if (xSpeed <= 0)
        set_sprite_flying_left();

//    printf("A");
    sprite_utils::update_frame(frameGfx, BAT_SPRITE_SIZE, mainSpriteInfo, subSpriteInfo);
}
