//
// Created by xdbeef on 29.07.18.
//

#include "got_collectible.h"
#include "../../globals_declarations.h"
#include "../../../build/gfx_spider_skeleton.h"
#include "../../collisions/collisions.h"
#include "bone.h"
#include "../sprite_utils.h"

#define BONE_POS_INC_DELTA 35
#define BONE_ANIM_FRAME_DELTA  45
#define BONE_FRAMES 8
#define COLLIDED_FRAMES 7
#define FRAME_OFFSET_BONE 25
#define FRAME_OFFSET_COLLIDED 33

void Bone::draw() {

    if (ready_to_dispose)
        return;

    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_priority(OBJPRIORITY_0, mainSpriteInfo, subSpriteInfo);
    set_position();

    animFrameTimer += *global::timer;

    if (animFrameTimer > BONE_ANIM_FRAME_DELTA) {
        animFrameTimer = 0;
        animFrame++;
        match_animation();
    }

}


void Bone::init() {
    initSprite();
}

void Bone::initSprite() {

    delete mainSpriteInfo;
    delete subSpriteInfo;

    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_spider_skeletonPal, gfx_spider_skeletonPalLen,
                                                        nullptr, BONE_SPRITE_SIZE, sprite_width,
                                                        spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_spider_skeletonPal, gfx_spider_skeletonPalLen,
                                                          nullptr, BONE_SPRITE_SIZE, sprite_width,
                                                          spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_priority(OBJPRIORITY_0, mainSpriteInfo, subSpriteInfo);
    match_animation();
    set_position();
}

void Bone::set_position() {
    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);
    sprite_utils::set_entry_xy(mainSpriteInfo, main_x, main_y);
    sprite_utils::set_entry_xy(subSpriteInfo, sub_x, sub_y);
}

Bone::Bone() {
    sprite_height = BONE_SPRITE_HEIGHT;
    sprite_width = BONE_SPRITE_WIDTH;
    physical_height = BONE_PHYSICAL_HEIGHT;
    physical_width = BONE_PHYSICAL_WIDTH;
    spritesheet_type = SpritesheetType::SKELETON_SPIDER;
}

void Bone::updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) {

    MapTile *t[9];
    Collisions::getNeighboringTiles(global::current_level->map_tiles,
                                    x_current_pos_in_tiles, y_current_pos_in_tiles, t);

    upperCollision = Collisions::checkUpperCollision(t, &x, &y, &ySpeed, physical_width, true, 0.35);
    bottomCollision = Collisions::checkBottomCollision(t, &x, &y, &ySpeed, physical_width, physical_height, true, 0.35);
    leftCollision = Collisions::checkLeftCollision(t, &x, &y, &xSpeed, physical_width, physical_height, true, 0.15);
    rightCollision = Collisions::checkRightCollision(t, &x, &y, &xSpeed, physical_width, physical_height, true, 0.15);

    if (upperCollision || bottomCollision || leftCollision || rightCollision) {
        collided = true;
        animFrame = 0;
    }

}

void Bone::updateSpeed() {

    limit_speed(MAX_X_SPEED_BONE, MAX_Y_SPEED_BONE);

    pos_inc_timer += *global::timer;

    bool change_pos = (pos_inc_timer > BONE_POS_INC_DELTA) && !hold_by_main_dude;

    if (change_pos) {
        update_position();
        apply_friction(0.055);
        apply_gravity(GRAVITY_DELTA_SPEED);
        pos_inc_timer = 0;
    }

}

void Bone::match_animation() {

    if (collided) {

        if (animFrame >= COLLIDED_FRAMES) {
            //poof animation done, bone is ready to dispose
            sprite_utils::set_visibility(false, mainSpriteInfo, subSpriteInfo);
            ready_to_dispose = true;
        } else
            //continue poof animation
            frameGfx = sprite_utils::get_frame((u8 *) gfx_spider_skeletonTiles, BONE_SPRITE_SIZE,
                                               animFrame + FRAME_OFFSET_COLLIDED);

    } else {
        //rotating bone animation
        if (animFrame >= BONE_FRAMES)
            animFrame = 0;

        frameGfx = sprite_utils::get_frame((u8 *) gfx_spider_skeletonTiles, BONE_SPRITE_SIZE,
                                           animFrame + FRAME_OFFSET_BONE);
    }

    sprite_utils::update_frame(frameGfx, BONE_SPRITE_SIZE, mainSpriteInfo, subSpriteInfo);
}
