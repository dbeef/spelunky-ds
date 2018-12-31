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
#include "../../tiles/map_tile.hpp"
#include "../../tiles/level.hpp"
#include "../../camera/camera.hpp"
#include "../sprite_info.h"
#include "../items/bomb.hpp"
#include "../../input/input_handler.hpp"

// Called externally in game loop
void MainDude::handle_key_input() {

    if (!stunned && !exiting_level && !dead) {
        if (global::input_handler->b_key_down && time_since_last_jump > 100) {

            if (_bottom_collision || climbing) {
                if (carrying_spring_shoes)
                    _y_speed = -MAIN_DUDE_JUMP_SPEED * 1.65;
                else
                    _y_speed = -MAIN_DUDE_JUMP_SPEED;
                climbing = false;
                started_climbing_rope = false;
                started_climbing_ladder = false;
                can_climb_rope = false;
                time_since_last_jump = 0;

                mmEffect(SFX_XJUMP);
            } else if (_y_speed > 0 && carrying_cape) {
                using_cape = true;
            }
            if ((hanging_on_tile_left || hanging_on_tile_right) && hanging_timer > MAIN_DUDE_MIN_HANGING_TIME &&
                time_since_last_jump > 100) {

                mmEffect(SFX_XJUMP);

                _y_speed = -MAIN_DUDE_JUMP_SPEED;
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
                _y_speed -= MAIN_DUDE_JUMP_SPEED;
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
                    _x_speed -= MAIN_DUDE_X_SPEED_DELTA_VALUE;
                    speed_inc_timer = 0;
                }

        }

        if (global::input_handler->right_key_held) {

            sprite_state = W_RIGHT;
            hanging_on_tile_right = false;
            if (!(hanging_on_tile_right || hanging_on_tile_left) && !climbing) {
                if (speed_inc_timer > MAIN_DUDE_X_SPEED_DELTA_TIME_MS) {
                    _x_speed += MAIN_DUDE_X_SPEED_DELTA_VALUE;
                    speed_inc_timer = 0;
                }
            }

        }

        int xx = floor_div(this->_x + 0.5 * _physical_width, TILE_W);
        int yy = floor_div(this->_y + 0.5 * _physical_height, TILE_H);

        current_x_in_tiles = xx;
        current_y_in_tiles = yy;

        if (global::input_handler->l_bumper_held || global::input_handler->up_key_held ||
            global::input_handler->down_key_held) {

            if (climbing) {

                if (global::game_state->in_main_menu && !global::game_state->exiting_game) {
                    if (_y <= 100) {
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

                _x = neighboringTiles[CENTER]->x * 16;
                _y = neighboringTiles[CENTER]->y * 16;

                animFrame = 0;
                animation_frame_timer = 0;
                _x_speed = 0;
                _y_speed = 0;

            }

            on_top_of_climbing_space = on_top_of_climbing_space ||
                                       (neighboringTiles[UP_MIDDLE] != nullptr &&
                                        neighboringTiles[UP_MIDDLE]->collidable);

            if (can_climb_ladder) {
                _x = neighboringTiles[CENTER]->x * 16;
            }

            if (can_climb_rope || can_climb_ladder) {

                climbing = true;
                jumping_timer = 0;
                _x_speed = 0;

                if (global::input_handler->up_key_held)
                    _y_speed = -1;

                if (can_climb_rope)
                    started_climbing_rope = true;
                else
                    started_climbing_ladder = true;
            }

            if (!can_climb_rope && climbing && on_top_of_climbing_space && !can_climb_ladder) {
                _y_speed = 0;
                jumping_timer = 0;
                _x_speed = 0;
            }

        } else if (climbing) {
            _y_speed = 0;
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
                _y_speed = 1;
            }

            if ((!can_climb_rope && climbing && !on_top_of_climbing_space) || (!can_climb_ladder && climbing)) {
                jumping_timer = 0;
                climbing = false;
                started_climbing_rope = false;
                started_climbing_ladder = false;
            }

            hanging_on_tile_left = false;
            hanging_on_tile_right = false;
            if (_bottom_collision) {
                crawling = true;
                _pos_update_delta = main_dude_pos_update_delta_crawling;
                _max_x_speed = main_dude_max_x_crawling;
            }
        } else {
            crawling = false;
            if (!global::input_handler->r_bumper_held) {
                _max_x_speed = main_dude_max_x_speed_walking;
                _pos_update_delta = main_dude_pos_update_delta_walking_running;
            }
        }


    } else {
        crawling = false;
        if (!global::input_handler->r_bumper_held) {
            _max_x_speed = main_dude_max_x_speed_walking;
            _pos_update_delta = main_dude_pos_update_delta_walking_running;
        }
    }

    update_sprites_position();

}

void MainDude::boost_going_through_map_holes(MapTile **const t) {
    if (!_bottom_collision) {
        if ((t[TileOrientation::RIGHT_MIDDLE] == nullptr || !t[TileOrientation::RIGHT_MIDDLE]->collidable) &&
            (t[TileOrientation::RIGHT_UP] != nullptr && t[TileOrientation::RIGHT_DOWN] != nullptr)) {
            //if there's no collidable tile on right-mid, but there are on right-up and right-down,
            //add extra x-pos to ease going through a hole
            if (_x_speed > 0)
                _x += 2;
        }

        if ((t[TileOrientation::LEFT_MIDDLE] == nullptr || !t[TileOrientation::LEFT_MIDDLE]->collidable) &&
            (t[TileOrientation::LEFT_UP] != nullptr && t[TileOrientation::LEFT_DOWN] != nullptr)) {
            //same but for left side
            if (_x_speed < 0)
                _x -= 2;
        }
    }
}

void MainDude::update_creature_specific() {
    match_animation();
    reset_values_checked_every_frame();
    apply_blinking_on_damage();

    // update max speed
    if (crawling) {
        _max_x_speed = MAIN_DUDE_MAX_X_SPEED_CRAWLING;
        _max_y_speed = MAIN_DUDE_MAX_Y_SPEED;
    } else if (global::input_handler->r_bumper_held) {
        // running fast
        _max_x_speed = main_dude_max_x_speed_running;
        _max_y_speed = MAIN_DUDE_MAX_Y_SPEED;
    } else if (using_cape) {
        // falling, but with cape
        _max_x_speed = MAIN_DUDE_MAX_X_SPEED;
        _max_y_speed = MAIN_DUDE_MAX_Y_SPEED_USING_CAPE;
    } else {
        // default values
        _max_x_speed = MAIN_DUDE_MAX_X_SPEED;
        _max_y_speed = MAIN_DUDE_MAX_Y_SPEED;
    }

    if (crawling) {
        _pos_update_delta = main_dude_pos_update_delta_crawling;
        _max_x_speed = main_dude_max_x_crawling;
    } else {
        if (!global::input_handler->r_bumper_held)
            _pos_update_delta = main_dude_pos_update_delta_walking_running;
    }
    // "Update timers"


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

    if ((_left_collision || _right_collision) && !crawling && !hanging_on_tile_left && !hanging_on_tile_right &&
        (global::input_handler->left_key_held || global::input_handler->right_key_held)) {
        pushing_timer += *global::timer;
        if (pushing_timer > MAIN_DUDE_PUSHING_TIME) {
            if (_left_collision) {
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


    if (!_bottom_collision && !hanging_on_tile_left && !hanging_on_tile_right && !climbing)
        jumping_timer += *global::timer;

    if (_bottom_collision && jumping_timer > MAIN_DUDE_STUN_FALLING_TIME) {


        if (global::hud->hearts > 0) {
            global::hud->hearts--;
            global::hud->draw_level_hud();
        }

        stunned = true;

        auto *f_left = new FallPoof(_x, _y);
        auto *f_right = new FallPoof(_x, _y);

        f_left->_x = _x - 4;
        f_right->_x = _x + MainDude::main_dude_physical_width - 6;

        f_left->_y = _y + 8;
        f_right->_y = _y + 8;

        global::creatures.push_back(f_left);
        global::creatures.push_back(f_right);

        if (global::hud->hearts == 0) {
            global::main_dude->set_dead();
        }

        mmEffect(SFX_XLAND);

        jumping_timer = 0;
    } else if (_bottom_collision && jumping_timer < MAIN_DUDE_STUN_FALLING_TIME) {
        jumping_timer = 0;
    }

    if (stunned)
        stunned_timer += *global::timer;
    if (stunned_timer > MAIN_DUDE_STUN_TIME) {
        stunned = false;
        stunned_timer = 0;
    }


    if (_x_speed != 0 || stunned || using_whip || (pushing_left || pushing_right) || (climbing && _y_speed != 0) ||
        exiting_level)
        animation_frame_timer += *global::timer;


    if (!_bottom_collision) {
        crawling = false;
        if (!global::input_handler->r_bumper_held) {
            _max_x_speed = main_dude_max_x_speed_walking;
            _pos_update_delta = main_dude_pos_update_delta_walking_running;
        }
    }

    time_since_last_jump += *global::timer;
    time_since_last_damage += *global::timer;

    // Map collisions

    if (_map_collisions_checked) {
        can_hang_on_tile(_neighboring_tiles);
        boost_going_through_map_holes(_neighboring_tiles);
        _map_collisions_checked = false;
    }

}

void MainDude::init_sprites() {

    delete_sprites();

    main_sprite_info = global::main_oam_manager->initSprite(gfx_spelunkerPal, gfx_spelunkerPalLen, nullptr,
                                                            _sprite_size, 16, _spritesheet_type, true, false,
                                                            LAYER_LEVEL::MIDDLE_TOP);


    sub_sprite_info = global::sub_oam_manager->initSprite(gfx_spelunkerPal, gfx_spelunkerPalLen, nullptr,
                                                          _sprite_size, 16, _spritesheet_type, true, false,
                                                          LAYER_LEVEL::MIDDLE_TOP);

    sprite_utils::set_vertical_flip(false, main_sprite_info, sub_sprite_info);
    sprite_utils::set_horizontal_flip(false, main_sprite_info, sub_sprite_info);

    update_sprites_position();
    match_animation();
}


void MainDude::reset_values_checked_every_frame() {
    can_climb_rope = false;
    can_climb_ladder = false;
}

