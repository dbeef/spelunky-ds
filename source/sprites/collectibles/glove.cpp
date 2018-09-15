//
// Created by xdbeef on 19.05.18.
//


#include "../spritesheet_type.hpp"
#include "mitt.h"
#include "glove.h"
#include "../../globals_declarations.h"
#include "../../collisions/collisions.h"
#include "../../../build/gfx_saleable.h"
#include "../animations/got_collectible.h"
#include "../sprite_utils.h"

void Glove::draw() {

    if (ready_to_dispose) return;

    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);
    update_anim_icon(x, y, physical_width);

    if (collected) return;

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

    set_position();
}


void Glove::init() {
    initSprite();
    init_anim_icon();
    update_anim_icon(x, y, physical_width);
}

void Glove::updateSpeed() {

    if (collected)
        return;

    limit_speed(MAX_X_SPEED_GLOVE, MAX_Y_SPEED_GLOVE);

    pos_inc_timer += *global::timer;

    bool change_pos = (pos_inc_timer > 15) && !hold_by_main_dude;

    if (change_pos) {
        update_position();
        apply_friction(0.055);
        apply_gravity(GRAVITY_DELTA_SPEED);
        pos_inc_timer = 0;
    }

}

void Glove::updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) {

    if (collected) return;

    MapTile *t[9];
    Collisions::getNeighboringTiles(global::current_level->map_tiles,
                                    x_current_pos_in_tiles, y_current_pos_in_tiles, t);

    upperCollision = Collisions::checkUpperCollision(t, &x, &y, &ySpeed, physical_width, true, 0.35);
    bottomCollision = Collisions::checkBottomCollision(t, &x, &y, &ySpeed, physical_width, physical_height, true, 0.35);
    leftCollision = Collisions::checkLeftCollision(t, &x, &y, &xSpeed, physical_width, physical_height, true, 0.15);
    rightCollision = Collisions::checkRightCollision(t, &x, &y, &xSpeed, physical_width, physical_height, true, 0.15);
}

void Glove::initSprite() {

    delete mainSpriteInfo;
    delete subSpriteInfo;

    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_saleablePal, gfx_saleablePalLen,
                                                        nullptr, GLOVE_SPRITE_SIZE, sprite_width,
                                                        spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_saleablePal, gfx_saleablePalLen,
                                                          nullptr, GLOVE_SPRITE_SIZE, sprite_width,
                                                          spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
    match_animation();
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);
    set_position();
}

void Glove::set_position() {
    if (collected) {
        mainSpriteInfo->entry->priority = OBJPRIORITY_0;
        sprite_utils::set_entry_xy(mainSpriteInfo, x, y);
        subSpriteInfo->entry->isHidden = true;
        mainSpriteInfo->entry->isHidden = false;
    } else {
        int main_x, main_y, sub_x, sub_y;
        get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);
        sprite_utils::set_entry_xy(mainSpriteInfo, main_x, main_y);
        sprite_utils::set_entry_xy(subSpriteInfo, sub_x, sub_y);
    }
}

Glove::Glove() {
    cost = 9 * 1000;
    name = "GLOVE";
    physical_height = GLOVE_PHYSICAL_HEIGHT;
    physical_width = GLOVE_PHYSICAL_WIDTH;
    sprite_height = GLOVE_SPRITE_HEIGHT;
    sprite_width = GLOVE_SPRITE_WIDTH;
    spritesheet_type = SpritesheetType::SALEABLE;
    //todo set it everywhere
    activated = true;
}

void Glove::equip() {
    collected = true;

    auto *g = new GotCollectible();
    g->x = x - 12;
    g->y = y - 20;
    g->collectible_type = 0;
    g->init();
    global::sprites_to_add.push_back(g);

    if (!global::main_dude->carrying_glove) {
        global::main_dude->carrying_glove = true;
        set_position();
        x = HUD_ITEMS_ROW_X;
        y = global::hud->items_offset_y;
        global::hud->increment_offset_on_grabbed_item();
    } else {
        sprite_utils::set_visibility(false, mainSpriteInfo, subSpriteInfo);
        ready_to_dispose = true;
    }
}

void Glove::match_animation() {
    frameGfx = sprite_utils::get_frame((u8 *) gfx_saleableTiles, GLOVE_SPRITE_SIZE, 1);
    sprite_utils::update_frame(frameGfx, GLOVE_SPRITE_SIZE, mainSpriteInfo, subSpriteInfo);
}

