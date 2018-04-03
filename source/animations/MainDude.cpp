//
// Created by xdbeef on 24.02.18.
//

#include <nds.h>
#include <nds/arm9/sprite.h>
#include <iostream>
#include "MainDude.h"
#include "../Consts.h"
#include "../../build/spelunker.h"
#include "Collisions.h"
#include "../level_layout/MapUtils.h"

#define SPRITESHEET_ROW_WIDTH 6

/**
 * For debugging purposes
 * @param mapTiles
 */
void MainDude::clearTilesOnRight(MapTile *mapTiles[32][32]) {

    int xx = floor_div(this->x, TILE_W);
    int yy = floor_div(this->y, TILE_H);

    MapTile *left_middle = mapTiles[xx - 1][yy];
    MapTile *right_middle = mapTiles[xx + 1][yy];
    MapTile *up_middle = mapTiles[xx][yy - 1];
    MapTile *down_middle = mapTiles[xx][yy + 1];
    MapTile *center = mapTiles[xx][yy];
    MapTile *left_up = mapTiles[xx - 1][yy - 1];
    MapTile *right_up = mapTiles[xx + 1][yy - 1];
    MapTile *left_down = mapTiles[xx - 1][yy + 1];
    MapTile *right_down = mapTiles[xx + 1][yy + 1];

    up_middle->values[0] = 16;
    up_middle->values[1] = 16;
    up_middle->values[2] = 16;
    up_middle->values[3] = 16;

    down_middle->values[0] = 0;
    down_middle->values[1] = 0;
    down_middle->values[2] = 0;
    down_middle->values[3] = 0;

}

void MainDude::handleKeyInput(int keys_held, int keys_down) {


    if (keys_down && !stunned) {
        if (keys_down & KEY_R) {
            if (bottomCollision) {
                ySpeed = -JUMP_SPEED;
            }
            if ((hangingOnTileLeft || hangingOnTileRight) && hangingTimer > MIN_HANGING_TIME) {
                ySpeed = -JUMP_SPEED;
                hangingOnTileLeft = false;
                hangingOnTileRight = false;
                hangingTimer = 0;
            }
        }
        if (keys_down & KEY_L) {
            if (!stunned && !whip) {
                whip = true;
                animFrame = 0;
            }
        }
    }
    if (keys_held && !stunned) {


        if (keys_held & KEY_LEFT) {
            key_left = true;
            state = W_LEFT;
            hangingOnTileLeft = false;
            if (xSpeed > -MAX_X_SPEED && !(hangingOnTileRight || hangingOnTileLeft))
                if (speedIncTimer > X_SPEED_DELTA_TIME_MS) {
                    xSpeed -= X_SPEED_DELTA;
                    speedIncTimer = 0;
                }
        } else
            key_left = false;

        if (keys_held & KEY_RIGHT) {
            key_right = true;
            state = W_RIGHT;
            hangingOnTileRight = false;
            if (xSpeed < MAX_X_SPEED && !(hangingOnTileRight || hangingOnTileLeft)) {
                if (speedIncTimer > X_SPEED_DELTA_TIME_MS) {
                    xSpeed += X_SPEED_DELTA;
                    speedIncTimer = 0;
                }
            }
        } else
            key_right = false;

        if (keys_held & KEY_DOWN) {
            hangingOnTileLeft = false;
            hangingOnTileRight = false;
            if (bottomCollision)
                crawling = true;
        } else
            crawling = false;
    } else
        crawling = false;

    if (!keys_held) {
        key_left = false;
        key_right = false;
    }
}


