//
// Created by xdbeef on 04.04.18.
//

#include "bomb.h"
#include "../../globals_declarations.h"
#include "../../tiles/map_utils.h"
#include "../../collisions/collisions.h"
#include "../main_dude/main_dude.h"
#include "../../../build/gfx_explosion.h"
#include "../../../build/soundbank.h"
#include "../../tiles/map_utils.h"
#include "../../collisions/collisions.h"
#include "../animations/flame.h"
#include <math.h>       /* floor */
#include <maxmod9.h>

#define BOMB_POS_INC_DELTA 15
#define BOMB_ANIM_FRAME_DELTA 50

void Bomb::draw() {

    if (ready_to_dispose) {
        mainSpriteInfo->entry->isHidden = true;
        subSpriteInfo->entry->isHidden = true;
        return;
    } else {
        mainSpriteInfo->entry->isHidden = false;
        subSpriteInfo->entry->isHidden = false;
    }

    if (explosionFrame == 0)
        check_if_can_be_pickuped();

    if (hold_by_main_dude) {

        y = global::main_dude->y + 6;

        if (global::main_dude->sprite_state == SpriteState::W_LEFT) {
            x = global::main_dude->x - 2;
        } else
            x = global::main_dude->x + 10;
    }


    if (activated && !armed) {
        arm();
        armed = true;
        armedTimer = 0;
    }

    if (armed) {
        armedTimer += *global::timer;
        if (armedTimer < ARMED_TIME_BLINK_SLOW) {
            if ((armedTimer) % 250 < 125) {
                disarm();
            } else {
                arm();
            }
        } else if (armedTimer < ARMED_TIME_BLINK_FAST) {
            if ((armedTimer) % 120 < 60) {
                disarm();
            } else {
                arm();
            }
        } else {


            explosionTimer += *global::timer;
            if (explosionTimer > BOMB_ANIM_FRAME_DELTA && explosionFrame < 10) {

                if (explosionFrame == 0) {

                    for (int a = 0; a < 4; a++) {
                        Flame *flame = new Flame();
                        flame->init();
                        flame->x = x;
                        flame->y = y;

                        if (a % 2 == 0)
                            flame->xSpeed = (1.3 / a);
                        else
                            flame->xSpeed = (-1.3 / a);

                        flame->ySpeed = -2 / a;
                        global::sprites.push_back(flame);
                    }

                    mmEffect(SFX_XEXPLOSION);

                    int xx = floor_div(this->x + 0.5 * BOMB_SIZE, TILE_W);
                    int yy = floor_div(this->y + 0.5 * BOMB_SIZE, TILE_H);

                    Collisions::bombNeighboringTiles(global::level_generator->map_tiles, xx, yy);
                    global::game_state->bombed = true;
                }

                explosionTimer = 0;
                explosionFrame++;

                if (explosionFrame >= 10) {
                    subSpriteInfo->updateFrame(nullptr, sprite_width * sprite_height);
                    mainSpriteInfo->updateFrame(nullptr, sprite_width * sprite_height);
                    ready_to_dispose = true;
                } else {
                    frameGfx = (u8 *) gfx_explosionTiles + (2 + explosionFrame) * sprite_width * sprite_height / 2;
                    subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
                    mainSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
                }
            }
        }

    }


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

    mainSpriteInfo->entry->x = main_x;
    mainSpriteInfo->entry->y = main_y;

    subSpriteInfo->entry->x = sub_x;
    subSpriteInfo->entry->y = sub_y;

    kill_mobs_if_thrown(1);

}


void Bomb::init() {
    initSprite();
    disarm();
    explosionFrame = 0;
}

void Bomb::arm() {
    frameGfx = (u8 *) gfx_explosionTiles + 1 * sprite_width * sprite_height / 2;
    subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
    mainSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
}

void Bomb::disarm() {
    frameGfx = (u8 *) gfx_explosionTiles;
    subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
    mainSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
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

    MapTile *tiles[9];
    Collisions::getNeighboringTiles(global::level_generator->map_tiles, x_current_pos_in_tiles, y_current_pos_in_tiles,
                                    tiles);

    upperCollision = Collisions::checkUpperCollision(tiles, &x, &y, &ySpeed, physical_width, true, BOUNCING_FACTOR_Y);
    bottomCollision = Collisions::checkBottomCollision(tiles, &x, &y, &ySpeed, physical_width, physical_height, true,
                                                       BOUNCING_FACTOR_Y);
    leftCollision = Collisions::checkLeftCollision(tiles, &x, &y, &xSpeed, physical_width, physical_height, true,
                                                   BOUNCING_FACTOR_X);
    rightCollision = Collisions::checkRightCollision(tiles, &x, &y, &xSpeed, physical_width, physical_height, true,
                                                     BOUNCING_FACTOR_X);

}

void Bomb::initSprite() {
    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_explosionPal, gfx_explosionPalLen,
                                                        nullptr, sprite_width * sprite_height, 64, BOMB, true, false,
                                                        LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_explosionPal, gfx_explosionPalLen,
                                                          nullptr, sprite_width * sprite_height, 64, BOMB, true, false,
                                                          LAYER_LEVEL::MIDDLE_TOP);
}

Bomb::Bomb() {
    physical_height = BOMB_PHYSICAL_HEIGHT;
    physical_width = BOMB_PHYSICAL_WIDTH;
    sprite_height = BOMB_SPRITE_HEIGHT;
    sprite_width = BOMB_SPRITE_WIDTH;
}



