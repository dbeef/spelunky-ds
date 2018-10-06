//
// Created by xdbeef on 02.06.18.
//


#include "cape.hpp"
#include "../../../build/gfx_goldbars.h"
#include "../../globals_declarations.hpp"
#include "../animations/got_collectible.hpp"
#include "../../collisions/collisions.hpp"
#include "../sprite_utils.hpp"

#define CAPE_ANIM_FRAME_DELTA 60
#define CAPE_POS_INC_DELTA 15

void Cape::draw() {

    if (global::main_dude->carrying_jetpack && collected) {
        global::main_dude->carrying_cape = false;
        sprite_utils::set_visibility(false, mainSpriteInfo, subSpriteInfo);
        ready_to_dispose = true;
    }

    if (ready_to_dispose) return;

    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);
    update_anim_icon(x, y, physical_width);
    sprite_utils::set_visibility(true, mainSpriteInfo, subSpriteInfo);

    if (bought && !collected && check_if_can_be_equipped())
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

    if (collected) {

        //match sprite rendering priority and x/y offset to the main dude's sprite state
        if (global::main_dude->climbing || global::main_dude->exiting_level) {
            set_pickuped_position_not_checking(0, 4);
            sprite_utils::set_priority(OBJPRIORITY_0, mainSpriteInfo, subSpriteInfo);
        } else if (global::input_handler->down_key_held || global::main_dude->dead || global::main_dude->stunned) {
            set_pickuped_position_not_checking(0, 4);
            sprite_utils::set_priority(OBJPRIORITY_1, mainSpriteInfo, subSpriteInfo);
        } else {
            sprite_utils::set_priority(OBJPRIORITY_1, mainSpriteInfo, subSpriteInfo);
            set_pickuped_position_not_checking(-3, -1);
        }

        anim_frame_timer += *global::timer;

        if (anim_frame_timer > CAPE_ANIM_FRAME_DELTA) {

            if (global::main_dude->xSpeed != 0 || global::main_dude->ySpeed != 0)
                anim_frame++;

            anim_frame_timer = 0;
            match_animation();
        }

        sprite_utils::set_horizontal_flip(global::main_dude->sprite_state == SpriteState::W_RIGHT,
                                          mainSpriteInfo, subSpriteInfo);

        if (global::main_dude->bottomCollision)
            global::main_dude->using_cape = false;

    }

}


void Cape::init() {
    initSprite();
    init_anim_icon();
    update_anim_icon(x, y, physical_width);
}

void Cape::updateSpeed() {

    limit_speed(MAX_X_SPEED_CAPE, MAX_Y_SPEED_CAPE);

    pos_inc_timer += *global::timer;

    bool change_pos = (pos_inc_timer > CAPE_POS_INC_DELTA) && !collected;

    if (change_pos) {
        update_position();
        apply_friction(0.055);
        apply_gravity(GRAVITY_DELTA_SPEED);
        pos_inc_timer = 0;
    }

}

void Cape::updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) {

    if (collected) return;

    MapTile *t[9];
    Collisions::getNeighboringTiles(global::current_level->map_tiles,
                                    x_current_pos_in_tiles, y_current_pos_in_tiles, t);

    upperCollision = Collisions::checkUpperCollision(t, &x, &y, &ySpeed, physical_width, true, 0.35);
    bottomCollision = Collisions::checkBottomCollision(t, &x, &y, &ySpeed, physical_width, physical_height, true, 0.35);
    leftCollision = Collisions::checkLeftCollision(t, &x, &y, &xSpeed, physical_width, physical_height, true, 0.15);
    rightCollision = Collisions::checkRightCollision(t, &x, &y, &xSpeed, physical_width, physical_height, true, 0.15);
}

void Cape::initSprite() {

    delete mainSpriteInfo;
    delete subSpriteInfo;

    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_goldbarsPal, gfx_goldbarsPalLen,
                                                        nullptr, CAPE_SPRITE_SIZE, sprite_width,
                                                        spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_goldbarsPal, gfx_goldbarsPalLen,
                                                          nullptr, CAPE_SPRITE_SIZE, sprite_width,
                                                          spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
    set_position();
    match_animation();
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);

    if (collected)
        sprite_utils::set_horizontal_flip(global::main_dude->sprite_state == SpriteState::W_RIGHT,
                                          mainSpriteInfo, subSpriteInfo);
    else
        sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);
}

void Cape::set_position() {
    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);
    sprite_utils::set_entry_xy(mainSpriteInfo, main_x, main_y);
    sprite_utils::set_entry_xy(subSpriteInfo, sub_x, sub_y);
}

Cape::Cape() {
    cost = 13 * 1000;
    name = "CAPE";
    physical_height = CAPE_PHYSICAL_HEIGHT;
    physical_width = CAPE_PHYSICAL_WIDTH;
    sprite_height = CAPE_SPRITE_HEIGHT;
    sprite_width = CAPE_SPRITE_WIDTH;
    spritesheet_type = SpritesheetType::MONIEZ_GOLDBARS;
}

void Cape::match_animation() {

    if (!collected) {
        frameGfx = sprite_utils::get_frame((u8 *) gfx_goldbarsTiles, CAPE_SPRITE_SIZE, 2);
    } else if (global::main_dude->climbing) {
        //climbing
        frameGfx = sprite_utils::get_frame((u8 *) gfx_goldbarsTiles, CAPE_SPRITE_SIZE, 3);
    } else if (global::input_handler->down_key_held || global::main_dude->dead || global::main_dude->stunned ||
               (!global::main_dude->using_cape && !global::main_dude->stunned &&
                (global::main_dude->xSpeed != 0 || global::main_dude->ySpeed != 0))) {

        if (anim_frame >= 5)
            anim_frame = 0;

        frameGfx = sprite_utils::get_frame((u8 *) gfx_goldbarsTiles, CAPE_SPRITE_SIZE, 5 + anim_frame);

    } else if (global::main_dude->xSpeed == 0 && global::main_dude->ySpeed == 0) {
        //staying still
        frameGfx = sprite_utils::get_frame((u8 *) gfx_goldbarsTiles, CAPE_SPRITE_SIZE, 4);
    } else {

        //falling
        if (anim_frame >= 2)
            anim_frame = 0;

        frameGfx = sprite_utils::get_frame((u8 *) gfx_goldbarsTiles, CAPE_SPRITE_SIZE, 10 + anim_frame);
    }

    sprite_utils::update_frame(frameGfx, CAPE_SPRITE_SIZE, mainSpriteInfo, subSpriteInfo);

}

void Cape::equip() {

    auto g = new GotCollectible();
    g->x = x - 12;
    g->y = y - 20;
    g->collectible_type = 0;
    g->init();
    global::sprites_to_add.push_back(g);

    if (global::main_dude->carrying_jetpack)
        global::main_dude->carrying_jetpack = false;

    if (!global::main_dude->carrying_cape) {
        global::main_dude->carrying_cape = true;
        set_position();
        collected = true;
    } else {
        //we've collected an item that is already in inventory, dispose
        ready_to_dispose = true;
        sprite_utils::set_visibility(false, mainSpriteInfo, subSpriteInfo);
    }

}

void Cape::deleteSprite() {
    delete mainSpriteInfo;
    delete subSpriteInfo;
    mainSpriteInfo = nullptr;
    subSpriteInfo = nullptr;
}

