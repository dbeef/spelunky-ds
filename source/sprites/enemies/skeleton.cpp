//
// Created by xdbeef on 22.07.18.
//

#include <cstdlib>
#include <maxmod9.h>
#include "skeleton.h"

#include "../../../build/gfx_spider_skeleton.h"
#include "../../globals_declarations.h"
#include "../../collisions/collisions.h"
#include "../../tiles/map_utils.h"
#include "../../../build/soundbank.h"
#include "../animations/bone.h"
#include "../animations/skull.h"
#include "../sprite_utils.h"

#define SKELETON_POS_INC_DELTA 19
#define SKELETON_ANIM_FRAME_DELTA 90
#define SKELETON_HITPOINTS 1

void Skeleton::draw() {

    if (ready_to_dispose)
        return;

    set_position();
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_visibility(true, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(!(sprite_state == SpriteState::W_LEFT), mainSpriteInfo, subSpriteInfo);

    if (summoned || can_be_summoned) {
        if (summoning_delay_timer > 350) {
            anim_frame_timer += *global::timer;

            if (can_be_summoned && !set_up) {

                set_up = true;
                if (main_dude_orientation_at_summoning_moment == SpriteState::W_LEFT) {
                    sprite_state = SpriteState::W_RIGHT;
                } else {
                    sprite_state = SpriteState::W_LEFT;
                }

            }

        } else
            summoning_delay_timer += *global::timer;

        change_turn_timer += *global::timer;
    }

    check_if_can_be_summoned();

    if (anim_frame_timer > SKELETON_ANIM_FRAME_DELTA) {

        anim_frame_counter++;
        anim_frame_timer = 0;
        match_animation();
    }

    if (summoned || (can_be_summoned && summoning_delay_timer > 350)) {
        kill_if_whip(1);
        kill_if_main_dude_jumped_on_you(1);
        deal_damage_main_dude_on_collision(1);
    }
}


void Skeleton::init() {
    initSprite();
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

    if (ready_to_dispose)
        return;

    bool death_speed = ySpeed > 3.5f; //skeleton disintegrates in contact with the bottom above some speed

    MapTile *t[9] = {};
    Collisions::getNeighboringTiles(global::level_generator->map_tiles,
                                    x_current_pos_in_tiles, y_current_pos_in_tiles, t);

    standingOnLeftEdge = Collisions::isStandingOnLeftEdge(t, x, physical_width, x_current_pos_in_tiles);
    standingOnRightEdge = Collisions::isStandingOnRightEdge(t, x, physical_width, x_current_pos_in_tiles);
    bottomCollision = Collisions::checkBottomCollision(t, &x, &y, &ySpeed, physical_width, physical_height, false,
                                                       0);
    leftCollision = Collisions::checkLeftCollision(t, &x, &y, &xSpeed, physical_width, physical_height, false, 0);
    rightCollision = Collisions::checkRightCollision(t, &x, &y, &xSpeed, physical_width, physical_height, false, 0);
    upperCollision = Collisions::checkUpperCollision(t, &x, &y, &ySpeed, physical_width, false, 0);

    if (bottomCollision) {

        if (death_speed)
            apply_dmg(1);

        if (summoned && xSpeed == 0) {
            if (sprite_state == SpriteState::W_RIGHT)
                xSpeed = 1;
            else
                xSpeed = -1;
        }

        if (change_turn_timer > 650) {
            if (leftCollision) {
                xSpeed = -1;
                sprite_state = SpriteState::W_LEFT;
                change_turn_timer = 0;

            } else if (rightCollision) {
                xSpeed = 1;
                sprite_state = SpriteState::W_RIGHT;
                change_turn_timer = 0;
            }
        }

    }

}

//!> skeleton has only 1 dmg point, always kill if any dmg_apply
void Skeleton::apply_dmg(int dmg_to_apply) {

    sprite_utils::set_visibility(false, mainSpriteInfo, subSpriteInfo);

    killed = true;
    ready_to_dispose = true;
    global::killed_npcs.push_back(SpriteType::S_SKELETON);
    mmEffect(SFX_XBREAK);
    spawn_remnants();
}

void Skeleton::initSprite() {

    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_spider_skeletonPal, gfx_spider_skeletonPalLen,
                                                        nullptr, SKELETON_SPRITE_SIZE, 16, SKELETON_SPIDER,
                                                        true, false, LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_spider_skeletonPal, gfx_spider_skeletonPalLen,
                                                          nullptr, SKELETON_SPRITE_SIZE, 16, SKELETON_SPIDER,
                                                          true, false, LAYER_LEVEL::MIDDLE_TOP);

    set_position();
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_visibility(true, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(!(sprite_state == SpriteState::W_LEFT), mainSpriteInfo, subSpriteInfo);
    match_animation();

}

void Skeleton::set_position() {
    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);
    sprite_utils::set_entry_xy(mainSpriteInfo, main_x, main_y);
    sprite_utils::set_entry_xy(subSpriteInfo, sub_x, sub_y);
}

