//
// Created by xdbeef on 01.06.18.
//

#include "got_collectible.h"
#include "../../globals_declarations.h"
#include "../../../build/gfx_blood_rock_rope_poof.h"
#include "../moving_object.h"
#include "fall_poof.h"
#include "../../collisions/collisions.h"
#include "../sprite_utils.h"


#define FALL_POOF_POS_INC_DELTA 35
#define FALL_POOF_ANIM_FRAME_DELTA  50
#define FALL_POOF_FRAMES 6

void FallPoof::draw() {

    if (ready_to_dispose)
        return;

    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_priority(OBJPRIORITY_0, mainSpriteInfo, subSpriteInfo);
    set_position();

    animFrameTimer += *global::timer;

    if (animFrameTimer > FALL_POOF_ANIM_FRAME_DELTA) {

        animFrameTimer = 0;
        animFrame++;

        if (animFrame >= FALL_POOF_FRAMES) {
            ready_to_dispose = true;
            sprite_utils::set_visibility(false, mainSpriteInfo, subSpriteInfo);
        } else
            match_animation();

    }

}


void FallPoof::init() {
    initSprite();
}

void FallPoof::initSprite() {

    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_blood_rock_rope_poofPal, gfx_blood_rock_rope_poofPalLen,
                                                        nullptr, FALL_POOF_SPRITE_SIZE, sprite_width,
                                                        spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_blood_rock_rope_poofPal, gfx_blood_rock_rope_poofPalLen,
                                                          nullptr, FALL_POOF_SPRITE_SIZE, sprite_width,
                                                          spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);

    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_priority(OBJPRIORITY_0, mainSpriteInfo, subSpriteInfo);
    match_animation();
    set_position();

}

void FallPoof::set_position() {
    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);
    sprite_utils::set_entry_xy(mainSpriteInfo, main_x, main_y);
    sprite_utils::set_entry_xy(subSpriteInfo, sub_x, sub_y);
}

FallPoof::FallPoof() {
    sprite_height = FALL_POOF_SPRITE_HEIGHT;
    sprite_width = FALL_POOF_SPRITE_WIDTH;
    physical_height = FALL_POOF_PHYSICAL_HEIGHT;
    physical_width = FALL_POOF_PHYSICAL_WIDTH;
    spritesheet_type = SpritesheetType::BLOOD_ROCK_ROPE_POOF;
}

void FallPoof::updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) {

    if (!gravity)
        return;

    MapTile *t[9];
    Collisions::getNeighboringTiles(global::level_generator->map_tiles,
                                    x_current_pos_in_tiles, y_current_pos_in_tiles, t);
    upperCollision = Collisions::checkUpperCollision(t, &x, &y, &ySpeed, physical_width, true, 0.35);
    bottomCollision = Collisions::checkBottomCollision(t, &x, &y, &ySpeed, physical_width, physical_height, true, 0.35);
    leftCollision = Collisions::checkLeftCollision(t, &x, &y, &xSpeed, physical_width, physical_height, true, 0.15);
    rightCollision = Collisions::checkRightCollision(t, &x, &y, &xSpeed, physical_width, physical_height, true, 0.15);

}

void FallPoof::updateSpeed() {

    if (!gravity)
        return;

    limit_speed(MAX_X_SPEED_FALL_POOF, MAX_Y_SPEED_FALL_POOF);

    pos_inc_timer += *global::timer;

    bool change_pos = (pos_inc_timer > FALL_POOF_POS_INC_DELTA) && !hold_by_main_dude;

    if (change_pos) {
        update_position();
        apply_friction(0.055);
        apply_gravity(GRAVITY_DELTA_SPEED);
        pos_inc_timer = 0;
    }

}

void FallPoof::match_animation() {
    frameGfx = sprite_utils::get_frame((u8 *) gfx_blood_rock_rope_poofTiles, FALL_POOF_SPRITE_SIZE, animFrame + 13);
    sprite_utils::update_frame(frameGfx, FALL_POOF_SPRITE_SIZE, mainSpriteInfo, subSpriteInfo);
}
