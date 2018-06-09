//
// Created by xdbeef on 31.05.18.
//


#include "caveman.h"
#include "../../globals_declarations.h"
#include "../../collisions/collisions.h"
#include "../../../build/gfx_caveman_damsel.h"
#include "../../tiles/tile_orientation.h"
#include "damsel.h"

#define DAMSEL_SPRITESHEET_OFFSET 25
#define DAMSEL_POS_INC_DELTA 20
#define DAMSEL_TRIGGERED_SPEED 1.2
#define DAMSEL_ANIM_FRAME_DELTA 80
#define DAMSEL_INVERT_SPEED_DELTA 400

void Damsel::draw() {

    if (rescued) {
        mainSpriteInfo->entry->isHidden = true;
        subSpriteInfo->entry->isHidden = true;
        return;
    }

    invert_speed_timer += *global::timer;
    blood_spawn_timer += *global::timer;

    if (spriteState == SpriteState::W_LEFT) {
        mainSpriteInfo->entry->hFlip = false;
        subSpriteInfo->entry->hFlip = false;
    } else if (spriteState == SpriteState::W_RIGHT) {
        mainSpriteInfo->entry->hFlip = true;
        subSpriteInfo->entry->hFlip = true;
    }

    mainSpriteInfo->entry->isHidden = false;
    subSpriteInfo->entry->isHidden = false;
    mainSpriteInfo->entry->vFlip = false;
    subSpriteInfo->entry->vFlip = false;

    check_if_can_be_pickuped();
    set_pickuped_position(6, -2);

    if (hold_by_main_dude) {

        if (!killed)
            global::main_dude->carrying_damsel = true;

        if (global::main_dude->killed) {
            hold_by_main_dude = false;
            stunned = true;
            stunned_timer = 0;
        }

        spriteState = global::main_dude->state;
        mainSpriteInfo->entry->priority = OBJPRIORITY_0;
        subSpriteInfo->entry->priority = OBJPRIORITY_0;

    } else {
        mainSpriteInfo->entry->priority = OBJPRIORITY_2;
        subSpriteInfo->entry->priority = OBJPRIORITY_2;
    }

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

    if (animFrameTimer > DAMSEL_ANIM_FRAME_DELTA) {

        animFrame++;

        if (exiting_level) {
            apply_exiting_level_sprites();
        } else if (stunned && !hold_by_main_dude)
            apply_stunned_sprites();
        else if (hold_by_main_dude)
            apply_dead_carried_sprites();
        else if (killed)
            apply_dead_sprites();
        else
            apply_walking_sprites();

        subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
        mainSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
        animFrameTimer = 0;

    }

    make_some_movement();

//    if (!stunned && !killed)
//        kill_if_whip(0);

    if (stunned) {
        stunned_timer += *global::timer;
        if (stunned_timer > DAMSEL_STUN_TIME) {
            stunned = false;
            stunned_timer = 0;
        }
    }


    if (hold_by_main_dude && !killed) {
        MapTile *tiles[9] = {};
        Collisions::getNeighboringTiles(global::level_generator->map_tiles, global::main_dude->current_x_in_tiles,
                                        global::main_dude->current_y_in_tiles, tiles);

        exiting_level = tiles[CENTER] != nullptr && tiles[CENTER]->mapTileType == MapTileType::EXIT;
        if (exiting_level) {
            x = tiles[CENTER]->x * 16;
            y = tiles[CENTER]->y * 16;
            hold_by_main_dude = false;
            if (global::main_dude->carrying_damsel) {
                global::main_dude->holding_item = false;
                global::main_dude->carrying_damsel = false;
            }
        }
    }
}


void Damsel::init() {

    activated_by_main_dude = true;
    initSprite();

    frameGfx = (u8 *) gfx_caveman_damselTiles;
    subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
    mainSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);

    spriteType = SpritesheetType::CAVEMAN_DAMSEL;

    randomizeMovement();
}

