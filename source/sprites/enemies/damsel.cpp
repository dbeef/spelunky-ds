//
// Created by xdbeef on 31.05.18.
//

#include <maxmod9.h>
#include <cstdlib>
#include "caveman.h"
#include "../../globals_declarations.h"
#include "../../collisions/collisions.h"
#include "../../../build/gfx_caveman_damsel.h"
#include "../../tiles/tile_orientation.h"
#include "damsel.h"
#include "../../../build/soundbank.h"

#define DAMSEL_SPRITESHEET_OFFSET 25
#define DAMSEL_POS_INC_DELTA 18
#define DAMSEL_TRIGGERED_SPEED 1.2
#define DAMSEL_ANIM_FRAME_DELTA 65
#define DAMSEL_INVERT_SPEED_DELTA 400

void Damsel::draw() {

    if (rescued) {
        mainSpriteInfo->entry->isHidden = true;
        subSpriteInfo->entry->isHidden = true;
        yell_mainSpriteInfo->entry->isHidden = true;
        yell_subSpriteInfo->entry->isHidden = true;
        return;
    }

    yell_subSpriteInfo->entry->vFlip = false;
    yell_mainSpriteInfo->entry->vFlip = false;
    yell_subSpriteInfo->entry->hFlip = false;
    yell_mainSpriteInfo->entry->hFlip = false;

    if (killed)
        yelling = false;

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


    if (yelling) {
        yell_mainSpriteInfo->entry->isHidden = false;
        yell_subSpriteInfo->entry->isHidden = false;
    } else {
        yell_mainSpriteInfo->entry->isHidden = true;
        yell_subSpriteInfo->entry->isHidden = true;
    }

    if (yelling) {
        if (animFrameTimer > DAMSEL_ANIM_FRAME_DELTA * 2) {
            update_animation();
        }
    } else {
        if (animFrameTimer > DAMSEL_ANIM_FRAME_DELTA) {
            update_animation();
        }
    }

    make_some_movement();

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

    frameGfx = (u8 *) gfx_caveman_damselTiles;
    subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
    mainSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);

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
        spawn_blood();
        blood_spawn_timer = 0;
        ySpeed = -2.5;
    }

    if (killed)
        return;

    if (hitpoints <= 0) {
        killed = true;
        stunned = false;
        global::killed_npcs.push_back(spritesheet_type);
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

    yell_subSpriteInfo = global::sub_oam_manager->initSprite(gfx_caveman_damselPal, gfx_caveman_damselPalLen,
                                                             nullptr, sprite_width * sprite_height, 16, CAVEMAN_DAMSEL,
                                                             true,
                                                             false,
                                                             LAYER_LEVEL::MIDDLE_BOT);
    yell_mainSpriteInfo = global::main_oam_manager->initSprite(gfx_caveman_damselPal, gfx_caveman_damselPalLen,
                                                               nullptr, sprite_width * sprite_height, 16,
                                                               CAVEMAN_DAMSEL,
                                                               true,
                                                               false, LAYER_LEVEL::MIDDLE_BOT);

    frameGfx = (u8 *) gfx_caveman_damselTiles +
               ((sprite_width * sprite_height * (DAMSEL_SPRITESHEET_OFFSET + 31)) / 2);

    yell_subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
    yell_mainSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);


    subSpriteInfo->entry->isHidden = false;
    mainSpriteInfo->entry->isHidden = false;
    yell_subSpriteInfo->entry->isHidden = true;
    yell_mainSpriteInfo->entry->isHidden = true;
    yell_subSpriteInfo->entry->vFlip = false;
    yell_mainSpriteInfo->entry->vFlip = false;

    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);

    set_position();

    if (sprite_state == SpriteState::W_LEFT) {
        mainSpriteInfo->entry->vFlip = false;
        subSpriteInfo->entry->vFlip = false;
    } else if (sprite_state == SpriteState::W_RIGHT) {
        mainSpriteInfo->entry->vFlip = true;
        subSpriteInfo->entry->vFlip = true;
    }


}

void Damsel::set_position() {

    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);

    mainSpriteInfo->entry->x = main_x;
    mainSpriteInfo->entry->y = main_y;

    subSpriteInfo->entry->x = sub_x;
    subSpriteInfo->entry->y = sub_y;

    int temp_y = y;
    y -= 13 + (animFrame * 0.5);

    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);

    yell_mainSpriteInfo->entry->x = main_x;
    yell_mainSpriteInfo->entry->y = main_y;

    yell_subSpriteInfo->entry->x = sub_x;
    yell_subSpriteInfo->entry->y = sub_y;

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
    else if (xSpeed != 0 && ySpeed == 0) {
        frameGfx = (u8 *) gfx_caveman_damselTiles +
                   ((sprite_width * sprite_height * (DAMSEL_SPRITESHEET_OFFSET + animFrame + 6)) / 2);
    } else {
        frameGfx = (u8 *) gfx_caveman_damselTiles +
                   ((sprite_width * sprite_height * (DAMSEL_SPRITESHEET_OFFSET + 3)) / 2);

    }

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
        global::game_state->damsels_rescued_this_level++;
    } else {

        frameGfx = (u8 *) gfx_caveman_damselTiles +
                   ((sprite_width * sprite_height * (DAMSEL_SPRITESHEET_OFFSET + animFrame + 14)) / 2);
    }
}

void Damsel::apply_yelling_sprites() {

    if (animFrame >= 10) {
        yelling = false;
        yell_mainSpriteInfo->entry->isHidden = true;
        yell_subSpriteInfo->entry->isHidden = true;
    } else {
        frameGfx = (u8 *) gfx_caveman_damselTiles +
                   ((sprite_width * sprite_height * (DAMSEL_SPRITESHEET_OFFSET + animFrame + 31)) / 2);
    }


}

void Damsel::update_animation() {
    animFrame++;

    if (global::game_state->smooching && global::game_state->smooch_timer > 0) {
        apply_smooching_sprites();
    } else if (yelling) {
        apply_yelling_sprites();
    } else if (exiting_level) {
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

void Damsel::apply_smooching_sprites() {

    if (animFrame >= 10) {
        global::game_state->smooching = false;
        global::game_state->spawned_smooch = false;
        global::game_state->smooch_timer = 0;
        global::input_handler->right_key_held = true;
    } else {
        frameGfx = (u8 *) gfx_caveman_damselTiles +
                   ((sprite_width * sprite_height * (DAMSEL_SPRITESHEET_OFFSET + animFrame + 43)) / 2);
    }


}
