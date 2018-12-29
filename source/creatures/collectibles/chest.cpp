//
// Created by xdbeef on 16.05.18.
//

#include <maxmod9.h>
#include <cstdlib>
#include "../../../build/gfx_spike_collectibles_flame.h"
#include "chest.hpp"
#include "../../collisions/collisions.hpp"
#include "../../globals_declarations.hpp"
#include "../../../build/soundbank.h"
#include "moniez.hpp"
#include "../sprite_utils.hpp"
#include "ruby_small.h"
#include "ruby_big.h"

#define SPEED_OF_THROWING_CHEST_X 1
#define SPEED_OF_THROWING_CHEST_Y 1
#define CHEST_PICKUP_OFFSET_X 8
#define CHECK_PICKUP_OFFSET_Y 2
#define CHEST_POS_INC_DELTA 15
#define CHEST_FRICTION 0.2

void Chest::draw() {

    if (ready_to_dispose) return;

    sprite_utils::set_visibility(true, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);

    check_if_can_be_pickuped();
    set_pickuped_position(CHEST_PICKUP_OFFSET_X, CHECK_PICKUP_OFFSET_Y);

    if (check_if_can_be_opened()) {
        match_animation();
        mmEffect(SFX_XCHESTOPEN);
        spawn_treasure();
    }

    kill_mobs_if_thrown(1);
    set_position();
}


void Chest::init() {
    initSprite();
}

void Chest::updateSpeed() {

    limit_speed(MAX_X_SPEED_CHEST, MAX_Y_SPEED_CHEST);

    pos_inc_timer += *global::timer;

    bool change_pos = (pos_inc_timer > CHEST_POS_INC_DELTA) && !hold_by_main_dude;

    if (change_pos) {
        update_position();

        if (bottomCollision)
            apply_friction(CHEST_FRICTION);

        apply_gravity(GRAVITY_DELTA_SPEED);
        pos_inc_timer = 0;
    }
}

void Chest::updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) {
    MapTile *t[9];
    Collisions::getNeighboringTiles(global::current_level->map_tiles,
                                    x_current_pos_in_tiles, y_current_pos_in_tiles, t);

    upperCollision = Collisions::checkUpperCollision(t, &x, &y, &ySpeed, physical_width, true, 0.35);
    bottomCollision = Collisions::checkBottomCollision(t, &x, &y, &ySpeed, physical_width, physical_height, true, 0.42);
    leftCollision = Collisions::checkLeftCollision(t, &x, &y, &xSpeed, physical_width, physical_height, true, 0.15);
    rightCollision = Collisions::checkRightCollision(t, &x, &y, &xSpeed, physical_width, physical_height, true, 0.15);
}

void Chest::initSprite() {

    delete mainSpriteInfo;
    delete subSpriteInfo;

    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_spike_collectibles_flamePal,
                                                        gfx_spike_collectibles_flamePalLen,
                                                        nullptr, CHEST_SPRITE_SIZE, sprite_width,
                                                        spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_spike_collectibles_flamePal,
                                                          gfx_spike_collectibles_flamePalLen,
                                                          nullptr, CHEST_SPRITE_SIZE, sprite_width,
                                                          spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);

    sprite_utils::set_visibility(true, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);
    set_position();
    match_animation();
}

void Chest::set_position() {
    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);
    sprite_utils::set_entry_xy(mainSpriteInfo, main_x, main_y);
    sprite_utils::set_entry_xy(subSpriteInfo, sub_x, sub_y);
}

Chest::Chest() {
    physical_height = CHEST_PHYSICAL_HEIGHT;
    physical_width = CHEST_PHYSICAL_WIDTH;
    sprite_height = CHEST_SPRITE_HEIGHT;
    sprite_width = CHEST_SPRITE_WIDTH;
    speed_of_throwing_x = SPEED_OF_THROWING_CHEST_X;
    speed_of_throwing_y = SPEED_OF_THROWING_CHEST_Y;
    spritesheet_type = SpritesheetType::SPIKES_COLLECTIBLES;
}

void Chest::spawn_treasure() {

    for (int a = 0; a < 4; a++) {

        int ruby_type = rand() % 2;

        if (ruby_type == 0) {
            
            auto *ruby_small = new RubySmall();

            ruby_small->x = x + CHEST_PHYSICAL_WIDTH * 0.5;
            ruby_small->y = y + CHEST_PHYSICAL_HEIGHT * 0.5;
            ruby_small->init();
            ruby_small->ySpeed = -1.7;
            ruby_small->collectible_timer = 0;
            if (rand() % 2 == 0)
                ruby_small->xSpeed = -0.8;
            else
                ruby_small->xSpeed = 0.8;
            global::creatures_to_add.push_back(ruby_small);

        } else {
            
            auto *ruby_big = new RubyBig();
            ruby_big->x = x + CHEST_PHYSICAL_WIDTH * 0.5;
            ruby_big->y = y + CHEST_PHYSICAL_HEIGHT * 0.5;
            ruby_big->init();
            ruby_big->ySpeed = -1.7;
            ruby_big->collectible_timer = 0;
            if (rand() % 2 == 0)
                ruby_big->xSpeed = -0.8;
            else
                ruby_big->xSpeed = 0.8;
            global::creatures_to_add.push_back(ruby_big);
        }
    }
}

void Chest::match_animation() {

    if (activated)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spike_collectibles_flameTiles, CHEST_SPRITE_SIZE, 3);
    else
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spike_collectibles_flameTiles, CHEST_SPRITE_SIZE, 2);

    sprite_utils::update_frame(frameGfx, CHEST_SPRITE_SIZE, mainSpriteInfo, subSpriteInfo);
}

void Chest::deleteSprite() {
    delete mainSpriteInfo;
    delete subSpriteInfo;
    mainSpriteInfo = nullptr;
    subSpriteInfo = nullptr;
}

