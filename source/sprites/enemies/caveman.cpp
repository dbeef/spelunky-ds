//
// Created by xdbeef on 31.05.18.
//


#include <maxmod9.h>
#include <cstdlib>
#include "caveman.h"
#include "../../globals_declarations.h"
#include "../../collisions/collisions.h"
#include "../../../build/soundbank.h"
#include "../animations/blood.h"
#include "../../../build/gfx_caveman_damsel.h"
#include "../../tiles/map_utils.h"
#include "../../tiles/tile_orientation.hpp"

#define CAVEMAN_POS_INC_DELTA 20
#define CAVEMAN_TRIGGERED_SPEED 3
#define CAVEMAN_NORMAL_SPEED 0.75
#define CAVEMAN_ANIM_FRAME_DELTA 80
#define CAVEMAN_INVERT_SPEED_DELTA 400

void Caveman::draw() {

    if(ready_to_dispose)
        return;

    invert_speed_timer += *global::timer;
    blood_spawn_timer += *global::timer;

    if (sprite_state == SpriteState::W_LEFT) {
        mainSpriteInfo->entry->hFlip = false;
        subSpriteInfo->entry->hFlip = false;
    } else if (sprite_state == SpriteState::W_RIGHT) {
        mainSpriteInfo->entry->hFlip = true;
        subSpriteInfo->entry->hFlip = true;
    }

    mainSpriteInfo->entry->isHidden = false;
    subSpriteInfo->entry->isHidden = false;
    mainSpriteInfo->entry->vFlip = false;
    subSpriteInfo->entry->vFlip = false;

    if (killed || stunned) {

        check_if_can_be_pickuped();
        set_pickuped_position(6, -4);

        if (hold_by_main_dude) {
            sprite_state = global::main_dude->sprite_state;
            mainSpriteInfo->entry->priority = OBJPRIORITY_0;
            subSpriteInfo->entry->priority = OBJPRIORITY_0;

        } else {
            mainSpriteInfo->entry->priority = OBJPRIORITY_2;
            subSpriteInfo->entry->priority = OBJPRIORITY_2;
        }

        kill_mobs_if_thrown(1);
    }


    if (bottomCollision && (stunned || killed)) {
        apply_friction(0.5f);
    }

    set_position();

    if (!stunned && !killed) {
        if (hold_by_main_dude) {
            hold_by_main_dude = false;
            global::main_dude->holding_item = false;
            goTimer = 2000;
            waitTimer = 0;
        }
    }

    animFrameTimer += *global::timer;

    if (animFrameTimer > CAVEMAN_ANIM_FRAME_DELTA) {

        animFrame++;

        if (stunned && hold_by_main_dude)
            apply_stunned_carried_sprites();
        else if (stunned)
            apply_stunned_sprites();
        else if (killed && hold_by_main_dude)
            apply_dead_carried_sprites();
        else if (killed)
            apply_dead_sprites();
        else
            apply_walking_sprites();

        subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
        mainSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
        animFrameTimer = 0;

    }

    if (!triggered && !killed && !stunned) {
        check_if_can_be_triggered();
        if(triggered)
            mmEffect(SFX_XALERT);
    }

    make_some_movement();

    if (!stunned && !killed)
        kill_if_whip(1);
    if (!stunned && !killed) {
        if (global::main_dude->carrying_spike_shoes)
            kill_if_main_dude_jumped_on_you(3);
        else
            kill_if_main_dude_jumped_on_you(1);
    }
    if (!stunned && !killed)
        deal_damage_main_dude_on_collision(1);

    if (stunned) {
        stunned_timer += *global::timer;
        if (stunned_timer > CAVEMAN_STUN_TIME) {
            stunned = false;
            stunned_timer = 0;
        }
    }

}


void Caveman::init() {

    activated = true;
    initSprite();

    frameGfx = (u8 *) gfx_caveman_damselTiles;
    subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
    mainSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);

    spritesheet_type = SpritesheetType::CAVEMAN_DAMSEL;

    randomizeMovement();
}

void Caveman::randomizeMovement() {

    int r = rand() % 2;

    if (r == 0) {
        sprite_state = SpriteState::W_LEFT;

    } else if (r == 1) {
        if (sprite_state == SpriteState::W_RIGHT)
            sprite_state = SpriteState::W_RIGHT;
    }

    goTimer = (rand() % (1 * 2000)) + 1000;

    waitTimer = rand() % 1500;
}

void Caveman::updateSpeed() {

    limit_speed(MAX_X_SPEED_CAVEMAN, MAX_Y_SPEED_CAVEMAN);

    pos_inc_timer += *global::timer;

    if (pos_inc_timer > CAVEMAN_POS_INC_DELTA) {
        update_position();
        apply_gravity(GRAVITY_DELTA_SPEED * 1.5);
        pos_inc_timer = 0;
    }

}