void Damsel::randomizeMovement() {

    int r = rand() % 2;

    if (r == 0) {
        spriteState = SpriteState::W_LEFT;

    } else if (r == 1) {
        if (spriteState == SpriteState::W_RIGHT)
            spriteState = SpriteState::W_RIGHT;
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

    MapTile *tiles[9] = {};
    Collisions::getNeighboringTiles(global::level_generator->map_tiles, x_current_pos_in_tiles,
                                    y_current_pos_in_tiles, tiles);


    bool bounce = (stunned || killed);

    standingOnLeftEdge = Collisions::isStandingOnLeftEdge(tiles, x, physical_width, x_current_pos_in_tiles);
    standingOnRightEdge = Collisions::isStandingOnRightEdge(tiles, x, physical_width, x_current_pos_in_tiles);
    bottomCollision = Collisions::checkBottomCollision(tiles, &x, &y, &ySpeed, physical_width, physical_height,
                                                       bounce, BOUNCING_FACTOR_Y);
    leftCollision = Collisions::checkLeftCollision(tiles, &x, &y, &xSpeed, physical_width, physical_height, bounce,
                                                   BOUNCING_FACTOR_X);
    rightCollision = Collisions::checkRightCollision(tiles, &x, &y, &xSpeed, physical_width, physical_height,
                                                     bounce, BOUNCING_FACTOR_X);
    upperCollision = Collisions::checkUpperCollision(tiles, &x, &y, &ySpeed, physical_width, false, 0);


    if (stunned || killed || exiting_level)
        return;

    if (bottomCollision && tiles[TileOrientation::RIGHT_MIDDLE] != nullptr &&
        tiles[TileOrientation::RIGHT_MIDDLE]->collidable &&
        tiles[TileOrientation::LEFT_MIDDLE] != nullptr && tiles[TileOrientation::LEFT_MIDDLE]->collidable) {
        ySpeed = -2.6 - ((rand() % 10) / 5);
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

void Damsel::apply_dmg(int dmg_to_apply) {

    if (dmg_to_apply == 0 || dmg_to_apply == 1) {
        return;
    }

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
        global::killed_npcs.push_back(spriteType);
    } else {
        stunned = true;
    }

}

void Damsel::initSprite() {
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

    if (spriteState == SpriteState::W_LEFT) {
        mainSpriteInfo->entry->vFlip = false;
    } else if (spriteState == SpriteState::W_RIGHT) {
        mainSpriteInfo->entry->vFlip = true;
    }


}

void Damsel::set_position() {

    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);

    mainSpriteInfo->entry->x = main_x;
    mainSpriteInfo->entry->y = main_y;

    subSpriteInfo->entry->x = sub_x;
    subSpriteInfo->entry->y = sub_y;

}

Damsel::Damsel() {
    physical_height = DAMSEL_PHYSICAL_HEIGHT;
    physical_width = DAMSEL_PHYSICAL_WIDTH;
    sprite_height = DAMSEL_SPRITE_HEIGHT;
    sprite_width = DAMSEL_SPRITE_WIDTH;
    hitpoints = DAMSEL_HITPOINTS;
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
            if (spriteState == SpriteState::W_RIGHT)
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

            if (spriteState == SpriteState::W_LEFT)
                spriteState = SpriteState::W_RIGHT;
            else
                spriteState = SpriteState::W_LEFT;

            xSpeed *= -1;
            rightCollision = false;
            leftCollision = false;
        }

    }

}

void Damsel::check_if_can_be_triggered() {


}

void Damsel::apply_stunned_carried_sprites() {

    animFrame = 0;
    frameGfx = (u8 *) gfx_caveman_damselTiles + ((sprite_width * sprite_height * (DAMSEL_SPRITESHEET_OFFSET + 4)) / 2);
}

void Damsel::apply_dead_carried_sprites() {
    animFrame = 0;
    frameGfx = (u8 *) gfx_caveman_damselTiles + ((sprite_width * sprite_height * (DAMSEL_SPRITESHEET_OFFSET + 4)) / 2);
}

void Damsel::apply_dead_sprites() {
    animFrame = 0;
    if (ySpeed == 0)
        frameGfx =
                (u8 *) gfx_caveman_damselTiles + ((sprite_width * sprite_height * (DAMSEL_SPRITESHEET_OFFSET)) / 2);
    else if (ySpeed > 0)
        frameGfx =
                (u8 *) gfx_caveman_damselTiles + ((sprite_width * sprite_height * (DAMSEL_SPRITESHEET_OFFSET + 3)) / 2);
    else
        frameGfx =
                (u8 *) gfx_caveman_damselTiles + ((sprite_width * sprite_height * (DAMSEL_SPRITESHEET_OFFSET + 1)) / 2);

}


void Damsel::apply_walking_sprites() {

    if (animFrame >= 4)
        animFrame = 0;

    if (xSpeed == 0 && ySpeed == 0)
        frameGfx = (u8 *) gfx_caveman_damselTiles +
                   ((sprite_width * sprite_height * (DAMSEL_SPRITESHEET_OFFSET + 5)) / 2);
    else
        frameGfx = (u8 *) gfx_caveman_damselTiles +
                   ((sprite_width * sprite_height * (DAMSEL_SPRITESHEET_OFFSET + animFrame + 6)) / 2);

}

void Damsel::apply_stunned_sprites() {
    if (animFrame >= 5)
        animFrame = 0;

    if (ySpeed == 0)
        frameGfx = (u8 *) gfx_caveman_damselTiles +
                   ((sprite_width * sprite_height * (DAMSEL_SPRITESHEET_OFFSET + animFrame + 10)) / 2);
    else if (ySpeed > 0)
        frameGfx =
                (u8 *) gfx_caveman_damselTiles + ((sprite_width * sprite_height * (DAMSEL_SPRITESHEET_OFFSET + 4)) / 2);
    else
        frameGfx =
                (u8 *) gfx_caveman_damselTiles + ((sprite_width * sprite_height * (DAMSEL_SPRITESHEET_OFFSET + 2)) / 2);
}

void Damsel::apply_exiting_level_sprites() {
    if (animFrame >= 16) {
        rescued = true;
    } else {

        frameGfx = (u8 *) gfx_caveman_damselTiles +
                   ((sprite_width * sprite_height * (DAMSEL_SPRITESHEET_OFFSET + animFrame + 14)) / 2);
    }
}