//
// Created by xdbeef on 31.05.18.
//

#include <maxmod9.h>
#include <cstdlib>
#include "caveman.h"
#include "../../globals_declarations.h"
#include "../../collisions/collisions.h"
#include "../../../build/gfx_caveman_damsel.h"
#include "../../tiles/tile_orientation.hpp"
#include "damsel.h"
#include "../../../build/soundbank.h"
#include "../sprite_utils.h"

#define DAMSEL_SPRITESHEET_OFFSET 25
#define DAMSEL_POS_INC_DELTA 18
#define DAMSEL_TRIGGERED_SPEED 1.2
#define DAMSEL_ANIM_FRAME_DELTA 65
#define DAMSEL_INVERT_SPEED_DELTA 400

void Damsel::draw() {

    if (ready_to_dispose)
        return;

    if (rescued) {
        sprite_utils::set_visibility(false, mainSpriteInfo, subSpriteInfo, yell_mainSpriteInfo, yell_subSpriteInfo);
        return;
    }

    sprite_utils::set_vertical_flip(false, yell_mainSpriteInfo, yell_subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, yell_mainSpriteInfo, yell_subSpriteInfo);

    if (killed)
        yelling = false;

    invert_speed_timer += *global::timer;
    blood_spawn_timer += *global::timer;

    sprite_utils::set_horizontal_flip(sprite_state == SpriteState::W_RIGHT, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_visibility(true, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);

    check_if_can_be_pickuped();
    set_pickuped_position(6, -2);

    if (hold_by_main_dude) {

        yelling = false;

        if (!killed)
            global::main_dude->carrying_damsel = true;

        if (global::main_dude->killed) {
            hold_by_main_dude = false;
            stunned = true;
            stunned_timer = 0;
        } else
            stunned = false;

        sprite_state = global::main_dude->sprite_state;
        sprite_utils::set_priority(OBJPRIORITY_0, mainSpriteInfo, subSpriteInfo);

    } else
        sprite_utils::set_priority(OBJPRIORITY_2, mainSpriteInfo, subSpriteInfo);

    if (!hold_by_main_dude)
        kill_mobs_if_thrown(1);

    if (bottomCollision && (stunned || killed)) {
        apply_friction(0.5f);
    }

    set_position();

    if (!hold_by_main_dude && global::main_dude->carrying_damsel) {
        global::main_dude->carrying_damsel = false;
        stunned = true;
        stunned_timer = 0;
        triggered = true;
    }

    animFrameTimer += *global::timer;

    sprite_utils::set_visibility(yelling, yell_mainSpriteInfo, yell_subSpriteInfo);

    if (yelling) {
        if (animFrameTimer > DAMSEL_ANIM_FRAME_DELTA * 2) {
            animFrameTimer = 0;
            animFrame++;
            match_animation();
        }
    } else {
        if (animFrameTimer > DAMSEL_ANIM_FRAME_DELTA) {
            animFrameTimer = 0;
            animFrame++;
            match_animation();
        }
    }

    make_some_movement();

//FIXME Jumping if whipped
//    if (!stunned && !killed)
//        kill_if_whip(0);

    if (call_for_help && !triggered && !yelling && !killed) {
        yell_timer += *global::timer;
        if (yell_timer > 5 * 1000) {
            yell_timer = 0;
            yelling = true;
        }
    }


    if (stunned) {
        stunned_timer += *global::timer;
        if (stunned_timer > DAMSEL_STUN_TIME) {
            stunned = false;
            stunned_timer = 0;
        }
    }

    if (hold_by_main_dude && !killed && !stunned && !exiting_level) {

        //search for an exit
        MapTile *tiles[9] = {};
        Collisions::getNeighboringTiles(global::level_generator->map_tiles, global::main_dude->current_x_in_tiles,
                                        global::main_dude->current_y_in_tiles, tiles);

        exiting_level = tiles[CENTER] != nullptr && tiles[CENTER]->mapTileType == MapTileType::EXIT;
        if (exiting_level) {
            xSpeed = 0;
            ySpeed = 0;
            x = tiles[CENTER]->x * 16;
            y = tiles[CENTER]->y * 16;
            hold_by_main_dude = false;
            mmEffect(SFX_XSTEPS);
            if (global::main_dude->carrying_damsel) {
                global::main_dude->holding_item = false;
                global::main_dude->carrying_damsel = false;
            }
        }
    }

}


void Damsel::init() {
    sprite_state = SpriteState::W_LEFT;
    activated = true;
    initSprite();
    spritesheet_type = SpritesheetType::CAVEMAN_DAMSEL;
    randomizeMovement();
}

void Damsel::randomizeMovement() {

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

void Damsel::updateSpeed() {

    limit_speed(MAX_X_SPEED_DAMSEL, MAX_Y_SPEED_DAMSEL);

    pos_inc_timer += *global::timer;

    if (pos_inc_timer > DAMSEL_POS_INC_DELTA) {
        update_position();
        apply_gravity(GRAVITY_DELTA_SPEED * 1.5);
        pos_inc_timer = 0;
    }

}

void Damsel::updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) {

    MapTile *t[9] = {};
    Collisions::getNeighboringTiles(global::level_generator->map_tiles, x_current_pos_in_tiles,
                                    y_current_pos_in_tiles, t);


    bool bounce = (stunned || killed);

    standingOnLeftEdge = Collisions::isStandingOnLeftEdge(t, x, physical_width, x_current_pos_in_tiles);
    standingOnRightEdge = Collisions::isStandingOnRightEdge(t, x, physical_width, x_current_pos_in_tiles);
    upperCollision = Collisions::checkUpperCollision(t, &x, &y, &ySpeed, physical_width, false, 0);
    bottomCollision = Collisions::checkBottomCollision(t, &x, &y, &ySpeed, physical_width, physical_height,
                                                       bounce, BOUNCING_FACTOR_Y);
    leftCollision = Collisions::checkLeftCollision(t, &x, &y, &xSpeed, physical_width, physical_height, bounce,
                                                   BOUNCING_FACTOR_X);
    rightCollision = Collisions::checkRightCollision(t, &x, &y, &xSpeed, physical_width, physical_height,
                                                     bounce, BOUNCING_FACTOR_X);


    if (bounce || exiting_level)
        return;

    //TODO This piece of code should be shared between shopkeeper/damsel/caveman in some utils file

    if (bottomCollision && t[TileOrientation::RIGHT_MIDDLE] != nullptr &&
        t[TileOrientation::RIGHT_MIDDLE]->collidable &&
        t[TileOrientation::LEFT_MIDDLE] != nullptr && t[TileOrientation::LEFT_MIDDLE]->collidable) {
        //high jump if damsel's surrounded by tiles
        ySpeed = -2.6 - ((rand() % 10) / 5);
        landlocked = true;
    } else
        landlocked = false;

    if (!bottomCollision) {

        if ((t[TileOrientation::RIGHT_MIDDLE] == nullptr || !t[TileOrientation::RIGHT_MIDDLE]->collidable) &&
            (t[TileOrientation::RIGHT_UP] != nullptr && t[TileOrientation::RIGHT_DOWN] != nullptr)) {
            //if there's no collidable tile on right-mid, but there are on right-up and right-down,
            //add extra x-pos to ease going through a hole
            if (xSpeed > 0)
                x += 2;
        }

        if ((t[TileOrientation::LEFT_MIDDLE] == nullptr || !t[TileOrientation::LEFT_MIDDLE]->collidable) &&
            (t[TileOrientation::LEFT_UP] != nullptr && t[TileOrientation::LEFT_DOWN] != nullptr)) {
            //same but for left side
            if (xSpeed < 0)
                x -= 2;
        }

    }

}

void Damsel::apply_dmg(int dmg_to_apply) {

    if (dmg_to_apply == 0 || dmg_to_apply == 1) {
        return;
    }

    //TODO Share this piece of code between damsel/shopkeeper/caveman/main dude
    hitpoints -= dmg_to_apply;
    if (blood_spawn_timer > 1000) {
        blood_spawn_timer = 0;
        ySpeed = -2.5; //jump a little bit on receiving dmg, even if dead
        if (!killed)
            spawn_blood();
    }

    if (killed)
        return;

    if (hitpoints <= 0) {
        killed = true;
        stunned = false;
        global::killed_npcs.push_back(SpriteType::S_DAMSEL);
    } else
        stunned = true;

}

void Damsel::initSprite() {
    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_caveman_damselPal, gfx_caveman_damselPalLen,
                                                        nullptr, DAMSEL_SPRITE_SIZE, 16, CAVEMAN_DAMSEL,
                                                        true, false, LAYER_LEVEL::MIDDLE_BOT);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_caveman_damselPal, gfx_caveman_damselPalLen,
                                                          nullptr, DAMSEL_SPRITE_SIZE, 16, CAVEMAN_DAMSEL,
                                                          true, false, LAYER_LEVEL::MIDDLE_BOT);
    yell_subSpriteInfo = global::sub_oam_manager->initSprite(gfx_caveman_damselPal, gfx_caveman_damselPalLen,
                                                             nullptr, DAMSEL_SPRITE_SIZE, 16, CAVEMAN_DAMSEL,
                                                             true, false, LAYER_LEVEL::MIDDLE_BOT);
    yell_mainSpriteInfo = global::main_oam_manager->initSprite(gfx_caveman_damselPal, gfx_caveman_damselPalLen,
                                                               nullptr, DAMSEL_SPRITE_SIZE, 16,
                                                               CAVEMAN_DAMSEL, true, false, LAYER_LEVEL::MIDDLE_BOT);

    match_animation();

    //set yell sprite frame here - it won't change later
    frameGfx = sprite_utils::get_frame((u8 *) gfx_caveman_damselTiles,
                                       DAMSEL_SPRITE_SIZE, DAMSEL_SPRITESHEET_OFFSET + 31);
    sprite_utils::update_frame(frameGfx, DAMSEL_SPRITE_SIZE, yell_mainSpriteInfo, yell_subSpriteInfo);

    if (rescued) {
        sprite_utils::set_visibility(false, mainSpriteInfo, subSpriteInfo, yell_mainSpriteInfo, yell_subSpriteInfo);
        return;
    }

    sprite_utils::set_vertical_flip(false, yell_mainSpriteInfo, yell_subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, yell_mainSpriteInfo, yell_subSpriteInfo);
    sprite_utils::set_horizontal_flip(sprite_state == SpriteState::W_RIGHT, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_visibility(true, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);

    set_position();
}

