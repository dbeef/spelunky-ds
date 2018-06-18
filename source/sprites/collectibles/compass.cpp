//
// Created by xdbeef on 19.05.18.
//


#include "compass.h"
#include "../../globals_declarations.h"
#include "../../collisions/collisions.h"
#include "../../../build/gfx_saleable.h"
#include "../animations/got_collectible.h"

#define COMPASS_POS_INC_DELTA 15

void Compass::draw() {


    if (ready_to_dispose) {
        mainSpriteInfo->entry->isHidden = true;
        subSpriteInfo->entry->isHidden = true;
        return;
    } else {
        mainSpriteInfo->entry->isHidden = false;
        subSpriteInfo->entry->isHidden = false;
    }

    set_position();

    if (collected) {
        draw_arrow_to_exit();
    } else {


        if (bought && check_if_can_be_equipped()) {
            equip();
        } else if (!bought && !hold_by_main_dude) {
            check_if_can_be_pickuped();
        }

        if (hold_by_main_dude) {
            set_pickuped_position(4, -4);
            if (shopping_transaction(this)) {
                collected = true;
                equip();
            }
        }

    }

}


void Compass::init() {
    initSprite();
    init_anim_icon();
}

void Compass::updateSpeed() {

    if (collected)
        return;

    limit_speed(MAX_X_SPEED_COMPASS, MAX_Y_SPEED_COMPASS);

    pos_inc_timer += *global::timer;

    bool change_pos = (pos_inc_timer > COMPASS_POS_INC_DELTA) && !hold_by_main_dude;

    if (change_pos) {
        update_position();
        apply_friction(0.055);
        apply_gravity(GRAVITY_DELTA_SPEED);
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
                                                           true, BOUNCING_FACTOR_Y);
        leftCollision = Collisions::checkLeftCollision(tiles, &x, &y, &xSpeed, physical_width, physical_height, true,
                                                       BOUNCING_FACTOR_X);
        rightCollision = Collisions::checkRightCollision(tiles, &x, &y, &xSpeed, physical_width, physical_height, true,
                                                         BOUNCING_FACTOR_X);
        upperCollision = Collisions::checkUpperCollision(tiles, &x, &y, &ySpeed, physical_width, true,
                                                         BOUNCING_FACTOR_Y);
    }
}

void Compass::initSprite() {


    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_saleablePal, gfx_saleablePalLen,
                                                        nullptr, sprite_width * sprite_height, sprite_width,
                                                        spriteType, true, false, LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_saleablePal, gfx_saleablePalLen,
                                                          nullptr, sprite_width * sprite_height, sprite_width,
                                                          spriteType, true, false, LAYER_LEVEL::MIDDLE_TOP);

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
        //hud
        mainSpriteInfo->entry->x = x;
        mainSpriteInfo->entry->y = y;
        mainSpriteInfo->entry->priority = OBJPRIORITY_0;
        subSpriteInfo->entry->priority = OBJPRIORITY_0;

    } else {

        int main_x, main_y, sub_x, sub_y;
        get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);

        mainSpriteInfo->entry->x = main_x;
        mainSpriteInfo->entry->y = main_y;

        subSpriteInfo->entry->x = sub_x;
        subSpriteInfo->entry->y = sub_y;
    }

    update_anim_icon(x, y, physical_width);

    mainSpriteInfo->entry->vFlip = false;
    mainSpriteInfo->entry->hFlip = false;

    subSpriteInfo->entry->vFlip = false;
    subSpriteInfo->entry->hFlip = false;

}

Compass::Compass() {
    cost = 3 * 1000;
    name = "COMPASS";
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

void Compass::draw_arrow_to_exit() {

    MapTile *exit = nullptr;
    global::level_generator->get_first_tile(MapTileType::EXIT, exit);

    if (exit != nullptr) {

        int tile_x = exit->x * TILE_W;
        int tile_y = exit->y * TILE_H;

        //camera is centered on main dude
        int diff_x = abs((global::camera->x + 0.5 * SCREEN_WIDTH) - tile_x);
        int diff_y = abs((global::camera->y + 1.5 * SCREEN_HEIGHT) - tile_y);

        subSpriteInfo->entry->isHidden = false;

        if (diff_y <= SCREEN_HEIGHT * 0.5 && diff_x <= SCREEN_WIDTH * 0.5) {

            subSpriteInfo->entry->isHidden = true;

        } else if (diff_x < 6 * TILE_W) {
            //down arrow
            subSpriteInfo->entry->x = (SCREEN_WIDTH * 0.5) - 8;
            subSpriteInfo->entry->y = SCREEN_HEIGHT - 2 - sprite_height;
            apply_down_arrow();

        } else if (diff_y < 3 * TILE_H) {

            if (global::main_dude->x > tile_x) {
                //left_arrow
                apply_left_arrow();

                subSpriteInfo->entry->x = 4;
                subSpriteInfo->entry->y = SCREEN_HEIGHT * 0.5;

            } else {
                //right arrow
                apply_right_arrow();

                subSpriteInfo->entry->x = SCREEN_WIDTH - 4 - sprite_width;
                subSpriteInfo->entry->y = SCREEN_HEIGHT * 0.5;
            }

        } else {

            if (global::main_dude->x > tile_x) {
                //down-left arrow
                apply_down_left_arrow();

                subSpriteInfo->entry->x = 4;
                subSpriteInfo->entry->y = SCREEN_HEIGHT - 4 - sprite_height;

            } else {
                //down-right arrow
                subSpriteInfo->entry->x = SCREEN_WIDTH - 4 - sprite_width;
                subSpriteInfo->entry->y = SCREEN_HEIGHT - 4 - sprite_height;

                apply_down_right_arrow();
            }

        }
    }

}

void Compass::equip() {
    collected = true;

    GotCollectible *g = new GotCollectible();
    g->x = x - 12;
    g->y = y - 20;
    g->collectible_type = 0;
    g->init();
    global::sprites.push_back(g);

    if (!global::main_dude->carrying_compass) {
        global::main_dude->carrying_compass = true;
        set_position();
        x = HUD_ITEMS_ROW_X;
        y = global::hud->items_offset_y;
        global::hud->next_item();
    } else {
        mainSpriteInfo->entry->isHidden = true;
        subSpriteInfo->entry->isHidden = true;
        ready_to_dispose = true;
    }

}
