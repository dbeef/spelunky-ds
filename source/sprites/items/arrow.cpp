//
// Created by xdbeef on 04.04.18.
//

#include <cmath>
#include <iostream>
#include "../../globals_declarations.h"
#include "../main_dude/main_dude.h"
#include "../../../build/gfx_arrow.h"
#include "rock.h"
#include "../../collisions/collisions.h"
#include "../../tiles/map_utils.h"
#include "arrow.h"

#define ARROW_POS_INC_DELTA 15

void Arrow::draw() {


//    if (angle > 360 || angle < 0)
//        std::cout << angle << '\n';

    if (ready_to_dispose) {
        mainSpriteInfo->entry->isHidden = true;
        subSpriteInfo->entry->isHidden = true;
        return;
    } else {
        mainSpriteInfo->entry->isHidden = false;
        subSpriteInfo->entry->isHidden = false;
    }

    check_if_can_be_pickuped();

    if (hold_by_main_dude) {

        y = global::main_dude->y + 6;

        if (global::main_dude->sprite_state == SpriteState::W_LEFT) {
            x = global::main_dude->x - 2;
        } else
            x = global::main_dude->x + 10;


        if (global::main_dude->sprite_state == SpriteState::W_RIGHT)
            angle = 90;
        else
            angle = 270;

        double frame = floor(angle / 22.5f);
        update_frame(frame);
    }

    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);

    mainSpriteInfo->entry->x = main_x;
    mainSpriteInfo->entry->y = main_y;

    subSpriteInfo->entry->x = sub_x;
    subSpriteInfo->entry->y = sub_y;

    mainSpriteInfo->entry->hFlip = false;
    subSpriteInfo->entry->hFlip = false;

    mainSpriteInfo->entry->vFlip = false;
    subSpriteInfo->entry->vFlip = false;

    if(kill_mobs_if_thrown(1)){
//        ready_to_dispose = true;
    }

}


void Arrow::init() {
    activated = true;
    angle = 90;
    initSprite();
}

void Arrow::updateSpeed() {

    limit_speed(MAX_X_SPEED_ARROW, MAX_Y_SPEED_ARROW);

    pos_inc_timer += *global::timer;

    bool change_pos = (pos_inc_timer > ARROW_POS_INC_DELTA) && !hold_by_main_dude;

    if (change_pos) {
        apply_friction(0.085);
        update_position();
        apply_gravity(GRAVITY_DELTA_SPEED * 0.8);
        pos_inc_timer = 0;


        if (!bottomCollision) {

            double frame_before = floor(angle / 22.5f);

            //JAKO FUNKCJA PRĘDKOŚCI NA OBU OSIACH
            //change angle so arrow would point to ground
            if (angle > 0 && angle < 180) {
                angle += 8 * (1 / ((xSpeed)));
            } else if (angle > 180 && angle < 360) {
                angle -= 8 * (1 / ((xSpeed)));
            }

            if (angle < 0)
                angle = 0;
            if (angle > 360) {
                angle = 0;
            }

            double frame_after = floor(angle / 22.5f);

            if (frame_before != frame_after) {
                update_frame(frame_after);
            }
        }

    }

}


void Arrow::updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) {

    MapTile *tiles[9];
    Collisions::getNeighboringTiles(global::level_generator->map_tiles, x_current_pos_in_tiles, y_current_pos_in_tiles,
                                    tiles);


    upperCollision = Collisions::checkUpperCollision(tiles, &x, &y, &ySpeed, physical_width, true,
                                                     BOUNCING_FACTOR_Y * 2);

    bottomCollision = Collisions::checkBottomCollision(tiles, &x, &y, &ySpeed, physical_width, physical_height, true,
                                                       BOUNCING_FACTOR_Y*1.5);
    leftCollision = Collisions::checkLeftCollision(tiles, &x, &y, &xSpeed, physical_width, physical_height, true,
                                                   BOUNCING_FACTOR_X);
    rightCollision = Collisions::checkRightCollision(tiles, &x, &y, &xSpeed, physical_width, physical_height, true,
                                                     BOUNCING_FACTOR_X);


    double temp_angle = angle;

    if (bottomCollision) {
        if (angle > 90 && angle < 270) {
            if (angle > 90 && angle < 180) {
                //right-bottom collision
                angle = 180 - angle;
            } else if (angle > 180 && angle < 270) {
                //left-bottom collision
                angle = 360 - (angle - 180);
            }
        }
    }

    if (upperCollision) {
        if (angle > 270 && angle < 360) {
            //left-upper collision
            angle = 90 - (angle - 270);
        } else if (angle > 0 && angle < 90) {
            //right-upper collision
            angle = 270 + (90 - angle);
        }
    }

    if (rightCollision) {
        if (angle > 0 && angle < 90) {
            angle = 270 + (90 - angle);
        }
    }

    if (leftCollision) {
        if (angle > 270 && angle < 360) {
            angle = 360 - angle;
        }
    }


    if (bottomCollision && abs(ySpeed < 0.2f)) {
        if (angle > 0 && angle <= 180) {
            angle = 90;
        }
        else {
            angle = 270;
        }

    }

    if (temp_angle != angle) {
        double frame = floor(angle / 22.5f);
        update_frame(frame);
    }

}

void Arrow::initSprite() {

    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_arrowPal, gfx_arrowPalLen,
                                                        nullptr, sprite_width * sprite_height, 8,
                                                        SpritesheetType::ARROW, true, false,
                                                        LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_arrowPal, gfx_arrowPalLen,
                                                          nullptr, sprite_width * sprite_height, 8,
                                                          SpritesheetType::ARROW, true, false,
                                                          LAYER_LEVEL::MIDDLE_TOP);

    double frame = floor(angle / 22.5f);
    update_frame(frame);

}

Arrow::Arrow() {
    physical_height = ARROW_PHYSICAL_HEIGHT;
    physical_width = ARROW_PHYSICAL_WIDTH;
    sprite_height = ARROW_SPRITE_HEIGHT;
    sprite_width = ARROW_SPRITE_WIDTH;
    spritesheet_type = SpritesheetType::ARROW;
}

void Arrow::update_frame(int frame_num) {

//    if (frame_num >= 16)
//    std::cout << frame_num << " " << angle << '\n';

    frameGfx = (u8 *) gfx_arrowTiles + (sprite_height * sprite_width * (frame_num) / 2);
    subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
    mainSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
}