void Caveman::updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) {

    MapTile *tiles[9] = {};
    Collisions::getNeighboringTiles(global::level_generator->map_tiles, x_current_pos_in_tiles,
                                    y_current_pos_in_tiles, tiles);

    bool bounce = (stunned || killed);


    standingOnLeftEdge = Collisions::isStandingOnLeftEdge(tiles, x, physical_width, x_current_pos_in_tiles);
    standingOnRightEdge = Collisions::isStandingOnRightEdge(tiles, x, physical_width, x_current_pos_in_tiles);

    if(bounce) {
        upperCollision = Collisions::checkUpperCollision(tiles, &x, &y, &ySpeed, physical_width, false, 0);
        bottomCollision = Collisions::checkBottomCollision(tiles, &x, &y, &ySpeed, physical_width, physical_height,
                                                           bounce, BOUNCING_FACTOR_Y);
        leftCollision = Collisions::checkLeftCollision(tiles, &x, &y, &xSpeed, physical_width, physical_height, bounce,
                                                       BOUNCING_FACTOR_X);
        rightCollision = Collisions::checkRightCollision(tiles, &x, &y, &xSpeed, physical_width, physical_height,
                                                         bounce, BOUNCING_FACTOR_X);
    }
    else
    {
        bottomCollision = Collisions::checkBottomCollision(tiles, &x, &y, &ySpeed, physical_width, physical_height,
                                                           bounce, BOUNCING_FACTOR_Y);
        leftCollision = Collisions::checkLeftCollision(tiles, &x, &y, &xSpeed, physical_width, physical_height, bounce,
                                                       BOUNCING_FACTOR_X);
        rightCollision = Collisions::checkRightCollision(tiles, &x, &y, &xSpeed, physical_width, physical_height,
                                                         bounce, BOUNCING_FACTOR_X);
        upperCollision = Collisions::checkUpperCollision(tiles, &x, &y, &ySpeed, physical_width, false, 0);
    }

    if (stunned || killed)
        return;

    if (bottomCollision && tiles[TileOrientation::RIGHT_MIDDLE] != nullptr &&
        tiles[TileOrientation::RIGHT_MIDDLE]->collidable &&
        tiles[TileOrientation::LEFT_MIDDLE] != nullptr && tiles[TileOrientation::LEFT_MIDDLE]->collidable) {
        ySpeed = -3.6 - ((rand() % 10) / 5);
        landlocked = true;
    } else
        landlocked = false;

    if (!bottomCollision) {

        if (((tiles[1] == nullptr || !tiles[1]->collidable) && (tiles[6] != nullptr && tiles[8] != nullptr))) {
            if (xSpeed > 0)
                x += 2;
        }

        if (((tiles[0] == nullptr || !tiles[0]->collidable) && (tiles[5] != nullptr && tiles[7] != nullptr))) {
            if (xSpeed < 0)
                x -= 2;
        }

    }

}

void Caveman::apply_dmg(int dmg_to_apply) {

    if (dmg_to_apply == 1 && (stunned))
        return;

    hitpoints -= dmg_to_apply;

    if (blood_spawn_timer > 1000) {
        if (!killed)
            spawn_blood();
        blood_spawn_timer = 0;
        ySpeed = -2.5;
    }

    if (killed)
        return;

    if (hitpoints <= 0) {
        killed = true;
        stunned = false;
        global::killed_npcs.push_back(SpriteType::S_CAVEMAN);
    } else {
        stunned = true;
    }

}

void Caveman::initSprite() {
    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_caveman_damselPal, gfx_caveman_damselPalLen,
                                                        nullptr, sprite_width * sprite_height, 16, CAVEMAN_DAMSEL, true,
                                                        false,
                                                        LAYER_LEVEL::MIDDLE_BOT);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_caveman_damselPal, gfx_caveman_damselPalLen,
                                                          nullptr, sprite_width * sprite_height, 16, CAVEMAN_DAMSEL,
                                                          true,
                                                          false, LAYER_LEVEL::MIDDLE_BOT);
    subSpriteInfo->entry->isHidden = false;
    mainSpriteInfo->entry->isHidden = false;

    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);

    set_position();

    if (sprite_state == SpriteState::W_LEFT) {
        mainSpriteInfo->entry->vFlip = false;
    } else if (sprite_state == SpriteState::W_RIGHT) {
        mainSpriteInfo->entry->vFlip = true;
    }


}

void Caveman::set_position() {

    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);

    mainSpriteInfo->entry->x = main_x;
    mainSpriteInfo->entry->y = main_y;

    subSpriteInfo->entry->x = sub_x;
    subSpriteInfo->entry->y = sub_y;

}

Caveman::Caveman() {
    physical_height = CAVEMAN_PHYSICAL_HEIGHT;
    physical_width = CAVEMAN_PHYSICAL_WIDTH;
    sprite_height = CAVEMAN_SPRITE_HEIGHT;
    sprite_width = CAVEMAN_SPRITE_WIDTH;
    hitpoints = CAVEMAN_HITPOINTS;
}


