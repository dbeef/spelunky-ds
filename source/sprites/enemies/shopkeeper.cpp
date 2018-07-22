//
// Created by xdbeef on 25.05.18.
//

#include <iostream>
#include "shopkeeper.h"
#include "../../globals_declarations.h"
#include "../../collisions/collisions.h"
#include "../../tiles/tile_orientation.h"
#include "../../../build/gfx_shopkeeper.h"

#define SHOPKEEPER_SPRITESHEET_OFFSET 0
#define SHOPKEEPER_POS_INC_DELTA 18
#define SHOPKEEPER_TRIGGERED_SPEED 3
#define SHOPKEEPER_ANIM_FRAME_DELTA 65
#define SHOPKEEPER_INVERT_SPEED_DELTA 100

//todo Make "standby" mode for shopkeeper (not triggered, but having shotgun and triggering just on walking into a shop)
//todo Make separate 8x8 spritesheet for bomb and 64x64 for explosion animation
//1) Shopkeeper does not shoot if there's no main dude nearby (and triggered)
//2) Shopkeeper can jump very high when triggered
//3) Inverts speed very fast
//4) If shopkeeper is on ~same y-height as main dude it does not jump
//todo Main dude field dead duplicates field killed

void Shopkeeper::draw() {

    kill_if_whip(1);

    if (!global::main_dude->dead)
        kill_if_main_dude_jumped_on_you(1);

    check_if_dude_in_shop_bounds();

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

    if (stunned || killed)
        check_if_can_be_pickuped();

    set_pickuped_position(9, -4);

    //TODO Odszukiwanie shotguna

    if (stunned || killed) {
        holding_shotgun = false;
        shotgun->hold_by_anyone = false;
        shotgun = nullptr;
    } else if (triggered && !holding_shotgun) {
        for (int a = 0; a < global::sprites.size(); a++) {
            if (global::sprites.at(a)->sprite_type == SpriteType::S_SHOTGUN) {
                Shotgun *sh = (Shotgun *) global::sprites.at(a);

                if (Collisions::checkCollisionBodies(x, y, physical_width, physical_height,
                                                     sh->x, sh->y, sh->physical_width, sh->physical_height)) {
                    holding_shotgun = true;
                    shotgun = sh;
                    break;
                }

            }
        }
    }

    if (shotgun != nullptr && holding_shotgun) {

        shotgun->hold_by_anyone = true;

        set_pickuped_position_on_another_moving_obj(5, 3, shotgun);

        if (xSpeed > 0) {
            shotgun->sprite_state = SpriteState::W_RIGHT;
        } else if (xSpeed < 0) {
            shotgun->sprite_state = SpriteState::W_LEFT;
        }

        if (!global::main_dude->dead && triggered) {
            shotgun->activated = true;
        }
    }

    if (hold_by_main_dude) {

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

    if (bottomCollision) {
        if (!triggered || killed || stunned) {
            apply_friction(0.5f);
        }
    }

    set_position();

    anim_frame_timer += *global::timer;

    if (anim_frame_timer > SHOPKEEPER_ANIM_FRAME_DELTA) {
        update_animation();
    }

    make_some_movement();

    if (stunned) {
        stunned_timer += *global::timer;
        if (stunned_timer > SHOPKEEPER_STUN_TIME) {
            stunned = false;
            stunned_timer = 0;
        }
    }

    jumping_timer += *global::timer;


    if (triggered) {
        if (global::main_dude->dead) {
            triggered = false;
            standby = true;
        }
    }
}

void Shopkeeper::init() {

    if (global::game_state->robbed_killed_shopkeeper) {
        standby = true;
        spawn_shotgun();
    }

    activated = true;
    initSprite();

    frameGfx = (u8 *) gfx_shopkeeperTiles;
    subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
    mainSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);

    spritesheet_type = SpritesheetType::SHOPKEEPER;

    randomizeMovement();
}

void Shopkeeper::randomizeMovement() {

    int r = rand() % 2;

    if (r == 0) {
        sprite_state = SpriteState::W_LEFT;
        if (shotgun != nullptr)
            shotgun->sprite_state = SpriteState::W_LEFT;
    } else if (r == 1) {

        sprite_state = SpriteState::W_RIGHT;

        if (shotgun != nullptr)
            shotgun->sprite_state = SpriteState::W_RIGHT;
    }

    if (standby) {
        go_timer = (rand() % 500) + 100;
        waitTimer = (rand() % 1500) + 2000;
    } else if (triggered) {
        go_timer = (rand() % (1 * 2000)) + 1000;
        waitTimer = rand() % 1500;
    }
}

