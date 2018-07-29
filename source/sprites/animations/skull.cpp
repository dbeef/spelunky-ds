//
// Created by xdbeef on 29.07.18.
//



#include <cmath>
#include "got_collectible.h"
#include "../../globals_declarations.h"
#include "../../../build/gfx_spider_skeleton.h"
#include "../../collisions/collisions.h"
#include "skull.h"

#define SKULL_POS_INC_DELTA 35
#define SKULL_ANIM_FRAME_DELTA  45
#define COLLIDED_FRAMES 7
#define FRAME_OFFSET_SKULL 24
#define FRAME_OFFSET_COLLIDED 33

void Skull::draw() {

    if (ready_to_dispose) {
        mainSpriteInfo->entry->isHidden = true;
        subSpriteInfo->entry->isHidden = true;
        return;
    }

    if (collided)
        animFrameTimer += *global::timer;

    if (animFrameTimer > SKULL_ANIM_FRAME_DELTA) {

        animFrameTimer = 0;
        animFrame++;


        if (animFrame >= COLLIDED_FRAMES)
            ready_to_dispose = true;
        else {

            frameGfx = (u8 *) gfx_spider_skeletonTiles +
                       (sprite_width * sprite_height * (animFrame + FRAME_OFFSET_COLLIDED) / 2);
        }

        subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
        mainSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);

    }


    set_position();
}


void Skull::init() {
    initSprite();
}

void Skull::initSprite() {

    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_spider_skeletonPal, gfx_spider_skeletonPalLen,
                                                        nullptr, sprite_width * sprite_height, sprite_width,
                                                        spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_spider_skeletonPal, gfx_spider_skeletonPalLen,
                                                          nullptr, sprite_width * sprite_height, sprite_width,
                                                          spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);

    frameGfx = (u8 *) gfx_spider_skeletonTiles +
               (sprite_width * sprite_height * (animFrame + FRAME_OFFSET_SKULL) / 2);

    subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
    mainSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);

}

void Skull::set_position() {

    if (ready_to_dispose)
        return;

    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);

    mainSpriteInfo->entry->x = main_x;
    mainSpriteInfo->entry->y = main_y;

    subSpriteInfo->entry->x = sub_x;
    subSpriteInfo->entry->y = sub_y;

    mainSpriteInfo->entry->vFlip = false;
    mainSpriteInfo->entry->hFlip = false;

    subSpriteInfo->entry->vFlip = false;
    subSpriteInfo->entry->hFlip = false;

    mainSpriteInfo->entry->priority = OBJPRIORITY_0;
    subSpriteInfo->entry->priority = OBJPRIORITY_0;

}

Skull::Skull() {
    sprite_height = SKULL_SPRITE_HEIGHT;
    sprite_width = SKULL_SPRITE_WIDTH;
    physical_height = SKULL_PHYSICAL_HEIGHT;
    physical_width = SKULL_PHYSICAL_WIDTH;
    spritesheet_type = SpritesheetType::SKELETON_SPIDER;
}

void Skull::updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) {

    MapTile *tiles[9];
    Collisions::getNeighboringTiles(global::level_generator->map_tiles, x_current_pos_in_tiles, y_current_pos_in_tiles,
                                    tiles);

    upperCollision = Collisions::checkUpperCollision(tiles, &x, &y, &ySpeed, physical_width, true, BOUNCING_FACTOR_Y);
    bottomCollision = Collisions::checkBottomCollision(tiles, &x, &y, &ySpeed, physical_width, physical_height, true,
                                                       BOUNCING_FACTOR_Y);
    leftCollision = Collisions::checkLeftCollision(tiles, &x, &y, &xSpeed, physical_width, physical_height, true,
                                                   BOUNCING_FACTOR_X);
    rightCollision = Collisions::checkRightCollision(tiles, &x, &y, &xSpeed, physical_width, physical_height, true,
                                                     BOUNCING_FACTOR_X);

    if ((upperCollision || bottomCollision || leftCollision || rightCollision)
        && (fabs(xSpeed) > 0 || fabs(ySpeed) > 0)) {
        collided = true;
        animFrame = 0;
    }

}

void Skull::updateSpeed() {

    limit_speed(MAX_X_SPEED_SKULL, MAX_Y_SPEED_SKULL);

    pos_inc_timer += *global::timer;

    bool change_pos = (pos_inc_timer > SKULL_POS_INC_DELTA) && !hold_by_main_dude;

    if (change_pos) {
        update_position();
        apply_friction(0.055);
        apply_gravity(GRAVITY_DELTA_SPEED);
        pos_inc_timer = 0;
    }

}
