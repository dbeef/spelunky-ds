//
// Created by xdbeef on 02.05.18.
//

#include "bat.h"
#include "../../globals_declarations.h"
#include "../../../build/gfx_bat.h"
#include "../items/rock.h"
#include "../animations/blood.h"
#include "../../collisions/collisions.h"
#include "../../tiles/map_utils.h"

void Bat::draw() {

    if(ready_to_dispose)
        return;

    set_position();

    mainSpriteInfo->entry->isHidden = false;
    subSpriteInfo->entry->isHidden = false;
    //idk why do i have to do that, if it is already flipped in image
    subSpriteInfo->entry->hFlip = true;
    mainSpriteInfo->entry->hFlip = true;


    if (!hunting)
        hunting = abs(x - global::main_dude->x) < 7 * 16 && abs(y - global::main_dude->y) < 7 * 16 &&
                  global::main_dude->y > y;
    else
        hunting = abs(x - global::main_dude->x) < 9 * 16 && abs(y - global::main_dude->y) < 9 * 16;

    animFrameTimer += *global::timer;

    if (animFrameTimer > 100) {
        animFrame++;

        if (animFrame >= 3)
            animFrame = 0;

        if (hanging) {
            set_sprite_hanging();
        } else if (xSpeed >= 0) {
            set_sprite_flying_right();
        } else if (xSpeed <= 0) {
            set_sprite_flying_left();
        }

        animFrameTimer = 0;
        }



    if (global::main_dude->whip && !killed && global::main_dude->whip_timer > 120) {


        if (Collisions::checkCollisionWithMainDudeWhip(x, y, 16, 16)) {
            kill();
        }
    }


    if (!killed && Collisions::checkCollisionWithMainDude(x, y, 16, 16) && global::main_dude->ySpeed > 0 &&
        global::main_dude->y - 4 < y) {
        kill();
        global::main_dude->ySpeed = -2;
        global::main_dude->jumpingTimer = 0;
    }


    if (!killed && !global::main_dude->dead && Collisions::checkCollisionWithMainDude(x, y, 16, 16) &&
        global::main_dude->time_since_last_damage > 1000) {

        global::main_dude->time_since_last_damage = 0;
        global::hud->hearts--;
        global::hud->draw();

        if (global::hud->hearts == 0) {
            global::hud->hide();
            global::main_dude->ySpeed = -MAIN_DUDE_JUMP_SPEED * 0.25;
            global::main_dude->dead = true;
        }

    }

    if (hunting) {
        if (global::main_dude->x > x)
            xSpeed = 1;
        else
            xSpeed = -1;


        if (global::main_dude->y == y)
            ySpeed = 0;
        else if (global::main_dude->y > y)
            ySpeed = 1;
        else
            ySpeed = -1;
    } else {
        xSpeed = 0;
        ySpeed = -1;
    }
}


void Bat::init() {

    initSprite();

    frameGfx = (u8 *) gfx_batTiles;
    subSpriteInfo->updateFrame(frameGfx, 16 * 16);
    mainSpriteInfo->updateFrame(frameGfx, 16 * 16);

    spriteType = SpritesheetType ::BAT;
}

void Bat::updateSpeed() {

    if (xSpeed > MAX_X_SPEED_ROCK)
        xSpeed = MAX_X_SPEED_ROCK;
    if (xSpeed < -MAX_X_SPEED_ROCK)
        xSpeed = -MAX_X_SPEED_ROCK;

    if (ySpeed > MAX_Y_SPEED_ROCK)
        ySpeed = MAX_Y_SPEED_ROCK;
    if (ySpeed < -MAX_Y_SPEED_ROCK)
        ySpeed = -MAX_Y_SPEED_ROCK;

    pos_inc_timer += *global::timer;

    bool change_pos = (pos_inc_timer > 30);

    if (change_pos) {
        updatePosition();
    }

}

void Bat::updatePosition() {

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
    pos_inc_timer = 0;
}

