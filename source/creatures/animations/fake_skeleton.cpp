//
// Created by xdbeef on 29.07.18.
//

#include <cmath>
#include "../../decorations/got_collectible.hpp"
#include "../../globals_declarations.hpp"
#include "../../../build/gfx_spider_skeleton.h"
#include "../../collisions/collisions.hpp"
#include "fake_skeleton.hpp"
#include "skull.hpp"
#include "../sprite_utils.hpp"

#define FAKE_SKELETON_POS_INC_DELTA 25
#define FRAME_OFFSET_FAKE_SKELETON_WITH_SKULL 33
#define FRAME_OFFSET_FAKE_SKELETON_WOUT_SKULL 23

//Skull object is spawned only when main dude attempts to pick it up,
//before that a single sprite is drawn that has both skeleton and skull. (saving some memory)

void FakeSkeleton::draw() {

    if (ready_to_dispose)
        return;

    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_priority(OBJPRIORITY_0, mainSpriteInfo, subSpriteInfo);
    set_position();

    if (!tried_to_pickup) {

        if (global::input_handler->y_key_down &&
            global::input_handler->down_key_held &&
            !global::main_dude->holding_item &&
            Collisions::checkCollisionWithMainDude(x, y, physical_width, physical_height)) {

            //pickup item from the ground

            global::main_dude->holding_item = true;
            global::input_handler->y_key_down = false;

            tried_to_pickup = true;
            match_animation();
            spawn_skull();
            physical_height = FAKE_SKELETON_PHYSICAL_HEIGHT_WOUT_SKULL;
            y += 2;
        }

    }

}


void FakeSkeleton::init() {
    initSprite();
}

void FakeSkeleton::initSprite() {

    delete mainSpriteInfo;
    delete subSpriteInfo;

    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_spider_skeletonPal, gfx_spider_skeletonPalLen,
                                                        nullptr, FAKE_SKELETON_SPRITE_SIZE, sprite_width,
                                                        spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_spider_skeletonPal, gfx_spider_skeletonPalLen,
                                                          nullptr, FAKE_SKELETON_SPRITE_SIZE, sprite_width,
                                                          spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_priority(OBJPRIORITY_0, mainSpriteInfo, subSpriteInfo);
    set_position();
    match_animation();
}

void FakeSkeleton::set_position() {
    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);
    sprite_utils::set_entry_xy(mainSpriteInfo, main_x, main_y);
    sprite_utils::set_entry_xy(subSpriteInfo, sub_x, sub_y);
}

FakeSkeleton::FakeSkeleton() {
    sprite_height = FAKE_SKELETON_SPRITE_HEIGHT;
    sprite_width = FAKE_SKELETON_SPRITE_WIDTH;
    physical_height = FAKE_SKELETON_PHYSICAL_HEIGHT_WITH_SKULL;
    physical_width = FAKE_SKELETON_PHYSICAL_WIDTH;
    spritesheet_type = SpritesheetType::SKELETON_SPIDER;
}

void FakeSkeleton::updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) {

    MapTile *t[9];
    Collisions::getNeighboringTiles(global::current_level->map_tiles,
                                    x_current_pos_in_tiles, y_current_pos_in_tiles, t);

    upperCollision = Collisions::checkUpperCollision(t, &x, &y, &ySpeed, physical_width, true, 0.35);
    bottomCollision = Collisions::checkBottomCollision(t, &x, &y, &ySpeed, physical_width, physical_height, true, 0.3f);
    leftCollision = Collisions::checkLeftCollision(t, &x, &y, &xSpeed, physical_width, physical_height, true, 0.15);
    rightCollision = Collisions::checkRightCollision(t, &x, &y, &xSpeed, physical_width, physical_height, true, 0.15);

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

void FakeSkeleton::spawn_skull() {
    auto s = new Skull();
    s->x = x;
    s->y = y;
    s->hold_by_main_dude = true;
    s->init();
    global::creatures_to_add.push_back(s);
}

void FakeSkeleton::match_animation() {

    if (tried_to_pickup)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spider_skeletonTiles, FAKE_SKELETON_SPRITE_SIZE,
                                           animFrame + FRAME_OFFSET_FAKE_SKELETON_WOUT_SKULL);
    else
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spider_skeletonTiles, FAKE_SKELETON_SPRITE_SIZE,
                                           animFrame + FRAME_OFFSET_FAKE_SKELETON_WITH_SKULL);

    sprite_utils::update_frame(frameGfx, FAKE_SKELETON_SPRITE_SIZE, mainSpriteInfo, subSpriteInfo);

}

void FakeSkeleton::deleteSprite() {
    delete mainSpriteInfo;
    delete subSpriteInfo;
    mainSpriteInfo = nullptr;
    subSpriteInfo = nullptr;
}
