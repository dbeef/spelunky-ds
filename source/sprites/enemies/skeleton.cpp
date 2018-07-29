//
// Created by xdbeef on 22.07.18.
//

#include <cstdlib>
#include <iostream>
#include <maxmod9.h>
#include "skeleton.h"

#include "../../../build/gfx_spider_skeleton.h"
#include "../../globals_declarations.h"
#include "../../collisions/collisions.h"
#include "../../tiles/map_utils.h"
#include "../../../build/soundbank.h"
#include "../animations/bone.h"
#include "../animations/skull.h"

#define SKELETON_POS_INC_DELTA 19
#define SKELETON_ANIM_FRAME_DELTA 90
#define SKELETON_HITPOINTS 1

void Skeleton::draw() {

    if (ready_to_dispose)
        return;

    set_position();

    mainSpriteInfo->entry->vFlip = false;
    subSpriteInfo->entry->vFlip = false;

    mainSpriteInfo->entry->isHidden = false;
    subSpriteInfo->entry->isHidden = false;


    if (sprite_state == SpriteState::W_LEFT) {
        mainSpriteInfo->entry->hFlip = false;
        subSpriteInfo->entry->hFlip = false;
    } else {
        mainSpriteInfo->entry->hFlip = true;
        subSpriteInfo->entry->hFlip = true;
    }

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

        if (summoned) {
            set_sprite_walking();
        } else if (can_be_summoned) {
            set_sprite_summoning();
        }
    }

    if (summoned || (can_be_summoned > summoning_delay_timer > 350)) {
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

    bool death_speed = ySpeed > 3.5f;

    MapTile *tiles[9] = {};
    Collisions::getNeighboringTiles(global::level_generator->map_tiles, x_current_pos_in_tiles,
                                    y_current_pos_in_tiles, tiles);

    standingOnLeftEdge = Collisions::isStandingOnLeftEdge(tiles, x, physical_width, x_current_pos_in_tiles);
    standingOnRightEdge = Collisions::isStandingOnRightEdge(tiles, x, physical_width, x_current_pos_in_tiles);
    bottomCollision = Collisions::checkBottomCollision(tiles, &x, &y, &ySpeed, physical_width, physical_height, false,
                                                       0);
    leftCollision = Collisions::checkLeftCollision(tiles, &x, &y, &xSpeed, physical_width, physical_height, false, 0);
    rightCollision = Collisions::checkRightCollision(tiles, &x, &y, &xSpeed, physical_width, physical_height, false, 0);
    upperCollision = Collisions::checkUpperCollision(tiles, &x, &y, &ySpeed, physical_width, false, 0);

    if (bottomCollision) {

        if (death_speed) {
            apply_dmg(1);
        }

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

void Skeleton::apply_dmg(int dmg_to_apply) {

    //skeleton has only 1 dmg point, always kill if any dmg_apply

    subSpriteInfo->entry->isHidden = true;
    mainSpriteInfo->entry->isHidden = true;

    subSpriteInfo = nullptr;
    mainSpriteInfo = nullptr;

    killed = true;
    ready_to_dispose = true;
    global::hud->draw();
    global::killed_npcs.push_back(spritesheet_type);
    mmEffect(SFX_XBREAK);

    Bone *b_1 = new Bone();
    b_1->x = x;
    b_1->y = y - 5;
    b_1->xSpeed = 0;
    b_1->ySpeed = -1.4f;
    b_1->animFrame=2;
    b_1->init();
    
    Bone *b_2 = new Bone();
    b_2->x = x + 5;
    b_2->y = y - 2;
    b_2->xSpeed = -1.5f;
    b_2->ySpeed = -1.6f;
    b_1->animFrame=6;
    b_2->init();


    Skull *s= new Skull();
    s->x = x - 6;
    s->y = y - 4;
    s->xSpeed = -0.5f;
    s->ySpeed = -1.8f;
    s->init();

    global::sprites.push_back(b_1);
    global::sprites.push_back(b_2);
    global::sprites.push_back(s);

}

void Skeleton::initSprite() {


    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_spider_skeletonPal, gfx_spider_skeletonPalLen,
                                                        nullptr, sprite_width * sprite_height, 16, SKELETON_SPIDER,
                                                        true, false,
                                                        LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_spider_skeletonPal, gfx_spider_skeletonPalLen,
                                                          nullptr, sprite_width * sprite_height, 16, SKELETON_SPIDER,
                                                          true, false,
                                                          LAYER_LEVEL::MIDDLE_TOP);
    subSpriteInfo->entry->isHidden = false;
    mainSpriteInfo->entry->isHidden = false;

    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);

    mainSpriteInfo->entry->x = main_x;
    mainSpriteInfo->entry->y = main_y;

    subSpriteInfo->entry->x = sub_x;
    subSpriteInfo->entry->y = sub_y;

    mainSpriteInfo->entry->vFlip = false;
    subSpriteInfo->entry->vFlip = false;

    if (summoned) {
        set_sprite_walking();
    } else if (can_be_summoned) {
        set_sprite_summoning();
    } else
        set_sprite_pile_of_bones();

}

