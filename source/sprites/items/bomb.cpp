//
// Created by xdbeef on 04.04.18.
//

#include "bomb.h"
#include "../../globals_declarations.h"
#include "../../tiles/map_utils.h"
#include "../../collisions/collisions.h"
#include "../main_dude/main_dude.h"
#include "../../../build/gfx_explosion.h"
#include "../../../build/soundbank.h"
#include "../../tiles/map_utils.h"
#include "../../collisions/collisions.h"
#include <math.h>       /* floor */
#include <maxmod9.h>


void Bomb::draw() {

    if(ready_to_dispose)
        return;

    if (hold_by_main_dude && global::input_handler->y_key_down && global::input_handler->down_key_held) {
        hold_by_main_dude = false;
        global::main_dude->holding_item = false;
    } else if (global::input_handler->y_key_down && global::input_handler->down_key_held && explosionFrame == 0 &&
               !global::main_dude->holding_item) {

        if (Collisions::checkCollisionWithMainDude(x, y, 8, 8)) {
            hold_by_main_dude = true;
            global::main_dude->holding_item = true;
//            std::cout << "TOOK ITEM";
        }
    }


    if (hold_by_main_dude) {

        y = global::main_dude->y + 6;

        if (global::main_dude->state == 1) {
            x = global::main_dude->x - 2;
        } else
            x = global::main_dude->x + 10;
    }


    if (activated_by_main_dude && !armed) {
        arm();
        armed = true;
        armedTimer = 0;
//        std::cout<< main_x << " " << main_y << " " << x << " "<<  y << '\n';
    }

    if (armed) {
        armedTimer += *global::timer;
        if (armedTimer < ARMED_TIME_BLINK_SLOW) {
            if ((armedTimer) % 250 < 125) {
                disarm();
            } else {
                arm();
            }
        } else if (armedTimer < ARMED_TIME_BLINK_FAST) {
            if ((armedTimer) % 120 < 60) {
                disarm();
            } else {
                arm();
            }
        } else {


            explosionTimer += *global::timer;
            if (explosionTimer > 50 && explosionFrame < 10) {

                if (explosionFrame == 0) {

                    mmEffect(SFX_XEXPLOSION);

                    int xx = floor_div(this->x + 0.5 * BOMB_SIZE, 16);
                    int yy = floor_div(this->y + 0.5 * BOMB_SIZE, 16);

                    Collisions::bombNeighboringTiles(global::level_generator->map_tiles, xx, yy);
                    global::bombed = true;
//                    std::cout<< main_x << " " << main_y << " " << x << " "<<  y << '\n';

                }

                explosionTimer = 0;
                explosionFrame++;

                if (explosionFrame >= 10) {

                    subSpriteInfo->updateFrame(nullptr, 64 * 64);
                    mainSpriteInfo->updateFrame(nullptr, 64 * 64);

                    subSpriteInfo = nullptr;
                    mainSpriteInfo= nullptr;

                    ready_to_dispose = true;
                } else {
                    frameGfx = (u8 *) gfx_explosionTiles + (2 + explosionFrame) * 64 * 64 / 2;
                    subSpriteInfo->updateFrame(frameGfx, 64 * 64);
                    mainSpriteInfo->updateFrame(frameGfx, 64 * 64);
                }
            }
        }

    }


    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);


    //that's because 8x8 bomb sprite is stored in 64x64 sprite, packed with explosion animation
    if (explosionFrame > 0) {
        main_x -= 32;
        main_y -= 32;

        sub_x -= 32;
        sub_y -= 32;

    } else {
        main_x -= 29;
        main_y -= 29;

        sub_x -= 29;
        sub_y -= 29;
    }


    mainSpriteInfo->entry->x = main_x;
    mainSpriteInfo->entry->y = main_y;

    subSpriteInfo->entry->x = sub_x;
    subSpriteInfo->entry->y = sub_y;

    if (xSpeed > 0 || ySpeed > 0) {
        for (int a = 0; a < global::sprites.size(); a++) {
            if((global::sprites.at(a)->spriteType == SpritesheetType::SNAKE || global::sprites.at(a)->spriteType == SpritesheetType::BAT|| global::sprites.at(a)->spriteType == SpritesheetType::SPIDER)
               && !global::sprites.at(a)->killed){
                if(Collisions::checkCollisionBodies(x, y, 8, 8, global::sprites.at(a)->x, global::sprites.at(a)->y, 16, 16)){
                    global::sprites.at(a)->kill();
                }
            }
        }
    }

}


