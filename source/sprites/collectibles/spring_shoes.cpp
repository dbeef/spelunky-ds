//
// Created by xdbeef on 18.05.18.
//


#include "../spritesheet_type.hpp"
#include "pistol.h"
#include "spring_shoes.h"
#include "../../globals_declarations.h"
#include "../../collisions/collisions.h"
#include "../../../build/gfx_saleable.h"
#include "../animations/got_collectible.h"

#define SPRING_SHOES_POS_INC_DELTA 15

void SpringShoes::draw() {


    if (ready_to_dispose) {
        mainSpriteInfo->entry->isHidden = true;
        subSpriteInfo->entry->isHidden = true;
        return;
    } else {
        mainSpriteInfo->entry->isHidden = false;
        subSpriteInfo->entry->isHidden = false;
    }


    set_position();

    if (collected)
        return;


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


void SpringShoes::init() {
    initSprite();
    init_anim_icon();

}

void SpringShoes::updateSpeed() {

    if (collected)
        return;

    limit_speed(MAX_X_SPEED_SPRING_SHOES, MAX_Y_SPEED_SPRING_SHOES);

    pos_inc_timer += *global::timer;

    bool change_pos = (pos_inc_timer > SPRING_SHOES_POS_INC_DELTA) && !hold_by_main_dude;

    if (change_pos) {
        update_position();
        apply_friction(0.055);
        apply_gravity(GRAVITY_DELTA_SPEED);
        pos_inc_timer = 0;
    }

}

void SpringShoes::updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) {

    if (collected)
        return;

    MapTile *tiles[9];
    Collisions::getNeighboringTiles(global::level_generator->map_tiles, x_current_pos_in_tiles,
                                    y_current_pos_in_tiles,
                                    tiles);

    upperCollision = Collisions::checkUpperCollision(tiles, &x, &y, &ySpeed, physical_width, true, BOUNCING_FACTOR_Y);
    bottomCollision = Collisions::checkBottomCollision(tiles, &x, &y, &ySpeed, physical_width, physical_height, true, BOUNCING_FACTOR_Y);
    leftCollision = Collisions::checkLeftCollision(tiles, &x, &y, &xSpeed, physical_width, physical_height, true, BOUNCING_FACTOR_X);
    rightCollision = Collisions::checkRightCollision(tiles, &x, &y, &xSpeed, physical_width, physical_height, true, BOUNCING_FACTOR_X);
}

void SpringShoes::initSprite() {


    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_saleablePal, gfx_saleablePalLen,
                                                        nullptr, sprite_width * sprite_height, sprite_width,
                                                        spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_saleablePal, gfx_saleablePalLen,
                                                          nullptr, sprite_width * sprite_height, sprite_width,
                                                          spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);

    frameGfx = (u8 *) gfx_saleableTiles + (sprite_width * sprite_height * (3) / 2);

    subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
    mainSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);

}

void SpringShoes::set_position() {

    if (collected) {
        mainSpriteInfo->entry->priority = OBJPRIORITY_0;

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

    update_anim_icon(x, y, physical_width);


}

SpringShoes::SpringShoes() {
    cost = 5000;
    name = "SPRING SHOES";
    physical_height = SPRING_SHOES_PHYSICAL_HEIGHT;
    physical_width = SPRING_SHOES_PHYSICAL_WIDTH;
    sprite_height = SPRING_SHOES_SPRITE_HEIGHT;
    sprite_width = SPRING_SHOES_SPRITE_WIDTH;
    spritesheet_type = SpritesheetType::SALEABLE;
}

void SpringShoes::equip() {

    GotCollectible *g = new GotCollectible();
    g->x = x - 12;
    g->y = y - 20;
    g->collectible_type = 0;
    g->init();
    global::sprites.push_back(g);

    if (!global::main_dude->carrying_spring_shoes) {
        global::main_dude->carrying_spring_shoes = true;
        set_position();
        collected = true;
        x = HUD_ITEMS_ROW_X;
        y = global::hud->items_offset_y;
        global::hud->increment_offset_on_grabbed_item();
    } else {
        mainSpriteInfo->entry->isHidden = true;
        subSpriteInfo->entry->isHidden = true;
        ready_to_dispose = true;
    }
}

