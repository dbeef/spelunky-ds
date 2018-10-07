//
// Created by xdbeef on 24.02.18.
//

#include <nds.h>
#include <nds/arm9/sprite.h>
#include <maxmod9.h>
#include <cstdlib>
#include "main_dude.hpp"
#include "../../globals_declarations.hpp"
#include "../../collisions/collisions.hpp"
#include "../../tiles/level_rendering_utils.hpp"
#include "../../../build/gfx_spelunker.h"
#include "../../../build/soundbank.h"
#include "../../tiles/tile_orientation.hpp"
#include "../animations/fall_poof.hpp"
#include "../../sound/sound_utils.hpp"
#include "../sprite_utils.hpp"

void MainDude::handle_key_input() {

    if (!stunned && !exiting_level && !dead) {
        if (global::input_handler->b_key_down && time_since_last_jump > 100) {

            if (bottomCollision || climbing) {
                if (carrying_spring_shoes)
                    ySpeed = -MAIN_DUDE_JUMP_SPEED * 1.65;
                else
                    ySpeed = -MAIN_DUDE_JUMP_SPEED;
                climbing = false;
                started_climbing_rope = false;
                started_climbing_ladder = false;
                can_climb_rope = false;
                time_since_last_jump = 0;

                mmEffect(SFX_XJUMP);
            } else if (ySpeed > 0 && carrying_cape) {
                using_cape = true;
            }
            if ((hanging_on_tile_left || hanging_on_tile_right) && hanging_timer > MAIN_DUDE_MIN_HANGING_TIME &&
                time_since_last_jump > 100) {

                mmEffect(SFX_XJUMP);

                ySpeed = -MAIN_DUDE_JUMP_SPEED;
                hanging_on_tile_left = false;
                hanging_on_tile_right = false;
                hanging_timer = 0;
                time_since_last_jump = 0;

            }
        }

        if (global::input_handler->b_key_down && time_since_last_jump > 100) {

            if (!climbing && carrying_jetpack && jetpack_fuel_counter > 0) {
                jumping_timer = 0;
                using_jetpack = true;
                ySpeed -= MAIN_DUDE_JUMP_SPEED;
                jetpack_fuel_counter--;
                mmEffect(SFX_XJETPACK);
                time_since_last_jump = 0;
            } else
                using_jetpack = false;

        }

        if (global::input_handler->y_key_down) {

            if (!stunned && !using_whip) {
                if (holding_item) {
                    throw_item();
                } else {
                    mmEffect(SFX_XWHIP);
                    using_whip = true;
                    animFrame = 0;
                }
            }

        }

        if (global::input_handler->x_key_down && global::hud->ropes > 0) {
            throw_rope();
        } else if (global::input_handler->a_key_down && !holding_item && global::hud->bombs > 0) {
            take_out_bomb();
        }

        if (global::input_handler->left_key_held) {

            sprite_state = W_LEFT;
            hanging_on_tile_left = false;
            if (!(hanging_on_tile_right || hanging_on_tile_left) && !climbing)
                if (speed_inc_timer > MAIN_DUDE_X_SPEED_DELTA_TIME_MS) {
                    xSpeed -= MAIN_DUDE_X_SPEED_DELTA_VALUE;
                    speed_inc_timer = 0;
                }

        }

        if (global::input_handler->right_key_held) {

            sprite_state = W_RIGHT;
            hanging_on_tile_right = false;
            if (!(hanging_on_tile_right || hanging_on_tile_left) && !climbing) {
                if (speed_inc_timer > MAIN_DUDE_X_SPEED_DELTA_TIME_MS) {
                    xSpeed += MAIN_DUDE_X_SPEED_DELTA_VALUE;
                    speed_inc_timer = 0;
                }
            }

        }

        int xx = floor_div(this->x + 0.5 * MAIN_DUDE_PHYSICAL_WIDTH, TILE_W);
        int yy = floor_div(this->y + 0.5 * MAIN_DUDE_PHYSICAL_HEIGHT, TILE_H);

        current_x_in_tiles = xx;
        current_y_in_tiles = yy;

        if (global::input_handler->l_bumper_held || global::input_handler->up_key_held ||
            global::input_handler->down_key_held) {

            if (climbing) {

                if (global::game_state->in_main_menu && !global::game_state->exiting_game) {
                    if (y <= 100) {
                        global::game_state->exiting_game = true;
                    }
                }

                climbing_timer += *global::timer;
                if (climbing_timer > 260) {
                    climbing_timer = 0;
                    climbing_sound++;
                    if (climbing_sound % 2 == 0)
                        mmEffect(SFX_XCLIMB1);
                    else
                        mmEffect(SFX_XCLIMB2);
                }
            } else {
                climbing_timer = 200;
            }

            MapTile *neighboringTiles[9] = {};
            Collisions::getNeighboringTiles(global::current_level->map_tiles, xx, yy, neighboringTiles);


            can_climb_ladder = neighboringTiles[CENTER] != nullptr &&
                               (neighboringTiles[CENTER]->mapTileType == MapTileType::LADDER ||
                                neighboringTiles[CENTER]->mapTileType == MapTileType::LADDER_DECK);

            can_climb_ladder = can_climb_ladder && global::input_handler->up_key_held;
            can_climb_rope = can_climb_rope && global::input_handler->up_key_held;

            exiting_level = neighboringTiles[CENTER] != nullptr &&
                            (neighboringTiles[CENTER]->mapTileType == MapTileType::EXIT) &&
                            global::input_handler->l_bumper_held;

            if (exiting_level) {

                mmEffect(SFX_XSTEPS);

                sound::stop_cave_music();

                x = neighboringTiles[CENTER]->x * 16;
                y = neighboringTiles[CENTER]->y * 16;

                animFrame = 0;
                animation_frame_timer = 0;
                xSpeed = 0;
                ySpeed = 0;

            }

            on_top_of_climbing_space = on_top_of_climbing_space ||
                                       (neighboringTiles[UP_MIDDLE] != nullptr &&
                                        neighboringTiles[UP_MIDDLE]->collidable);

            if (can_climb_ladder) {
                x = neighboringTiles[CENTER]->x * 16;
            }

            if (can_climb_rope || can_climb_ladder) {

                climbing = true;
                jumping_timer = 0;
                xSpeed = 0;

                if (global::input_handler->up_key_held)
                    ySpeed = -1;

                if (can_climb_rope)
                    started_climbing_rope = true;
                else
                    started_climbing_ladder = true;
            }

            if (!can_climb_rope && climbing && on_top_of_climbing_space && !can_climb_ladder) {
                ySpeed = 0;
                jumping_timer = 0;
                xSpeed = 0;
            }

        } else if (climbing) {
            ySpeed = 0;
        }

        if (global::input_handler->down_key_held) {

            MapTile *neighboringTiles[9] = {};

            Collisions::getNeighboringTiles(global::current_level->map_tiles, xx, yy, neighboringTiles);
            can_climb_ladder = neighboringTiles[CENTER]->mapTileType == MapTileType::LADDER ||
                               neighboringTiles[CENTER]->mapTileType == MapTileType::LADDER_DECK;

            if (climbing) {
                can_climb_ladder = neighboringTiles[CENTER] != nullptr &&
                                   (neighboringTiles[CENTER]->mapTileType == MapTileType::LADDER ||
                                    neighboringTiles[CENTER]->mapTileType == MapTileType::LADDER_DECK) &&
                                   (neighboringTiles[DOWN_MIDDLE] == nullptr ||
                                    !neighboringTiles[DOWN_MIDDLE]->collidable);
            }

            if (climbing) {
                ySpeed = 1;
            }

            if ((!can_climb_rope && climbing && !on_top_of_climbing_space) || (!can_climb_ladder && climbing)) {
                jumping_timer = 0;
                climbing = false;
                started_climbing_rope = false;
                started_climbing_ladder = false;
            }

            hanging_on_tile_left = false;
            hanging_on_tile_right = false;
            if (bottomCollision)
                crawling = true;
        } else
            crawling = false;


    } else
        crawling = false;


}


