//
// Created by xdbeef on 04.04.18.
//

#include "bomb.hpp"
#include "../../globals_declarations.hpp"
#include "../../tiles/level_rendering_utils.hpp"
#include "../../collisions/collisions.hpp"
#include "../main_dude/main_dude.hpp"
#include "../../../build/gfx_explosion.h"
#include "../../../build/soundbank.h"
#include "../../tiles/level_rendering_utils.hpp"
#include "../../collisions/collisions.hpp"
#include "../animations/flame.hpp"
#include "../sprite_utils.hpp"
#include <math.h>       /* floor */
#include <maxmod9.h>

#define BOMB_POS_INC_DELTA 15
#define BOMB_ANIM_FRAME_DELTA 50

void Bomb::draw() {

    if (ready_to_dispose) return;

    if (explosionFrame == 0)
        check_if_can_be_pickuped();

    set_pickuped_position(1, 10, 6);

    if (activated && !armed) {
        arm();
        armed = true;
        armedTimer = 0;
    }

    if (armed) {

        armedTimer += *global::timer;

        if (armedTimer < ARMED_TIME_BLINK_SLOW) {

            if ((armedTimer) % 250 < 125)
                disarm();
            else
                arm();

        } else if (armedTimer < ARMED_TIME_BLINK_FAST) {

            if ((armedTimer) % 120 < 60)
                disarm();
            else
                arm();

        } else {

            explosionTimer += *global::timer;
            if (explosionTimer > BOMB_ANIM_FRAME_DELTA && explosionFrame < 10) {

                if (explosionFrame == 0)
                    explode();

                explosionTimer = 0;
                explosionFrame++;

                if (explosionFrame >= 10) {
                    ready_to_dispose = true;
                    sprite_utils::set_visibility(false, mainSpriteInfo, subSpriteInfo);
                } else {
                    frameGfx = sprite_utils::get_frame((u8 *) gfx_explosionTiles, BOMB_SPRITE_SIZE, 2 + explosionFrame);
                    sprite_utils::update_frame(frameGfx, BOMB_SPRITE_SIZE, mainSpriteInfo, subSpriteInfo);
                }
            }
        }

    }

    set_position();

    kill_mobs_if_thrown(1);
}


void Bomb::init() {
    initSprite();
    disarm();
    explosionFrame = 0;
}

void Bomb::arm() {
    frameGfx = sprite_utils::get_frame((u8 *) gfx_explosionTiles, BOMB_SPRITE_SIZE, 1);
    sprite_utils::update_frame(frameGfx, BOMB_SPRITE_SIZE, mainSpriteInfo, subSpriteInfo);
}

void Bomb::disarm() {
    frameGfx = sprite_utils::get_frame((u8 *) gfx_explosionTiles, BOMB_SPRITE_SIZE, 0);
    sprite_utils::update_frame(frameGfx, BOMB_SPRITE_SIZE, mainSpriteInfo, subSpriteInfo);
}

void Bomb::updateSpeed() {

    limit_speed(MAX_X_SPEED_BOMB, MAX_Y_SPEED_BOMB);

    pos_inc_timer += *global::timer;

    bool change_pos = (pos_inc_timer > BOMB_POS_INC_DELTA) && !hold_by_main_dude && explosionFrame == 0;

    if (change_pos) {
        update_position();
        apply_friction(0.055);
        apply_gravity(GRAVITY_DELTA_SPEED);
        pos_inc_timer = 0;
    }

}

void Bomb::updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) {

    MapTile *t[9];
    Collisions::getNeighboringTiles(global::current_level->map_tiles,
                                    x_current_pos_in_tiles, y_current_pos_in_tiles, t);

    upperCollision = Collisions::checkUpperCollision(t, &x, &y, &ySpeed, physical_width, true, 0.35);
    bottomCollision = Collisions::checkBottomCollision(t, &x, &y, &ySpeed, physical_width, physical_height, true, 0.35);
    leftCollision = Collisions::checkLeftCollision(t, &x, &y, &xSpeed, physical_width, physical_height, true, 0.15);
    rightCollision = Collisions::checkRightCollision(t, &x, &y, &xSpeed, physical_width, physical_height, true, 0.15);
}

void Bomb::initSprite() {

    delete mainSpriteInfo;
    delete subSpriteInfo;

    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_explosionPal, gfx_explosionPalLen,
                                                        nullptr, BOMB_SPRITE_SIZE, 64, BOMB, true, false,
                                                        LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_explosionPal, gfx_explosionPalLen,
                                                          nullptr, BOMB_SPRITE_SIZE, 64, BOMB, true, false,
                                                          LAYER_LEVEL::MIDDLE_TOP);
    if (armed) {
        if (armedTimer < ARMED_TIME_BLINK_SLOW) {
            if ((armedTimer) % 250 < 125)
                disarm();
            else
                arm();
        } else if (armedTimer < ARMED_TIME_BLINK_FAST) {
            if ((armedTimer) % 120 < 60)
                disarm();
            else
                arm();
        }
    } else disarm();

    set_position();
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_visibility(true, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_priority(OBJPRIORITY_0, mainSpriteInfo, subSpriteInfo);
}

Bomb::Bomb() {
    physical_height = BOMB_PHYSICAL_HEIGHT;
    physical_width = BOMB_PHYSICAL_WIDTH;
    sprite_height = BOMB_SPRITE_HEIGHT;
    sprite_width = BOMB_SPRITE_WIDTH;
}

void Bomb::explode() {

    for (int a = 0; a < 4; a++) {
        auto *flame = new Flame();
        flame->init();
        flame->x = x;
        flame->y = y;

        if (a % 2 == 0)
            flame->xSpeed = (1.3 / a);
        else
            flame->xSpeed = (-1.3 / a);

        flame->ySpeed = (-2 / a);
        global::sprites_to_add.push_back(flame);
    }

    mmEffect(SFX_XEXPLOSION);

    int xx = floor_div(this->x + 0.5 * BOMB_SIZE, TILE_W);
    int yy = floor_div(this->y + 0.5 * BOMB_SIZE, TILE_H);

    Collisions::bombNeighboringTiles(global::current_level->map_tiles, xx, yy);
    global::game_state->bombed = true;

}

//FIXME This viewporting works buggy, it would be better to split this class into 8x8 bomb and 64x64 explosion animation
void Bomb::set_position() {

    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);

    //that's because 8x8 bomb sprite is stored in 64x64 sprite, packed with explosion animation
    if (explosionFrame > 0) {
        main_x -= 32;
        main_y -= 32;

        sub_x -= 32;
        sub_y -= 32;

    } else {
        main_x -= 29;
        main_y -= 29;

        sub_x -= 29;
        sub_y -= 29;
    }

    sprite_utils::set_entry_xy(mainSpriteInfo, main_x, main_y);
    sprite_utils::set_entry_xy(subSpriteInfo, sub_x, sub_y);

}

void Bomb::deleteSprite() {
    delete mainSpriteInfo;
    delete subSpriteInfo;
    mainSpriteInfo = nullptr;
    subSpriteInfo = nullptr;
}