void Bomb::init() {
    initSprite();
    disarm();
    explosionFrame = 0;

//    frameGfx = (u8 *) explosionTiles + 3* 64 * 64 / 2;
//    sub_sprite_info->updateFrame(frameGfx, 64 * 64);
//    main_sprite_info->updateFrame(frameGfx, 64 * 64);



}

void Bomb::arm() {
    frameGfx = (u8 *) gfx_explosionTiles + 1 * 64 * 64 / 2;
    subSpriteInfo->updateFrame(frameGfx, 64 * 64);
    mainSpriteInfo->updateFrame(frameGfx, 64 * 64);
}

void Bomb::disarm() {
    frameGfx = (u8 *) gfx_explosionTiles; //+ 1 * 8 * 8  / 2; dla drugiej klatki
    subSpriteInfo->updateFrame(frameGfx, 64 * 64);
    mainSpriteInfo->updateFrame(frameGfx, 64 * 64);
}

void Bomb::updateSpeed() {

    if (xSpeed > MAX_X_SPEED_BOMB)
        xSpeed = MAX_X_SPEED_BOMB;
    if (xSpeed < -MAX_X_SPEED_BOMB)
        xSpeed = -MAX_X_SPEED_BOMB;

    if (ySpeed > MAX_Y_SPEED_BOMB)
        ySpeed = MAX_Y_SPEED_BOMB;
    if (ySpeed < -MAX_Y_SPEED_BOMB)
        ySpeed = -MAX_Y_SPEED_BOMB;

    pos_inc_timer += *global::timer;

    bool change_pos = (pos_inc_timer > 15) && !hold_by_main_dude && explosionFrame == 0;

    if (change_pos) {
        update_position();


        if (bottomCollision && xSpeed > 0) {
            xSpeed -= 0.055;
            if (xSpeed < 0)
                xSpeed = 0;
        }
        if (bottomCollision && xSpeed < 0) {
            xSpeed += 0.055;
            if (xSpeed > 0)
                xSpeed = 0;
        }

        if (!bottomCollision)
            ySpeed += GRAVITY_DELTA_SPEED;

        pos_inc_timer = 0;
    }

}

void Bomb::updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) {

    MapTile *tiles[9];
    Collisions::getNeighboringTiles(global::level_generator->map_tiles, x_current_pos_in_tiles, y_current_pos_in_tiles,
                                    tiles);

    bottomCollision = Collisions::checkBottomCollision(tiles, &x, &y, &ySpeed, 8, 8, true);
    leftCollision = Collisions::checkLeftCollision(tiles, &x, &y, &xSpeed, 8, 8, true);
    rightCollision = Collisions::checkRightCollision(tiles, &x, &y, &xSpeed, 8, 8, true);
    upperCollision = Collisions::checkUpperCollision(tiles, &x, &y, &ySpeed, 8, true);

    if (bottomCollision) {

    }

}

void Bomb::initSprite() {
    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_explosionPal, gfx_explosionPalLen,
                                                        nullptr, 64 * 64, 64, BOMB, true, false,LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_explosionPal, gfx_explosionPalLen,
                                                          nullptr, 64 * 64, 64, BOMB, true, false,LAYER_LEVEL::MIDDLE_TOP);
}

Bomb::Bomb() {
    physical_height = BOMB_PHYSICAL_HEIGHT;
    physical_width = BOMB_PHYSICAL_WIDTH;
    sprite_height = BOMB_SPRITE_HEIGHT;
    sprite_width = BOMB_SPRITE_WIDTH;
}



