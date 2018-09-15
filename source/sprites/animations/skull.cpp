//
// Created by xdbeef on 29.07.18.
//

#include <cmath>
#include "got_collectible.h"
#include "../../globals_declarations.h"
#include "../../../build/gfx_spider_skeleton.h"
#include "../../collisions/collisions.h"
#include "skull.h"
#include "bone.h"
#include "../sprite_utils.h"

#define SKULL_POS_INC_DELTA 22
#define SKULL_ANIM_FRAME_DELTA  45
#define COLLIDED_FRAMES 7
#define FRAME_OFFSET_SKULL 24
#define FRAME_OFFSET_COLLIDED 33

void Skull::draw() {

    if (ready_to_dispose)
        return;

    sprite_utils::set_visibility(true, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_priority(OBJPRIORITY_0, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    set_position();

    if (hold_by_main_dude)
        sprite_utils::set_horizontal_flip(global::main_dude->sprite_state == SpriteState::W_RIGHT,
                                          mainSpriteInfo, subSpriteInfo);

    set_pickuped_position(0, 3);

    if (collided)
        animFrameTimer += *global::timer;

    if (animFrameTimer > SKULL_ANIM_FRAME_DELTA) {

        animFrameTimer = 0;
        animFrame++;

        match_animation();
    }

}


void Skull::init() {
    initSprite();
}

void Skull::initSprite() {

    delete mainSpriteInfo;
    delete subSpriteInfo;

    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_spider_skeletonPal, gfx_spider_skeletonPalLen,
                                                        nullptr, SKULL_SPRITE_SIZE, sprite_width,
                                                        spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_spider_skeletonPal, gfx_spider_skeletonPalLen,
                                                          nullptr, SKULL_SPRITE_SIZE, sprite_width,
                                                          spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
    match_animation();
    sprite_utils::set_visibility(true, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_priority(OBJPRIORITY_0, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    set_position();

    if (hold_by_main_dude)
        sprite_utils::set_horizontal_flip(global::main_dude->sprite_state == SpriteState::W_RIGHT,
                                          mainSpriteInfo, subSpriteInfo);

}

void Skull::set_position() {
    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);
    sprite_utils::set_entry_xy(mainSpriteInfo, main_x, main_y);
    sprite_utils::set_entry_xy(subSpriteInfo, sub_x, sub_y);
}

Skull::Skull() {
    sprite_height = SKULL_SPRITE_HEIGHT;
    sprite_width = SKULL_SPRITE_WIDTH;
    physical_height = SKULL_PHYSICAL_HEIGHT;
    physical_width = SKULL_PHYSICAL_WIDTH;
    spritesheet_type = SpritesheetType::SKELETON_SPIDER;
}

void Skull::updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) {

    MapTile *t[9];
    Collisions::getNeighboringTiles(global::current_level->map_tiles,
                                    x_current_pos_in_tiles, y_current_pos_in_tiles, t);

    upperCollision = Collisions::checkUpperCollision(t, &x, &y, &ySpeed, physical_width, true, 0.35);
    bottomCollision = Collisions::checkBottomCollision(t, &x, &y, &ySpeed, physical_width, physical_height, true, 0.35);
    leftCollision = Collisions::checkLeftCollision(t, &x, &y, &xSpeed, physical_width, physical_height, true, 0.15);
    rightCollision = Collisions::checkRightCollision(t, &x, &y, &xSpeed, physical_width, physical_height, true, 0.15);

    if (((upperCollision || bottomCollision || leftCollision || rightCollision)
         && (fabs(xSpeed) > 0 || fabs(ySpeed) > 0)) && !collided) {

        collided = true;
        animFrame = 0;

        auto b = new Bone();
        b->x = x;
        b->y = y - 5;
        b->xSpeed = 0;
        b->ySpeed = -1.4f;
        b->animFrame = 2;
        b->init();
        global::sprites_to_add.push_back(b);
    }

}

void Skull::updateSpeed() {

    if (collided)
        return;

    limit_speed(MAX_X_SPEED_SKULL, MAX_Y_SPEED_SKULL);
    pos_inc_timer += *global::timer;

    bool change_pos = (pos_inc_timer > SKULL_POS_INC_DELTA) && !hold_by_main_dude;

    if (change_pos) {
        update_position();
        apply_friction(0.055);
        apply_gravity(GRAVITY_DELTA_SPEED * 3);
        pos_inc_timer = 0;
    }

}

void Skull::match_animation() {


    if (collided) {

        if (animFrame >= COLLIDED_FRAMES) {
            ready_to_dispose = true;
            sprite_utils::set_visibility(false, mainSpriteInfo, subSpriteInfo);
        }

        frameGfx = sprite_utils::get_frame((u8 *) gfx_spider_skeletonTiles, SKULL_SPRITE_SIZE,
                                           animFrame + FRAME_OFFSET_COLLIDED);

    } else
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spider_skeletonTiles, SKULL_SPRITE_SIZE,
                                           FRAME_OFFSET_SKULL);

    sprite_utils::update_frame(frameGfx, SKULL_SPRITE_SIZE, mainSpriteInfo, subSpriteInfo);
}
