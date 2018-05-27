//
// Created by xdbeef on 02.05.18.
//

#include <maxmod9.h>
#include "bat.h"
#include "../../globals_declarations.h"
#include "../../../build/gfx_bat.h"
#include "../items/rock.h"
#include "../animations/blood.h"
#include "../../collisions/collisions.h"
#include "../../tiles/map_utils.h"
#include "../../../build/soundbank.h"

void Bat::draw() {

    if (ready_to_dispose)
        return;

    set_position();
    mainSpriteInfo->entry->isHidden = false;
    subSpriteInfo->entry->isHidden = false;
    subSpriteInfo->entry->hFlip = true;
    mainSpriteInfo->entry->hFlip = true;
    subSpriteInfo->entry->vFlip = false;
    mainSpriteInfo->entry->vFlip = false;


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
    subSpriteInfo->updateFrame(frameGfx, sprite_height * sprite_width);
    mainSpriteInfo->updateFrame(frameGfx, sprite_height * sprite_width);

    spriteType = SpritesheetType::BAT;
}

void Bat::updateSpeed() {

    limit_speed(MAX_X_SPEED_BAT, MAX_Y_SPEED_BAT);

    pos_inc_timer += *global::timer;

    if (pos_inc_timer > 30) {
        update_position();
        pos_inc_timer = 0;
    }

}


void Bat::updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) {

    MapTile *tiles[9] = {};
    Collisions::getNeighboringTiles(global::level_generator->map_tiles, x_current_pos_in_tiles,
                                    y_current_pos_in_tiles, tiles);

    standingOnLeftEdge = Collisions::isStandingOnLeftEdge(tiles, x, physical_width, x_current_pos_in_tiles);
    standingOnRightEdge = Collisions::isStandingOnRightEdge(tiles, x, 16, x_current_pos_in_tiles);
    bottomCollision = Collisions::checkBottomCollision(tiles, &x, &y, &ySpeed, physical_width, physical_height, false);
    leftCollision = Collisions::checkLeftCollision(tiles, &x, &y, &xSpeed, physical_width, physical_height, false);
    rightCollision = Collisions::checkRightCollision(tiles, &x, &y, &xSpeed, physical_width, physical_height, false);
    upperCollision = Collisions::checkUpperCollision(tiles, &x, &y, &ySpeed, physical_width, false);

    hanging = upperCollision && !hunting;

}

void Bat::kill() {
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

void Bat::initSprite() {
    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_batPal, gfx_batPalLen,
                                                        nullptr, sprite_width * sprite_height, 16, BAT, true, false,
                                                        LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_batPal, gfx_batPalLen,
                                                          nullptr, sprite_width * sprite_height, 16, BAT, true, false,
                                                          LAYER_LEVEL::MIDDLE_TOP);
    subSpriteInfo->entry->isHidden = false;
    mainSpriteInfo->entry->isHidden = false;
}

void Bat::set_sprite_hanging() {
    frameGfx = (u8 *) gfx_batTiles + (sprite_width * sprite_height * (0) / 2);
    subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
    mainSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
}

void Bat::set_sprite_flying_right() {
    frameGfx = (u8 *) gfx_batTiles + (sprite_width * sprite_height * (animFrame + 1) / 2);
    subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
    mainSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);

    //idk why do i have to do that, if it is already flipped in image
    subSpriteInfo->entry->hFlip = true;
    mainSpriteInfo->entry->hFlip = true;

    mainSpriteInfo->entry->isHidden = false;
    subSpriteInfo->entry->isHidden = false;
}

void Bat::set_sprite_flying_left() {
    frameGfx = (u8 *) gfx_batTiles + (sprite_width * sprite_height * (animFrame + 4) / 2);
    subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
    mainSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);

    subSpriteInfo->entry->hFlip = true;
    mainSpriteInfo->entry->hFlip = true;

    mainSpriteInfo->entry->isHidden = false;
    subSpriteInfo->entry->isHidden = false;
}

void Bat::set_position() {

    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);

    mainSpriteInfo->entry->x = main_x;
    mainSpriteInfo->entry->y = main_y;

    subSpriteInfo->entry->x = sub_x;
    subSpriteInfo->entry->y = sub_y;

}

Bat::Bat() {
    physical_height = BAT_PHYSICAL_HEIGHT;
    physical_width = BAT_PHYSICAL_WIDTH;
    sprite_height = BAT_SPRITE_HEIGHT;
    sprite_width = BAT_SPRITE_WIDTH;
}
