//
// Created by xdbeef on 02.06.18.
//

#include "../../../build/gfx_bat_snake_jetpack.h"
#include "../../globals_declarations.hpp"
#include "../animations/got_collectible.hpp"
#include "../../collisions/collisions.hpp"
#include "jetpack.hpp"
#include "../animations/fall_poof.hpp"

#define JETPACK_POS_INC_DELTA 15

void Jetpack::draw() {


    if (global::main_dude->carrying_cape && collected) {
        global::main_dude->carrying_jetpack = false;
        ready_to_dispose = true;
    }

    if (ready_to_dispose) {
        mainSpriteInfo->entry->isHidden = true;
        subSpriteInfo->entry->isHidden = true;
        return;
    } else {
        mainSpriteInfo->entry->isHidden = false;
        subSpriteInfo->entry->isHidden = false;
    }


    if (bought && !collected && check_if_can_be_equipped()) {
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


    if (collected) {

        if (global::main_dude->climbing || global::main_dude->exiting_level) {
            mainSpriteInfo->entry->priority = OBJPRIORITY_0;
            subSpriteInfo->entry->priority = OBJPRIORITY_0;
            set_pickuped_position_not_checking(-3, 2);
            mainSpriteInfo->entry->hFlip = false;
            subSpriteInfo->entry->hFlip = false;
        } else if (global::main_dude->sprite_state == SpriteState::W_LEFT) {
            mainSpriteInfo->entry->priority = OBJPRIORITY_1;
            subSpriteInfo->entry->priority = OBJPRIORITY_1;
            set_pickuped_position_not_checking(-6, 0);
            mainSpriteInfo->entry->hFlip = false;
            subSpriteInfo->entry->hFlip = false;
        } else if (global::main_dude->sprite_state == SpriteState::W_RIGHT) {
            mainSpriteInfo->entry->priority = OBJPRIORITY_1;
            subSpriteInfo->entry->priority = OBJPRIORITY_1;
            set_pickuped_position_not_checking(-3, 0);
            mainSpriteInfo->entry->hFlip = true;
            subSpriteInfo->entry->hFlip = true;
        }


        if (global::main_dude->using_jetpack) {
            poof_spawn_counter++;
            poof_spawn_timer += *global::timer;

            if (global::main_dude->jetpack_fuel_counter <= 0) {
                global::main_dude->using_jetpack = false;
            }

            if (poof_spawn_counter >= 2 && poof_spawn_timer > 130) {

                poof_spawn_timer = 0;
                poof_spawn_counter = 0;

                FallPoof *f = new FallPoof();
                f->gravity = true;
                f->x = x + 4;
                f->y = y + 4;
                f->init();
                global::sprites_to_add.push_back(f);

            }

        }

        frameGfx = (u8 *) gfx_bat_snake_jetpackTiles + (sprite_width * sprite_height * (8) / 2);
    } else {
        frameGfx = (u8 *) gfx_bat_snake_jetpackTiles + (sprite_width * sprite_height * (7) / 2);
    }

    subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
    mainSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);


    mainSpriteInfo->entry->isHidden = false;
    subSpriteInfo->entry->isHidden = false;
    mainSpriteInfo->entry->vFlip = false;
    subSpriteInfo->entry->vFlip = false;


    if (global::main_dude->bottomCollision || global::main_dude->hanging_on_tile_left ||
        global::main_dude->hanging_on_tile_right) {
        global::main_dude->using_jetpack = false;
        global::main_dude->jetpack_fuel_counter = 15;
    }

    set_position();
}


void Jetpack::init() {
    initSprite();
    init_anim_icon();

}

void Jetpack::updateSpeed() {

    limit_speed(MAX_X_SPEED_JETPACK, MAX_Y_SPEED_JETPACK);

    pos_inc_timer += *global::timer;

    bool change_pos = (pos_inc_timer > JETPACK_POS_INC_DELTA) && !collected;

    if (change_pos) {
        update_position();
        apply_friction(0.055);
        apply_gravity(GRAVITY_DELTA_SPEED);
        pos_inc_timer = 0;
    }

}

void Jetpack::updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) {

    if (collected)
        return;

    MapTile *tiles[9];
    Collisions::getNeighboringTiles(global::current_level->map_tiles, x_current_pos_in_tiles,
                                    y_current_pos_in_tiles,
                                    tiles);

    upperCollision = Collisions::checkUpperCollision(tiles, &x, &y, &ySpeed, physical_width, true, BOUNCING_FACTOR_Y);
    bottomCollision = Collisions::checkBottomCollision(tiles, &x, &y, &ySpeed, physical_width, physical_height, true,
                                                       BOUNCING_FACTOR_Y);
    leftCollision = Collisions::checkLeftCollision(tiles, &x, &y, &xSpeed, physical_width, physical_height, true,
                                                   BOUNCING_FACTOR_X);
    rightCollision = Collisions::checkRightCollision(tiles, &x, &y, &xSpeed, physical_width, physical_height, true,
                                                     BOUNCING_FACTOR_X);
}

void Jetpack::initSprite() {

    delete mainSpriteInfo;
    delete subSpriteInfo;

    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_bat_snake_jetpackPal, gfx_bat_snake_jetpackPalLen,
                                                        nullptr, sprite_width * sprite_height, sprite_width,
                                                        spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_bat_snake_jetpackPal, gfx_bat_snake_jetpackPalLen,
                                                          nullptr, sprite_width * sprite_height, sprite_width,
                                                          spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);

    if (collected) {
        frameGfx = (u8 *) gfx_bat_snake_jetpackTiles + (sprite_width * sprite_height * (8) / 2);
    } else {
        frameGfx = (u8 *) gfx_bat_snake_jetpackTiles + (sprite_width * sprite_height * (7) / 2);
    }

    subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
    mainSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);

}

void Jetpack::set_position() {
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

    update_anim_icon(x, y, physical_width);

}

Jetpack::Jetpack() {
    cost = 16000;
    name = "JETPACK";
    physical_height = JETPACK_PHYSICAL_HEIGHT;
    physical_width = JETPACK_PHYSICAL_WIDTH;
    sprite_height = JETPACK_SPRITE_HEIGHT;
    sprite_width = JETPACK_SPRITE_WIDTH;
    spritesheet_type = SpritesheetType::BAT_JETPACK;
}

void Jetpack::equip() {

    GotCollectible *g = new GotCollectible();
    g->x = x - 12;
    g->y = y - 20;
    g->collectible_type = 0;
    g->init();
    global::sprites_to_add.push_back(g);

    if (global::main_dude->carrying_cape) {
        global::main_dude->carrying_cape = false;
    }

    if (!global::main_dude->carrying_jetpack) {
        global::main_dude->carrying_jetpack = true;
        set_position();
        collected = true;
    } else {
        //we've collected an item that is already in inventory, dispose
        ready_to_dispose = true;
    }
}