void MainDude::updateTimers(int timeElapsed) {

    posIncTimer += timeElapsed;
    frictionTimer += timeElapsed;
    speedIncTimer += timeElapsed;
    hangingTimer += timeElapsed;

    if (whip) {
        whip_timer += timeElapsed;
        //4 frames idle
        if (whip_timer > 420 + 0 * 70) {
            whip_timer = 0;
            whip = false;
            hideWhipLeftSub();
            hideWhipLeftMain();
            hideWhipRightSub();
            hideWhipRightMain();
            hidePreWhipLeftSub();
            hidePreWhipLeftMain();
            hidePreWhipRightSub();
            hidePreWhipRightMain();
        }
    }

    if (animationFrameTimer > 65) {
        animationFrameTimer = 0;

        if (!whip || (whip && animFrame < 5))
            animFrame++;

    }

    if (!key_left && pushing_left) {
        pushing_left = false;
        pushingTimer = 0;
    }
    if (!key_right && pushing_right) {
        pushing_right = false;
        pushingTimer = 0;
    }

    if ((leftCollision || rightCollision) && !crawling && !hangingOnTileLeft && !hangingOnTileRight &&
        (key_left || key_right)) {
        pushingTimer += timeElapsed;
        if (pushingTimer > PUSHING_TIME)
            if (leftCollision) {
                pushing_right = true;
                pushingTimer = 0;
            } else {
                pushing_left = true;
                pushingTimer = 0;
            }
    } else {
        pushingTimer = 0;
        pushing_left = false;
        pushing_right = false;
    }

    if (animFrame >= FRAMES_PER_ANIMATION && !crawling && !pushing_left && !pushing_right)
        animFrame = 0;

    if (animFrame >= 9 && crawling)
        animFrame = 0;

    if ((pushing_left || pushing_right) && animFrame >= 7)
        animFrame = 0;


    if (!bottomCollision && !hangingOnTileLeft && !hangingOnTileRight)
        jumpingTimer += timeElapsed;

    if (bottomCollision && jumpingTimer > STUN_JUMPING_TIME) {
        stunned = true;
        jumpingTimer = 0;
    } else if (bottomCollision && jumpingTimer < STUN_JUMPING_TIME) {
        jumpingTimer = 0;
    }

    if (stunned)
        stunnedTimer += timeElapsed;
    if (stunnedTimer > STUN_TIME) {
        stunned = false;
        stunnedTimer = 0;
    }


    if (xSpeed != 0 || stunned || whip)
        animationFrameTimer += timeElapsed;
}


void MainDude::updateSpeed(MapTile *mapTiles[32][32]) {

    if (crawling) {

        if (xSpeed > MAX_X_SPEED_CRAWLING)
            xSpeed = MAX_X_SPEED_CRAWLING;
        if (xSpeed < -MAX_X_SPEED_CRAWLING)
            xSpeed = -MAX_X_SPEED_CRAWLING;

    } else {
        if (xSpeed > MAX_X_SPEED)
            xSpeed = MAX_X_SPEED;
        if (xSpeed < -MAX_X_SPEED)
            xSpeed = -MAX_X_SPEED;
    }

    if (ySpeed > MAX_Y_SPEED)
        ySpeed = MAX_Y_SPEED;
    if (ySpeed < -MAX_Y_SPEED)
        ySpeed = -MAX_Y_SPEED;


    bool change_pos = (crawling && posIncTimer > 20) || (!crawling && posIncTimer > 10);

    if (change_pos) {

//        std::cout << xSpeed << '\n';

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

            xx = floor_div(this->x + 0.5 * MAIN_DUDE_WIDTH, 16);
            yy = floor_div(this->y + 0.5 * MAIN_DUDE_HEIGHT, 16);

            if (old_xx != xx || old_yy != yy) {
                checkCollisionWithMap(mapTiles, xx, yy);
            }

            old_xx = xx;
            old_yy = yy;

            tempXspeed--;
            tempYspeed--;
        }


        if (!bottomCollision && !(hangingOnTileLeft || hangingOnTileRight))
            ySpeed += GRAVITY_DELTA_SPEED;

        posIncTimer = 0;

    }
}