void Damsel::set_position() {

    int main_x, main_y, sub_x, sub_y;

    //viewporting damsel
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);

    sprite_utils::set_entry_xy(mainSpriteInfo, main_x, main_y);
    sprite_utils::set_entry_xy(subSpriteInfo, sub_x, sub_y);

    //viewporting 'help' prompt
    int temp_y = y;
    y -= 13 + (animFrame * 0.5);

    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);

    sprite_utils::set_entry_xy(yell_mainSpriteInfo, main_x, main_y);
    sprite_utils::set_entry_xy(yell_subSpriteInfo, sub_x, sub_y);

    y = temp_y;
}

Damsel::Damsel() {
    physical_height = DAMSEL_PHYSICAL_HEIGHT;
    physical_width = DAMSEL_PHYSICAL_WIDTH;
    sprite_height = DAMSEL_SPRITE_HEIGHT;
    sprite_width = DAMSEL_SPRITE_WIDTH;
    hitpoints = DAMSEL_HITPOINTS;
    call_for_help = true;
}


void Damsel::make_some_movement() {

    if (killed || stunned || exiting_level)
        return;

    if (waitTimer > 0 && !triggered) {
        waitTimer -= *global::timer;
    } else {

        if (goTimer > 0)
            goTimer -= *global::timer;

        if (triggered) {
            if (sprite_state == SpriteState::W_RIGHT)
                xSpeed = DAMSEL_TRIGGERED_SPEED;
            else
                xSpeed = -DAMSEL_TRIGGERED_SPEED;
        }

        if (goTimer <= 0 && !triggered) {
            randomizeMovement();
            xSpeed = 0;
        }

        if (invert_speed_timer < DAMSEL_INVERT_SPEED_DELTA)
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

//!> after calling this function, call sprite_utils::update_frame to update OAM with current frameGfx
void Damsel::apply_stunned_carried_sprites() {
    animFrame = 0;
    frameGfx = sprite_utils::get_frame((u8 *) gfx_caveman_damselTiles, DAMSEL_SPRITE_SIZE,
                                       DAMSEL_SPRITESHEET_OFFSET + 4);
}

//!> after calling this function, call sprite_utils::update_frame to update OAM with current frameGfx
void Damsel::apply_dead_carried_sprites() {
    animFrame = 0;
    frameGfx = sprite_utils::get_frame((u8 *) gfx_caveman_damselTiles, DAMSEL_SPRITE_SIZE,
                                       DAMSEL_SPRITESHEET_OFFSET + 4);
}

//!> after calling this function, call sprite_utils::update_frame to update OAM with current frameGfx
void Damsel::apply_dead_sprites() {
    animFrame = 0;
    if (ySpeed == 0)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_caveman_damselTiles, DAMSEL_SPRITE_SIZE,
                                           DAMSEL_SPRITESHEET_OFFSET);
    else if (ySpeed > 0)
        frameGfx =
                sprite_utils::get_frame((u8 *) gfx_caveman_damselTiles, DAMSEL_SPRITE_SIZE,
                                        DAMSEL_SPRITESHEET_OFFSET + 3);
    else
        frameGfx = sprite_utils::get_frame((u8 *) gfx_caveman_damselTiles, DAMSEL_SPRITE_SIZE,
                                           DAMSEL_SPRITESHEET_OFFSET + 1);

}

//!> after calling this function, call sprite_utils::update_frame to update OAM with current frameGfx
void Damsel::apply_walking_sprites() {

    if (animFrame >= 4)
        animFrame = 0;

    if (xSpeed == 0 && ySpeed == 0)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_caveman_damselTiles, DAMSEL_SPRITE_SIZE,
                                           DAMSEL_SPRITESHEET_OFFSET + 5);
    else if (xSpeed != 0 && ySpeed == 0)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_caveman_damselTiles, DAMSEL_SPRITE_SIZE,
                                           DAMSEL_SPRITESHEET_OFFSET + 6 + animFrame);
    else
        frameGfx = sprite_utils::get_frame((u8 *) gfx_caveman_damselTiles, DAMSEL_SPRITE_SIZE,
                                           DAMSEL_SPRITESHEET_OFFSET + 3);
}

