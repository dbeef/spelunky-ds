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

#define MITT_POS_INC_DELTA 15

void Mitt::draw() {

    if (ready_to_dispose)
        return;

    set_position();

    if (collected)
        return;

    if (check_if_can_be_equipped()) {

        collected = true;

        GotCollectible *g = new GotCollectible();
        g->x = x - 12;
        g->y = y - 20;
        g->collectible_type = 0;
        g->init();
        global::sprites.push_back(g);

        if (!global::main_dude->carrying_mitt) {
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


void Mitt::init() {
    initSprite();
}

void Mitt::updateSpeed() {

    if (collected)
        return;

    limit_speed(MAX_X_SPEED_MITT, MAX_Y_SPEED_MITT);

    pos_inc_timer += *global::timer;

    bool change_pos = (pos_inc_timer > MITT_POS_INC_DELTA) && !hold_by_main_dude;

    if (change_pos) {
        update_position();
        apply_friction(0.055);
        apply_gravity(GRAVITY_DELTA_SPEED);
        pos_inc_timer = 0;
    }

}

void Mitt::updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) {

    if (collected)
        return;

    MapTile *tiles[9];
    Collisions::getNeighboringTiles(global::level_generator->map_tiles, x_current_pos_in_tiles,
                                    y_current_pos_in_tiles,
                                    tiles);

    bottomCollision = Collisions::checkBottomCollision(tiles, &x, &y, &ySpeed, physical_width, physical_height, true);
    leftCollision = Collisions::checkLeftCollision(tiles, &x, &y, &xSpeed, physical_width, physical_height, true);
    rightCollision = Collisions::checkRightCollision(tiles, &x, &y, &xSpeed, physical_width, physical_height, true);
    upperCollision = Collisions::checkUpperCollision(tiles, &x, &y, &ySpeed, physical_width, true);

}

void Mitt::initSprite() {


    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_saleablePal, gfx_saleablePalLen,
                                                        nullptr, sprite_width * sprite_height, sprite_width,
                                                        spriteType, true, false, LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_saleablePal, gfx_saleablePalLen,
                                                          nullptr, sprite_width * sprite_height, sprite_width,
                                                          spriteType, true, false, LAYER_LEVEL::MIDDLE_TOP);

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

