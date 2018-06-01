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
#define BAT_ANIM_FRAME_DELTA 100
#define BAT_POS_INC_DELTA 30
#define BAT_HITPOINTS 1

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

    if (animFrameTimer > BAT_ANIM_FRAME_DELTA) {

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

    kill_if_whip(1);
    kill_if_main_dude_jumped_on_you(1);
    deal_damage_main_dude_on_collision(1);

    if (hunting) {
        follow_main_dude();
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

    if (pos_inc_timer > BAT_POS_INC_DELTA) {
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
    bottomCollision = Collisions::checkBottomCollision(tiles, &x, &y, &ySpeed, physical_width, physical_height, false, 0);
    leftCollision = Collisions::checkLeftCollision(tiles, &x, &y, &xSpeed, physical_width, physical_height, false, 0);
    rightCollision = Collisions::checkRightCollision(tiles, &x, &y, &xSpeed, physical_width, physical_height, false, 0);
    upperCollision = Collisions::checkUpperCollision(tiles, &x, &y, &ySpeed, physical_width, false, 0);

    hanging = upperCollision && !hunting;

}

void Bat::apply_dmg(int dmg_to_apply) {

    //bat has only 1 dmg point, always kill if any dmg_apply

    subSpriteInfo->entry->isHidden = true;
    mainSpriteInfo->entry->isHidden = true;

    spawn_blood();

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
    hitpoints = BAT_HITPOINTS;
}

void Bat::follow_main_dude() {
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
}
