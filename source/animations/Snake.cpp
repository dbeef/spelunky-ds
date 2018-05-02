//
// Created by xdbeef on 23.04.18.
//

#include "Snake.h"
#include "../Globals.h"
#include "Collisions.h"
#include "../level_layout/MapUtils.h"
#include "Rock.h"
#include "../../build/snake.h"
#include "Blood.h"


void Snake::draw() {

    int main_x = x - global::camera->x;
    int main_y = y - global::camera->y;
    int sub_x = x - global::camera->x;
    int sub_y = y - global::camera->y - 192;

    if (global::camera->y + 192 > this->y + 16 || global::camera->y + 192 + 192 < this->y - 16) {
        sub_x = -128;
        sub_y = -128;
    }
    if (global::camera->y > this->y + 16 || global::camera->y + 192 < this->y - 16) {
        main_x = -128;
        main_y = -128;
    }

    if (sub_y + 16 < 0 || sub_x + 16 < 0) {
        sub_x = -128;
        sub_y = -128;
    }

    if (main_y + 16 < 0 || main_x + 16 < 0) {
        main_x = -128;
        main_y = -128;
    }


    if (activated_by_main_dude) {
        //nothing
    }

    mainSpriteInfo->entry->x = main_x;
    mainSpriteInfo->entry->y = main_y;

    subSpriteInfo->entry->x = sub_x;
    subSpriteInfo->entry->y = sub_y;

    animFrameTimer += *global::timer;

    if (animFrameTimer > 125) {
//        std::cout<< animFrame << '\n';
        animFrame++;
        if (animFrame >= 4)
            animFrame = 0;

        if (spriteState == SpriteState::W_LEFT) {
            frameGfx = (u8 *) snakeTiles + ((16 * 16 * (animFrame + 4)) / 2);
        } else if (spriteState == SpriteState::W_RIGHT) {
            frameGfx = (u8 *) snakeTiles + ((16 * 16 * animFrame) / 2);
        }
        animFrameTimer = 0;

        subSpriteInfo->updateFrame(frameGfx, 16 * 16);
        mainSpriteInfo->updateFrame(frameGfx, 16 * 16);
    }


    if (bottomCollision) {
        if (waitTimer > 0) {
            waitTimer -= *global::timer;
        } else {
            if (goTimer > 0)


                goTimer -= *global::timer;

//            std::cout << goTimer << '\n';

            if (standingOnLeftEdge && spriteState == 1 || standingOnRightEdge && spriteState == 0) {
                xSpeed = 0;
            } else if (spriteState == 0)
                xSpeed = 0.5;
            else
                xSpeed = -0.5;

            if (goTimer <= 0) {
//                std::cout << "RAND" << '\n';
                randomizeMovement();
                xSpeed = 0;
            }
        }
    }


    if (global::main_dude->whip && !killed && global::main_dude->whip_timer > 120) {


        if (Collisions::checkCollisionWithMainDudeWhip(x, y, 16, 16)) {
            subSpriteInfo->entry->isHidden = true;
            mainSpriteInfo->entry->isHidden = true;

            for (int a = 0; a < 4; a++) {
                Blood *blood = new Blood();
                blood->init();
                blood->x = x;
                blood->y = y;

                if (a % 2 == 0)
                    blood->xSpeed = (1 / a);
                else
                    blood->xSpeed = (-1 / a);

                blood->ySpeed = -1 / a;
                global::sprites.push_back(blood);
            }
            killed = true;

        }
    }

    if (!killed && !global::main_dude->dead && Collisions::checkCollisionWithMainDude(x, y, 16, 16) && global::main_dude->time_since_last_damage > 1000) {

        global::main_dude->time_since_last_damage = 0;
        global::hud->hearts--;
        global::hud->draw();

        if (global::hud->hearts == 0) {
            global::hud->hide();
            global::main_dude->ySpeed = -MAIN_DUDE_JUMP_SPEED * 0.25;
            global::main_dude->dead = true;
        }

    }

}