void Caveman::make_some_movement() {

    if (killed || stunned)
        return;

    if (waitTimer > 0 && !triggered) {
        waitTimer -= *global::timer;
    } else {

        if (goTimer > 0)
            goTimer -= *global::timer;

        if (triggered) {
            if (sprite_state == SpriteState::W_RIGHT)
                xSpeed = CAVEMAN_TRIGGERED_SPEED;
            else
                xSpeed = -CAVEMAN_TRIGGERED_SPEED;
        } else {
            if (sprite_state == SpriteState::W_RIGHT)
                xSpeed = CAVEMAN_NORMAL_SPEED;
            else
                xSpeed = -CAVEMAN_NORMAL_SPEED;
        }


        if (goTimer <= 0 && !triggered) {
            randomizeMovement();
            xSpeed = 0;
        }

        if (invert_speed_timer < CAVEMAN_INVERT_SPEED_DELTA)
            return;

        invert_speed_timer = 0;

        if ((standingOnLeftEdge || standingOnRightEdge) && !triggered) {
            if (rand() % 2 == 0) {
                goTimer = 0;
            } else
                xSpeed *= -1;

        }

        if ((leftCollision || rightCollision) && !landlocked) {

            if (sprite_state == SpriteState::W_LEFT)
                sprite_state = SpriteState::W_RIGHT;
            else
                sprite_state = SpriteState::W_LEFT;

            xSpeed *= -1;
            rightCollision = false;
            leftCollision = false;
        }

    }

}

void Caveman::check_if_can_be_triggered() {

    int diff = x - global::main_dude->x;
    int diff_abs = abs(diff);

    if (!triggered && diff_abs < 9 * TILE_W && abs(y - global::main_dude->y) < 0.8 * TILE_H) {

        int xx = floor_div(this->x + 0.5 * physical_width, TILE_W);
        int yy = floor_div(this->y + 0.5 * physical_height, TILE_H);

        int dude_xx = floor_div(global::main_dude->x + 0.5 * MAIN_DUDE_PHYSICAL_WIDTH, TILE_W);

        triggered = true;

        MapTile *tiles[9] = {};
        if (sprite_state == SpriteState::W_RIGHT && diff < 0)
            Collisions::getTilesOnRightFromXY(global::level_generator->map_tiles, xx, yy, tiles);
        else if (sprite_state == SpriteState::W_LEFT && diff > 0)
            Collisions::getTilesOnLeftFromXY(global::level_generator->map_tiles, xx, yy, tiles);
        else triggered = false;


        for (int a = 0; a < 9; a++) {

            if (tiles[a] != nullptr && tiles[a]->collidable) {
                triggered = false;
                break;
            }

            if (abs(xx - dude_xx) - a == 0)
                break;

        }
    }

}

void Caveman::apply_stunned_carried_sprites() {

    if (animFrame >= 4)
        animFrame = 0;

    frameGfx = (u8 *) gfx_caveman_damselTiles + ((sprite_width * sprite_height * (6 + animFrame)) / 2);
}

void Caveman::apply_dead_carried_sprites() {
    animFrame = 0;
    frameGfx = (u8 *) gfx_caveman_damselTiles + ((sprite_width * sprite_height * (0)) / 2);
}

void Caveman::apply_dead_sprites() {
    animFrame = 0;
    if (ySpeed == 0)
        frameGfx = (u8 *) gfx_caveman_damselTiles + ((sprite_width * sprite_height * (1)) / 2);
    else if (ySpeed > 0)
        frameGfx = (u8 *) gfx_caveman_damselTiles + ((sprite_width * sprite_height * (4)) / 2);
    else
        frameGfx = (u8 *) gfx_caveman_damselTiles + ((sprite_width * sprite_height * (2)) / 2);

}


void Caveman::apply_walking_sprites() {

    if (animFrame >= 4)
        animFrame = 0;

    if (xSpeed == 0 && ySpeed == 0)
        frameGfx = (u8 *) gfx_caveman_damselTiles + ((sprite_width * sprite_height * (10)) / 2);
    else
        frameGfx = (u8 *) gfx_caveman_damselTiles + ((sprite_width * sprite_height * (animFrame + 11)) / 2);

}

void Caveman::apply_stunned_sprites() {
    if (animFrame >= 5)
        animFrame = 0;

    if (ySpeed == 0)
        frameGfx = (u8 *) gfx_caveman_damselTiles + ((sprite_width * sprite_height * (animFrame + 15)) / 2);
    else if (ySpeed > 0)
        frameGfx = (u8 *) gfx_caveman_damselTiles + ((sprite_width * sprite_height * (4)) / 2);
    else
        frameGfx = (u8 *) gfx_caveman_damselTiles + ((sprite_width * sprite_height * (2)) / 2);
}


Caveman::Caveman(int x, int y) : Caveman() {
    this->x = x;
    this->y = y;
}