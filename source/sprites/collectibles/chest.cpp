//
// Created by xdbeef on 16.05.18.
//

#include <maxmod9.h>
#include "../../../build/gfx_spike_collectibles.h"
#include "chest.h"
#include "../../collisions/collisions.h"
#include "../../globals_declarations.h"
#include "../../../build/soundbank.h"


void Chest::draw() {


    if (hold_by_main_dude && global::input_handler->y_key_down && global::input_handler->down_key_held) {
        hold_by_main_dude = false;
        global::main_dude->holding_item = false;
    } else if (global::input_handler->y_key_held && global::input_handler->down_key_held &&
               /*bottomCollision &&*/ !global::main_dude->holding_item) {
        if (Collisions::checkCollisionWithMainDude(x, y, 8, 8)) {
            hold_by_main_dude = true;
            global::main_dude->holding_item = true;
//            std::cout << "TOOK ITEM";
            global::input_handler->y_key_down = false;
            global::input_handler->y_key_held = false;
        }
    }

    if (hold_by_main_dude) {

        y = global::main_dude->y - 2;

        if (global::main_dude->state == 1) {
            x = global::main_dude->x - 8;
        } else
            x = global::main_dude->x + 8;

    }

    if (!activated_by_main_dude &&
        Collisions::checkCollisionWithMainDudeWidthBoundary(x, y, physical_width, physical_height, 8)
        && global::input_handler->up_key_held && global::input_handler->y_key_down) {
        global::hud->draw();
        activated_by_main_dude = true;
        frameGfx = (u8 *) gfx_spike_collectiblesTiles + (sprite_width * sprite_height * (3) / 2);
        subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
        mainSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
        global::input_handler->y_key_down = false;
        mmEffect(SFX_XCHESTOPEN);
    }


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


    set_position();
}


void Chest::init() {
    initSprite();
}

void Chest::updateSpeed() {

    if (xSpeed > MAX_X_SPEED_CHEST)
        xSpeed = MAX_X_SPEED_CHEST;
    if (xSpeed < -MAX_X_SPEED_CHEST)
        xSpeed = -MAX_X_SPEED_CHEST;

    if (ySpeed > MAX_Y_SPEED_CHEST)
        ySpeed = MAX_Y_SPEED_CHEST;
    if (ySpeed < -MAX_Y_SPEED_CHEST)
        ySpeed = -MAX_Y_SPEED_CHEST;

    pos_inc_timer += *global::timer;

    bool change_pos = (pos_inc_timer > 15) && !hold_by_main_dude;

    if (change_pos) {
        updatePosition();

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

void Chest::updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) {
    MapTile *tiles[9];
    Collisions::getNeighboringTiles(global::level_generator->map_tiles, x_current_pos_in_tiles, y_current_pos_in_tiles,
                                    tiles);

    bottomCollision = Collisions::checkBottomCollision(tiles, &x, &y, &ySpeed, physical_width, physical_height, true);
    leftCollision = Collisions::checkLeftCollision(tiles, &x, &y, &xSpeed, physical_width, physical_height, true);
    rightCollision = Collisions::checkRightCollision(tiles, &x, &y, &xSpeed, physical_width, physical_height, true);
    upperCollision = Collisions::checkUpperCollision(tiles, &x, &y, &ySpeed, physical_width, true);

}

void Chest::initSprite() {


    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_spike_collectiblesPal, gfx_spike_collectiblesPalLen,
                                                        nullptr, sprite_width * sprite_height, sprite_width,
                                                        spriteType, true, false);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_spike_collectiblesPal, gfx_spike_collectiblesPalLen,
                                                          nullptr, sprite_width * sprite_height, sprite_width,
                                                          spriteType, true, false);
    if (activated_by_main_dude)
        frameGfx = (u8 *) gfx_spike_collectiblesTiles + (sprite_width * sprite_height * (3) / 2);
    else
        frameGfx = (u8 *) gfx_spike_collectiblesTiles + (sprite_width * sprite_height * (2) / 2);

    subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
    mainSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
}

void Chest::set_position() {

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

}

Chest::Chest() {
    physical_height = CHEST_PHYSICAL_HEIGHT;
    physical_width = CHEST_PHYSICAL_WIDTH;
    sprite_height = CHEST_SPRITE_HEIGHT;
    sprite_width = CHEST_SPRITE_WIDTH;
    spriteType = SpritesheetType::SPIKES_COLLECTIBLES;
}