void MainDude::updateTimers() {

    pos_inc_timer += *global::timer;
    speed_inc_timer += *global::timer;
    hanging_timer += *global::timer;

    if (animation_frame_timer > 65) {

        animation_frame_timer = 0;

        if (!using_whip || (using_whip && animFrame < 5) || (started_climbing_rope && animFrame < 12) ||
            (exiting_level && animFrame < 16) || (started_climbing_ladder && animFrame < 6))
            animFrame++;

    }

    if (!global::input_handler->left_key_held && pushing_left) {
        pushing_left = false;
        pushing_timer = 0;
    }
    if (!global::input_handler->right_key_held && pushing_right) {
        pushing_right = false;
        pushing_timer = 0;
    }

    if ((leftCollision || rightCollision) && !crawling && !hanging_on_tile_left && !hanging_on_tile_right &&
        (global::input_handler->left_key_held || global::input_handler->right_key_held)) {
        pushing_timer += *global::timer;
        if (pushing_timer > MAIN_DUDE_PUSHING_TIME) {
            if (leftCollision) {
                pushing_right = true;
                pushing_timer = 0;
            } else {
                pushing_left = true;
                pushing_timer = 0;
            }
        }

    } else {
        pushing_timer = 0;
        pushing_left = false;
        pushing_right = false;
    }

    if (animFrame >= MAIN_DUDE_FRAMES_PER_ANIMATION && !crawling && !pushing_left && !pushing_right && !exiting_level)
        animFrame = 0;

    if (animFrame >= 9 && crawling)
        animFrame = 0;

    if ((pushing_left || pushing_right) && animFrame >= 7)
        animFrame = 0;


    if (!bottomCollision && !hanging_on_tile_left && !hanging_on_tile_right && !climbing)
        jumping_timer += *global::timer;

    if (bottomCollision && jumping_timer > MAIN_DUDE_STUN_FALLING_TIME) {


        if (global::hud->hearts > 0) {
            global::hud->hearts--;
            global::hud->draw_level_hud();
        }

        stunned = true;

        auto *f_left = new FallPoof();
        auto *f_right = new FallPoof();

        f_left->x = x - 4;
        f_right->x = x + MAIN_DUDE_PHYSICAL_WIDTH - 6;

        f_left->y = y + 8;
        f_right->y = y + 8;

        f_left->init();
        f_right->init();

        global::sprites.push_back(f_left);
        global::sprites.push_back(f_right);

        if (global::hud->hearts == 0) {
            global::main_dude->ySpeed = -MAIN_DUDE_JUMP_SPEED * 0.25;
            global::main_dude->dead = true;
            consoleClear();

            mmEffect(SFX_XDIE);
            sound::stop_cave_music();

            global::hud->ropes = 0;
            global::hud->bombs = 0;

        }

        mmEffect(SFX_XLAND);

        jumping_timer = 0;
    } else if (bottomCollision && jumping_timer < MAIN_DUDE_STUN_FALLING_TIME) {
        jumping_timer = 0;
    }

    if (stunned)
        stunned_timer += *global::timer;
    if (stunned_timer > MAIN_DUDE_STUN_TIME) {
        stunned = false;
        stunned_timer = 0;
    }


    if (xSpeed != 0 || stunned || using_whip || (pushing_left || pushing_right) || (climbing && ySpeed != 0) ||
        exiting_level)
        animation_frame_timer += *global::timer;


    if (!bottomCollision)
        crawling = false;

    time_since_last_jump += *global::timer;
    time_since_last_damage += *global::timer;
}


