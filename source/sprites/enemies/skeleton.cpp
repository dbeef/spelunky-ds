//
// Created by xdbeef on 22.07.18.
//

#include "skeleton.h"


#include <maxmod9.h>
#include <cstdlib>
#include "snake.h"
#include "../../globals_declarations.h"
#include "../items/rock.h"
#include "../../../build/gfx_bat_snake_jetpack.h"
#include "../animations/blood.h"
#include "../../collisions/collisions.h"
#include "../../tiles/map_utils.h"
#include "../../../build/soundbank.h"

#define SKELETON_POS_INC_DELTA 35
#define SKELETON_ANIM_FRAME_DELTA 125
#define SKELETON_HITPOINTS 1

void Skeleton::draw() {

    if (ready_to_dispose)
        return;

    set_position();

    mainSpriteInfo->entry->hFlip = true;
    subSpriteInfo->entry->hFlip = true;

    mainSpriteInfo->entry->vFlip = false;
    subSpriteInfo->entry->vFlip = false;

    mainSpriteInfo->entry->isHidden = false;
    subSpriteInfo->entry->isHidden = false;

    animFrameTimer += *global::timer;

    if (animFrameTimer > SKELETON_ANIM_FRAME_DELTA) {

        animFrame++;
        if (animFrame >= 4)
            animFrame = 0;

        if (spriteState == SpriteState::W_LEFT) {
            set_sprite_left();
        } else if (spriteState == SpriteState::W_RIGHT) {
            set_sprite_right();
        }
        animFrameTimer = 0;

    }


/*
    if (bottomCollision) {
        if (waitTimer > 0) {
            waitTimer -= *global::timer;
        } else {
            if (goTimer > 0)


                goTimer -= *global::timer;

            if (standingOnLeftEdge && spriteState == 1 || standingOnRightEdge && spriteState == 0) {
                xSpeed = 0;
            } else if (spriteState == 0)
                xSpeed = 0.5;
            else
                xSpeed = -0.5;

            if (goTimer <= 0) {
                randomizeMovement();
                xSpeed = 0;
            }
        }
    }
*/


    kill_if_whip(1);
    kill_if_main_dude_jumped_on_you(1);
    deal_damage_main_dude_on_collision(1);
}


void Skeleton::init() {

    initSprite();

    frameGfx = (u8 *) gfx_bat_snake_jetpackTiles + ((sprite_width * sprite_height * (9)) / 2);
    subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
    mainSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);

    spritesheet_type = SpritesheetType::SKELETON;
    sprite_type = SpriteType::S_SKELETON;

    randomizeMovement();
}


void Skeleton::updateSpeed() {

    limit_speed(MAX_X_SPEED_SKELETON, MAX_Y_SPEED_SKELETON);

    pos_inc_timer += *global::timer;

    if (pos_inc_timer > SKELETON_POS_INC_DELTA) {
        update_position();
        apply_gravity(GRAVITY_DELTA_SPEED);
        pos_inc_timer = 0;
    }

}

void Skeleton::updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) {

    MapTile *tiles[9] = {};
    Collisions::getNeighboringTiles(global::level_generator->map_tiles, x_current_pos_in_tiles,
                                    y_current_pos_in_tiles, tiles);

    standingOnLeftEdge = Collisions::isStandingOnLeftEdge(tiles, x, physical_width, x_current_pos_in_tiles);
    standingOnRightEdge = Collisions::isStandingOnRightEdge(tiles, x, physical_width, x_current_pos_in_tiles);
    bottomCollision = Collisions::checkBottomCollision(tiles, &x, &y, &ySpeed, physical_width, physical_height, false, 0);
    leftCollision = Collisions::checkLeftCollision(tiles, &x, &y, &xSpeed, physical_width, physical_height, false, 0);
    rightCollision = Collisions::checkRightCollision(tiles, &x, &y, &xSpeed, physical_width, physical_height, false, 0);
    upperCollision = Collisions::checkUpperCollision(tiles, &x, &y, &ySpeed, physical_width, false, 0);

}

void Skeleton::apply_dmg(int dmg_to_apply) {

    //snake has only 1 dmg point, always kill if any dmg_apply

    subSpriteInfo->entry->isHidden = true;
    mainSpriteInfo->entry->isHidden = true;

    subSpriteInfo = nullptr;
    mainSpriteInfo = nullptr;

    spawn_blood();
    killed = true;
    ready_to_dispose = true;
    global::hud->draw();
    global::killed_npcs.push_back(spritesheet_type);

}

void Skeleton::initSprite() {
    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_bat_snake_jetpackPal, gfx_bat_snake_jetpackPalLen,
                                                        nullptr, sprite_width * sprite_height, 16, SKELETON, true, false,
                                                        LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_bat_snake_jetpackPal, gfx_bat_snake_jetpackPalLen,
                                                          nullptr, sprite_width * sprite_height, 16, SKELETON, true, false,
                                                          LAYER_LEVEL::MIDDLE_TOP);
    subSpriteInfo->entry->isHidden = false;
    mainSpriteInfo->entry->isHidden = false;

    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);

    mainSpriteInfo->entry->x = main_x;
    mainSpriteInfo->entry->y = main_y;

    subSpriteInfo->entry->x = sub_x;
    subSpriteInfo->entry->y = sub_y;


}

void Skeleton::set_position() {

    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);

    mainSpriteInfo->entry->x = main_x;
    mainSpriteInfo->entry->y = main_y;

    subSpriteInfo->entry->x = sub_x;
    subSpriteInfo->entry->y = sub_y;
}

void Skeleton::set_sprite_left() {
    frameGfx = (u8 *) gfx_bat_snake_jetpackTiles + ((sprite_width * sprite_height * (animFrame + 13)) / 2);
    subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
    mainSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
    mainSpriteInfo->entry->hFlip = true;
    subSpriteInfo->entry->hFlip = true;

    mainSpriteInfo->entry->isHidden = false;
    subSpriteInfo->entry->isHidden = false;
}

void Skeleton::set_sprite_right() {
    frameGfx = (u8 *) gfx_bat_snake_jetpackTiles + ((sprite_width * sprite_height * (animFrame + 9)) / 2);
    subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
    mainSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
    mainSpriteInfo->entry->hFlip = true;
    subSpriteInfo->entry->hFlip = true;

    mainSpriteInfo->entry->isHidden = false;
    subSpriteInfo->entry->isHidden = false;
}

Skeleton::Skeleton() {
    physical_height = SKELETON_PHYSICAL_HEIGHT;
    physical_width = SKELETON_PHYSICAL_WIDTH;
    sprite_height = SKELETON_SPRITE_HEIGHT;
    sprite_width = SKELETON_SPRITE_WIDTH;
    hitpoints = SKELETON_HITPOINTS;
}