void Bat::updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) {

    MapTile *tiles[9] = {};
    Collisions::getNeighboringTiles(global::level_generator->mapTiles, x_current_pos_in_tiles,
                                    y_current_pos_in_tiles, tiles);

    standingOnLeftEdge = Collisions::isStandingOnLeftEdge(tiles, x, 16, x_current_pos_in_tiles);
    standingOnRightEdge = Collisions::isStandingOnRightEdge(tiles, x, 16, x_current_pos_in_tiles);
    bottomCollision = Collisions::checkBottomCollision(tiles, &x, &y, &ySpeed, 16, 16, false);
    leftCollision = Collisions::checkLeftCollision(tiles, &x, &y, &xSpeed, 16, 16, false);
    rightCollision = Collisions::checkRightCollision(tiles, &x, &y, &xSpeed, 16, 16, false);
    upperCollision = Collisions::checkUpperCollision(tiles, &x, &y, &ySpeed, 16, false);

    hanging = upperCollision && !hunting;

}

void Bat::kill() {
    subSpriteInfo->entry->isHidden = true;
    mainSpriteInfo->entry->isHidden = true;

    subSpriteInfo = nullptr;
    mainSpriteInfo= nullptr;

    for (int a = 0; a < 4; a++) {
        Blood *blood = new Blood();
        blood->init();
        blood->x = x;
        blood->y = y;

        if (a % 2 == 0)
            blood->xSpeed = (1.3 / a);
        else
            blood->xSpeed = (-1.3 / a);

        blood->ySpeed = -2 / a;
        global::sprites.push_back(blood);
    }
    killed = true;
    ready_to_dispose = true;
    global::hud->draw();
    global::killed_npcs.push_back(spriteType);


}

void Bat::initSprite() {
    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_batPal, gfx_batPalLen,
                                                        nullptr, 16 * 16, 16, BAT, true, false);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_batPal, gfx_batPalLen,
                                                          nullptr, 16 * 16, 16, BAT, true, false);
    subSpriteInfo->entry->isHidden = false;
    mainSpriteInfo->entry->isHidden = false;
/*
    subSpriteInfo->entry->vFlip = false;
    subSpriteInfo->entry->hFlip= false;

    mainSpriteInfo->entry->vFlip = false;
    mainSpriteInfo->entry->hFlip= false;*/
}

void Bat::set_sprite_hanging(){
    frameGfx = (u8 *) gfx_batTiles + (16 * 16 * (0) / 2);
    subSpriteInfo->updateFrame(frameGfx, 16 * 16);
    mainSpriteInfo->updateFrame(frameGfx, 16 * 16);
}

void Bat::set_sprite_flying_right(){
    frameGfx = (u8 *) gfx_batTiles + (16 * 16 * (animFrame + 1) / 2);
    subSpriteInfo->updateFrame(frameGfx, 16 * 16);
    mainSpriteInfo->updateFrame(frameGfx, 16 * 16);

    //idk why do i have to do that, if it is already flipped in image
    subSpriteInfo->entry->hFlip = true;
    mainSpriteInfo->entry->hFlip = true;

    mainSpriteInfo->entry->isHidden = false;
    subSpriteInfo->entry->isHidden = false;
}

void Bat::set_sprite_flying_left() {
    frameGfx = (u8 *) gfx_batTiles + (16 * 16 * (animFrame + 4) / 2);
    subSpriteInfo->updateFrame(frameGfx, 16 * 16);
    mainSpriteInfo->updateFrame(frameGfx, 16 * 16);

    //idk why do i have to do that, if it is already flipped in image
    subSpriteInfo->entry->hFlip = true;
    mainSpriteInfo->entry->hFlip = true;

    mainSpriteInfo->entry->isHidden = false;
    subSpriteInfo->entry->isHidden = false;
}

void Bat::set_position() {

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

    mainSpriteInfo->entry->x = main_x;
    mainSpriteInfo->entry->y = main_y;

    subSpriteInfo->entry->x = sub_x;
    subSpriteInfo->entry->y = sub_y;

}
