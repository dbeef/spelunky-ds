//
// Created by xdbeef on 28.04.18.
//

#include "blood.h"
#include "../../globals_declarations.h"
#include "../../../build/gfx_blood_rock_rope.h"
#include "../../collisions/collisions.h"
#include "../../tiles/map_utils.h"

void Blood::draw() {

    if (ready_to_dispose)
        return;

    if (finished) {

        bool chain_finished = true;

        for (int a = 0; a < bloodTrail.size(); a++) {
            if (!bloodTrail.at(a)->finished) {
                chain_finished = false;
                break;
            } else
                bloodTrail.at(a)->ready_to_dispose = true;
        }

        if (chain_finished)
            ready_to_dispose = true;


    } else {
        animFrameTimer += *global::timer;

        if (animFrameTimer > BLOOD_ANIM_FRAME_DELTA) {
            animFrameTimer = 0;
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


            if (currentFrame % 2 == 0 && !finished && bloodTrail.size() < 3) {
                spawn_blood();
            }

        }


        int main_x, main_y, sub_x, sub_y;
        get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);

        for (int a = 0; a < bloodTrail.size(); a++) {
            bloodTrail.at(a)->draw();
            bloodTrail.at(a)->update();
        }

        mainSpriteInfo->entry->x = main_x;
        mainSpriteInfo->entry->y = main_y;

        subSpriteInfo->entry->x = sub_x;
        subSpriteInfo->entry->y = sub_y;

    }

}


void Blood::init() {

    initSprite();

    hold_by_main_dude = false;
    activated_by_main_dude = false;

}

void Blood::updateSpeed() {

    limit_speed(MAX_X_SPEED_BLOOD, MAX_Y_SPEED_BLOOD);

    pos_inc_timer += *global::timer;

    bool change_pos = (pos_inc_timer > BLOOD_CHANGE_POS_DELTA) && !finished;

    if (change_pos) {
        apply_friction(0.055);
        apply_gravity(GRAVITY_DELTA_SPEED);
        pos_inc_timer = 0;
        update_position();
    }

}

void Blood::updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) {

    MapTile *tiles[9];
    Collisions::getNeighboringTiles(global::level_generator->map_tiles, x_current_pos_in_tiles, y_current_pos_in_tiles,
                                    tiles);

    bottomCollision = Collisions::checkBottomCollision(tiles, &x, &y, &ySpeed, physical_width, physical_height, true);
    leftCollision = Collisions::checkLeftCollision(tiles, &x, &y, &xSpeed, physical_width, physical_height, true);
    rightCollision = Collisions::checkRightCollision(tiles, &x, &y, &xSpeed, physical_width, physical_height, true);
    upperCollision = Collisions::checkUpperCollision(tiles, &x, &y, &ySpeed, physical_width, true);

}

void Blood::initSprite() {
    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_blood_rock_ropePal, gfx_blood_rock_ropePalLen,
                                                        nullptr, sprite_width * sprite_height, 8, BLOOD_ROCK_ROPE, true,
                                                        false,
                                                        LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_blood_rock_ropePal, gfx_blood_rock_ropePalLen,
                                                          nullptr, sprite_width * sprite_height, 8, BLOOD_ROCK_ROPE,
                                                          true, false,
                                                          LAYER_LEVEL::MIDDLE_TOP);

    frameGfx = (u8 *) gfx_blood_rock_ropeTiles + 0 * sprite_width * sprite_height / 2;
    subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
    mainSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
}

Blood::Blood() {
    physical_width = BLOOD_PHYSICAL_WIDTH;
    physical_height = BLOOD_PHYSICAL_HEIGHT;
    sprite_width = BLOOD_SPRITE_WIDTH;
    sprite_height = BLOOD_SPRITE_HEIGHT;
}

void Blood::spawn_blood() {
    BloodElement *element = new BloodElement();
    element->x = x;
    element->y = y - 4;
    element->xSpeed = xSpeed * 0.9;
    element->ySpeed = ySpeed * 0.9;
    element->currentFrame = currentFrame;
    element->init();
    element->draw();
    bloodTrail.push_back(element);
}