void MainDude::updateSpeed() {

    if (crawling)
        limit_speed(MAIN_DUDE_MAX_X_SPEED_CRAWLING, MAIN_DUDE_MAX_Y_SPEED);
    else if (global::input_handler->r_bumper_held)
        limit_speed(MAIN_DUDE_MAX_X_SPEED_RUNNING, MAIN_DUDE_MAX_Y_SPEED);
    else if (using_cape)
        limit_speed(MAIN_DUDE_MAX_X_SPEED, MAIN_DUDE_MAX_Y_SPEED_USING_CAPE);
    else
        limit_speed(MAIN_DUDE_MAX_X_SPEED, MAIN_DUDE_MAX_Y_SPEED);

    bool change_pos = (crawling && pos_inc_timer > 20) || (!crawling && pos_inc_timer > 1);

    if (change_pos) {

        apply_friction(MAIN_DUDE_FRICTION_DELTA_SPEED * 0.9f);
        update_position();
        pos_inc_timer = 0;

        if (!bottomCollision && !(hanging_on_tile_left || hanging_on_tile_right) && !climbing) {
            ySpeed += GRAVITY_DELTA_SPEED;
        }
    }


}


void MainDude::updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) {

    MapTile *t[9];
    Collisions::getNeighboringTiles(global::current_level->map_tiles,
                                    x_current_pos_in_tiles, y_current_pos_in_tiles, t);

    bottomCollision = Collisions::checkBottomCollision(t, &this->x, &this->y, &ySpeed, 16, 16, dead, BOUNCING_FACTOR_Y);
    leftCollision = Collisions::checkLeftCollision(t, &this->x, &this->y, &xSpeed, 16, 16, dead, BOUNCING_FACTOR_X);
    rightCollision = Collisions::checkRightCollision(t, &this->x, &this->y, &xSpeed, 16, 16, dead, BOUNCING_FACTOR_X);
    upperCollision = Collisions::checkUpperCollision(t, &this->x, &this->y, &ySpeed, 16, dead, BOUNCING_FACTOR_Y);

    can_hang_on_tile(t);

    if (hanging_on_tile_right || hanging_on_tile_left) {
        using_cape = false;
        using_jetpack = false;
    }

    if (upperCollision || bottomCollision) {
        hanging_on_tile_left = false;
        hanging_on_tile_right = false;
        if (using_cape)
            jumping_timer = 0;
    }

    if (!bottomCollision) {
        if ((t[TileOrientation::RIGHT_MIDDLE] == nullptr || !t[TileOrientation::RIGHT_MIDDLE]->collidable) &&
            (t[TileOrientation::RIGHT_UP] != nullptr && t[TileOrientation::RIGHT_DOWN] != nullptr)) {
            //if there's no collidable tile on right-mid, but there are on right-up and right-down,
            //add extra x-pos to ease going through a hole
            if (xSpeed > 0)
                x += 2;
        }

        if ((t[TileOrientation::LEFT_MIDDLE] == nullptr || !t[TileOrientation::LEFT_MIDDLE]->collidable) &&
            (t[TileOrientation::LEFT_UP] != nullptr && t[TileOrientation::LEFT_DOWN] != nullptr)) {
            //same but for left side
            if (xSpeed < 0)
                x -= 2;
        }
    }

}