//!> after calling this function, call sprite_utils::update_frame to update OAM with current frameGfx
void Damsel::apply_stunned_sprites() {
    if (animFrame >= 5)
        animFrame = 0;

    if (ySpeed == 0)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_caveman_damselTiles, DAMSEL_SPRITE_SIZE,
                                           DAMSEL_SPRITESHEET_OFFSET + 10 + animFrame);
    else if (ySpeed > 0)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_caveman_damselTiles, DAMSEL_SPRITE_SIZE,
                                           DAMSEL_SPRITESHEET_OFFSET + 4);
    else
        frameGfx = sprite_utils::get_frame((u8 *) gfx_caveman_damselTiles, DAMSEL_SPRITE_SIZE,
                                           DAMSEL_SPRITESHEET_OFFSET + 2);
}

//!> after calling this function, call sprite_utils::update_frame to update OAM with current frameGfx
void Damsel::apply_exiting_level_sprites() {
    if (animFrame >= 16) {
        rescued = true;
        global::game_state->damsels_rescued_this_level++;
    } else
        frameGfx = sprite_utils::get_frame((u8 *) gfx_caveman_damselTiles, DAMSEL_SPRITE_SIZE,
                                           DAMSEL_SPRITESHEET_OFFSET + 14 + animFrame);
}