void Snake::init() {
    subSpriteInfo = global::sub_oam_manager->initSprite(snakePal, snakePalLen,
                                                        nullptr, 16 * 16, 16, SNAKE, true, false);
    mainSpriteInfo = global::main_oam_manager->initSprite(snakePal, snakePalLen,
                                                          nullptr, 16 * 16, 16, SNAKE, true, false);

    sameDirectionInRow = 0;
    frameGfx = (u8 *) snakeTiles;
    subSpriteInfo->updateFrame(frameGfx, 16 * 16);
    mainSpriteInfo->updateFrame(frameGfx, 16 * 16);

    //idk why do i have to do that, if it is already flipped in image
    subSpriteInfo->entry->hFlip = true;

    randomizeMovement();
}

void Snake::randomizeMovement() {
//    srand(*global::timer + x + y + global::main_dude->x + +global::main_dude->y);

    int r = rand() % 2;

    if (r == 0) {
        if (spriteState == SpriteState::W_LEFT)
            sameDirectionInRow++;
        else
            sameDirectionInRow = 0;

        if (sameDirectionInRow == 2)
            spriteState = SpriteState::W_RIGHT;
        else
            spriteState = SpriteState::W_LEFT;

    } else if (r == 1) {
        if (spriteState == SpriteState::W_RIGHT)
            sameDirectionInRow++;
        else
            sameDirectionInRow = 0;

        if (sameDirectionInRow == 2)
            spriteState = SpriteState::W_LEFT;
        else
            spriteState = SpriteState::W_RIGHT;
    }

    goTimer = (rand() % 2000) + 1000;
    waitTimer = rand() % 500;
}

void Snake::updateSpeed() {

    if (xSpeed > MAX_X_SPEED_ROCK)
        xSpeed = MAX_X_SPEED_ROCK;
    if (xSpeed < -MAX_X_SPEED_ROCK)
        xSpeed = -MAX_X_SPEED_ROCK;

    if (ySpeed > MAX_Y_SPEED_ROCK)
        ySpeed = MAX_Y_SPEED_ROCK;
    if (ySpeed < -MAX_Y_SPEED_ROCK)
        ySpeed = -MAX_Y_SPEED_ROCK;

    pos_inc_timer += *global::timer;

    bool change_pos = (pos_inc_timer > 35);

    if (change_pos) {
        updatePosition();
    }

}

void Snake::updatePosition() {

    if (bottomCollision && xSpeed > 0) {
        xSpeed -= 0.055;
        if (xSpeed < 0)
            xSpeed = 0;
    }
    if (bottomCollision && xSpeed < 0) {
        xSpeed += 0.055;
        if (xSpeed > 0)
            xSpeed = 0;
    }

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

        xx = floor_div(x + (0.5 * 16), 16);
        yy = floor_div(y + (0.5 * 16), 16);

        if (old_xx != xx || old_yy != yy) {
            updateCollisionsMap(xx, yy);
        }

        old_xx = xx;
        old_yy = yy;

        tempXspeed--;
        tempYspeed--;
    }


    if (!bottomCollision)
        ySpeed += GRAVITY_DELTA_SPEED;

    pos_inc_timer = 0;
}

void Snake::updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) {

    MapTile *tiles[9] = {};
    Collisions::getNeighboringTiles(global::level_generator->mapTiles, x_current_pos_in_tiles,
                                    y_current_pos_in_tiles, tiles);

    standingOnLeftEdge = Collisions::isStandingOnLeftEdge(tiles, x, 16, x_current_pos_in_tiles);
    standingOnRightEdge = Collisions::isStandingOnRightEdge(tiles, x, 16, x_current_pos_in_tiles);
    bottomCollision = Collisions::checkBottomCollision(tiles, &x, &y, &ySpeed, 16, 16, false);
    leftCollision = Collisions::checkLeftCollision(tiles, &x, &y, &xSpeed, 16, 16, false);
    rightCollision = Collisions::checkRightCollision(tiles, &x, &y, &xSpeed, 16, 16, false);
    upperCollision = Collisions::checkUpperCollision(tiles, &x, &y, &ySpeed, 16, false);

    if (bottomCollision) {
        //nothing
    }


}
