//
// Created by xdbeef on 19.05.18.
//


#include "compass.h"
#include "../../globals_declarations.h"
#include "../../collisions/collisions.h"
#include "../../../build/gfx_saleable.h"

void Compass::draw() {

    if (ready_to_dispose)
        return;

    set_position();

    if (!collected) {
        if (global::input_handler->y_key_down && global::input_handler->down_key_held &&
            !global::main_dude->holding_item) {

            if (Collisions::checkCollisionWithMainDude(x, y, sprite_width, sprite_height)) {
                collected = true;

                if (!global::main_dude->carrying_compass) {
                    global::main_dude->carrying_compass = true;
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
    } else {

        MapTile *exit = nullptr;
        global::level_generator->get_first_tile(MapTileType::EXIT, exit);

        if (exit != nullptr) {

            int tile_x = exit->x * TILE_W;
            int tile_y = exit->y * TILE_H;

            //camera is centered on main dude
            int diff_x = abs((global::camera->x + 0.5*SCREEN_WIDTH) - tile_x);
            int diff_y = abs((global::camera->y + 1.5*SCREEN_HEIGHT) - tile_y);


            subSpriteInfo->entry->isHidden = false;

            if (diff_y <= SCREEN_HEIGHT * 0.5 && diff_x <= SCREEN_WIDTH * 0.5) {

                subSpriteInfo->entry->isHidden = true;

            } else if (diff_x < 6 * TILE_W) {
                //down arrow
                subSpriteInfo->entry->x = (SCREEN_WIDTH *0.5) - 8;
                subSpriteInfo->entry->y = SCREEN_HEIGHT - 10 - 16;
                apply_down_arrow();

            } else if (diff_y < 2 * TILE_H) {

                if (global::main_dude->x > tile_x) {
                    //left_arrow
                    apply_left_arrow();

                    subSpriteInfo->entry->x = 10;
                    subSpriteInfo->entry->y = SCREEN_HEIGHT*0.5;

                } else {
                    //right arrow
                    apply_right_arrow();

                    subSpriteInfo->entry->x = SCREEN_WIDTH - 10 - 16;
                    subSpriteInfo->entry->y = SCREEN_HEIGHT*0.5;
                }

            } else {

                if (global::main_dude->x > tile_x) {
                    //down-left arrow
                    apply_down_left_arrow();

                    subSpriteInfo->entry->x = 10;
                    subSpriteInfo->entry->y = SCREEN_HEIGHT - 10 - 16;

                } else {
                    //down-right arrow
                    subSpriteInfo->entry->x = SCREEN_WIDTH - 10 - 16;
                    subSpriteInfo->entry->y = SCREEN_HEIGHT - 10 - 16;

                    apply_down_right_arrow();
                }

            }
        }

    }
}


void Compass::init() {
    initSprite();
}

void Compass::updateSpeed() {

    if (collected)
        return;

    if (xSpeed > MAX_X_SPEED_COMPASS)
        xSpeed = MAX_X_SPEED_COMPASS;
    if (xSpeed < -MAX_X_SPEED_COMPASS)
        xSpeed = -MAX_X_SPEED_COMPASS;

    if (ySpeed > MAX_Y_SPEED_COMPASS)
        ySpeed = MAX_Y_SPEED_COMPASS;
    if (ySpeed < -MAX_Y_SPEED_COMPASS)
        ySpeed = -MAX_Y_SPEED_COMPASS;

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

void Compass::updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) {
    if (!collected) {
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

void Compass::initSprite() {


    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_saleablePal, gfx_saleablePalLen,
                                                        nullptr, sprite_width * sprite_height, sprite_width,
                                                        spriteType, true, false);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_saleablePal, gfx_saleablePalLen,
                                                          nullptr, sprite_width * sprite_height, sprite_width,
                                                          spriteType, true, false);

    frameGfx = (u8 *) gfx_saleableTiles + (sprite_width * sprite_height * (2) / 2);

    mainSpriteInfo->entry->x = x;
    mainSpriteInfo->entry->y = y;

    subSpriteInfo->entry->x = x;
    subSpriteInfo->entry->y = y;

    subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
    mainSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);


}

void Compass::set_position() {

    if (collected) {

        mainSpriteInfo->entry->x = x;
        mainSpriteInfo->entry->y = y;

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

Compass::Compass() {
    physical_height = COMPASS_PHYSICAL_HEIGHT;
    physical_width = COMPASS_PHYSICAL_WIDTH;
    sprite_height = COMPASS_SPRITE_HEIGHT;
    sprite_width = COMPASS_SPRITE_WIDTH;
    spriteType = SpritesheetType::SALEABLE;
}

void Compass::apply_down_arrow() {
    frameGfx = (u8 *) gfx_saleableTiles + (sprite_width * sprite_height * (6) / 2);
    subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
}

void Compass::apply_left_arrow() {
    frameGfx = (u8 *) gfx_saleableTiles + (sprite_width * sprite_height * (5) / 2);
    subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
}

void Compass::apply_right_arrow() {
    frameGfx = (u8 *) gfx_saleableTiles + (sprite_width * sprite_height * (4) / 2);
    subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
}

void Compass::apply_down_right_arrow() {
    frameGfx = (u8 *) gfx_saleableTiles + (sprite_width * sprite_height * (9) / 2);
    subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
}

void Compass::apply_down_left_arrow() {
    frameGfx = (u8 *) gfx_saleableTiles + (sprite_width * sprite_height * (8) / 2);
    subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
}