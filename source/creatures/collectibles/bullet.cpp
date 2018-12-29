//
// Created by xdbeef on 17.05.18.
//

#include "../../globals_declarations.hpp"
#include "../../collisions/collisions.hpp"
#include "../../../build/gfx_spike_collectibles_flame.h"
#include "../_base_creature.h"
#include "bullet.hpp"
#include "../../tiles/level_rendering_utils.hpp"
#include "../sprite_utils.hpp"

#define BULLET_POSITION_INC_DELTA 15
#define BULLET_ANIM_X_OFFSET 5
#define BULLET_ANIM_Y_OFFSET 5
#define BULLET_ANIM_FRAME_DELTA 50

void Bullet::draw() {

    if (ready_to_dispose)
        return;

    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_visibility(true, mainSpriteInfo, subSpriteInfo);
    set_position();

    lifetime += *global::timer;

    if (killed) {

        animFrameTimer += *global::timer;

        if (animFrameTimer > BULLET_ANIM_FRAME_DELTA) {

            animFrame++;
            animFrameTimer = 0;

            match_animation();
        }

        if (animFrame >= 6) {
            ready_to_dispose = true;
            sprite_utils::set_visibility(false, mainSpriteInfo, subSpriteInfo);
        }

    } else if (kill_mobs_items_if_thrown(4) || kill_main_dude_if_thrown(4))
        killed = true;

}


void Bullet::init() {
    //checking this in case of firing just in front of a wall 
    int xx = floor_div(this->x + 0.5 * physical_width, TILE_W);
    int yy = floor_div(this->y + 0.5 * physical_height, TILE_H);
    updateCollisionsMap(xx, yy);
    if (killed) {
        ready_to_dispose = true;
        sprite_utils::set_visibility(false, mainSpriteInfo, subSpriteInfo);
    } else
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

    if (killed)
        return;

    MapTile *t[9];
    Collisions::getNeighboringTiles(global::current_level->map_tiles,
                                    x_current_pos_in_tiles, y_current_pos_in_tiles, t);

    upperCollision = Collisions::checkUpperCollision(t, &x, &y, &ySpeed, physical_width, false, 0);
    if (upperCollision) {
        killed = true;
        return;
    }

    leftCollision = Collisions::checkLeftCollision(t, &x, &y, &xSpeed, physical_width, physical_height, false, 0);
    if (leftCollision) {
        killed = true;
        return;
    }

    rightCollision = Collisions::checkRightCollision(t, &x, &y, &xSpeed, physical_width, physical_height, false, 0);
    if (rightCollision) {
        killed = true;
        return;
    }


    bottomCollision = Collisions::checkBottomCollision(t, &x, &y, &ySpeed, physical_width, physical_height, false,
                                                       0);
    if (bottomCollision) {
        killed = true;
        return;
    }

}

void Bullet::initSprite() {

    delete mainSpriteInfo;
    delete subSpriteInfo;

    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_spike_collectibles_flamePal,
                                                        gfx_spike_collectibles_flamePalLen,
                                                        nullptr, BULLET_SPRITE_SIZE, sprite_width,
                                                        spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_spike_collectibles_flamePal,
                                                          gfx_spike_collectibles_flamePalLen,
                                                          nullptr, BULLET_SPRITE_SIZE, sprite_width,
                                                          spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);

    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_visibility(true, mainSpriteInfo, subSpriteInfo);
    set_position();
    match_animation();
}

void Bullet::set_position() {
    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);
    if (!killed) {
        sprite_utils::set_entry_xy(mainSpriteInfo, main_x, main_y);
        sprite_utils::set_entry_xy(subSpriteInfo, sub_x, sub_y);
    } else {
        sprite_utils::set_entry_xy(mainSpriteInfo, main_x - BULLET_ANIM_X_OFFSET, main_y - BULLET_ANIM_Y_OFFSET);
        sprite_utils::set_entry_xy(subSpriteInfo, sub_x - BULLET_ANIM_X_OFFSET, sub_y - BULLET_ANIM_Y_OFFSET);
    }
}

Bullet::Bullet() {
    physical_height = BULLET_PHYSICAL_HEIGHT;
    physical_width = BULLET_PHYSICAL_WIDTH;
    sprite_height = BULLET_SPRITE_HEIGHT;
    sprite_width = BULLET_SPRITE_WIDTH;
    spritesheet_type = SpritesheetType::SPIKES_COLLECTIBLES;
}

void Bullet::match_animation() {
    if (killed)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spike_collectibles_flameTiles, BULLET_SPRITE_SIZE,
                                           25 + animFrame);
    else
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spike_collectibles_flameTiles, BULLET_SPRITE_SIZE, 25);

    sprite_utils::update_frame(frameGfx, BULLET_SPRITE_SIZE, mainSpriteInfo, subSpriteInfo);
}

void Bullet::deleteSprite() {
    delete mainSpriteInfo;
    delete subSpriteInfo;
    mainSpriteInfo = nullptr;
    subSpriteInfo = nullptr;
}

