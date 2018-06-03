//
// Created by xdbeef on 02.06.18.
//


#include "cape.h"
#include "../../../build/gfx_goldbars.h"
#include "../../globals_declarations.h"
#include "../animations/got_collectible.h"
#include "../../collisions/collisions.h"

#define CAPE_ANIM_FRAME_DELTA 60
#define CAPE_POS_INC_DELTA 15

void Cape::draw() {


    if (global::main_dude->carrying_jetpack && collected) {
        global::main_dude->carrying_cape = false;
        ready_to_dispose = true;
    }

    if (ready_to_dispose) {
        mainSpriteInfo->entry->isHidden = true;
        subSpriteInfo->entry->isHidden = true;
        return;
    } else{
        mainSpriteInfo->entry->isHidden = false;
        subSpriteInfo->entry->isHidden = false;
    }

    set_position();

    if (!collected && check_if_can_be_equipped()) {

        GotCollectible *g = new GotCollectible();
        g->x = x - 12;
        g->y = y - 20;
        g->collectible_type = 0;
        g->init();
        global::sprites.push_back(g);

        if (global::main_dude->carrying_jetpack) {
            global::main_dude->carrying_jetpack = false;
        }

        if (!global::main_dude->carrying_cape) {
            global::main_dude->carrying_cape = true;
            set_position();
            collected = true;
        } else {
            //we've collected an item that is already in inventory, dispose
            ready_to_dispose = true;
        }
    }


    if (collected) {

        anim_frame_timer += *global::timer;

        if (global::main_dude->climbing || global::main_dude->exiting_level) {
            set_pickuped_position_not_checking(0, 4);
            mainSpriteInfo->entry->priority = OBJPRIORITY_0;
            subSpriteInfo->entry->priority = OBJPRIORITY_0;
        } else if (global::input_handler->down_key_held || global::main_dude->dead || global::main_dude->stunned) {
            set_pickuped_position_not_checking(0, 4);
            mainSpriteInfo->entry->priority = OBJPRIORITY_1;
            subSpriteInfo->entry->priority = OBJPRIORITY_1;
        } else {
            mainSpriteInfo->entry->priority = OBJPRIORITY_1;
            subSpriteInfo->entry->priority = OBJPRIORITY_1;
            set_pickuped_position_not_checking(-3, -1);
        }

        if (anim_frame_timer > CAPE_ANIM_FRAME_DELTA) {

            if (global::main_dude->xSpeed != 0 || global::main_dude->ySpeed != 0)
                anim_frame++;
            anim_frame_timer = 0;

            set_frame_gfx();

            subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
            mainSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);

        }


        if (global::main_dude->state == SpriteState::W_LEFT) {
            mainSpriteInfo->entry->hFlip = false;
            subSpriteInfo->entry->hFlip = false;
        } else if (global::main_dude->state == SpriteState::W_RIGHT) {
            mainSpriteInfo->entry->hFlip = true;
            subSpriteInfo->entry->hFlip = true;
        }

        mainSpriteInfo->entry->isHidden = false;
        subSpriteInfo->entry->isHidden = false;
        mainSpriteInfo->entry->vFlip = false;
        subSpriteInfo->entry->vFlip = false;


        if (global::main_dude->bottomCollision)
            global::main_dude->using_cape = false;

    }

}


void Cape::init() {
    initSprite();
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

    if (collected)
        return;

    MapTile *tiles[9];
    Collisions::getNeighboringTiles(global::level_generator->map_tiles, x_current_pos_in_tiles,
                                    y_current_pos_in_tiles,
                                    tiles);

    bottomCollision = Collisions::checkBottomCollision(tiles, &x, &y, &ySpeed, physical_width, physical_height, true,
                                                       BOUNCING_FACTOR_Y);
    leftCollision = Collisions::checkLeftCollision(tiles, &x, &y, &xSpeed, physical_width, physical_height, true,
                                                   BOUNCING_FACTOR_X);
    rightCollision = Collisions::checkRightCollision(tiles, &x, &y, &xSpeed, physical_width, physical_height, true,
                                                     BOUNCING_FACTOR_X);
    upperCollision = Collisions::checkUpperCollision(tiles, &x, &y, &ySpeed, physical_width, true, BOUNCING_FACTOR_Y);
}

void Cape::initSprite() {

    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_goldbarsPal, gfx_goldbarsPalLen,
                                                        nullptr, sprite_width * sprite_height, sprite_width,
                                                        spriteType, true, false, LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_goldbarsPal, gfx_goldbarsPalLen,
                                                          nullptr, sprite_width * sprite_height, sprite_width,
                                                          spriteType, true, false, LAYER_LEVEL::MIDDLE_TOP);

    set_frame_gfx();

    subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
    mainSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);

}

void Cape::set_position() {
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

Cape::Cape() {
    physical_height = CAPE_PHYSICAL_HEIGHT;
    physical_width = CAPE_PHYSICAL_WIDTH;
    sprite_height = CAPE_SPRITE_HEIGHT;
    sprite_width = CAPE_SPRITE_WIDTH;
    spriteType = SpritesheetType::MONIEZ_GOLDBARS;
}

void Cape::set_frame_gfx() {


    if (!collected) {
        frameGfx = (u8 *) gfx_goldbarsTiles + (sprite_width * sprite_height * (2) / 2);

    }
    else if (global::main_dude->climbing) {
        //climbing
        frameGfx = (u8 *) gfx_goldbarsTiles + (sprite_width * sprite_height * (3) / 2);

    } else if (global::input_handler->down_key_held || global::main_dude->dead || global::main_dude->stunned) {

        if (anim_frame >= 5)
            anim_frame = 0;

        frameGfx = (u8 *) gfx_goldbarsTiles + (sprite_width * sprite_height * (5 + anim_frame) / 2);
    } else if (!global::main_dude->using_cape && !global::main_dude->stunned &&
               (global::main_dude->xSpeed != 0 || global::main_dude->ySpeed != 0)) {

        //falling, using cape
        if (anim_frame >= 5)
            anim_frame = 0;
        frameGfx = (u8 *) gfx_goldbarsTiles + (sprite_width * sprite_height * (5 + anim_frame) / 2);

    } else if (global::main_dude->xSpeed == 0 && global::main_dude->ySpeed == 0) {
        //staying still
        frameGfx = (u8 *) gfx_goldbarsTiles + (sprite_width * sprite_height * (4) / 2);
    } else {

        //falling
        if (anim_frame >= 2)
            anim_frame = 0;

        frameGfx = (u8 *) gfx_goldbarsTiles + (sprite_width * sprite_height * (10 + anim_frame) / 2);

    }

}

