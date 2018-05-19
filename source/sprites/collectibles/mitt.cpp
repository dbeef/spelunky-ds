//
// Created by xdbeef on 19.05.18.
//


#include "../spritesheet_type.h"
#include "pistol.h"
#include "spring_shoes.h"
#include "../../globals_declarations.h"
#include "../../collisions/collisions.h"
#include "../../../build/gfx_saleable.h"
#include "mitt.h"
#include "../animations/got_collectible.h"

void Mitt::draw() {

    if (ready_to_dispose)
        return;

    set_position();

    if (collected)
        return;

    if (global::input_handler->y_key_down && global::input_handler->down_key_held && !global::main_dude->holding_item) {

        if (Collisions::checkCollisionWithMainDude(x, y, sprite_width, sprite_height)) {
            collected = true;


            GotCollectible *g = new GotCollectible();
            g->x = x - 12;
            g->y = y - 20;
            g->collectible_type = 0;
            g->init();
            global::sprites.push_back(g);
            if(!global::main_dude->carrying_mitt) {
                global::main_dude->carrying_mitt = true;
                global::hud->next_item();
                set_position();
                x = global::hud->items_offset_x;
                y = global::hud->items_offset_y;
            } else {
                mainSpriteInfo->entry->isHidden = true;
                subSpriteInfo->entry->isHidden = true;
                ready_to_dispose = true;
            }
        }

    }
}


void Mitt::init() {
    initSprite();
}

void Mitt::updateSpeed() {

    if(collected)
        return;

    if (xSpeed > MAX_X_SPEED_MITT)
        xSpeed = MAX_X_SPEED_MITT;
    if (xSpeed < -MAX_X_SPEED_MITT)
        xSpeed = -MAX_X_SPEED_MITT;

    if (ySpeed > MAX_Y_SPEED_MITT)
        ySpeed = MAX_Y_SPEED_MITT;
    if (ySpeed < -MAX_Y_SPEED_MITT)
        ySpeed = -MAX_Y_SPEED_MITT;

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

void Mitt::updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) {
    if(!collected) {
        MapTile *tiles[9];
        Collisions::getNeighboringTiles(global::level_generator->map_tiles, x_current_pos_in_tiles,
                                        y_current_pos_in_tiles,
                                        tiles);

        bottomCollision = Collisions::checkBottomCollision(tiles, &x, &y, &ySpeed, physical_width, physical_height,
                                                           true);
        leftCollision = Collisions::checkLeftCollision(tiles, &x, &y, &xSpeed, physical_width, physical_height, true);
        rightCollision = Collisions::checkRightCollision(tiles, &x, &y, &xSpeed, physical_width, physical_height, true);
        upperCollision = Collisions::checkUpperCollision(tiles, &x, &y, &ySpeed, physical_width, true);
    }
}

void Mitt::initSprite() {


    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_saleablePal, gfx_saleablePalLen,
                                                        nullptr, sprite_width * sprite_height, sprite_width,
                                                        spriteType, true, false);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_saleablePal, gfx_saleablePalLen,
                                                          nullptr, sprite_width * sprite_height, sprite_width,
                                                          spriteType, true, false);

    frameGfx = (u8 *) gfx_saleableTiles + (sprite_width * sprite_height * (0) / 2);

    mainSpriteInfo->entry->x = x;
    subSpriteInfo->entry->y = y;

    subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
    mainSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);


}

void Mitt::set_position() {

    if (collected) {

        mainSpriteInfo->entry->x = x;
        mainSpriteInfo->entry->y = y;

        subSpriteInfo->entry->isHidden = true;
        mainSpriteInfo->entry->isHidden = false;

    } else {
        int main_x, main_y, sub_x, sub_y;
        get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);

        mainSpriteInfo->entry->x = main_x;
        mainSpriteInfo->entry->y = main_y;

        subSpriteInfo->entry->x = sub_x;
        subSpriteInfo->entry->y = sub_y;
    }

    mainSpriteInfo->entry->vFlip = false;
    mainSpriteInfo->entry->hFlip = false;

    subSpriteInfo->entry->vFlip = false;
    subSpriteInfo->entry->hFlip = false;

}

Mitt::Mitt() {
    physical_height = MITT_PHYSICAL_HEIGHT;
    physical_width = MITT_PHYSICAL_WIDTH;
    sprite_height = MITT_SPRITE_HEIGHT;
    sprite_width = MITT_SPRITE_WIDTH;
    spriteType = SpritesheetType::SALEABLE;
}