void Skeleton::set_position() {

    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);

    mainSpriteInfo->entry->x = main_x;
    mainSpriteInfo->entry->y = main_y;

    subSpriteInfo->entry->x = sub_x;
    subSpriteInfo->entry->y = sub_y;
}

void Skeleton::check_if_can_be_summoned() {

    if (summoned || can_be_summoned)
        return;

    int diff = x - global::main_dude->x;
    int diff_abs = abs(diff);

    if (diff_abs < 2 * TILE_W && abs(y - global::main_dude->y) < 0.8 * TILE_H) {

        int xx = floor_div(this->x + 0.5 * physical_width, TILE_W);
        int yy = floor_div(this->y + 0.5 * physical_height, TILE_H);

        int dude_xx = floor_div(global::main_dude->x + 0.5 * MAIN_DUDE_PHYSICAL_WIDTH, TILE_W);

        can_be_summoned = true;

        if (diff > 0)
            main_dude_orientation_at_summoning_moment = SpriteState::W_RIGHT;
        else
            main_dude_orientation_at_summoning_moment = SpriteState::W_LEFT;

        MapTile *tiles[9] = {};
        Collisions::getTilesOnLeftFromXY(global::level_generator->map_tiles, xx, yy, tiles);

        for (int a = 0; a < 9; a++) {

            if (tiles[a] != nullptr && tiles[a]->collidable) {
                can_be_summoned = false;
                break;
            }

            if (abs(xx - dude_xx) - a == 0)
                break;

        }

        if (!can_be_summoned) {
            Collisions::getTilesOnRightFromXY(global::level_generator->map_tiles, xx, yy, tiles);

            for (int a = 0; a < 9; a++) {

                if (tiles[a] != nullptr && tiles[a]->collidable) {
                    can_be_summoned = false;
                    break;
                }

                if (abs(xx - dude_xx) - a == 0)
                    break;

            }

        }

    }

}

void Skeleton::set_sprite_walking() {
    if (anim_frame_counter >= 5)
        anim_frame_counter = 0;

    frameGfx = (u8 *) gfx_spider_skeletonTiles + ((sprite_width * sprite_height * (anim_frame_counter + 18)) / 2);
    subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
    mainSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
}

void Skeleton::set_sprite_summoning() {
    if (anim_frame_counter >= 6) {
        anim_frame_counter = 0;
        summoned = true;

        if (main_dude_orientation_at_summoning_moment == SpriteState::W_LEFT) {
            xSpeed = 1;
            sprite_state = SpriteState::W_RIGHT;
        } else {
            xSpeed = -1;
            sprite_state = SpriteState::W_LEFT;
        }

    } else {
        frameGfx = (u8 *) gfx_spider_skeletonTiles + ((sprite_width * sprite_height * (anim_frame_counter + 12)) / 2);
        subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
        mainSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
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

void Skeleton::set_sprite_pile_of_bones() {
    frameGfx = (u8 *) gfx_spider_skeletonTiles + ((sprite_width * sprite_height * (12)) / 2);
    subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
    mainSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);

}

