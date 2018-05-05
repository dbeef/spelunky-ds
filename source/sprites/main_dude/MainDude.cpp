//
// Created by xdbeef on 24.02.18.
//

#include <nds.h>
#include <nds/arm9/sprite.h>
#include <iostream>
#include <maxmod9.h>
#include <nds/arm9/console.h>
#include <algorithm>
#include "MainDude.h"
#include "../../Globals.h"
#include "../../collisions/Collisions.h"
#include "../../tiles/MapUtils.h"
#include "../../../build/spelunker.h"
#include "../../../build/whip.h"
#include "../items/Rope.h"

#include "../../../build/soundbank_bin.h"
#include "../../../build/soundbank.h"
#include "../animations/Blood.h"
#include "../..//GameLoop.h"
#include "../../tiles/TileOrientation.h"
#include "../../tiles/SplashScreenType.h"

void MainDude::handleKeyInput() {

    if (!stunned && !exitingLevel && !dead) {
        if (global::input_handler->r_bumper_down && timeSinceLastJump > 100) {
            if (bottomCollision || climbing) {
                ySpeed = -MAIN_DUDE_JUMP_SPEED;
                climbing = false;
                canClimbRope = false;
                timeSinceLastJump = 0;

                mmEffect(SFX_XJUMP);

            }
            if ((hangingOnTileLeft || hangingOnTileRight) && hangingTimer > MIN_HANGING_TIME &&
                timeSinceLastJump > 100) {
                mmEffect(SFX_XJUMP);

                ySpeed = -MAIN_DUDE_JUMP_SPEED;
                hangingOnTileLeft = false;
                hangingOnTileRight = false;
                hangingTimer = 0;
                timeSinceLastJump = 0;
            }
        }
        if (global::input_handler->l_bumper_down) {
            if (!stunned && !whip) {
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

                    whip = true;
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
            bomb->timer = timer;
            bomb->hold_by_main_dude = true;

            global::sprites.push_back(bomb);
            holding_item = true;
        } else if (global::input_handler->a_key_down && !holding_item && global::hud->ropes > 0) {
            //make new bomb object
            global::hud->ropes--;
            global::hud->draw();

            Rope *rope = new Rope();
            rope->init();
            rope->timer = timer;
            rope->hold_by_main_dude = true;

            global::sprites.push_back(rope);
            holding_item = true;
        }

//        if (global::input_handler->y_key_down && !holding_item && global::input_handler->down_key_held) {
//            take item from the ground
//            holding_item = true;
//        }


        if (global::input_handler->left_key_held) {
            state = W_LEFT;
            hangingOnTileLeft = false;
            if (xSpeed > -MAIN_DUDE_MAX_X_SPEED && !(hangingOnTileRight || hangingOnTileLeft) && !climbing)
                if (speedIncTimer > X_SPEED_DELTA_TIME_MS) {
                    xSpeed -= X_SPEED_DELTA_VALUE;
                    speedIncTimer = 0;
                }
        }
        if (global::input_handler->right_key_held) {
            state = W_RIGHT;
            hangingOnTileRight = false;
            if (xSpeed < MAIN_DUDE_MAX_X_SPEED && !(hangingOnTileRight || hangingOnTileLeft) && !climbing) {
                if (speedIncTimer > X_SPEED_DELTA_TIME_MS) {
                    xSpeed += X_SPEED_DELTA_VALUE;
                    speedIncTimer = 0;
                }
            }
        }


        if (global::input_handler->up_key_held) {


            int xx = floor_div(this->x + 0.5 * MAIN_DUDE_WIDTH, 16);
            int yy = floor_div(this->y + 0.5 * MAIN_DUDE_HEIGHT, 16);

//            int yy_a_bit_down = floor_div(this->y + 0.5 * MAIN_DUDE_HEIGHT - 4, 16);

            MapTile **neighboringTiles;
//            MapTile **neighboringTiles_a_bit_down;
            Collisions::getNeighboringTiles(global::level_generator->mapTiles, xx, yy, neighboringTiles);
//            Collisions::getNeighboringTiles(global::level_generator->mapTiles, xx, yy_a_bit_down, neighboringTiles_a_bit_down);

            canClimbLadder = neighboringTiles[CENTER] != 0 &&
                             (neighboringTiles[CENTER]->mapTileType == MapTileType::LADDER ||
                              neighboringTiles[CENTER]->mapTileType == MapTileType::LADDER_DECK);

            exitingLevel = neighboringTiles[CENTER] != 0 &&
                           (neighboringTiles[CENTER]->mapTileType == MapTileType::EXIT);

            if (exitingLevel) {

                mmEffect(SFX_XSTEPS);
                mmEffectCancel(global::cave_music_handler);

                x = neighboringTiles[CENTER]->x * 16;
                y = neighboringTiles[CENTER]->y * 16;

                animFrame = 0;
                animationFrameTimer = 0;
                xSpeed = 0;
                ySpeed = 0;
            }

            onTopOfClimbingSpace = onTopOfClimbingSpace ||
                                   neighboringTiles[UP_MIDDLE] != 0 &&
                                   neighboringTiles[UP_MIDDLE]->collidable;

            if (canClimbLadder) {
                x = neighboringTiles[CENTER]->x * 16;
            }

            if (canClimbRope || canClimbLadder/*&& !climbing*/) {
//                std::cout<<"CLIMBING" << '\n'
                climbing = true;
                jumpingTimer = 0;
                xSpeed = 0;
                ySpeed = -1;
            }

            if (!canClimbRope && climbing && onTopOfClimbingSpace && !canClimbLadder) {
                ySpeed = 0;
                jumpingTimer = 0;
                xSpeed = 0;
            }

        } else if (climbing) {
            ySpeed = 0;
        }

        if (global::input_handler->down_key_held) {


            int xx = floor_div(this->x + 0.5 * MAIN_DUDE_WIDTH, 16);
            int yy = floor_div(this->y + 0.5 * MAIN_DUDE_HEIGHT, 16);

            MapTile **neighboringTiles;
            Collisions::getNeighboringTiles(global::level_generator->mapTiles, xx, yy, neighboringTiles);
            canClimbLadder =
                    neighboringTiles[CENTER]->mapTileType == MapTileType::LADDER ||
                    MapTileType::LADDER_DECK;


            if (climbing) {
                canClimbLadder = neighboringTiles[CENTER] != 0 &&
                                 (neighboringTiles[CENTER]->mapTileType == MapTileType::LADDER ||
                                  neighboringTiles[CENTER]->mapTileType == MapTileType::LADDER_DECK) &&
                                 (neighboringTiles[DOWN_MIDDLE] == nullptr ||
                                  !neighboringTiles[DOWN_MIDDLE]->collidable);
            }

            if (climbing) {
                ySpeed = 1;
            }

            if ((!canClimbRope && climbing && !onTopOfClimbingSpace) || (!canClimbLadder && climbing)) {
//                ySpeed = 0;
                jumpingTimer = 0;
//                xSpeed = 0;
                climbing = false;
            }


            hangingOnTileLeft = false;
            hangingOnTileRight = false;
            if (bottomCollision)
                crawling = true;
        } else
            crawling = false;
    } else
        crawling = false;

}


void MainDude::updateTimers() {

    posIncTimer += *timer;
    frictionTimer += *timer;
    speedIncTimer += *timer;
    hangingTimer += *timer;

    if (whip) {
        whip_timer += *timer;
        if (whip_timer > 420 + 0 * 70) {
            whip_timer = 0;
            whip = false;
            sub_whip->entry->isHidden = true;
            main_whip->entry->isHidden = true;
            sub_pre_whip->entry->isHidden = true;
            main_pre_whip->entry->isHidden = true;
        }
    }

    if (animationFrameTimer > 65) {

        animationFrameTimer = 0;

        if (!whip || (whip && animFrame < 5) || (climbing && animFrame < 12) || (exitingLevel && animFrame < 16))
            animFrame++;

    }

    if (!global::input_handler->left_key_held && pushing_left) {
        pushing_left = false;
        pushingTimer = 0;
    }
    if (!global::input_handler->right_key_held && pushing_right) {
        pushing_right = false;
        pushingTimer = 0;
    }

    if ((leftCollision || rightCollision) && !crawling && !hangingOnTileLeft && !hangingOnTileRight &&
        (global::input_handler->left_key_held || global::input_handler->right_key_held)) {
        pushingTimer += *timer;
        if (pushingTimer > PUSHING_TIME) {
            if (leftCollision) {
                pushing_right = true;
                pushingTimer = 0;
            } else {
                pushing_left = true;
                pushingTimer = 0;
            }
        }

    } else {
        pushingTimer = 0;
        pushing_left = false;
        pushing_right = false;
    }

    if (animFrame >= FRAMES_PER_ANIMATION && !crawling && !pushing_left && !pushing_right && !exitingLevel)
        animFrame = 0;

    if (animFrame >= 9 && crawling)
        animFrame = 0;

    if ((pushing_left || pushing_right) && animFrame >= 7)
        animFrame = 0;


    if (!bottomCollision && !hangingOnTileLeft && !hangingOnTileRight && !climbing)
        jumpingTimer += *timer;

    if (bottomCollision && jumpingTimer > STUN_FALLING_TIME) {
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

        jumpingTimer = 0;
    } else if (bottomCollision && jumpingTimer < STUN_FALLING_TIME) {
        jumpingTimer = 0;
    }

    if (stunned)
        stunnedTimer += *timer;
    if (stunnedTimer > STUN_TIME) {
        stunned = false;
        stunnedTimer = 0;
    }


    if (xSpeed != 0 || stunned || whip || (pushing_left || pushing_right) || (climbing && ySpeed != 0) || exitingLevel)
        animationFrameTimer += *timer;


    if (!bottomCollision)
        crawling = false;

    timeSinceLastJump += *timer;
    time_since_last_damage += *timer;
}


void MainDude::updateSpeed() {

    if (crawling) {

        if (xSpeed > MAIN_DUDE_MAX_X_SPEED_CRAWLING)
            xSpeed = MAIN_DUDE_MAX_X_SPEED_CRAWLING;
        if (xSpeed < -MAIN_DUDE_MAX_X_SPEED_CRAWLING)
            xSpeed = -MAIN_DUDE_MAX_X_SPEED_CRAWLING;

    } else {
        if (xSpeed > MAIN_DUDE_MAX_X_SPEED)
            xSpeed = MAIN_DUDE_MAX_X_SPEED;
        if (xSpeed < -MAIN_DUDE_MAX_X_SPEED)
            xSpeed = -MAIN_DUDE_MAX_X_SPEED;
    }

    if (ySpeed > MAIN_DUDE_MAX_Y_SPEED)
        ySpeed = MAIN_DUDE_MAX_Y_SPEED;
    if (ySpeed < -MAIN_DUDE_MAX_Y_SPEED)
        ySpeed = -MAIN_DUDE_MAX_Y_SPEED;

    if (frictionTimer > FRICTION_DELTA_TIME_MS) {
        frictionTimer = 0;
        if (xSpeed > 0) {
            xSpeed -= FRICTION_DELTA_SPEED;
            if (xSpeed < 0)
                xSpeed = 0;
        }
        if (xSpeed < 0) {
            xSpeed += FRICTION_DELTA_SPEED;
            if (xSpeed > 0)
                xSpeed = 0;
        }
    }


    bool change_pos = (crawling && posIncTimer > 20) || (!crawling && posIncTimer > 1);


    if (change_pos) {
        updatePosition();
        posIncTimer = 0;

        if (!bottomCollision && !(hangingOnTileLeft || hangingOnTileRight) && !climbing)
            ySpeed += GRAVITY_DELTA_SPEED;
    }


}


void MainDude::updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) {

    MapTile *tiles[9];
    Collisions::getNeighboringTiles(global::level_generator->mapTiles, x_current_pos_in_tiles, y_current_pos_in_tiles,
                                    tiles);

    bottomCollision = Collisions::checkBottomCollision(tiles, &this->x, &this->y, &ySpeed, 16, 16, dead);
    leftCollision = Collisions::checkLeftCollision(tiles, &this->x, &this->y, &xSpeed, 16, 16, dead);
    rightCollision = Collisions::checkRightCollision(tiles, &this->x, &this->y, &xSpeed, 16, 16, dead);
    upperCollision = Collisions::checkUpperCollision(tiles, &this->x, &this->y, &ySpeed, 16, dead);

    canHangOnTile(tiles);

    if (upperCollision || bottomCollision) {
        hangingOnTileLeft = false;
        hangingOnTileRight = false;
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

    frameGfx = (u8 *) spelunkerTiles;
    sub_whip->entry->isHidden = true;
    main_whip->entry->isHidden = true;
    sub_pre_whip->entry->isHidden = true;
    main_pre_whip->entry->isHidden = true;
    time_since_last_damage = DAMAGE_PROTECTION_TIME + 1;
}

//todo split
void MainDude::draw() {

    int main_x = x - global::camera->x;
    int main_y = y - global::camera->y;
    int sub_x = x - global::camera->x;
    int sub_y = y - global::camera->y - 192;

    /*if (this->y <= 320 + 16 && this->y >= 320 - 16) {

    } else if (this->y > 320) {
        main_x = -16;
        main_y = -16;
    } else if (this->y < 320) {
        sub_x = -16;
        sub_y = -16;
    }

*/
    if (global::camera->y + 192 > this->y + 16 || global::camera->y + 192 + 192 < this->y - 16) {
        sub_x = -128;
        sub_y = -128;
    }
    if (global::camera->y > this->y + 16 || global::camera->y + 192 < this->y - 16) {
        main_x = -128;
        main_y = -128;
    }




    main_spelunker->entry->x = main_x;
    main_spelunker->entry->y = main_y;

    sub_spelunker->entry->x = sub_x;
    sub_spelunker->entry->y = sub_y;

    int frame;
    u8 *offset;

    if (whip) {

        sub_pre_whip->entry->y = sub_y - 2;
        main_pre_whip->entry->y = main_y - 2;
        sub_whip->entry->y = sub_y;
        main_whip->entry->y = main_y;

        if (whip_timer > 100 && whip_timer < 180) {
            main_pre_whip->entry->isHidden = false;
            sub_pre_whip->entry->isHidden = false;
        } else if (whip_timer >= 220) {
            main_pre_whip->entry->isHidden = true;
            sub_pre_whip->entry->isHidden = true;
            main_whip->entry->isHidden = false;
            sub_whip->entry->isHidden = false;
        }

        if (state == 1) {

            main_whip->entry->hFlip = true;
            sub_whip->entry->hFlip = true;
            main_pre_whip->entry->hFlip = true;
            sub_pre_whip->entry->hFlip = true;

            sub_pre_whip->entry->x = sub_x + 8;
            main_pre_whip->entry->x = main_x + 8;
            sub_whip->entry->x = sub_x - 12;
            main_whip->entry->x = main_x - 12;

        } else {

            main_whip->entry->hFlip = false;
            sub_whip->entry->hFlip = false;
            main_pre_whip->entry->hFlip = false;
            sub_pre_whip->entry->hFlip = false;

            sub_pre_whip->entry->x = sub_x - 8;
            main_pre_whip->entry->x = main_x - 8;
            main_whip->entry->x = main_x + 8;
            sub_whip->entry->x = sub_x + 8;
        }

    }


    if (exitingLevel || (dead && global::input_handler->l_bumper_down)) {

        if ((dead && global::input_handler->l_bumper_down) || (animFrame >= 16 && !global::splash_screen)) {
            animFrame = 0;

            dmaCopyHalfWords(DMA_CHANNEL, global::base_map, global::current_map, sizeof(global::current_map));
            global::level_generator->newLayout(*global::timer);

            if (global::in_main_menu || global::levels_transition_screen) {

                mmEffectCancel(global::menu_music_handler);
                mmEffectCancel(global::cave_music_handler);
                global::cave_music_handler = mmEffect(SFX_MCAVE);

                global::level_generator->generateFrame();
                global::level_generator->generateRooms();
            } else {
                if (global::scores_screen) {

                    mmEffectCancel(global::cave_music_handler);
                    global::menu_music_handler = mmEffect(SFX_MTITLE);
                    global::level_generator->generateSplashScreen(SplashScreenType::MAIN_MENU_UPPER);
                    global::level_generator->generateSplashScreen(SplashScreenType::MAIN_MENU_LOWER);
                } else if (dead) {

                    global::level_generator->generateSplashScreen(SplashScreenType::SCORES_UPPER);
                    global::level_generator->generateSplashScreen(SplashScreenType::SCORES_LOWER);
                } else {
                    global::level_generator->generateSplashScreen(SplashScreenType::ON_LEVEL_DONE_UPPER);
                    global::level_generator->generateSplashScreen(SplashScreenType::ON_LEVEL_DONE_LOWER);
                }
            }

            global::level_generator->tilesToMap();
            sectorize_map();
            dmaCopyHalfWords(DMA_CHANNEL, global::current_map, bgGetMapPtr(global::bg_main_address),
                             sizeof(global::current_map));
            dmaCopyHalfWords(DMA_CHANNEL, global::current_map, bgGetMapPtr(global::bg_sub_address),
                             sizeof(global::current_map));
            global::main_dude->bottomCollision = false;


            MapTile *entrance;
            global::level_generator->getFirstTile(MapTileType::ENTRANCE, entrance);

            if (entrance == nullptr) {
                global::main_dude->x = 32;
                global::main_dude->y = 280;
            } else {
                global::main_dude->x = entrance->x * 16;
                global::main_dude->y = entrance->y * 16;
            }


            global::main_oam_manager->clearAllSprites();
            global::sub_oam_manager->clearAllSprites();
            global::sprites.clear();

            init();

            global::sprites.push_back(global::main_dude);
            consoleClear();

            global::camera->followMainDude = true;
            for (int a = 0; a < 400; a++) {
                global::camera->updatePosition(global::main_dude->x, global::main_dude->y);
            }

            if (global::in_main_menu || global::levels_transition_screen) {

                if(global::in_main_menu)
                {
                    global::hud->hearts = 4;
                    global::hud->ropes = 4;
                    global::hud->bombs = 4;
                    global::hud->dollars = 0;
                }

                global::hud->init();
                gameloop::populateCaveNpcs();
                gameloop::populateCaveItems();
                gameloop::populateCaveMoniez();
                global::levels_transition_screen = false;
                global::in_main_menu = false;
                global::killedNpcs.clear();
                global::collectedLoot.clear();
                global::hud->money_on_this_level = 0;

                this->holding_item = false;


            } else {
                if (global::scores_screen) {
                    global::camera->followMainDude = false;
                    global::in_main_menu = true;
                    global::levels_transition_screen = false;
                    global::scores_screen = false;
                    gameloop::populate_main_menu();
                } else if (dead) {
                    dead = false;
                    global::scores_screen = true;
                    global::hud->hide();
                    global::hud->draw_scores();
                    global::camera->followMainDude = false;

                } else {
                    global::hud->total_time_spent += global::hud->time_spent_on_level;
                    global::hud->level++;

                    global::levels_transition_screen = true;

                    global::splash_screen = true;

                    global::input_handler->stop_handling = true;
                    global::input_handler->right_key_held = true;
                    global::input_handler->up_key_held = true;
                    global::camera->followMainDude = false;
                    global::hud->draw_on_level_done();
                }
            }

            exitingLevel = false;
            mmEffectCancel(SFX_MCAVE);

        } else if (animFrame >= 16 && global::splash_screen) {

            main_spelunker->entry->isHidden = true;
            sub_spelunker->entry->isHidden = true;
            global::input_handler->stop_handling = false;

            if (global::input_handler->l_bumper_down || global::input_handler->l_bumper_down) {
                global::splash_screen = false;
            }
        }

        frame = ((13) * SPRITESHEET_ROW_WIDTH) + animFrame + 2;
        offset = frameGfx + frame * MAIN_DUDE_WIDTH * MAIN_DUDE_HEIGHT / 2;
        main_spelunker->updateFrame(offset, 16 * 16);
        sub_spelunker->updateFrame(offset, 16 * 16);

    } else if (dead) {

        if (bottomCollision) {
            frame = ((2) * SPRITESHEET_ROW_WIDTH) + 5;
        } else
            frame = ((2) * SPRITESHEET_ROW_WIDTH) + 4;

        offset = frameGfx + frame * MAIN_DUDE_WIDTH * MAIN_DUDE_HEIGHT / 2;
        main_spelunker->updateFrame(offset, 16 * 16);
        sub_spelunker->updateFrame(offset, 16 * 16);

    } else if (climbing) {

        if (animFrame >= 12)
            animFrame = 0;

        frame = ((12) * SPRITESHEET_ROW_WIDTH) + animFrame + 2;
        offset = frameGfx + frame * MAIN_DUDE_WIDTH * MAIN_DUDE_HEIGHT / 2;
        main_spelunker->updateFrame(offset, 16 * 16);
        sub_spelunker->updateFrame(offset, 16 * 16);

    } else if (stunned) {
        if (animFrame > 4)
            animFrame = 0;

        frame = (3 * SPRITESHEET_ROW_WIDTH) + animFrame;
        offset = frameGfx + frame * MAIN_DUDE_WIDTH * MAIN_DUDE_HEIGHT / 2;
        main_spelunker->updateFrame(offset, 16 * 16);
        sub_spelunker->updateFrame(offset, 16 * 16);
    } else if (pushing_left || pushing_right) {

        if (pushing_left)
            frame = animFrame + (7) * SPRITESHEET_ROW_WIDTH;
        else
            frame = animFrame + (8) * SPRITESHEET_ROW_WIDTH + 1;


        offset = frameGfx + frame * MAIN_DUDE_WIDTH * MAIN_DUDE_HEIGHT / 2;
        main_spelunker->updateFrame(offset, 16 * 16);
        sub_spelunker->updateFrame(offset, 16 * 16);

    } else if (whip) {

        if (state == 1) {
            frame = (9 * SPRITESHEET_ROW_WIDTH) + 2 + animFrame;
            offset = frameGfx + frame * MAIN_DUDE_WIDTH * MAIN_DUDE_HEIGHT / 2;
        } else if (state == 0) {
            frame = (10 * SPRITESHEET_ROW_WIDTH) + 2 + animFrame;
            offset = frameGfx + frame * MAIN_DUDE_WIDTH * MAIN_DUDE_HEIGHT / 2;
        }

        main_spelunker->updateFrame(offset, 16 * 16);
        sub_spelunker->updateFrame(offset, 16 * 16);
    } else if (hangingOnTileRight) {
        frame = (2 * SPRITESHEET_ROW_WIDTH) + 1;
        offset = frameGfx + frame * MAIN_DUDE_WIDTH * MAIN_DUDE_HEIGHT / 2;
        main_spelunker->updateFrame(offset, 16 * 16);
        sub_spelunker->updateFrame(offset, 16 * 16);
    } else if (hangingOnTileLeft) {
        frame = (2 * SPRITESHEET_ROW_WIDTH);
        offset = frameGfx + frame * MAIN_DUDE_WIDTH * MAIN_DUDE_HEIGHT / 2;
        main_spelunker->updateFrame(offset, 16 * 16);
        sub_spelunker->updateFrame(offset, 16 * 16);
    } else if (crawling) {
        //left
        if (state == 1)
            frame = animFrame + (4) * SPRITESHEET_ROW_WIDTH;
        else
            frame = animFrame + (5) * SPRITESHEET_ROW_WIDTH + 3;


        offset = frameGfx + frame * MAIN_DUDE_WIDTH * MAIN_DUDE_HEIGHT / 2;
        main_spelunker->updateFrame(offset, 16 * 16);
        sub_spelunker->updateFrame(offset, 16 * 16);
    } else if (!bottomCollision) {

        if (state == 1) {
            frame = state * FRAMES_PER_ANIMATION;
            offset = frameGfx + frame * MAIN_DUDE_WIDTH * MAIN_DUDE_HEIGHT / 2;
        } else if (state == 0) {
            frame = state * FRAMES_PER_ANIMATION;
            offset = frameGfx + frame * MAIN_DUDE_WIDTH * MAIN_DUDE_HEIGHT / 2;
        }
        main_spelunker->updateFrame(offset, 16 * 16);
        sub_spelunker->updateFrame(offset, 16 * 16);

    } else {
        if (abs(xSpeed) != 0) {
            frame = animFrame + state * FRAMES_PER_ANIMATION;
            offset = frameGfx + frame * MAIN_DUDE_WIDTH * MAIN_DUDE_HEIGHT / 2;
        } else {
            if (state == 1) {
                frame = (2 * SPRITESHEET_ROW_WIDTH) + 2;
                offset = frameGfx + frame * MAIN_DUDE_WIDTH * MAIN_DUDE_HEIGHT / 2;
            } else if (state == 0) {
                frame = (2 * SPRITESHEET_ROW_WIDTH) + 3;
                offset = frameGfx + frame * MAIN_DUDE_WIDTH * MAIN_DUDE_HEIGHT / 2;
            }

        }
        main_spelunker->updateFrame(offset, 16 * 16);
        sub_spelunker->updateFrame(offset, 16 * 16);
    }

    canClimbRope = false;
    canClimbLadder = false;

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

void MainDude::canHangOnTile(MapTile *neighboringTiles[9]) {

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

    if ((y_bound && x_bound) && hangingTimer > MIN_HANGING_TIME) {

//        fprintf(stdout, "HANGING" + '\n');

        if (rightCollision && neighboringTiles[LEFT_MIDDLE]->collidable) {
            this->y = (neighboringTiles[LEFT_MIDDLE]->y * 16);
            hangingOnTileRight = true;
            jumpingTimer = 0;
            hangingTimer = 0;
            ySpeed = 0;
        }
        if (leftCollision && neighboringTiles[RIGHT_MIDDLE]->collidable) {
            jumpingTimer = 0;
            hangingOnTileLeft = true;
            this->y = (neighboringTiles[RIGHT_MIDDLE]->y * 16);
            hangingTimer = 0;
            ySpeed = 0;
        }
    }

}

void MainDude::updateOther() {
}

void MainDude::updatePosition() {

//    if (!canClimbRope)
//        climbing = false;

    double tempXspeed = abs(xSpeed);
    double tempYspeed = abs(ySpeed);

    int old_xx = -1;
    int old_yy = -1;
    int xx;
    int yy;

    while (tempXspeed > 0 || tempYspeed > 0) {
        if (tempXspeed > 0) {
            if (xSpeed > 0) {
                x += 1;
            } else if (xSpeed < 0) {
                x -= 1;
            }
        }
        if (tempYspeed > 0) {
            if (ySpeed > 0)
                y += 1;
            else if (ySpeed < 0)
                y -= 1;
        }



//            Collisions::getCenterTile(this->x, this->y, MAIN_DUDE_HEIGHT, MAIN_DUDE_WIDTH, xx, yy);
//fixme

        xx = floor_div(this->x + 0.5 * MAIN_DUDE_WIDTH, 16);
        yy = floor_div(this->y + 0.5 * MAIN_DUDE_HEIGHT, 16);

        if (old_xx != xx || old_yy != yy) {
            if (xx < 31 && yy < 31)
                updateCollisionsMap(xx, yy);
        }

        old_xx = xx;
        old_yy = yy;

        tempXspeed--;
        tempYspeed--;

    }
}

void MainDude::initSprite() {

    main_spelunker = global::main_oam_manager->initSprite(spelunkerPal, spelunkerPalLen, nullptr,
                                                          16 * 16, 16, MAIN_DUDE, true, false);

    main_pre_whip = global::main_oam_manager->initSprite(whipPal, whipPalLen,
                                                         nullptr, 16 * 16, 16, PRE_WHIP, true,
                                                         false);

    main_whip = global::main_oam_manager->initSprite(whipPal, whipPalLen,
                                                     nullptr, 16 * 16, 16, WHIP, true, false);

    sub_spelunker = global::sub_oam_manager->initSprite(spelunkerPal, spelunkerPalLen, nullptr,
                                                        16 * 16, 16, MAIN_DUDE, true, false);

    sub_pre_whip = global::sub_oam_manager->initSprite(whipPal, whipPalLen,
                                                       nullptr, 16 * 16, 16, PRE_WHIP, true,
                                                       false);

    sub_whip = global::sub_oam_manager->initSprite(whipPal, whipPalLen,
                                                   nullptr, 16 * 16, 16, WHIP, true, false);


    u8* frameGfx_whip = (u8 *) whipTiles;

    u8 *gfx_pre_whip = (frameGfx_whip+ (1 * 16 * 16 / 2));
    u8 *gfx_whip =  (frameGfx_whip+ (0 * 16 * 16 / 2));

    main_pre_whip->updateFrame(gfx_pre_whip, 16 * 16);
    sub_pre_whip->updateFrame(gfx_pre_whip, 16 * 16);

    main_whip->updateFrame(gfx_whip, 16 * 16);
    sub_whip->updateFrame(gfx_whip, 16 * 16);


    sub_whip->entry->isHidden = true;
    main_whip->entry->isHidden = true;
    sub_pre_whip->entry->isHidden = true;
    main_pre_whip->entry->isHidden = true;


    int main_x = x - global::camera->x;
    int main_y = y - global::camera->y;
    int sub_x = x - global::camera->x;
    int sub_y = y - global::camera->y - 192;

    if (this->y <= 320 + 16 && this->y >= 320 - 16) {

    } else if (this->y > 320) {
        main_x = -16;
        main_y = -16;
    } else if (this->y < 320) {
        sub_x = -16;
        sub_y = -16;
    }

    main_spelunker->entry->x = main_x;
    main_spelunker->entry->y = main_y;

    sub_spelunker->entry->x = sub_x;
    sub_spelunker->entry->y = sub_y;

}