void MainDude::checkCollisionWithMap(MapTile *mapTiles[32][32], int xx, int yy) {

    MapTile *left_middle = mapTiles[xx - 1][yy];
    MapTile *right_middle = mapTiles[xx + 1][yy];
    MapTile *up_middle = mapTiles[xx][yy - 1];
    MapTile *down_middle = mapTiles[xx][yy + 1];
    MapTile *center = mapTiles[xx][yy];
    MapTile *left_up = mapTiles[xx - 1][yy - 1];
    MapTile *right_up = mapTiles[xx + 1][yy - 1];
    MapTile *left_down = mapTiles[xx - 1][yy + 1];
    MapTile *right_down = mapTiles[xx + 1][yy + 1];
    MapTile *tiles[9] = {
            left_middle, //0
            right_middle, //1
            up_middle, //2
            down_middle, //3
            center, //4
            left_up, //5
            right_up, //6
            left_down, //7
            right_down //8
    };

    bottomCollision = Collisions::checkBottomCollision(tiles, &this->x, &this->y, &ySpeed, 16, 16);
    leftCollision = Collisions::checkLeftCollision(tiles, &this->x, &this->y, &xSpeed, 16, 16);
    rightCollision = Collisions::checkRightCollision(tiles, &this->x, &this->y, &xSpeed, 16, 16);
    upperCollision = Collisions::checkUpperCollision(tiles, &this->x, &this->y, &ySpeed, 16);
    Collisions::isStandingOnEdge(tiles, &
            this->x, &this->y, &ySpeed, 16, 16);


    canHangOnTile(tiles);

    if (upperCollision || bottomCollision) {
        hangingOnTileLeft = false;
        hangingOnTileRight = false;
    }


    if (!bottomCollision) {
        if ((right_middle == 0 && (right_up != 0 && right_down != 0))) {
            if (xSpeed > 0)
                x += 2;
        }
        if ((left_middle == 0 && (left_up != 0 && left_down != 0))) {
            if (xSpeed < 0)
                x -= 2;
        }
    }

}

void MainDude::init() {
    frameGfx = (u8 *) spelunkerTiles;

    hideWhipLeftSub();
    hideWhipLeftMain();
    hideWhipRightSub();
    hideWhipRightMain();
    hidePreWhipLeftSub();
    hidePreWhipLeftMain();
    hidePreWhipRightSub();
    hidePreWhipRightMain();
}


