//
// Created by xdbeef on 03.05.18.
//

#include <maxmod9.h>
#include "spider.h"
#include "../../globals_declarations.h"
#include "../animations/blood.h"
#include "../../../build/gfx_spider.h"
#include "../items/rock.h"
#include "../../collisions/collisions.h"
#include "../../tiles/map_utils.h"
#include "../../../build/soundbank.h"

#define SPIDER_POS_INC_DELTA 30
#define SPIDER_HANGING_OFFSET 8
#define SPIDER_ANIM_FRAME_DELTA 75

/**
 * If hunting;
 * - if in air, have the same constant speed,
 * - if on the ground, check main-dude's position, set own speed to catch him up, bounce up
 * - bounce velocity should be random
 */
void Spider::draw() {

    if (ready_to_dispose)
        return;

    set_position();

    subSpriteInfo->entry->hFlip = true;
    mainSpriteInfo->entry->hFlip = true;
    mainSpriteInfo->entry->isHidden = false;
    subSpriteInfo->entry->isHidden = false;

    if (!hunting)
        hunting = abs(y - global::main_dude->y) < 9 * TILE_H && global::main_dude->x + MAIN_DUDE_PHYSICAL_WIDTH > x &&
                  global::main_dude->x < x + physical_width && global::main_dude->y > y;
    else
        animFrameTimer += *global::timer;

    if (animFrameTimer > SPIDER_ANIM_FRAME_DELTA) {
        animFrame++;

        if (jumping && animFrame >= 4) {
            animFrame = 0;
            jumping = false;
        }

        if (hanging && hunting && animFrame >= 7) {
            animFrame = 0;
            hanging = false;
        }

        if (hanging && !hunting)
            set_sprite_hanging();
        else if (hanging && hunting)
            set_sprite_flipping();
        else if (!hanging && hunting && jumping)
            set_sprite_jumping();
        else
            set_sprite_falling();

        subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
        mainSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);

        animFrameTimer = 0;
    }


    kill_if_whip();
    kill_if_main_dude_jumped_on_you();
    deal_damage_main_dude_on_collision();

    if (hunting && bottomCollision && !hanging) {
        jump_to_main_dude();
    }
}


void Spider::init() {
    initSprite();
    random_speed = 0;
    hanging = true;
}

void Spider::updateSpeed() {

    xSpeed = random_speed;

    limit_speed(MAX_X_SPEED_SPIDER, MAX_Y_SPEED_SPIDER);

    pos_inc_timer += *global::timer;

    if (pos_inc_timer > SPIDER_POS_INC_DELTA) {
        update_position();
        if (hunting)
            apply_gravity(GRAVITY_DELTA_SPEED * 0.8);

        pos_inc_timer = 0;
    }

}


void Spider::updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) {

    MapTile *tiles[9] = {};
    Collisions::getNeighboringTiles(global::level_generator->map_tiles, x_current_pos_in_tiles,
                                    y_current_pos_in_tiles, tiles);

    standingOnLeftEdge = Collisions::isStandingOnLeftEdge(tiles, x, physical_width, x_current_pos_in_tiles);
    standingOnRightEdge = Collisions::isStandingOnRightEdge(tiles, x, physical_width, x_current_pos_in_tiles);
    bottomCollision = Collisions::checkBottomCollision(tiles, &x, &y, &ySpeed, physical_width, physical_height, false);
    leftCollision = Collisions::checkLeftCollision(tiles, &x, &y, &xSpeed, physical_width, physical_height, false);
    rightCollision = Collisions::checkRightCollision(tiles, &x, &y, &xSpeed, physical_width, physical_height, false);
    upperCollision = Collisions::checkUpperCollision(tiles, &x, &y, &ySpeed, physical_width, false);

}

void Spider::kill() {
    subSpriteInfo->entry->isHidden = true;
    mainSpriteInfo->entry->isHidden = true;

    spawn_blood();

    killed = true;
    ready_to_dispose = true;
    global::hud->draw();
    global::killed_npcs.push_back(spriteType);

}

void Spider::initSprite() {

    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_spiderPal, gfx_spiderPalLen,
                                                        nullptr, sprite_width * sprite_height, 16, SPIDER, true, false,
                                                        LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_spiderPal, gfx_spiderPalLen,
                                                          nullptr, sprite_width * sprite_height, 16, SPIDER, true,
                                                          false,
                                                          LAYER_LEVEL::MIDDLE_TOP);

    frameGfx = (u8 *) gfx_spiderTiles + sprite_width * sprite_height * 4 / 2;
    subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
    mainSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);

    spriteType = SpritesheetType::SPIDER;


    subSpriteInfo->entry->isHidden = false;
    mainSpriteInfo->entry->isHidden = false;

    subSpriteInfo->entry->vFlip = false;
    subSpriteInfo->entry->hFlip = false;

    mainSpriteInfo->entry->vFlip = false;
    mainSpriteInfo->entry->hFlip = false;
}

void Spider::set_sprite_hanging() {
    frameGfx = (u8 *) gfx_spiderTiles + (sprite_width * sprite_height * (4) / 2);
}

void Spider::set_sprite_flipping() {
    frameGfx = (u8 *) gfx_spiderTiles + (sprite_width * sprite_height * (animFrame + 4) / 2);
}

void Spider::set_sprite_jumping() {
    frameGfx = (u8 *) gfx_spiderTiles + (sprite_width * sprite_height * (animFrame) / 2);
}

void Spider::set_sprite_falling() {
    frameGfx = (u8 *) gfx_spiderTiles + (sprite_width * sprite_height * (0) / 2);

    subSpriteInfo->entry->hFlip = true;
    mainSpriteInfo->entry->hFlip = true;
    mainSpriteInfo->entry->isHidden = false;
    subSpriteInfo->entry->isHidden = false;
}


void Spider::set_position() {

    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);

    mainSpriteInfo->entry->x = main_x;
    subSpriteInfo->entry->x = sub_x;

    if (!hanging) {
        mainSpriteInfo->entry->y = main_y - SPIDER_HANGING_OFFSET;
        subSpriteInfo->entry->y = sub_y - SPIDER_HANGING_OFFSET;
    } else {
        mainSpriteInfo->entry->y = main_y;
        subSpriteInfo->entry->y = sub_y;
    }
}

Spider::Spider() {
    physical_height = SPIDER_PHYSICAL_HEIGHT;
    physical_width = SPIDER_PHYSICAL_WIDTH;
    sprite_height = SPIDER_SPRITE_HEIGHT;
    sprite_width = SPIDER_SPRITE_WIDTH;
}

void Spider::jump_to_main_dude() {
    int diff = global::main_dude->x - x > 0;

    if (diff > 40)
        diff = 40;

    if (diff)
        random_speed = 1.1 + ((rand() % diff) / 10);
    else
        random_speed = -1.1 - ((rand() % diff) / 10);

    jumping = true;
    animFrame = 0;
    ySpeed = -1.5 - ((rand() % diff) / 5);
}
