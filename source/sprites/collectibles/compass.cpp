//
// Created by xdbeef on 19.05.18.
//


#include <cstdlib>
#include "compass.h"
#include "../../globals_declarations.h"
#include "../../collisions/collisions.h"
#include "../../../build/gfx_saleable.h"
#include "../animations/got_collectible.h"
#include "../sprite_utils.h"

#define COMPASS_POS_INC_DELTA 15

void Compass::draw() {

    if (ready_to_dispose) return;

    update_anim_icon(x, y, physical_width);
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);

    if (collected) {
        draw_arrow_to_exit();
    } else {

        if (bought && check_if_can_be_equipped())
            equip();
        else if (!bought && !hold_by_main_dude)
            check_if_can_be_pickuped();

        if (hold_by_main_dude) {
            set_pickuped_position(4, -4);
            if (shopping_transaction(this)) {
                collected = true;
                equip();
            }
        }

    }

    set_position();

}


void Compass::init() {
    initSprite();
    init_anim_icon();
    update_anim_icon(x, y, physical_width);
}

void Compass::updateSpeed() {

    if (collected) return;

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
    if (collected) return;

    MapTile *t[9];
    Collisions::getNeighboringTiles(global::level_generator->map_tiles,
                                    x_current_pos_in_tiles, y_current_pos_in_tiles, t);
    upperCollision = Collisions::checkUpperCollision(t, &x, &y, &ySpeed, physical_width, true, 0.35);
    bottomCollision = Collisions::checkBottomCollision(t, &x, &y, &ySpeed, physical_width, physical_height, true, 0.35);
    leftCollision = Collisions::checkLeftCollision(t, &x, &y, &xSpeed, physical_width, physical_height, true, 0.15);
    rightCollision = Collisions::checkRightCollision(t, &x, &y, &xSpeed, physical_width, physical_height, true, 0.15);
}

void Compass::initSprite() {

    delete mainSpriteInfo;
    delete subSpriteInfo;

    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_saleablePal, gfx_saleablePalLen,
                                                        nullptr, COMPASS_SPRITE_SIZE, sprite_width,
                                                        spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_saleablePal, gfx_saleablePalLen,
                                                          nullptr, COMPASS_SPRITE_SIZE, sprite_width,
                                                          spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);

    set_position();
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);

    if (collected)
        //compass is collected and drawn as hud, visibility is set in the function call below
        draw_arrow_to_exit();
    else {
        //compass is an item just like any else so set as visible
        sprite_utils::set_visibility(true, mainSpriteInfo, subSpriteInfo);
        frameGfx = sprite_utils::get_frame((u8 *) gfx_saleableTiles, COMPASS_SPRITE_SIZE, 2);
        sprite_utils::update_frame(frameGfx, COMPASS_SPRITE_SIZE, mainSpriteInfo, subSpriteInfo);
    }

}

void Compass::set_position() {

    if (collected) {
        //draw as hud
        sprite_utils::set_entry_xy(mainSpriteInfo, x, y);
        sprite_utils::set_priority(OBJPRIORITY_0, mainSpriteInfo, subSpriteInfo);
    } else {

        int main_x, main_y, sub_x, sub_y;
        get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);
        sprite_utils::set_entry_xy(mainSpriteInfo, main_x, main_y);
        sprite_utils::set_entry_xy(subSpriteInfo, sub_x, sub_y);
    }

}

Compass::Compass() {
    cost = 3 * 1000;
    name = "COMPASS";
    physical_height = COMPASS_PHYSICAL_HEIGHT;
    physical_width = COMPASS_PHYSICAL_WIDTH;
    sprite_height = COMPASS_SPRITE_HEIGHT;
    sprite_width = COMPASS_SPRITE_WIDTH;
    spritesheet_type = SpritesheetType::SALEABLE;
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
            sprite_utils::set_entry_xy(subSpriteInfo, (SCREEN_WIDTH * 0.5) - 8, SCREEN_HEIGHT - 2 - sprite_height);
            frameGfx = sprite_utils::get_frame((u8 *) gfx_saleableTiles, COMPASS_SPRITE_SIZE, 6);
        } else if (diff_y < 3 * TILE_H) {

            if (global::main_dude->x > tile_x) {
                //left_arrow
                frameGfx = sprite_utils::get_frame((u8 *) gfx_saleableTiles, COMPASS_SPRITE_SIZE, 5);
                sprite_utils::set_entry_xy(subSpriteInfo, 4, SCREEN_HEIGHT * 0.5);

            } else {
                //right arrow
                frameGfx = sprite_utils::get_frame((u8 *) gfx_saleableTiles, COMPASS_SPRITE_SIZE, 4);
                sprite_utils::set_entry_xy(subSpriteInfo, SCREEN_WIDTH - 4 - sprite_width, SCREEN_HEIGHT * 0.5);
            }

        } else {

            if (global::main_dude->x > tile_x) {
                //down-left arrow
                frameGfx = sprite_utils::get_frame((u8 *) gfx_saleableTiles, COMPASS_SPRITE_SIZE, 8);
                sprite_utils::set_entry_xy(subSpriteInfo, 4, SCREEN_HEIGHT - 4 - sprite_height);

            } else {
                //down-right arrow
                sprite_utils::set_entry_xy(subSpriteInfo, SCREEN_WIDTH - 4 - sprite_width,
                                           SCREEN_HEIGHT - 4 - sprite_height);
                frameGfx = sprite_utils::get_frame((u8 *) gfx_saleableTiles, COMPASS_SPRITE_SIZE, 9);
            }

        }

        subSpriteInfo->updateFrame(frameGfx, COMPASS_SPRITE_SIZE);
    }

}

void Compass::equip() {
    collected = true;

    auto *g = new GotCollectible();
    g->x = x - 12;
    g->y = y - 20;
    g->collectible_type = 0;
    g->init();
    global::sprites_to_add.push_back(g);

    if (!global::main_dude->carrying_compass) {
        global::main_dude->carrying_compass = true;
        set_position();
        x = HUD_ITEMS_ROW_X;
        y = global::hud->items_offset_y;
        global::hud->increment_offset_on_grabbed_item();
    } else {
        sprite_utils::set_visibility(false, mainSpriteInfo, subSpriteInfo);
        ready_to_dispose = true;
    }

}
