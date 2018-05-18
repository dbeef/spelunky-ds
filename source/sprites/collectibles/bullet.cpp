//
// Created by xdbeef on 17.05.18.
//

#include "../../globals_declarations.h"
#include "../../collisions/collisions.h"
#include "../../../build/gfx_spike_collectibles.h"
#include "../moving_object.h"
#include "bullet.h"

void Bullet::draw() {

    if (ready_to_dispose)
        return;

    if (xSpeed > 0 || ySpeed > 0) {
        for (int a = 0; a < global::sprites.size(); a++) {
            if ((global::sprites.at(a)->spriteType == SpritesheetType::SNAKE ||
                 global::sprites.at(a)->spriteType == SpritesheetType::BAT ||
                 global::sprites.at(a)->spriteType == SpritesheetType::SPIDER)
                && !global::sprites.at(a)->killed) {
                if (Collisions::checkCollisionBodies(x, y, 4, 4, global::sprites.at(a)->x, global::sprites.at(a)->y, 16, 16)) {
                    global::sprites.at(a)->kill();
                }
            }
        }
    }

    if(killed){
        animFrameTimer += *global::timer;


        if(animFrameTimer > 50){
            animFrame++;
            animFrameTimer = 0;

            frameGfx = (u8 *) gfx_spike_collectiblesTiles + (sprite_width * sprite_height * (25 + animFrame) / 2);
            subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
            mainSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
        }



        if(animFrame >= 6){
            ready_to_dispose = true;
            mainSpriteInfo->entry->isHidden = true;
            subSpriteInfo->entry->isHidden = true;
        }
    }

    set_position();
}


void Bullet::init() {
    initSprite();
}

void Bullet::updateSpeed() {

    if(killed)
        return;

    if (xSpeed > MAX_X_SPEED_BULLET)
        xSpeed = MAX_X_SPEED_BULLET;
    if (xSpeed < -MAX_X_SPEED_BULLET)
        xSpeed = -MAX_X_SPEED_BULLET;

    if (ySpeed > MAX_Y_SPEED_BULLET)
        ySpeed = MAX_Y_SPEED_BULLET;
    if (ySpeed < -MAX_Y_SPEED_BULLET)
        ySpeed = -MAX_Y_SPEED_BULLET;

    pos_inc_timer += *global::timer;

    bool change_pos = (pos_inc_timer > 15) && !hold_by_main_dude;

    if (change_pos) {
        updatePosition();

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

        pos_inc_timer = 0;

    }

}

void Bullet::updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) {

    MapTile *tiles[9];
    Collisions::getNeighboringTiles(global::level_generator->map_tiles, x_current_pos_in_tiles, y_current_pos_in_tiles,
                                    tiles);

    leftCollision = Collisions::checkLeftCollision(tiles, &x, &y, &xSpeed, physical_width, physical_height, false);
    rightCollision = Collisions::checkRightCollision(tiles, &x, &y, &xSpeed, physical_width, physical_height, false);
    upperCollision = Collisions::checkUpperCollision(tiles, &x, &y, &ySpeed, physical_width, false);
    bottomCollision = Collisions::checkBottomCollision(tiles, &x, &y, &ySpeed, physical_width, physical_height, false);

    if(leftCollision || rightCollision || upperCollision || bottomCollision){
        killed = true;
    }

}

void Bullet::initSprite() {


    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_spike_collectiblesPal, gfx_spike_collectiblesPalLen,
                                                        nullptr, sprite_width * sprite_height, sprite_width,
                                                        spriteType, true, false);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_spike_collectiblesPal, gfx_spike_collectiblesPalLen,
                                                          nullptr, sprite_width * sprite_height, sprite_width,
                                                          spriteType, true, false);

    frameGfx = (u8 *) gfx_spike_collectiblesTiles + (sprite_width * sprite_height * (25) / 2);

    subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
    mainSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
}

void Bullet::set_position() {

    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);

    if(!killed) {
        mainSpriteInfo->entry->x = main_x;
        mainSpriteInfo->entry->y = main_y;
        subSpriteInfo->entry->x = sub_x;
        subSpriteInfo->entry->y = sub_y;
    }
    else
    {
        mainSpriteInfo->entry->x = main_x - 5;
        mainSpriteInfo->entry->y = main_y - 5;
        subSpriteInfo->entry->x = sub_x - 5;
        subSpriteInfo->entry->y = sub_y - 5;

    }

    mainSpriteInfo->entry->vFlip = false;
    mainSpriteInfo->entry->hFlip = false;

    subSpriteInfo->entry->vFlip = false;
    subSpriteInfo->entry->hFlip = false;

}

Bullet::Bullet() {
    physical_height = BULLET_PHYSICAL_HEIGHT;
    physical_width = BULLET_PHYSICAL_WIDTH;
    sprite_height = BULLET_SPRITE_HEIGHT;
    sprite_width = BULLET_SPRITE_WIDTH;
    spriteType = SpritesheetType::SPIKES_COLLECTIBLES;
}

