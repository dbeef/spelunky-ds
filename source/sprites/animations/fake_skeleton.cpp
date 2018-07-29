//
// Created by xdbeef on 29.07.18.
//

#include <cmath>
#include "got_collectible.h"
#include "../../globals_declarations.h"
#include "../../../build/gfx_spider_skeleton.h"
#include "../../collisions/collisions.h"
#include "fake_skeleton.h"
#include "skull.h"

#define FAKE_SKELETON_POS_INC_DELTA 25
#define FAKE_SKELETON_ANIM_FRAME_DELTA  45
#define FRAME_OFFSET_FAKE_SKELETON_WITH_SKULL 33
#define FRAME_OFFSET_FAKE_SKELETON_WOUT_SKULL 23
//Skull object is spawned only when main dude attempts to pick it up,
//before that a single sprite is drawn that has both skeleton and skull. (saving some memory)

void FakeSkeleton::draw() {

    if (ready_to_dispose) {
        mainSpriteInfo->entry->isHidden = true;
        subSpriteInfo->entry->isHidden = true;
        return;
    }

    if (!tried_to_pickup) {

        if (global::input_handler->y_key_down &&
            global::input_handler->down_key_held &&
            !global::main_dude->holding_item &&
            Collisions::checkCollisionWithMainDude(x, y, physical_width, physical_height)) {

            //pickup item from the ground

            global::main_dude->holding_item = true;
            global::input_handler->y_key_down = false;

            tried_to_pickup = true;
            frameGfx = (u8 *) gfx_spider_skeletonTiles +
                       (sprite_width * sprite_height * (animFrame + FRAME_OFFSET_FAKE_SKELETON_WOUT_SKULL) / 2);
            subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
            mainSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);

            //spawn skull

            Skull *s = new Skull();
            s->x = x;
            s->y = y;
            s->hold_by_main_dude = true;
            s->init();

            global::sprites.push_back(s);

            physical_height = FAKE_SKELETON_PHYSICAL_HEIGHT_WOUT_SKULL;
//            bottomCollision = false;
            y+=2;
//            y += FAKE_SKELETON_PHYSICAL_HEIGHT_WITH_SKULL - FAKE_SKELETON_PHYSICAL_HEIGHT_WOUT_SKULL;
        }

    }

    set_position();
}


void FakeSkeleton::init() {
    initSprite();
}

void FakeSkeleton::initSprite() {

    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_spider_skeletonPal, gfx_spider_skeletonPalLen,
                                                        nullptr, sprite_width * sprite_height, sprite_width,
                                                        spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_spider_skeletonPal, gfx_spider_skeletonPalLen,
                                                          nullptr, sprite_width * sprite_height, sprite_width,
                                                          spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);

    if (tried_to_pickup)
        frameGfx = (u8 *) gfx_spider_skeletonTiles +
                   (sprite_width * sprite_height * (animFrame + FRAME_OFFSET_FAKE_SKELETON_WOUT_SKULL) / 2);
    else
        frameGfx = (u8 *) gfx_spider_skeletonTiles +
                   (sprite_width * sprite_height * (animFrame + FRAME_OFFSET_FAKE_SKELETON_WITH_SKULL) / 2);

    subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
    mainSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);

}

void FakeSkeleton::set_position() {

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

FakeSkeleton::FakeSkeleton() {
    sprite_height = FAKE_SKELETON_SPRITE_HEIGHT;
    sprite_width = FAKE_SKELETON_SPRITE_WIDTH;
    physical_height = FAKE_SKELETON_PHYSICAL_HEIGHT_WITH_SKULL;
    physical_width = FAKE_SKELETON_PHYSICAL_WIDTH;
    spritesheet_type = SpritesheetType::SKELETON_SPIDER;
}

void FakeSkeleton::updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) {

    MapTile *tiles[9];
    Collisions::getNeighboringTiles(global::level_generator->map_tiles, x_current_pos_in_tiles, y_current_pos_in_tiles,
                                    tiles);

    upperCollision = Collisions::checkUpperCollision(tiles, &x, &y, &ySpeed, physical_width, true, BOUNCING_FACTOR_Y);
    bottomCollision = Collisions::checkBottomCollision(tiles, &x, &y, &ySpeed, physical_width, physical_height, true,
                                                       0.3f);
    leftCollision = Collisions::checkLeftCollision(tiles, &x, &y, &xSpeed, physical_width, physical_height, true,
                                                   BOUNCING_FACTOR_X);
    rightCollision = Collisions::checkRightCollision(tiles, &x, &y, &xSpeed, physical_width, physical_height, true,
                                                     BOUNCING_FACTOR_X);

}

void FakeSkeleton::updateSpeed() {

    limit_speed(MAX_X_SPEED_FAKE_SKELETON, MAX_Y_SPEED_FAKE_SKELETON);

    pos_inc_timer += *global::timer;

    bool change_pos = (pos_inc_timer > FAKE_SKELETON_POS_INC_DELTA) && !hold_by_main_dude;

    if (change_pos) {
        update_position();
        apply_friction(0.055);
        apply_gravity(GRAVITY_DELTA_SPEED * 3);
        pos_inc_timer = 0;
    }

}