//!> after calling this function, call sprite_utils::update_frame to update OAM with current frameGfx
void Damsel::apply_yelling_sprites() {

    if (animFrame >= 10) {
        yelling = false;
        yell_mainSpriteInfo->entry->isHidden = true;
        yell_subSpriteInfo->entry->isHidden = true;
    } else
        frameGfx = sprite_utils::get_frame((u8 *) gfx_caveman_damselTiles, DAMSEL_SPRITE_SIZE,
                                           DAMSEL_SPRITESHEET_OFFSET + 31 + animFrame);

}

//!> after calling this function, call sprite_utils::update_frame to update OAM with current frameGfx
void Damsel::match_animation() {

    if (global::game_state->smooching && global::game_state->smooch_timer > 0)
        apply_smooching_sprites();
    else if (yelling)
        apply_yelling_sprites();
    else if (exiting_level)
        apply_exiting_level_sprites();
    else if (stunned && !hold_by_main_dude)
        apply_stunned_sprites();
    else if (hold_by_main_dude)
        apply_dead_carried_sprites();
    else if (killed)
        apply_dead_sprites();
    else
        apply_walking_sprites();

    sprite_utils::update_frame(frameGfx, DAMSEL_SPRITE_SIZE, mainSpriteInfo, subSpriteInfo);
}

//!> after calling this function, call sprite_utils::update_frame to update OAM with current frameGfx
void Damsel::apply_smooching_sprites() {

    if (animFrame >= 10) {
        global::game_state->smooching = false;
        global::game_state->spawned_smooch = false;
        global::game_state->smooch_timer = 0;
        global::input_handler->right_key_held = true;
    } else
        frameGfx = (u8 *) gfx_caveman_damselTiles +
                   ((DAMSEL_SPRITE_SIZE * (DAMSEL_SPRITESHEET_OFFSET + animFrame + 43)) / 2);
}

Damsel::Damsel(int x, int y) : Damsel() {
    this->x = x;
    this->y = y;
}
