//
// Created by xdbeef on 17.05.18.
//

#include "../../globals_declarations.h"
#include "../../collisions/collisions.h"
#include "../../../build/gfx_spike_collectibles_flame.h"
#include "../moving_object.h"
#include "bullet.h"

#define BULLET_POSITION_INC_DELTA 15
#define BULLET_ANIM_X_OFFSET 5
#define BULLET_ANIM_Y_OFFSET 5
#define BULLET_ANIM_FRAME_DELTA 50

void Bullet::draw() {


    if (ready_to_dispose) {
        mainSpriteInfo->entry->isHidden = true;
        subSpriteInfo->entry->isHidden = true;
        return;
    } else {
        mainSpriteInfo->entry->isHidden = false;
        subSpriteInfo->entry->isHidden = false;
    }


    if (killed)
        apply_bullet_dispose_animation();

    set_position();

    if(kill_mobs_items_if_thrown(4))
        killed = true;

}


void Bullet::init() {
    initSprite();
}

void Bullet::updateSpeed() {

    if (killed)
        return;

    limit_speed(MAX_X_SPEED_BULLET, MAX_Y_SPEED_BULLET);

    pos_inc_timer += *global::timer;

    bool change_pos = (pos_inc_timer > BULLET_POSITION_INC_DELTA) && !hold_by_main_dude;

    if (change_pos) {
        update_position();
        pos_inc_timer = 0;
    }

}

void Bullet::updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) {

    MapTile *tiles[9];
    Collisions::getNeighboringTiles(global::level_generator->map_tiles, x_current_pos_in_tiles, y_current_pos_in_tiles,
                                    tiles);

    leftCollision = Collisions::checkLeftCollision(tiles, &x, &y, &xSpeed, physical_width, physical_height, false, 0);
    rightCollision = Collisions::checkRightCollision(tiles, &x, &y, &xSpeed, physical_width, physical_height, false, 0);
    upperCollision = Collisions::checkUpperCollision(tiles, &x, &y, &ySpeed, physical_width, false, 0);
    bottomCollision = Collisions::checkBottomCollision(tiles, &x, &y, &ySpeed, physical_width, physical_height, false, 0);

    if (leftCollision || rightCollision || upperCollision || bottomCollision) {
        killed = true;
    }

}

void Bullet::initSprite() {


    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_spike_collectibles_flamePal, gfx_spike_collectibles_flamePalLen,
                                                        nullptr, sprite_width * sprite_height, sprite_width,
                                                        spriteType, true, false, LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_spike_collectibles_flamePal, gfx_spike_collectibles_flamePalLen,
                                                          nullptr, sprite_width * sprite_height, sprite_width,
                                                          spriteType, true, false, LAYER_LEVEL::MIDDLE_TOP);

    frameGfx = (u8 *) gfx_spike_collectibles_flameTiles + (sprite_width * sprite_height * (25) / 2);

    subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
    mainSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
}

void Bullet::set_position() {

    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);

    if (!killed) {
        mainSpriteInfo->entry->x = main_x;
        mainSpriteInfo->entry->y = main_y;
        subSpriteInfo->entry->x = sub_x;
        subSpriteInfo->entry->y = sub_y;
    } else {
        mainSpriteInfo->entry->x = main_x - BULLET_ANIM_X_OFFSET;
        mainSpriteInfo->entry->y = main_y - BULLET_ANIM_Y_OFFSET;
        subSpriteInfo->entry->x = sub_x - BULLET_ANIM_X_OFFSET;
        subSpriteInfo->entry->y = sub_y - BULLET_ANIM_Y_OFFSET;
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

void Bullet::apply_bullet_dispose_animation() {
    animFrameTimer += *global::timer;

    if (animFrameTimer > BULLET_ANIM_FRAME_DELTA) {
        animFrame++;
        animFrameTimer = 0;

        frameGfx = (u8 *) gfx_spike_collectibles_flameTiles + (sprite_width * sprite_height * (25 + animFrame) / 2);
        subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
        mainSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
    }

    if (animFrame >= 6) {
        ready_to_dispose = true;
        mainSpriteInfo->entry->isHidden = true;
        subSpriteInfo->entry->isHidden = true;
    }
}