void Shopkeeper::updateSpeed() {

    limit_speed(MAX_X_SPEED_SHOPKEEPER, MAX_Y_SPEED_SHOPKEEPER);

    pos_inc_timer += *global::timer;

    if (pos_inc_timer > SHOPKEEPER_POS_INC_DELTA) {
        update_position();
        apply_gravity(GRAVITY_DELTA_SPEED * 1.5);
        pos_inc_timer = 0;
    }

}

void Shopkeeper::updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) {

    MapTile *tiles[9] = {};
    Collisions::getNeighboringTiles(global::level_generator->map_tiles, x_current_pos_in_tiles,
                                    y_current_pos_in_tiles, tiles);


    bool bounce = (stunned || killed);

    standingOnLeftEdge = Collisions::isStandingOnLeftEdge(tiles, x, physical_width, x_current_pos_in_tiles);
    standingOnRightEdge = Collisions::isStandingOnRightEdge(tiles, x, physical_width, x_current_pos_in_tiles);
    upperCollision = Collisions::checkUpperCollision(tiles, &x, &y, &ySpeed, physical_width, false, 0);
    bottomCollision = Collisions::checkBottomCollision(tiles, &x, &y, &ySpeed, physical_width, physical_height,
                                                       bounce, BOUNCING_FACTOR_Y);
    leftCollision = Collisions::checkLeftCollision(tiles, &x, &y, &xSpeed, physical_width, physical_height, bounce,
                                                   BOUNCING_FACTOR_X);
    rightCollision = Collisions::checkRightCollision(tiles, &x, &y, &xSpeed, physical_width, physical_height,
                                                     bounce, BOUNCING_FACTOR_X);


    if (stunned || killed)
        return;

    if (bottomCollision && tiles[TileOrientation::RIGHT_MIDDLE] != nullptr &&
        tiles[TileOrientation::RIGHT_MIDDLE]->collidable &&
        tiles[TileOrientation::LEFT_MIDDLE] != nullptr && tiles[TileOrientation::LEFT_MIDDLE]->collidable) {
        ySpeed = -5 - ((rand() % 10) / 5);
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

void Shopkeeper::apply_dmg(int dmg_to_apply) {

    if (dmg_to_apply == 0) {
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
        global::hud->disable_all_prompts();
        global::hud->draw();
        de_shopify_all_items();
        global::game_state->robbed_killed_shopkeeper = true;
        killed = true;
        stunned = false;
        global::killed_npcs.push_back(spritesheet_type);
    }

}

void Shopkeeper::initSprite() {
    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_shopkeeperPal, gfx_shopkeeperPalLen,
                                                        nullptr, sprite_width * sprite_height, 16, SHOPKEEPER,
                                                        true,
                                                        false,
                                                        LAYER_LEVEL::MIDDLE_BOT);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_shopkeeperPal, gfx_shopkeeperPalLen,
                                                          nullptr, sprite_width * sprite_height, 16, SHOPKEEPER,
                                                          true,
                                                          false, LAYER_LEVEL::MIDDLE_BOT);

    frameGfx = (u8 *) gfx_shopkeeperTiles +
               ((sprite_width * sprite_height * (SHOPKEEPER_SPRITESHEET_OFFSET + 31)) / 2);

    subSpriteInfo->entry->isHidden = false;
    mainSpriteInfo->entry->isHidden = false;

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

void Shopkeeper::set_position() {

    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);

    mainSpriteInfo->entry->x = main_x;
    mainSpriteInfo->entry->y = main_y;

    subSpriteInfo->entry->x = sub_x;
    subSpriteInfo->entry->y = sub_y;

    int temp_y = y;
    y -= 13 + (anim_frame * 0.5);

    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);

    y = temp_y;

}

Shopkeeper::Shopkeeper() {
    physical_height = SHOPKEEPER_PHYSICAL_HEIGHT;
    physical_width = SHOPKEEPER_PHYSICAL_WIDTH;
    sprite_height = SHOPKEEPER_SPRITE_HEIGHT;
    sprite_width = SHOPKEEPER_SPRITE_WIDTH;
    hitpoints = SHOPKEEPER_HITPOINTS;
}


