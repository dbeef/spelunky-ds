//
// Created by xdbeef on 28.04.18.
//

#include "blood_element.h"
#include "../../../build/gfx_blood_rock_rope.h"
#include "../../globals_declarations.h"
#include "blood.h"
#include "../../collisions/collisions.h"
#include "../../tiles/map_utils.h"

void BloodElement::draw() {

    if (ready_to_dispose)
        return;

    if (!finished) {
        frameTimer += *global::timer;

        if (frameTimer > BLOOD_ANIM_FRAME_DELTA) {
            frameTimer = 0;
            currentFrame++;

            if (currentFrame >= 7) {
                currentFrame = 0;
                finished = true;
                mainSpriteInfo->entry->isHidden = true;
                subSpriteInfo->entry->isHidden = true;
            } else {
                frameGfx = (u8 *) gfx_blood_rock_ropeTiles + (currentFrame * sprite_width * sprite_height / 2);
                subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
                mainSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
            }
        }

        int main_x, main_y, sub_x, sub_y;
        get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);

        mainSpriteInfo->entry->x = main_x;
        mainSpriteInfo->entry->y = main_y;

        subSpriteInfo->entry->x = sub_x;
        subSpriteInfo->entry->y = sub_y;

    }

}


void BloodElement::init() {
    initSprite();
}

void BloodElement::updateSpeed() {

    limit_speed(MAX_X_SPEED_BLOOD, MAX_Y_SPEED_BLOOD);

    pos_inc_timer += *global::timer;

    bool change_pos = (pos_inc_timer > BLOOD_CHANGE_POS_DELTA) && !finished;

    if (change_pos) {
        update_position();
        apply_friction(0.055);
        apply_gravity(GRAVITY_DELTA_SPEED);
        pos_inc_timer = 0;
    }

}

void BloodElement::updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) {

    MapTile *tiles[9];
    Collisions::getNeighboringTiles(global::level_generator->map_tiles, x_current_pos_in_tiles, y_current_pos_in_tiles,
                                    tiles);

    bottomCollision = Collisions::checkBottomCollision(tiles, &x, &y, &ySpeed, physical_width, physical_height, true);
    leftCollision = Collisions::checkLeftCollision(tiles, &x, &y, &xSpeed, physical_width, physical_height, true);
    rightCollision = Collisions::checkRightCollision(tiles, &x, &y, &xSpeed, physical_width, physical_height, true);
    upperCollision = Collisions::checkUpperCollision(tiles, &x, &y, &ySpeed, physical_width, true);


}

void BloodElement::initSprite() {
    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_blood_rock_ropePal, gfx_blood_rock_ropePalLen,
                                                        nullptr, sprite_width * sprite_height, 8, BLOOD_ROCK_ROPE, true,
                                                        false, LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_blood_rock_ropePal, gfx_blood_rock_ropePalLen,
                                                          nullptr, sprite_width * sprite_height, 8, BLOOD_ROCK_ROPE,
                                                          true, false, LAYER_LEVEL::MIDDLE_TOP);

    frameGfx = (u8 *) gfx_blood_rock_ropeTiles + (currentFrame * sprite_width * sprite_height / 2);
    subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
    mainSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);

}

BloodElement::BloodElement() {
    physical_width = BLOOD_PHYSICAL_WIDTH;
    physical_height = BLOOD_PHYSICAL_HEIGHT;
    sprite_width = BLOOD_SPRITE_WIDTH;
    sprite_height = BLOOD_SPRITE_HEIGHT;
}