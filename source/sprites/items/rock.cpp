//
// Created by xdbeef on 04.04.18.
//

#include "../../globals_declarations.h"
#include "../main_dude/main_dude.h"
#include "../../../build/gfx_blood_rock_rope.h"
#include "rock.h"
#include "../../collisions/collisions.h"
#include "../../tiles/map_utils.h"


void Rock::draw() {

//    if(ready_to_dispose){
//        std::cout<<"ROCK TO DISPOSE";
//    }

    if (hold_by_main_dude && global::input_handler->y_key_down && global::input_handler->down_key_held) {
        hold_by_main_dude = false;
        global::main_dude->holding_item = false;
    } else if (global::input_handler->y_key_held && global::input_handler->down_key_held &&
               /*bottomCollision &&*/ !global::main_dude->holding_item) {
        if (Collisions::checkCollisionWithMainDude(x, y, 7, 7)) {
            hold_by_main_dude = true;
            global::main_dude->holding_item = true;
//            std::cout << "TOOK ITEM";
            global::input_handler->y_key_down = false;
            global::input_handler->y_key_held = false;
        }
    }

    if (hold_by_main_dude) {

        y = global::main_dude->y + 6;

        if (global::main_dude->state == 1) {
            x = global::main_dude->x - 2;
        } else
            x = global::main_dude->x + 10;

    }
    
    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);

    mainSpriteInfo->entry->x = main_x;
    mainSpriteInfo->entry->y = main_y;

    subSpriteInfo->entry->x = sub_x;
    subSpriteInfo->entry->y = sub_y;

    mainSpriteInfo->entry->hFlip=false;
    subSpriteInfo->entry->hFlip= false;

    mainSpriteInfo->entry->vFlip=false;
    subSpriteInfo->entry->vFlip= false;

    if (xSpeed > 0 || ySpeed > 0) {
        for (int a = 0; a < global::sprites.size(); a++) {
            if((global::sprites.at(a)->spriteType == SpritesheetType::SNAKE || global::sprites.at(a)->spriteType == SpritesheetType::BAT|| global::sprites.at(a)->spriteType == SpritesheetType::SPIDER)
               && !global::sprites.at(a)->killed){
                if(Collisions::checkCollisionBodies(x, y, 7, 7, global::sprites.at(a)->x, global::sprites.at(a)->y, 16, 16)){
                    global::sprites.at(a)->kill();
                }
            }
        }
    }

}


void Rock::init() {
    initSprite();
    activated_by_main_dude = true;
}

void Rock::updateSpeed() {

    if (xSpeed > MAX_X_SPEED_ROCK)
        xSpeed = MAX_X_SPEED_ROCK;
    if (xSpeed < -MAX_X_SPEED_ROCK)
        xSpeed = -MAX_X_SPEED_ROCK;

    if (ySpeed > MAX_Y_SPEED_ROCK)
        ySpeed = MAX_Y_SPEED_ROCK;
    if (ySpeed < -MAX_Y_SPEED_ROCK)
        ySpeed = -MAX_Y_SPEED_ROCK;

    pos_inc_timer += *global::timer;

    bool change_pos = (pos_inc_timer > 15) && !hold_by_main_dude;

    if (change_pos) {


        if (bottomCollision && xSpeed > 0) {
            xSpeed -= 0.2;
            if (xSpeed < 0)
                xSpeed = 0;
        }
        if (bottomCollision && xSpeed < 0) {
            xSpeed += 0.2;
            if (xSpeed > 0)
                xSpeed = 0;
        }

        updatePosition();


        if (!bottomCollision)
            ySpeed += GRAVITY_DELTA_SPEED * 0.8;

        pos_inc_timer = 0;
    }

}


void Rock::updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) {

    MapTile *tiles[9];
    Collisions::getNeighboringTiles(global::level_generator->map_tiles, x_current_pos_in_tiles, y_current_pos_in_tiles,
                                    tiles);

    bottomCollision = Collisions::checkBottomCollision(tiles, &x, &y, &ySpeed, 7, 7, true);
    leftCollision = Collisions::checkLeftCollision(tiles, &x, &y, &xSpeed, 7, 7, true);
    rightCollision = Collisions::checkRightCollision(tiles, &x, &y, &xSpeed, 7, 7, true);
    upperCollision = Collisions::checkUpperCollision(tiles, &x, &y, &ySpeed, 7, true);

}

void Rock::initSprite() {

    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_blood_rock_ropePal, gfx_blood_rock_ropePalLen,
                                                        nullptr, 8 * 8, 8, BLOOD_ROCK_ROPE, true, false);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_blood_rock_ropePal, gfx_blood_rock_ropePalLen,
                                                          nullptr, 8 * 8, 8, BLOOD_ROCK_ROPE, true, false);

    frameGfx = (u8 *) gfx_blood_rock_ropeTiles + (sprite_height * sprite_width * 7 / 2);
    subSpriteInfo->updateFrame(frameGfx, 8 * 8);
    mainSpriteInfo->updateFrame(frameGfx, 8 * 8);

}

Rock::Rock() {
    physical_height = ROCK_PHYSICAL_HEIGHT;
    physical_width = ROCK_PHYSICAL_WIDTH;
    sprite_height = ROCK_SPRITE_HEIGHT;
    sprite_width = ROCK_SPRITE_WIDTH;
    spriteType = SpritesheetType::BLOOD_ROCK_ROPE;
}