void Shopkeeper::make_some_movement() {

    if (killed || stunned)
        return;

    if (waitTimer > 0 && (!triggered || standby)) {
        waitTimer -= *global::timer;
    } else {

        if (go_timer > 0)
            go_timer -= *global::timer;

        if (triggered) {
            if (sprite_state == SpriteState::W_RIGHT)
                xSpeed = SHOPKEEPER_TRIGGERED_SPEED;
            else
                xSpeed = -SHOPKEEPER_TRIGGERED_SPEED;
        } else if (standby) {
            if (sprite_state == SpriteState::W_RIGHT)
                xSpeed = 1;
            else
                xSpeed = -1;
        }

        if (go_timer <= 0 && (!triggered && standby)) {
            randomizeMovement();
            xSpeed = 0;
        }

        if (invert_speed_timer < SHOPKEEPER_INVERT_SPEED_DELTA)
            return;

        invert_speed_timer = 0;

        if ((standingOnLeftEdge || standingOnRightEdge) && !triggered) {
            if (rand() % 2 == 0) {
                go_timer = 0;
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

void Shopkeeper::check_if_can_be_triggered() {}

void Shopkeeper::apply_stunned_carried_sprites() {

    anim_frame = 0;
    frameGfx =
            (u8 *) gfx_shopkeeperTiles + ((sprite_width * sprite_height * (SHOPKEEPER_SPRITESHEET_OFFSET + 4)) / 2);
}

void Shopkeeper::apply_dead_carried_sprites() {
    anim_frame = 0;
    frameGfx =
            (u8 *) gfx_shopkeeperTiles + ((sprite_width * sprite_height * (SHOPKEEPER_SPRITESHEET_OFFSET + 0)) / 2);
}

void Shopkeeper::apply_dead_sprites() {
    anim_frame = 0;
    if (ySpeed == 0)
        frameGfx =
                (u8 *) gfx_shopkeeperTiles + ((sprite_width * sprite_height * (1)) / 2);
    else if (ySpeed > 0)
        frameGfx =
                (u8 *) gfx_shopkeeperTiles +
                ((sprite_width * sprite_height * (SHOPKEEPER_SPRITESHEET_OFFSET + 3)) / 2);
    else
        frameGfx =
                (u8 *) gfx_shopkeeperTiles +
                ((sprite_width * sprite_height * (SHOPKEEPER_SPRITESHEET_OFFSET + 3)) / 2);

}


void Shopkeeper::apply_walking_sprites() {

    if (anim_frame >= 6)
        anim_frame = 0;

    if (xSpeed == 0 && ySpeed == 0)
        frameGfx = (u8 *) gfx_shopkeeperTiles +
                   ((sprite_width * sprite_height * (SHOPKEEPER_SPRITESHEET_OFFSET + 24)) / 2);
    else if (xSpeed != 0 || ySpeed != 0) {
        frameGfx = (u8 *) gfx_shopkeeperTiles +
                   ((sprite_width * sprite_height * (SHOPKEEPER_SPRITESHEET_OFFSET + anim_frame + 12)) / 2);
    } /*else {
        frameGfx = (u8 *) gfx_shopkeeperTiles +
                   ((sprite_width * sprite_height * (SHOPKEEPER_SPRITESHEET_OFFSET + 3)) / 2);
    }*/

}

void Shopkeeper::apply_stunned_sprites() {

    if (anim_frame >= 6)
        anim_frame = 0;

    if (ySpeed == 0)
        frameGfx = (u8 *) gfx_shopkeeperTiles +
                   ((sprite_width * sprite_height * (SHOPKEEPER_SPRITESHEET_OFFSET + anim_frame + 18)) / 2);
    else if (ySpeed > 0)
        frameGfx =
                (u8 *) gfx_shopkeeperTiles +
                ((sprite_width * sprite_height * (SHOPKEEPER_SPRITESHEET_OFFSET + 4)) / 2);
    else
        frameGfx =
                (u8 *) gfx_shopkeeperTiles +
                ((sprite_width * sprite_height * (SHOPKEEPER_SPRITESHEET_OFFSET + 2)) / 2);
}


void Shopkeeper::update_animation() {
    anim_frame++;

    if (stunned && !hold_by_main_dude)
        apply_stunned_sprites();
    else if (hold_by_main_dude)
        apply_dead_carried_sprites();
    else if (killed)
        apply_dead_sprites();
    else
        apply_walking_sprites();

    subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
    mainSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
    anim_frame_timer = 0;

}

void Shopkeeper::set_shop_bounds() {

    //based on init position and constant shop layout defined in shops.h
    //right: X + 3*TILE_W
    //left: X - 7*TILE_W
    //up: y - 8*TILE_H
    //down: y + 2*TILE_H


    MapTile *shop_sign = nullptr;
    global::level_generator->get_first_tile(MapTileType::SHOP_SIGN_RARE, shop_sign);

    int tile_x = shop_sign->x * TILE_W;

    shop_bounds_up_y_px = y - (8 * TILE_H);
    shop_bounds_down_y_px = y + (2 * TILE_H);

    if (x - tile_x > 0) {
        //left oriented shop (exit/entrance is on the left side)
        sprite_state = SpriteState::W_LEFT;
        shop_bounds_right_x_px = x + (3 * TILE_W);
        if (standby)
            shop_bounds_left_x_px = x - (9 * TILE_W);
        else
            shop_bounds_left_x_px = x - (6 * TILE_W);

    } else {
        sprite_state = SpriteState::W_RIGHT;
        shop_bounds_left_x_px = x - (3 * TILE_W);
        if (standby)
            shop_bounds_right_x_px = x + (9 * TILE_W);
        else
            shop_bounds_right_x_px = x + (6 * TILE_W);
    }

}


//done
void Shopkeeper::check_if_dude_in_shop_bounds() {

    if (killed)
        return;

    if (global::main_dude->x < shop_bounds_right_x_px && global::main_dude->x > shop_bounds_left_x_px &&
        global::main_dude->y > shop_bounds_up_y_px && global::main_dude->y < shop_bounds_down_y_px) {

        if (standby) {
            trigger();
        }

        if (!introduced_shop_name) {
            global::hud->disable_all_prompts();
            global::hud->introduce_shop = true;
            global::hud->shop_name = "\n\n\n\nWELCOME TO SMITHY'S SUPPLY SHOP!";
            global::hud->draw();
            introduced_shop_name = true;
        }

        int diff = x - global::main_dude->x;

        if (!triggered) {

            if (diff > 0)
                sprite_state = SpriteState::W_LEFT;
            else
                sprite_state = SpriteState::W_RIGHT;

            if (global::hud->holding_item_shopping) {

                int abs_diff = abs(diff);

                if (abs_diff > 1.5 * TILE_W) {

                    if (diff > 0)
                        xSpeed = -2.5f;
                    else
                        xSpeed = 2.5f;

                }

            }

        }


    } else if (global::hud->holding_item_shopping && !triggered) {
        trigger();
    } else {

        bool shopkeeper_in_shop = x < shop_bounds_right_x_px && x > shop_bounds_left_x_px &&
                                  y > shop_bounds_up_y_px && y < shop_bounds_down_y_px;

        if ((bottomCollision && (leftCollision || rightCollision)) ||
            !shopkeeper_in_shop && jumping_timer > staying_on_ground && bottomCollision && !(killed || stunned)) {
            ySpeed = -5 - ((rand() % 10) / 5);
            jumping_timer = 0;
            staying_on_ground = 250 + (rand() % 1250);
        }
    }
}

void Shopkeeper::de_shopify_all_items() {
    for (int a = 0; a < 4; a++) {
        if (shop_items[a] != nullptr)
            shop_items[a]->bought = true;
    }
    global::game_state->robbed_killed_shopkeeper = true;
}

void Shopkeeper::spawn_shotgun() {
    shotgun = new Shotgun();
    shotgun->hold_by_main_dude = false;
    shotgun->init();
    shotgun->bought = true;
    shotgun->cooldown = 250;
    global::sprites.push_back(shotgun);
    holding_shotgun = true;

}

void Shopkeeper::trigger() {

    global::hud->disable_all_prompts();
    standby = false;
    triggered = true;
    global::hud->thief = true;
    global::hud->draw();
    de_shopify_all_items();
    if (!holding_shotgun) {
        spawn_shotgun();
    }
}
