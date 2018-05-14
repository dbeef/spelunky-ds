//
// Created by xdbeef on 24.02.18.
//

#include <nds.h>
#include <nds/arm9/sprite.h>
#include <iostream>
#include <maxmod9.h>
#include <nds/arm9/console.h>
#include <algorithm>
#include "main_dude.h"
#include "../../globals_declarations.h"
#include "../../collisions/collisions.h"
#include "../../tiles/map_utils.h"
#include "../../../build/gfx_spelunker.h"
#include "../../../build/gfx_whip.h"
#include "../items/rope.h"

#include "../../../build/soundbank_bin.h"
#include "../../../build/soundbank.h"
#include "../animations/blood.h"
#include "../../game_loop.h"
#include "../../tiles/tile_orientation.h"
#include "../../tiles/splash_screen_type.h"

void MainDude::handle_key_input() {

    if (!stunned && !exiting_level && !dead) {
        if (global::input_handler->r_bumper_down && time_since_last_jump > 100) {
            if (bottomCollision || climbing) {
                ySpeed = -MAIN_DUDE_JUMP_SPEED;
                climbing = false;
                started_climbing_rope = false;
                started_climbing_ladder = false;
                can_climb_rope = false;
                time_since_last_jump = 0;

                mmEffect(SFX_XJUMP);

            }
            if ((hanging_on_tile_left || hanging_on_tile_right) && hanging_timer > MIN_HANGING_TIME &&
                time_since_last_jump > 100) {
                mmEffect(SFX_XJUMP);

                ySpeed = -MAIN_DUDE_JUMP_SPEED;
                hanging_on_tile_left = false;
                hanging_on_tile_right = false;
                hanging_timer = 0;
                time_since_last_jump = 0;
            }
        }
        if (global::input_handler->l_bumper_down) {
            if (!stunned && !using_whip) {
                if (holding_item) {

                    //throw holding item
                    //if holding bomb, arm it only

                    for (int a = 0; a < global::sprites.size(); a++) {
                        if (global::sprites.at(a)) {
                            if ((*global::sprites.at(a)).hold_by_main_dude) {

                                if ((*global::sprites.at(a)).activated_by_main_dude) {

                                    if (!global::input_handler->down_key_held) {

                                        if (state == 1)
                                            (*global::sprites.at(a)).xSpeed = -4 - abs(xSpeed);
                                        else
                                            (*global::sprites.at(a)).xSpeed = 4 + abs(xSpeed);

                                    } else {

                                        if (state == 1)
                                            (*global::sprites.at(a)).xSpeed = -0.04;
                                        else
                                            (*global::sprites.at(a)).xSpeed = 0.04;

                                    }

                                    if (global::input_handler->up_key_held)
                                        (*global::sprites.at(a)).ySpeed = -3 - abs(ySpeed);
                                    else
                                        (*global::sprites.at(a)).ySpeed = -1;


                                    (*global::sprites.at(a)).hold_by_main_dude = false;
                                    holding_item = false;

                                    mmEffect(SFX_XTHROW);

                                } else {
                                    (*global::sprites.at(a)).activated_by_main_dude = true;
                                }

                            }
                        }
                    }


                } else {

                    mmEffect(SFX_XWHIP);

                    using_whip = true;
                    animFrame = 0;
                }
            }
        }
        if (global::input_handler->x_key_down && !holding_item && global::hud->bombs > 0) {
            //make new bomb object


            global::hud->bombs--;
            global::hud->draw();

            Bomb *bomb = new Bomb();
            bomb->init();
            bomb->hold_by_main_dude = true;

            global::sprites.push_back(bomb);
            holding_item = true;
        } else if (global::input_handler->a_key_down && !holding_item && global::hud->ropes > 0) {
            //make new bomb object
            global::hud->ropes--;
            global::hud->draw();

            Rope *rope = new Rope();
            rope->init();
            rope->hold_by_main_dude = true;

            global::sprites.push_back(rope);
            holding_item = true;
        }

//        if (global::InputHandler->y_key_down && !holding_item && global::InputHandler->down_key_held) {
//            take item from the ground
//            holding_item = true;
//        }


        if (global::input_handler->left_key_held) {
            state = W_LEFT;
            hanging_on_tile_left = false;
            if (xSpeed > -MAIN_DUDE_MAX_X_SPEED && !(hanging_on_tile_right || hanging_on_tile_left) && !climbing)
                if (speed_inc_timer > X_SPEED_DELTA_TIME_MS) {
                    xSpeed -= X_SPEED_DELTA_VALUE;
                    speed_inc_timer = 0;
                }
        }
        if (global::input_handler->right_key_held) {
            state = W_RIGHT;
            hanging_on_tile_right = false;
            if (xSpeed < MAIN_DUDE_MAX_X_SPEED && !(hanging_on_tile_right || hanging_on_tile_left) && !climbing) {
                if (speed_inc_timer > X_SPEED_DELTA_TIME_MS) {
                    xSpeed += X_SPEED_DELTA_VALUE;
                    speed_inc_timer = 0;
                }
            }
        }


        if (global::input_handler->up_key_held) {

//            std::cout<< *global::timer << '\n';

            int xx = floor_div(this->x + 0.5 * MAIN_DUDE_PHYSICAL_WIDTH, 16);
            int yy = floor_div(this->y + 0.5 * MAIN_DUDE_PHYSICAL_HEIGHT, 16);

//            std::cout<< *global::timer << '\n';

            MapTile *neighboringTiles[9] = {};
            Collisions::getNeighboringTiles(global::level_generator->map_tiles, xx, yy, neighboringTiles);


            can_climb_ladder = neighboringTiles[CENTER] != nullptr &&
                               (neighboringTiles[CENTER]->mapTileType == MapTileType::LADDER ||
                                neighboringTiles[CENTER]->mapTileType == MapTileType::LADDER_DECK);

            exiting_level = neighboringTiles[CENTER] != nullptr &&
                            (neighboringTiles[CENTER]->mapTileType == MapTileType::EXIT);

            if (exiting_level) {

                mmEffect(SFX_XSTEPS);
                mmEffectCancel(global::cave_music_handler);

                x = neighboringTiles[CENTER]->x * 16;
                y = neighboringTiles[CENTER]->y * 16;

                animFrame = 0;
                animation_frame_timer = 0;
                xSpeed = 0;
                ySpeed = 0;

            }

            on_top_of_climbing_space = on_top_of_climbing_space ||
                                       neighboringTiles[UP_MIDDLE] != nullptr &&
                                       neighboringTiles[UP_MIDDLE]->collidable;

            if (can_climb_ladder) {
                x = neighboringTiles[CENTER]->x * 16;
            }

            if (can_climb_rope || can_climb_ladder/*&& !climbing*/) {
//                std::cout<<"CLIMBING" << '\n'
                climbing = true;
                jumping_timer = 0;
                xSpeed = 0;
                ySpeed = -1;

                if(can_climb_rope)
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

            int xx = floor_div(this->x + 0.5 * MAIN_DUDE_PHYSICAL_WIDTH, 16);
            int yy = floor_div(this->y + 0.5 * MAIN_DUDE_PHYSICAL_HEIGHT, 16);

            MapTile *neighboringTiles[9] = {};

            Collisions::getNeighboringTiles(global::level_generator->map_tiles, xx, yy, neighboringTiles);
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
    friction_timer += *global::timer;
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
        if (pushing_timer > PUSHING_TIME) {
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

    if (animFrame >= FRAMES_PER_ANIMATION && !crawling && !pushing_left && !pushing_right && !exiting_level)
        animFrame = 0;

    if (animFrame >= 9 && crawling)
        animFrame = 0;

    if ((pushing_left || pushing_right) && animFrame >= 7)
        animFrame = 0;


    if (!bottomCollision && !hanging_on_tile_left && !hanging_on_tile_right && !climbing)
        jumping_timer += *global::timer;

    if (bottomCollision && jumping_timer > STUN_FALLING_TIME) {
        if (global::hud->hearts > 0) {
            global::hud->hearts--;
            global::hud->draw();
        }
        stunned = true;


        if (global::hud->hearts == 0) {
            global::hud->hide();
            global::main_dude->ySpeed = -MAIN_DUDE_JUMP_SPEED * 0.25;
            global::main_dude->dead = true;
        }

        mmEffect(SFX_XLAND);

        jumping_timer = 0;
    } else if (bottomCollision && jumping_timer < STUN_FALLING_TIME) {
        jumping_timer = 0;
    }

    if (stunned)
        stunned_timer += *global::timer;
    if (stunned_timer > STUN_TIME) {
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
    else
        limit_speed(MAIN_DUDE_MAX_X_SPEED, MAIN_DUDE_MAX_Y_SPEED);

    apply_friction(FRICTION_DELTA_TIME_MS, FRICTION_DELTA_SPEED);

    bool change_pos = (crawling && pos_inc_timer > 20) || (!crawling && pos_inc_timer > 1);


    if (change_pos) {
        updatePosition();
        pos_inc_timer = 0;

        if (!bottomCollision && !(hanging_on_tile_left || hanging_on_tile_right) && !climbing)
            ySpeed += GRAVITY_DELTA_SPEED;
    }


}


void MainDude::updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) {

    MapTile *tiles[9];
    Collisions::getNeighboringTiles(global::level_generator->map_tiles, x_current_pos_in_tiles, y_current_pos_in_tiles,
                                    tiles);

    bottomCollision = Collisions::checkBottomCollision(tiles, &this->x, &this->y, &ySpeed, 16, 16, dead);
    leftCollision = Collisions::checkLeftCollision(tiles, &this->x, &this->y, &xSpeed, 16, 16, dead);
    rightCollision = Collisions::checkRightCollision(tiles, &this->x, &this->y, &xSpeed, 16, 16, dead);
    upperCollision = Collisions::checkUpperCollision(tiles, &this->x, &this->y, &ySpeed, 16, dead);

    can_hang_on_tile(tiles);

    if (upperCollision || bottomCollision) {
        hanging_on_tile_left = false;
        hanging_on_tile_right = false;
    }


    if (!bottomCollision) {
        if (((tiles[1] == nullptr || !tiles[1]->collidable) && (tiles[6] != nullptr && tiles[8] != nullptr))) {
            if (xSpeed > 0)
                x += 2;
        }
        if (((tiles[0] == nullptr || !tiles[0]->collidable) && (tiles[5] != nullptr && tiles[7] != nullptr))) {
            if (xSpeed < 0)
                x -= 2;
        }
    }

}

void MainDude::init() {

    initSprite();
    frameGfx = (u8 *) gfx_spelunkerTiles;
    time_since_last_damage = DAMAGE_PROTECTION_TIME + 1;

    whip = new Whip();
    whip->init();
    global::sprites.push_back(whip);
}

void MainDude::draw() {

    //todo split this function into utils
    //todo use moving_object->get_x_y in every moving_object
    //todo use moving_object->update_position in every moving_object
    //todo make class static_object for copyrights sign, whip and other non-moving things
    //todo make file input/output class for savegames

    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);

    main_spelunker->entry->x = main_x;
    main_spelunker->entry->y = main_y;

    sub_spelunker->entry->x = sub_x;
    sub_spelunker->entry->y = sub_y;

    if (exiting_level || (dead && global::input_handler->l_bumper_down)) {

        if ((dead && global::input_handler->l_bumper_down) || (animFrame >= 16 && !global::splash_screen)) {
            animFrame = 0;

            dmaCopyHalfWords(DMA_CHANNEL, global::base_map, global::current_map, sizeof(global::current_map));
            global::level_generator->newLayout(*global::timer);

            if (global::in_main_menu || global::levels_transition_screen) {

                mmEffectCancel(global::menu_music_handler);
                mmEffectCancel(global::cave_music_handler);
                global::cave_music_handler = mmEffect(SFX_MCAVE);

                global::level_generator->generate_frame();
                global::level_generator->generate_rooms();
            } else {
                if (global::scores_screen) {

                    mmEffectCancel(global::cave_music_handler);
                    global::menu_music_handler = mmEffect(SFX_MTITLE);
                    global::level_generator->generate_splash_screen(SplashScreenType::MAIN_MENU_UPPER);
                    global::level_generator->generate_splash_screen(SplashScreenType::MAIN_MENU_LOWER);
                } else if (dead) {
                    global::level_generator->generate_splash_screen(SplashScreenType::SCORES_UPPER);
                    global::level_generator->generate_splash_screen(SplashScreenType::SCORES_LOWER);
                } else {
                    global::level_generator->generate_splash_screen(SplashScreenType::ON_LEVEL_DONE_UPPER);
                    global::level_generator->generate_splash_screen(SplashScreenType::ON_LEVEL_DONE_LOWER);
                }
            }

            global::level_generator->tiles_to_map();
            sectorize_map();
            dmaCopyHalfWords(DMA_CHANNEL, global::current_map, bgGetMapPtr(global::bg_main_address),
                             sizeof(global::current_map));
            dmaCopyHalfWords(DMA_CHANNEL, global::current_map, bgGetMapPtr(global::bg_sub_address),
                             sizeof(global::current_map));
            global::main_dude->bottomCollision = false;

            MapTile *entrance;
            global::level_generator->get_first_tile(MapTileType::ENTRANCE, entrance);

            if (entrance == nullptr) {
                global::main_dude->x = 32;
                global::main_dude->y = 280;
            } else {
                global::main_dude->x = entrance->x * 16;
                global::main_dude->y = entrance->y * 16;
            }

            //
            //possible memory leak?
            global::main_oam_manager->clearAllSprites();
            global::sub_oam_manager->clearAllSprites();

            for (int a = 0; a < global::sprites.size(); a++) {
                delete (global::sprites.at(a));
                global::sprites.erase(global::sprites.begin() + a);
            }

            global::sprites.clear();
            //

            init();
            global::sprites.push_back(global::main_dude);
            consoleClear();

            global::camera->follow_main_dude = true;
            global::camera->instant_focus();

            if (global::in_main_menu || global::levels_transition_screen) {

                if (global::in_main_menu) {
                    global::hud->hearts = 4;
                    global::hud->ropes = 4;
                    global::hud->bombs = 4;
                    global::hud->dollars = 0;
                }

                global::hud->init();
                gameloop::populate_cave_npcs();
                gameloop::populate_cave_moniez();
                global::levels_transition_screen = false;
                global::in_main_menu = false;
                global::killed_npcs.clear();
                global::collected_loot.clear();
                global::hud->money_on_this_level = 0;

                this->holding_item = false;


            } else {
                if (global::scores_screen) {
                    global::camera->follow_main_dude = false;
                    global::in_main_menu = true;
                    global::levels_transition_screen = false;
                    global::scores_screen = false;
                    gameloop::populate_main_menu();
                } else if (dead) {
                    dead = false;
                    global::scores_screen = true;
                    global::hud->hide();
                    global::hud->draw_scores();
                    global::camera->follow_main_dude = false;

                } else {
                    global::hud->total_time_spent += global::hud->time_spent_on_level;
                    global::hud->level++;

                    global::levels_transition_screen = true;

                    global::splash_screen = true;

                    global::input_handler->stop_handling = true;
                    global::input_handler->right_key_held = true;
                    global::input_handler->up_key_held = true;
                    global::camera->follow_main_dude = false;
                    global::hud->draw_on_level_done();
                }
            }

            exiting_level = false;
            mmEffectCancel(SFX_MCAVE);

        } else if (animFrame >= 16 && global::splash_screen) {

            main_spelunker->entry->isHidden = true;
            sub_spelunker->entry->isHidden = true;
            global::input_handler->stop_handling = false;

            if (global::input_handler->l_bumper_down || global::input_handler->l_bumper_down) {
                global::splash_screen = false;
            }
        }

        apply_exiting_level_sprite();
    } else if (using_whip) {
        apply_whip_sprite();
    } else if (dead) {
        apply_dead_sprite();
    } else if (climbing) {
        apply_climbing_sprite();
    } else if (stunned) {
        apply_stunned_sprite();
    } else if (pushing_left || pushing_right) {
        apply_pushing_sprite();
    } else if (hanging_on_tile_right || hanging_on_tile_left) {
        apply_hanging_on_tile_sprite();
    } else if (crawling) {
        apply_crawling_sprite();
    } else if (!bottomCollision) {
        apply_walking_sprite();
    } else {
        apply_falling_sprite();
    }

    reset_values_checked_every_frame();
    apply_blinking_on_damage();
}

void MainDude::can_hang_on_tile(MapTile **neighboringTiles) {

    if (bottomCollision || (!leftCollision && !rightCollision))
        return;

    if ((neighboringTiles[UP_MIDDLE] != 0 && neighboringTiles[UP_MIDDLE]->collidable) ||
        (neighboringTiles[DOWN_MIDDLE] != 0 && neighboringTiles[DOWN_MIDDLE]->collidable))
        return;

    bool y_bound = false;
    bool x_bound = false;

    if (rightCollision && state == W_LEFT) {

        if (neighboringTiles[LEFT_UP] != 0 && neighboringTiles[LEFT_UP]->collidable)
            return;

        x_bound = (this->x <= (neighboringTiles[LEFT_MIDDLE]->x * 16) + 16 &&
                   (this->x >= (neighboringTiles[LEFT_MIDDLE]->x * 16) + 12));
        y_bound = (this->y > (neighboringTiles[LEFT_MIDDLE]->y * 16) - 2) &&
                  (this->y < (neighboringTiles[LEFT_MIDDLE]->y * 16) + 8);
    } else if (leftCollision && state == W_RIGHT) {

        if (neighboringTiles[RIGHT_UP] != 0 && neighboringTiles[RIGHT_UP]->collidable)
            return;

        y_bound = (this->y > (neighboringTiles[RIGHT_MIDDLE]->y * 16) - 2) &&
                  (this->y < (neighboringTiles[RIGHT_MIDDLE]->y * 16) + 8);
        x_bound = (this->x <= (neighboringTiles[RIGHT_MIDDLE]->x * 16) - 12 &&
                   (this->x >= (neighboringTiles[RIGHT_MIDDLE]->x * 16) - 16));
    }

    if ((y_bound && x_bound) && hanging_timer > MIN_HANGING_TIME) {

//        fprintf(stdout, "HANGING" + '\n');

        if (rightCollision && neighboringTiles[LEFT_MIDDLE]->collidable) {
            this->y = (neighboringTiles[LEFT_MIDDLE]->y * 16);
            hanging_on_tile_right = true;
            jumping_timer = 0;
            hanging_timer = 0;
            ySpeed = 0;
        }
        if (leftCollision && neighboringTiles[RIGHT_MIDDLE]->collidable) {
            jumping_timer = 0;
            hanging_on_tile_left = true;
            this->y = (neighboringTiles[RIGHT_MIDDLE]->y * 16);
            hanging_timer = 0;
            ySpeed = 0;
        }
    }

}

void MainDude::updateOther() {
}


void MainDude::initSprite() {

    main_spelunker = global::main_oam_manager->initSprite(gfx_spelunkerPal, gfx_spelunkerPalLen, nullptr,
                                                          16 * 16, 16, MAIN_DUDE, true, false);


    sub_spelunker = global::sub_oam_manager->initSprite(gfx_spelunkerPal, gfx_spelunkerPalLen, nullptr,
                                                        16 * 16, 16, MAIN_DUDE, true, false);


    int main_x = x - global::camera->x;
    int main_y = y - global::camera->y;
    int sub_x = x - global::camera->x;
    int sub_y = y - global::camera->y - 192;

    main_spelunker->entry->x = main_x;
    main_spelunker->entry->y = main_y;

    sub_spelunker->entry->x = sub_x;
    sub_spelunker->entry->y = sub_y;

}

void MainDude::apply_crawling_sprite() {

    int frame;
    u8 *offset;

    if (state == SpriteState::W_LEFT)
        frame = animFrame + (4) * SPRITESHEET_ROW_WIDTH;
    else
        frame = animFrame + (5) * SPRITESHEET_ROW_WIDTH + 3;

    offset = frameGfx + frame * MAIN_DUDE_SPRITE_WIDTH * MAIN_DUDE_SPRITE_HEIGHT / 2;
    main_spelunker->updateFrame(offset, 16 * 16);
    sub_spelunker->updateFrame(offset, 16 * 16);
}

void MainDude::apply_hanging_on_tile_sprite() {

    int frame;
    u8 *offset;

    if (hanging_on_tile_right) {
        frame = (2 * SPRITESHEET_ROW_WIDTH) + 1;
        offset = frameGfx + frame * MAIN_DUDE_SPRITE_WIDTH * MAIN_DUDE_SPRITE_HEIGHT / 2;
    } else if (hanging_on_tile_left) {
        frame = (2 * SPRITESHEET_ROW_WIDTH);
        offset = frameGfx + frame * MAIN_DUDE_SPRITE_WIDTH * MAIN_DUDE_SPRITE_HEIGHT / 2;
    }

    main_spelunker->updateFrame(offset, 16 * 16);
    sub_spelunker->updateFrame(offset, 16 * 16);
}

void MainDude::apply_whip_sprite() {

    int frame;
    u8 *offset;

    if (state == SpriteState::W_LEFT) {
        frame = (9 * SPRITESHEET_ROW_WIDTH) + 2 + animFrame;
        offset = frameGfx + frame * MAIN_DUDE_SPRITE_WIDTH * MAIN_DUDE_SPRITE_HEIGHT / 2;
    } else if (state == 0) {
        frame = (10 * SPRITESHEET_ROW_WIDTH) + 2 + animFrame;
        offset = frameGfx + frame * MAIN_DUDE_SPRITE_WIDTH * MAIN_DUDE_SPRITE_HEIGHT / 2;
    }

    main_spelunker->updateFrame(offset, 16 * 16);
    sub_spelunker->updateFrame(offset, 16 * 16);

}

void MainDude::apply_pushing_sprite() {

    int frame;
    u8 *offset;

    if (pushing_left)
        frame = animFrame + (7) * SPRITESHEET_ROW_WIDTH;
    else
        frame = animFrame + (8) * SPRITESHEET_ROW_WIDTH + 1;


    offset = frameGfx + frame * MAIN_DUDE_SPRITE_WIDTH * MAIN_DUDE_SPRITE_HEIGHT / 2;
    main_spelunker->updateFrame(offset, 16 * 16);
    sub_spelunker->updateFrame(offset, 16 * 16);

}

void MainDude::apply_stunned_sprite() {

    int frame;
    u8 *offset;

    if (animFrame > 4)
        animFrame = 0;

    frame = (3 * SPRITESHEET_ROW_WIDTH) + animFrame;
    offset = frameGfx + frame * MAIN_DUDE_SPRITE_WIDTH * MAIN_DUDE_SPRITE_HEIGHT / 2;
    main_spelunker->updateFrame(offset, 16 * 16);
    sub_spelunker->updateFrame(offset, 16 * 16);
}

void MainDude::apply_climbing_sprite() {

    int frame;
    u8 *offset;

    if (started_climbing_rope) {
        if (animFrame >= 12)
            animFrame = 0;

        frame = ((12) * SPRITESHEET_ROW_WIDTH) + animFrame + 2;
        offset = frameGfx + frame * MAIN_DUDE_SPRITE_WIDTH * MAIN_DUDE_SPRITE_HEIGHT / 2;
        main_spelunker->updateFrame(offset, 16 * 16);
        sub_spelunker->updateFrame(offset, 16 * 16);

    } else if (started_climbing_ladder) {

        if (animFrame >= 6)
            animFrame = 0;

        frame = ((16) * SPRITESHEET_ROW_WIDTH) + animFrame;
        offset = frameGfx + frame * MAIN_DUDE_SPRITE_WIDTH * MAIN_DUDE_SPRITE_HEIGHT / 2;
        main_spelunker->updateFrame(offset, 16 * 16);
        sub_spelunker->updateFrame(offset, 16 * 16);
    }


}

void MainDude::apply_dead_sprite() {

    int frame;
    u8 *offset;

    if (bottomCollision) {
        frame = ((2) * SPRITESHEET_ROW_WIDTH) + 5;
    } else
        frame = ((2) * SPRITESHEET_ROW_WIDTH) + 4;

    offset = frameGfx + frame * MAIN_DUDE_SPRITE_WIDTH * MAIN_DUDE_SPRITE_HEIGHT / 2;
    main_spelunker->updateFrame(offset, 16 * 16);
    sub_spelunker->updateFrame(offset, 16 * 16);
}

void MainDude::apply_walking_sprite() {

    int frame;
    u8 *offset;

    if (state == SpriteState::W_LEFT) {
        frame = state * FRAMES_PER_ANIMATION;
        offset = frameGfx + frame * MAIN_DUDE_SPRITE_WIDTH * MAIN_DUDE_SPRITE_HEIGHT / 2;
    } else if (state == SpriteState::W_RIGHT) {
        frame = state * FRAMES_PER_ANIMATION;
        offset = frameGfx + frame * MAIN_DUDE_SPRITE_WIDTH * MAIN_DUDE_SPRITE_HEIGHT / 2;
    }
    main_spelunker->updateFrame(offset, 16 * 16);
    sub_spelunker->updateFrame(offset, 16 * 16);

}

void MainDude::apply_falling_sprite() {

    int frame;
    u8 *offset;

    if (abs(xSpeed) != 0) {
        frame = animFrame + state * FRAMES_PER_ANIMATION;
        offset = frameGfx + frame * MAIN_DUDE_SPRITE_WIDTH * MAIN_DUDE_SPRITE_HEIGHT / 2;
    } else {
        if (state == SpriteState::W_LEFT) {
            frame = (2 * SPRITESHEET_ROW_WIDTH) + 2;
            offset = frameGfx + frame * MAIN_DUDE_SPRITE_WIDTH * MAIN_DUDE_SPRITE_HEIGHT / 2;
        } else if (state == SpriteState::W_RIGHT) {
            frame = (2 * SPRITESHEET_ROW_WIDTH) + 3;
            offset = frameGfx + frame * MAIN_DUDE_SPRITE_WIDTH * MAIN_DUDE_SPRITE_HEIGHT / 2;
        }

    }
    main_spelunker->updateFrame(offset, 16 * 16);
    sub_spelunker->updateFrame(offset, 16 * 16);
}

void MainDude::apply_exiting_level_sprite() {

    int frame;
    u8 *offset;

    frame = ((13) * SPRITESHEET_ROW_WIDTH) + animFrame + 2;
    offset = frameGfx + frame * MAIN_DUDE_SPRITE_WIDTH * MAIN_DUDE_SPRITE_HEIGHT / 2;
    main_spelunker->updateFrame(offset, 16 * 16);
    sub_spelunker->updateFrame(offset, 16 * 16);

}

void MainDude::apply_blinking_on_damage() {

    if (!global::levels_transition_screen) {
        if (time_since_last_damage < DAMAGE_PROTECTION_TIME) {
            if (time_since_last_damage % 100 < 50) {
                main_spelunker->entry->isHidden = true;
                sub_spelunker->entry->isHidden = true;
            } else {
                main_spelunker->entry->isHidden = false;
                sub_spelunker->entry->isHidden = false;
            }
        } else {
            main_spelunker->entry->isHidden = false;
            sub_spelunker->entry->isHidden = false;
        }
    }
}

void MainDude::reset_values_checked_every_frame() {
    can_climb_rope = false;
    can_climb_ladder = false;
}

MainDude::MainDude() {
    exiting_level = false;
    holding_item = false;
    dead = false;
    physical_height = MAIN_DUDE_PHYSICAL_HEIGHT;
    physical_width = MAIN_DUDE_PHYSICAL_WIDTH;
    sprite_height = MAIN_DUDE_SPRITE_HEIGHT;
    sprite_width = MAIN_DUDE_SPRITE_WIDTH;

}
