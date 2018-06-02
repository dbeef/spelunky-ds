//
// Created by xdbeef on 04.04.18.
//

#include "../../globals_declarations.h"
#include "../main_dude/main_dude.h"
#include "../../../build/gfx_blood_rock_rope_poof.h"
#include "rock.h"
#include "../../collisions/collisions.h"
#include "../../tiles/map_utils.h"

#define ROCK_POS_INC_DELTA 15

void Rock::draw() {

    check_if_can_be_pickuped();

    if (hold_by_main_dude) {

        y = global::main_dude->y + 6;

        if (global::main_dude->state == 1) {
            x = global::main_dude->x - 2;
        } else
            x = global::main_dude->x + 10;

    }

    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);

    mainSpriteInfo->entry->x = main_x;
    mainSpriteInfo->entry->y = main_y;

    subSpriteInfo->entry->x = sub_x;
    subSpriteInfo->entry->y = sub_y;

    mainSpriteInfo->entry->hFlip = false;
    subSpriteInfo->entry->hFlip = false;

    mainSpriteInfo->entry->vFlip = false;
    subSpriteInfo->entry->vFlip = false;

    kill_mobs_if_thrown(1);

}


void Rock::init() {
    initSprite();
    activated_by_main_dude = true;
}

void Rock::updateSpeed() {

    limit_speed(MAX_X_SPEED_ROCK, MAX_Y_SPEED_ROCK);

    pos_inc_timer += *global::timer;

    bool change_pos = (pos_inc_timer > ROCK_POS_INC_DELTA) && !hold_by_main_dude;

    if (change_pos) {
        apply_friction(0.055);
        update_position();
        apply_gravity(GRAVITY_DELTA_SPEED * 0.8);
        pos_inc_timer = 0;
    }

}


void Rock::updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) {

    MapTile *tiles[9];
    Collisions::getNeighboringTiles(global::level_generator->map_tiles, x_current_pos_in_tiles, y_current_pos_in_tiles,
                                    tiles);

    bottomCollision = Collisions::checkBottomCollision(tiles, &x, &y, &ySpeed, physical_width, physical_height, true, BOUNCING_FACTOR_Y);
    leftCollision = Collisions::checkLeftCollision(tiles, &x, &y, &xSpeed, physical_width, physical_height, true, BOUNCING_FACTOR_X);
    rightCollision = Collisions::checkRightCollision(tiles, &x, &y, &xSpeed, physical_width, physical_height, true, BOUNCING_FACTOR_X);
    upperCollision = Collisions::checkUpperCollision(tiles, &x, &y, &ySpeed, physical_width, true, BOUNCING_FACTOR_Y);

}

void Rock::initSprite() {

    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_blood_rock_rope_poofPal, gfx_blood_rock_rope_poofPalLen,
                                                        nullptr, sprite_width * sprite_height, 8, BLOOD_ROCK_ROPE_POOF, true, false,
                                                        LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_blood_rock_rope_poofPal, gfx_blood_rock_rope_poofPalLen,
                                                          nullptr, sprite_width * sprite_height, 8, BLOOD_ROCK_ROPE_POOF, true, false,
                                                          LAYER_LEVEL::MIDDLE_TOP);

    frameGfx = (u8 *) gfx_blood_rock_rope_poofTiles + (sprite_height * sprite_width * 7 / 2);
    subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
    mainSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);

}

Rock::Rock() {
    physical_height = ROCK_PHYSICAL_HEIGHT;
    physical_width = ROCK_PHYSICAL_WIDTH;
    sprite_height = ROCK_SPRITE_HEIGHT;
    sprite_width = ROCK_SPRITE_WIDTH;
    spriteType = SpritesheetType::BLOOD_ROCK_ROPE_POOF;
}

