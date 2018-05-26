//
// Created by xdbeef on 25.05.18.
//


#include <maxmod9.h>
#include "shopkeeper.h"
#include "../../../build/gfx_shopkeeper.h"
#include "../../globals_declarations.h"
#include "../../../build/soundbank.h"
#include "../../collisions/collisions.h"
#include "../animations/blood.h"

void Shopkeeper::draw() {

    if (ready_to_dispose)
        return;

    set_position();
    mainSpriteInfo->entry->isHidden = false;
    subSpriteInfo->entry->isHidden = false;
    //idk why do i have to do that, if it is already flipped in image
    subSpriteInfo->entry->hFlip = true;
    mainSpriteInfo->entry->hFlip = true;
    subSpriteInfo->entry->vFlip = false;
    mainSpriteInfo->entry->vFlip = false;

/*

    if (!hunting) {
        hunting = abs(x - global::main_dude->x) < 7 * 16 && abs(y - global::main_dude->y) < 7 * 16 &&
                  global::main_dude->y > y;
        if (hunting)
            mmEffect(SFX_XBAT);

    } else
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

*/

    if (global::main_dude->using_whip && !killed && global::main_dude->whip->whip_timer > 120) {


        if (Collisions::checkCollisionWithMainDudeWhip(x, y, 16, 16)) {
            kill();
        }
    }


    if (!killed && Collisions::checkCollisionWithMainDude(x, y, 16, 16) && global::main_dude->ySpeed > 0 &&
        global::main_dude->y - 4 < y) {
        kill();
        global::main_dude->ySpeed = -2;
        global::main_dude->jumping_timer = 0;
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
            mmEffect(SFX_XDIE);
        } else
            mmEffect(SFX_XHIT);


    }
/*
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
    */
}


void Shopkeeper::init() {

    initSprite();

    frameGfx = (u8 *) gfx_shopkeeperTiles;
    subSpriteInfo->updateFrame(frameGfx, 16 * 16);
    mainSpriteInfo->updateFrame(frameGfx, 16 * 16);

    spriteType = SpritesheetType::SHOPKEEPER;
}

void Shopkeeper::updateSpeed() {

    if (xSpeed > MAX_X_SPEED_SHOPKEEPER)
        xSpeed = MAX_X_SPEED_SHOPKEEPER;
    if (xSpeed < -MAX_X_SPEED_SHOPKEEPER)
        xSpeed = -MAX_X_SPEED_SHOPKEEPER;

    if (ySpeed > MAX_Y_SPEED_SHOPKEEPER)
        ySpeed = MAX_Y_SPEED_SHOPKEEPER;
    if (ySpeed < -MAX_Y_SPEED_SHOPKEEPER)
        ySpeed = -MAX_Y_SPEED_SHOPKEEPER;

    pos_inc_timer += *global::timer;

    if (pos_inc_timer > 30) {
        update_position();
        pos_inc_timer = 0;
    }

}


void Shopkeeper::updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) {

    MapTile *tiles[9] = {};
    Collisions::getNeighboringTiles(global::level_generator->map_tiles, x_current_pos_in_tiles,
                                    y_current_pos_in_tiles, tiles);

    standingOnLeftEdge = Collisions::isStandingOnLeftEdge(tiles, x, 16, x_current_pos_in_tiles);
    standingOnRightEdge = Collisions::isStandingOnRightEdge(tiles, x, 16, x_current_pos_in_tiles);
    bottomCollision = Collisions::checkBottomCollision(tiles, &x, &y, &ySpeed, 16, 16, false);
    leftCollision = Collisions::checkLeftCollision(tiles, &x, &y, &xSpeed, 16, 16, false);
    rightCollision = Collisions::checkRightCollision(tiles, &x, &y, &xSpeed, 16, 16, false);
    upperCollision = Collisions::checkUpperCollision(tiles, &x, &y, &ySpeed, 16, false);

//    hanging = upperCollision && !hunting;

}

void Shopkeeper::kill() {
    subSpriteInfo->entry->isHidden = true;
    mainSpriteInfo->entry->isHidden = true;

    subSpriteInfo = nullptr;
    mainSpriteInfo = nullptr;

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

void Shopkeeper::initSprite() {
    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_shopkeeperPal, gfx_shopkeeperPalLen,
                                                        nullptr, 16 * 16, 16, BAT, true, false,LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_shopkeeperPal, gfx_shopkeeperPalLen,
                                                          nullptr, 16 * 16, 16, BAT, true, false,LAYER_LEVEL::MIDDLE_TOP);
    subSpriteInfo->entry->isHidden = false;
    mainSpriteInfo->entry->isHidden = false;
/*
    sub_sprite_info->entry->vFlip = false;
    sub_sprite_info->entry->hFlip= false;

    main_sprite_info->entry->vFlip = false;
    main_sprite_info->entry->hFlip= false;*/
}

void Shopkeeper::set_position() {

    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);

    mainSpriteInfo->entry->x = main_x;
    mainSpriteInfo->entry->y = main_y;

    subSpriteInfo->entry->x = sub_x;
    subSpriteInfo->entry->y = sub_y;

}

Shopkeeper::Shopkeeper() {
    physical_height = SHOPKEEPER_PHYSICAL_HEIGHT;
    physical_width = SHOPKEEPER_PHYSICAL_WIDTH;
    sprite_height = SHOPKEEPER_SPRITE_HEIGHT;
    sprite_width = SHOPKEEPER_SPRITE_WIDTH;
}