void MainDude::animate(Camera *camera) {

    int main_x = x - camera->x;
    int main_y = y - camera->y;
    int sub_x = x - camera->x;
    int sub_y = y - camera->y - 192;

    if (this->y > 320) {
        main_x = -16;
        main_y = -16;
    }

    if (this->y < 320) {
        sub_x = -16;
        sub_y = -16;
    }

    main_spriteInfo->entry->x = main_x;
    main_spriteInfo->entry->y = main_y;

    sub_spriteInfo->entry->x = sub_x;
    sub_spriteInfo->entry->y = sub_y;

    int frame;
    u8 *offset;

    if (whip) {

        if (state == 1) {

            hideWhipRightMain();
            hideWhipRightSub();
            hidePreWhipRightMain();
            hidePreWhipRightSub();

            if (whip_timer > 100 && whip_timer < 170) {
                showPreWhipLeftMain();
                showPreWhipLeftSub();
            } else if (whip_timer >= 200) {
                hidePreWhipLeftMain();
                hidePreWhipLeftSub();
                showWhipLeftMain();
                showWhipLeftSub();
            }

            sub_pre_whip_left_spriteInfo->entry->x = sub_x + 16;
            sub_pre_whip_left_spriteInfo->entry->y = sub_y;
            main_pre_whip_left_spriteInfo->entry->x = main_x + 16;
            main_pre_whip_left_spriteInfo->entry->y = main_y;

            sub_whip_left_spriteInfo->entry->x = sub_x - 16;
            sub_whip_left_spriteInfo->entry->y = sub_y;
            main_whip_left_spriteInfo->entry->x = main_x - 16;
            main_whip_left_spriteInfo->entry->y = main_y;

        } else {

            hideWhipLeftMain();
            hideWhipLeftSub();
            hidePreWhipLeftMain();
            hidePreWhipLeftSub();


            if (whip_timer > 100 && whip_timer < 170) {
                showPreWhipRightMain();
                showPreWhipRightSub();

            } else if (whip_timer >= 200) {
                hidePreWhipRightMain();
                hidePreWhipRightSub();
                showWhipRightMain();
                showWhipRightSub();
            }

            sub_pre_whip_right_spriteInfo->entry->x = sub_x - 16;
            sub_pre_whip_right_spriteInfo->entry->y = sub_y;
            main_pre_whip_right_spriteInfo->entry->x = main_x - 16;
            main_pre_whip_right_spriteInfo->entry->y = main_y;
            sub_whip_right_spriteInfo->entry->x = sub_x + 16;
            sub_whip_right_spriteInfo->entry->y = sub_y;
            main_whip_right_spriteInfo->entry->x = main_x + 16;
            main_whip_right_spriteInfo->entry->y = main_y;
        }

    }
    if (stunned) {
        if (animFrame > 4)
            animFrame = 0;

        frame = (3 * SPRITESHEET_ROW_WIDTH) + animFrame;
        offset = frameGfx + frame * MAIN_DUDE_WIDTH * MAIN_DUDE_HEIGHT / 2;
        main_spriteInfo->updateFrame(offset);
        sub_spriteInfo->updateFrame(offset);
    } else if (pushing_left || pushing_right) {

        if (pushing_left)
            frame = animFrame + (7) * SPRITESHEET_ROW_WIDTH;
        else
            frame = animFrame + (8) * SPRITESHEET_ROW_WIDTH + 1;


        offset = frameGfx + frame * MAIN_DUDE_WIDTH * MAIN_DUDE_HEIGHT / 2;
        main_spriteInfo->updateFrame(offset);
        sub_spriteInfo->updateFrame(offset);

    } else if (whip) {

        if (state == 1) {
            frame = (9 * SPRITESHEET_ROW_WIDTH) + 2 + animFrame;
            offset = frameGfx + frame * MAIN_DUDE_WIDTH * MAIN_DUDE_HEIGHT / 2;
        } else if (state == 0) {
            frame = (10 * SPRITESHEET_ROW_WIDTH) + 2 + animFrame;
            offset = frameGfx + frame * MAIN_DUDE_WIDTH * MAIN_DUDE_HEIGHT / 2;
        }

        main_spriteInfo->updateFrame(offset);
        sub_spriteInfo->updateFrame(offset);
    } else if (hangingOnTileRight) {
        frame = (2 * SPRITESHEET_ROW_WIDTH) + 1;
        offset = frameGfx + frame * MAIN_DUDE_WIDTH * MAIN_DUDE_HEIGHT / 2;
        main_spriteInfo->updateFrame(offset);
        sub_spriteInfo->updateFrame(offset);
    } else if (hangingOnTileLeft) {
        frame = (2 * SPRITESHEET_ROW_WIDTH);
        offset = frameGfx + frame * MAIN_DUDE_WIDTH * MAIN_DUDE_HEIGHT / 2;
        main_spriteInfo->updateFrame(offset);
        sub_spriteInfo->updateFrame(offset);
    } else if (crawling) {
        //left
        if (state == 1)
            frame = animFrame + (4) * SPRITESHEET_ROW_WIDTH;
        else
            frame = animFrame + (5) * SPRITESHEET_ROW_WIDTH + 3;


        offset = frameGfx + frame * MAIN_DUDE_WIDTH * MAIN_DUDE_HEIGHT / 2;
        main_spriteInfo->updateFrame(offset);
        sub_spriteInfo->updateFrame(offset);
    } else if (!bottomCollision) {

        if (state == 1) {
            frame = state * FRAMES_PER_ANIMATION;
            offset = frameGfx + frame * MAIN_DUDE_WIDTH * MAIN_DUDE_HEIGHT / 2;
        } else if (state == 0) {
            frame = state * FRAMES_PER_ANIMATION;
            offset = frameGfx + frame * MAIN_DUDE_WIDTH * MAIN_DUDE_HEIGHT / 2;
        }
        main_spriteInfo->updateFrame(offset);
        sub_spriteInfo->updateFrame(offset);

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
        main_spriteInfo->updateFrame(offset);
        sub_spriteInfo->updateFrame(offset);
    }

}

