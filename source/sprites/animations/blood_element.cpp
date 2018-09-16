//
// Created by xdbeef on 28.04.18.
//

#include "blood_element.hpp"
#include "../../../build/gfx_blood_rock_rope_poof.h"
#include "../../globals_declarations.hpp"
#include "blood.hpp"
#include "../../collisions/collisions.hpp"
#include "../../tiles/level_rendering_utils.hpp"
#include "../sprite_utils.hpp"

void BloodElement::draw() {

    if (ready_to_dispose || finished)
        return;

    //won't change speed untill this timer reaches treshold
    inactive_delay += *global::timer;

    //animation not finished
    frameTimer += *global::timer;

    if (frameTimer > BLOOD_ANIM_FRAME_DELTA) {

        frameTimer = 0;
        currentFrame++;

        if (currentFrame >= 7) {
            currentFrame = 0;
            finished = true;
            ready_to_dispose = true;
            mainSpriteInfo->entry->isHidden = true;
            subSpriteInfo->entry->isHidden = true;

        } else
            match_animation();

    }

    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);
    set_position();

}


void BloodElement::init() {
    initSprite();
}

void BloodElement::updateSpeed() {

    limit_speed(MAX_X_SPEED_BLOOD, MAX_Y_SPEED_BLOOD);

    if (inactive_delay < 90)
        return;

    pos_inc_timer += *global::timer;

    bool change_pos = (pos_inc_timer > BLOOD_CHANGE_POS_DELTA) && !finished;

    if (change_pos) {
        update_position();
        apply_gravity(GRAVITY_DELTA_SPEED * 0.2f);
        pos_inc_timer = 0;
    }

}

void BloodElement::updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) {
    MapTile *t[9];
    Collisions::getNeighboringTiles(global::current_level->map_tiles,
                                    x_current_pos_in_tiles, y_current_pos_in_tiles, t);
    upperCollision = Collisions::checkUpperCollision(t, &x, &y, &ySpeed, physical_width, true, 0.7f);
    bottomCollision = Collisions::checkBottomCollision(t, &x, &y, &ySpeed, physical_width, physical_height, true, 0.7f);
    leftCollision = Collisions::checkLeftCollision(t, &x, &y, &xSpeed, physical_width, physical_height, true, 0.7f);
    rightCollision = Collisions::checkRightCollision(t, &x, &y, &xSpeed, physical_width, physical_height, true, 0.7f);
}

void BloodElement::initSprite() {

    delete mainSpriteInfo;
    delete subSpriteInfo;

    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_blood_rock_rope_poofPal, gfx_blood_rock_rope_poofPalLen,
                                                        nullptr, BLOOD_SPRITE_SIZE, 8, BLOOD_ROCK_ROPE_POOF,
                                                        true, false, LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_blood_rock_rope_poofPal, gfx_blood_rock_rope_poofPalLen,
                                                          nullptr, BLOOD_SPRITE_SIZE, 8, BLOOD_ROCK_ROPE_POOF,
                                                          true, false, LAYER_LEVEL::MIDDLE_TOP);
    match_animation();
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);
    set_position();
}

BloodElement::BloodElement() {
    physical_width = BLOOD_PHYSICAL_WIDTH;
    physical_height = BLOOD_PHYSICAL_HEIGHT;
    sprite_width = BLOOD_SPRITE_WIDTH;
    sprite_height = BLOOD_SPRITE_HEIGHT;
}

void BloodElement::set_position() {
    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);
    sprite_utils::set_entry_xy(mainSpriteInfo, main_x, main_y);
    sprite_utils::set_entry_xy(subSpriteInfo, sub_x, sub_y);
}

void BloodElement::match_animation() {
    frameGfx = sprite_utils::get_frame((u8 *) gfx_blood_rock_rope_poofTiles, BLOOD_SPRITE_SIZE, currentFrame);
    sprite_utils::update_frame(frameGfx, BLOOD_SPRITE_SIZE, mainSpriteInfo, subSpriteInfo);
}
