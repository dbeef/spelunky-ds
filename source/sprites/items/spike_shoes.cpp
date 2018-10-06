//
// Created by xdbeef on 02.06.18.
//

#include <cstdio>
#include "../spritesheet_type.hpp"
#include "../../globals_declarations.hpp"
#include "../../collisions/collisions.hpp"
#include "../../../build/gfx_saleable.h"
#include "../animations/got_collectible.hpp"
#include "../moving_object.h"
#include "spike_shoes.hpp"
#include "../sprite_utils.hpp"

#define SPIKE_SHOES_POS_INC_DELTA 15

void SpikeShoes::draw() {

    if (ready_to_dispose) return;

    if (!collected) {

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

    set_sprite_attributes();
    update_anim_icon(x, y, physical_width);
}


void SpikeShoes::init() {
    initSprite();
    init_anim_icon();
}

void SpikeShoes::updateSpeed() {

    if (collected) return;

    limit_speed(MAX_X_SPEED_SPIKE_SHOES, MAX_Y_SPEED_SPIKE_SHOES);

    pos_inc_timer += *global::timer;

    bool change_pos = (pos_inc_timer > SPIKE_SHOES_POS_INC_DELTA) && !hold_by_main_dude;

    if (change_pos) {
        update_position();
        apply_friction(0.055);
        apply_gravity(GRAVITY_DELTA_SPEED);
        pos_inc_timer = 0;
    }

}

void SpikeShoes::updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) {

    if (collected) return;

    MapTile *t[9];
    Collisions::getNeighboringTiles(global::current_level->map_tiles,
                                    x_current_pos_in_tiles, y_current_pos_in_tiles, t);

    upperCollision = Collisions::checkUpperCollision(t, &x, &y, &ySpeed, physical_width, true, 0.35);
    bottomCollision = Collisions::checkBottomCollision(t, &x, &y, &ySpeed, physical_width, physical_height, true, 0.35);
    leftCollision = Collisions::checkLeftCollision(t, &x, &y, &xSpeed, physical_width, physical_height, true, 0.15);
    rightCollision = Collisions::checkRightCollision(t, &x, &y, &xSpeed, physical_width, physical_height, true, 0.15);
}

void SpikeShoes::initSprite() {

    delete mainSpriteInfo;
    delete subSpriteInfo;

    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_saleablePal, gfx_saleablePalLen,
                                                        nullptr, SPIKE_SHOES_SPRITE_SIZE, sprite_width,
                                                        spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_saleablePal, gfx_saleablePalLen,
                                                          nullptr, SPIKE_SHOES_SPRITE_SIZE, sprite_width,
                                                          spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);

    frameGfx = sprite_utils::get_frame((u8 *) gfx_saleableTiles, SPIKE_SHOES_SPRITE_SIZE, 12);
    sprite_utils::update_frame(frameGfx, SPIKE_SHOES_SPRITE_SIZE, mainSpriteInfo, subSpriteInfo);
    set_sprite_attributes();
}

void SpikeShoes::set_position() {

    if (collected) {
        sprite_utils::set_entry_xy(mainSpriteInfo, x, y);
        mainSpriteInfo->entry->priority = OBJPRIORITY_0;
        subSpriteInfo->entry->isHidden = true;
        mainSpriteInfo->entry->isHidden = false;

    } else {
        sprite_utils::set_visibility(true, mainSpriteInfo, subSpriteInfo);
        int main_x, main_y, sub_x, sub_y;
        get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);
        sprite_utils::set_entry_xy(mainSpriteInfo, main_x, main_y);
        sprite_utils::set_entry_xy(subSpriteInfo, sub_x, sub_y);
    }

}

SpikeShoes::SpikeShoes() {
    cost = 4 * 1000;
    name = "SPIKE SHOES";
    physical_height = SPIKE_SHOES_PHYSICAL_HEIGHT;
    physical_width = SPIKE_SHOES_PHYSICAL_WIDTH;
    sprite_height = SPIKE_SHOES_SPRITE_HEIGHT;
    sprite_width = SPIKE_SHOES_SPRITE_WIDTH;
    spritesheet_type = SpritesheetType::SALEABLE;
}

void SpikeShoes::equip() {

    auto *g = new GotCollectible();
    g->x = x - 12;
    g->y = y - 20;
    g->collectible_type = 0;
    g->init();
    global::sprites_to_add.push_back(g);

    if (!global::main_dude->carrying_spike_shoes) {
        global::main_dude->carrying_spike_shoes = true;
        set_position();
        collected = true;
        x = HUD_ITEMS_ROW_X;
        y = global::hud->items_offset_y;
        global::hud->increment_offset_on_grabbed_item();
    } else {
        sprite_utils::set_visibility(false, mainSpriteInfo, subSpriteInfo);
        ready_to_dispose = true;
    }

}

void SpikeShoes::set_sprite_attributes() {
    set_position();
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);
}

void SpikeShoes::deleteSprite() {
    delete mainSpriteInfo;
    delete subSpriteInfo;
    mainSpriteInfo = nullptr;
    subSpriteInfo = nullptr;
}