void Skeleton::check_if_can_be_summoned() {

    if (summoned || can_be_summoned)
        return;

    int diff = x - global::main_dude->x; //distance between skeleton and main dude
    int diff_abs = abs(diff); //absolute distance

    if (diff_abs < 2 * TILE_W && abs(y - global::main_dude->y) < 0.8 * TILE_H) {

        //TODO sprite_utils function, this xx, yy tile coordinates snippet is used widely
        int xx = floor_div(this->x + 0.5 * physical_width, TILE_W);
        int yy = floor_div(this->y + 0.5 * physical_height, TILE_H);

        int dude_xx = floor_div(global::main_dude->x + 0.5 * MAIN_DUDE_PHYSICAL_WIDTH, TILE_W);

        MapTile *tiles[9] = {};

        if (diff > 0) {
            //must have been walking towards skeleton from skeleton's left side, so pointing rightwards
            main_dude_orientation_at_summoning_moment = SpriteState::W_RIGHT;
            Collisions::getTilesOnLeftFromXY(global::level_generator->map_tiles, xx, yy, tiles);
        } else {
            //must have been walking towards skeleton from skeleton's right side, so pointing leftwards
            main_dude_orientation_at_summoning_moment = SpriteState::W_LEFT;
            Collisions::getTilesOnRightFromXY(global::level_generator->map_tiles, xx, yy, tiles);
        }

        for (int a = 0; a < 9; a++) {

            if (tiles[a] != nullptr && tiles[a]->collidable) {
                //there's a collidable obstacle between skeleton and main dude, no summoning
                break;
            }

            if (abs(xx - dude_xx) - a == 0) {
                can_be_summoned = true;
                //if distance in tiles between skeleton and main dude - current tile index == 0, then break,
                // because we didn't find any obstacles between skeleton and main dude and we already are on its tile
                break;
            }

        }

    }
}

void Skeleton::set_sprite_pile_of_bones() {
    frameGfx = sprite_utils::get_frame((u8 *) gfx_spider_skeletonTiles, SKELETON_SPRITE_SIZE, 12);
}

void Skeleton::set_sprite_walking() {

    if (anim_frame_counter >= 5)
        anim_frame_counter = 0;

    frameGfx = sprite_utils::get_frame((u8 *) gfx_spider_skeletonTiles, SKELETON_SPRITE_SIZE, anim_frame_counter + 18);
}

void Skeleton::set_sprite_summoning() {

    if (anim_frame_counter >= 6) {

        //summoning animation is completed,
        anim_frame_counter = 0;
        summoned = true;

        //turn the skeleton upwards main dude
        if (main_dude_orientation_at_summoning_moment == SpriteState::W_LEFT) {
            xSpeed = 1;
            sprite_state = SpriteState::W_RIGHT;
        } else {
            xSpeed = -1;
            sprite_state = SpriteState::W_LEFT;
        }

    } else {
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spider_skeletonTiles,
                                           SKELETON_SPRITE_SIZE, anim_frame_counter + 12);
    }
}

Skeleton::Skeleton() {
    physical_height = SKELETON_PHYSICAL_HEIGHT;
    physical_width = SKELETON_PHYSICAL_WIDTH;
    sprite_height = SKELETON_SPRITE_HEIGHT;
    sprite_width = SKELETON_SPRITE_WIDTH;
    hitpoints = SKELETON_HITPOINTS;
    spritesheet_type = SpritesheetType::SKELETON_SPIDER;
    sprite_type = SpriteType::S_SKELETON;
    sprite_state = SpriteState::W_LEFT;
}


Skeleton::Skeleton(int x, int y) : Skeleton() {
    this->x = x;
    this->y = y;
}

void Skeleton::match_animation() {

    if (summoned)
        set_sprite_walking();
    else if (can_be_summoned)
        set_sprite_summoning();
    else
        set_sprite_pile_of_bones();

    sprite_utils::update_frame(frameGfx, SKELETON_SPRITE_SIZE, mainSpriteInfo, subSpriteInfo);
}

void Skeleton::spawn_remnants() {

    auto *b_1 = new Bone();
    b_1->x = x;
    b_1->y = y - 5;
    b_1->xSpeed = 0;
    b_1->ySpeed = -1.4f;
    b_1->animFrame = 2;
    b_1->init();

    auto *b_2 = new Bone();
    b_2->x = x + 5;
    b_2->y = y - 2;
    b_2->xSpeed = -1.5f;
    b_2->ySpeed = -1.6f;
    b_1->animFrame = 6;
    b_2->init();


    auto *s = new Skull();
    s->x = x - 6;
    s->y = y - 4;
    s->xSpeed = -0.5f;
    s->ySpeed = -1.8f;
    s->init();

    global::sprites.push_back(b_1);
    global::sprites.push_back(b_2);
    global::sprites.push_back(s);

}