void MainDude::canHangOnTile(MapTile *neighboringTiles[9]) {

    if (bottomCollision || (!leftCollision && !rightCollision))
        return;

    if (neighboringTiles[2] != 0 || neighboringTiles[3] != 0)
        return;

    bool y_bound = false;
    bool x_bound = false;

    if (rightCollision && state == W_LEFT) {

        if (neighboringTiles[5] != 0)
            return;

        x_bound = (this->x <= (neighboringTiles[0]->x * 16) + 16 && (this->x >= (neighboringTiles[0]->x * 16) + 12));
        y_bound = (this->y > (neighboringTiles[0]->y * 16) - 2) && (this->y < (neighboringTiles[0]->y * 16) + 8);
    } else if (leftCollision && state == W_RIGHT) {

        if (neighboringTiles[6] != 0)
            return;

        y_bound = (this->y > (neighboringTiles[1]->y * 16) - 2) && (this->y < (neighboringTiles[1]->y * 16) + 8);
        x_bound = (this->x <= (neighboringTiles[1]->x * 16) - 12 && (this->x >= (neighboringTiles[1]->x * 16) - 16));
    }

    if ((y_bound && x_bound) && hangingTimer > MIN_HANGING_TIME) {
        hangingTimer = 0;
        ySpeed = 0;

//        fprintf(stdout, "HANGING" + '\n');

        if (rightCollision) {
            this->y = (neighboringTiles[0]->y * 16);
            hangingOnTileRight = true;
        }
        if (leftCollision) {
            hangingOnTileLeft = true;
            this->y = (neighboringTiles[1]->y * 16);
        }
    }

}

void MainDude::applyFriction() {

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

}

void MainDude::update(Camera *camera, int keys_held, int keys_down, LevelGenerator *l) {
    this->applyFriction();
    this->updateTimers(timerElapsed(0) / TICKS_PER_SECOND);
    this->animate(camera);
    this->updateSpeed(l->mapTiles);
    this->handleKeyInput(keys_held, keys_down);

    if (!bottomCollision)
        crawling = false;

}

void MainDude::hideWhipRightMain() {
    main_whip_right_spriteInfo->entry->isHidden = true;
}

void MainDude::hidePreWhipRightMain() {
    main_pre_whip_right_spriteInfo->entry->isHidden = true;
}

void MainDude::hideWhipRightSub() {
    sub_whip_right_spriteInfo->entry->isHidden = true;
}

void MainDude::hidePreWhipRightSub() {
    sub_pre_whip_right_spriteInfo->entry->isHidden = true;
}

void MainDude::hideWhipLeftMain() {
    main_whip_left_spriteInfo->entry->isHidden = true;
}

void MainDude::hidePreWhipLeftMain() {
    main_pre_whip_left_spriteInfo->entry->isHidden = true;
}

void MainDude::hideWhipLeftSub() {
    sub_whip_left_spriteInfo->entry->isHidden = true;
}

void MainDude::hidePreWhipLeftSub() {
    sub_pre_whip_left_spriteInfo->entry->isHidden = true;
}

void MainDude::showWhipRightMain() {
    main_whip_right_spriteInfo->entry->isHidden = false;
}

void MainDude::showPreWhipRightMain() {
    main_pre_whip_right_spriteInfo->entry->isHidden = false;
}

void MainDude::showWhipRightSub() {
    sub_whip_right_spriteInfo->entry->isHidden = false;
}

void MainDude::showPreWhipRightSub() {
    sub_pre_whip_right_spriteInfo->entry->isHidden = false;
}

void MainDude::showWhipLeftMain() {
    main_whip_left_spriteInfo->entry->isHidden = false;
}

void MainDude::showPreWhipLeftMain() {
    main_pre_whip_left_spriteInfo->entry->isHidden = false;
}

void MainDude::showWhipLeftSub() {
    sub_whip_left_spriteInfo->entry->isHidden = false;
}

void MainDude::showPreWhipLeftSub() {
    sub_pre_whip_left_spriteInfo->entry->isHidden = false;


}