void MainDude::init() {
    initSprite();
    whip = new Whip();
    whip->init();
//    global::sprites.push_back(whip);
}

void MainDude::draw() {

    //todo split this function into utils
    //todo use moving_object->get_x_y in every moving_object
    //todo use moving_object->update_position in every moving_object
    //todo make class static_object for copyrights sign, whip and other non-moving things
    //todo make file input/output class for savegames

    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);
    sprite_utils::set_entry_xy(main_spelunker, main_x, main_y);
    sprite_utils::set_entry_xy(sub_spelunker, sub_x, sub_y);

    match_animation();
    reset_values_checked_every_frame();
    apply_blinking_on_damage();

}

void MainDude::initSprite() {

    delete main_spelunker;
    delete sub_spelunker;


    main_spelunker = global::main_oam_manager->initSprite(gfx_spelunkerPal, gfx_spelunkerPalLen, nullptr,
                                                          sprite_width * sprite_height, 16, MAIN_DUDE, true, false,
                                                          LAYER_LEVEL::MIDDLE_TOP);


    sub_spelunker = global::sub_oam_manager->initSprite(gfx_spelunkerPal, gfx_spelunkerPalLen, nullptr,
                                                        sprite_width * sprite_height, 16, MAIN_DUDE, true, false,
                                                        LAYER_LEVEL::MIDDLE_TOP);

    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);
    sprite_utils::set_entry_xy(main_spelunker, main_x, main_y);
    sprite_utils::set_entry_xy(sub_spelunker, sub_x, sub_y);

}


void MainDude::reset_values_checked_every_frame() {
    can_climb_rope = false;
    can_climb_ladder = false;
}

MainDude::MainDude() {
    physical_height = MAIN_DUDE_PHYSICAL_HEIGHT;
    physical_width = MAIN_DUDE_PHYSICAL_WIDTH;
    sprite_height = MAIN_DUDE_SPRITE_HEIGHT;
    sprite_width = MAIN_DUDE_SPRITE_WIDTH;
    hitpoints = MAIN_DUDE_HITPOINTS;
}
