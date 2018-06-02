//
// Created by xdbeef on 28.04.18.
//

#include "blood_element.h"
#include "../../../build/gfx_blood_rock_rope_poof.h"
#include "../../globals_declarations.h"
#include "blood.h"
#include "../../collisions/collisions.h"
#include "../../tiles/map_utils.h"

void BloodElement::draw() {

    if (ready_to_dispose || finished) {
        return;
    }


    inactive_delay += *global::timer;

//    if (bottomCollision)
//        living_timer += 4 * *global::timer;
//    else
//        living_timer += *global::timer;


    if (!finished) {
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
//                if(living_timer > 1000) {
//                }

            } else {


//                if(living_timer < 1000 && currentFrame >= 5)
//                    currentFrame = 0;

                frameGfx = (u8 *) gfx_blood_rock_rope_poofTiles + (currentFrame * sprite_width * sprite_height / 2);
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

    if (inactive_delay < 90)
        return;

    pos_inc_timer += *global::timer;

    bool change_pos = (pos_inc_timer > BLOOD_CHANGE_POS_DELTA) && !finished;

    if (change_pos) {
//        apply_friction(0.01);
        update_position();
        apply_gravity(GRAVITY_DELTA_SPEED * 0.2f);
        pos_inc_timer = 0;
    }

}

void BloodElement::updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) {

    MapTile *tiles[9];
    Collisions::getNeighboringTiles(global::level_generator->map_tiles, x_current_pos_in_tiles, y_current_pos_in_tiles,
                                    tiles);

    bottomCollision = Collisions::checkBottomCollision(tiles, &x, &y, &ySpeed, physical_width, physical_height, true,
                                                       0.7f);
    leftCollision = Collisions::checkLeftCollision(tiles, &x, &y, &xSpeed, physical_width, physical_height, true, 0.7f);
    rightCollision = Collisions::checkRightCollision(tiles, &x, &y, &xSpeed, physical_width, physical_height, true,
                                                     0.7f);
    upperCollision = Collisions::checkUpperCollision(tiles, &x, &y, &ySpeed, physical_width, true, 0.7f);


}

void BloodElement::initSprite() {
    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_blood_rock_rope_poofPal, gfx_blood_rock_rope_poofPalLen,
                                                        nullptr, sprite_width * sprite_height, 8, BLOOD_ROCK_ROPE_POOF, true,
                                                        false, LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_blood_rock_rope_poofPal, gfx_blood_rock_rope_poofPalLen,
                                                          nullptr, sprite_width * sprite_height, 8, BLOOD_ROCK_ROPE_POOF,
                                                          true, false, LAYER_LEVEL::MIDDLE_TOP);

    frameGfx = (u8 *) gfx_blood_rock_rope_poofTiles + (currentFrame * sprite_width * sprite_height / 2);
    subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
    mainSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);

}

BloodElement::BloodElement() {
    physical_width = BLOOD_PHYSICAL_WIDTH;
    physical_height = BLOOD_PHYSICAL_HEIGHT;
    sprite_width = BLOOD_SPRITE_WIDTH;
    sprite_height = BLOOD_SPRITE_HEIGHT;
}