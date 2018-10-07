//
// Created by xdbeef on 25.05.18.
//

#include <cstdlib>
#include "shopkeeper.hpp"
#include "../../globals_declarations.hpp"
#include "../../collisions/collisions.hpp"
#include "../../tiles/tile_orientation.hpp"
#include "../../../build/gfx_shopkeeper.h"
#include "../sprite_utils.hpp"

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
//fixme Main dude field dead duplicates field killed

void Shopkeeper::draw() {

    if (ready_to_dispose)
        return;

    kill_if_whip(1);

    if (!global::main_dude->dead)
        kill_if_main_dude_jumped_on_you(1);

    check_if_dude_in_shop_bounds();

    invert_speed_timer += *global::timer;
    blood_spawn_timer += *global::timer;
    anim_frame_timer += *global::timer;
    jumping_timer += *global::timer;

    sprite_utils::set_horizontal_flip(sprite_state == SpriteState::W_RIGHT, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_visibility(true, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);

    if (stunned || killed) {

        check_if_can_be_pickuped();
        holding_shotgun = false;
        shotgun->hold_by_anyone = false;

    } else if (triggered && !holding_shotgun) {

        //searching for lost shotgun
        for (auto &sprite : global::sprites) {
            if (sprite->sprite_type == SpriteType::S_SHOTGUN) {

                auto *sh = (Shotgun *) sprite;
                if (Collisions::checkCollisionBodies(x, y, physical_width, physical_height,
                                                     sh->x, sh->y, sh->physical_width, sh->physical_height)) {
                    holding_shotgun = true;
                    shotgun = sh;
                    break;
                }
            }
        }

    }

    set_pickuped_position(9, -4);

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
        sprite_utils::set_priority(OBJPRIORITY_0, mainSpriteInfo, subSpriteInfo);

    } else
        sprite_utils::set_priority(OBJPRIORITY_2, mainSpriteInfo, subSpriteInfo);

    if (!hold_by_main_dude)
        kill_mobs_if_thrown(1);

    if (bottomCollision) {
        if (!triggered || killed || stunned) {
            apply_friction(0.5f);
        }
    }

    set_position();

    if (anim_frame_timer > SHOPKEEPER_ANIM_FRAME_DELTA) {
        anim_frame++;
        anim_frame_timer = 0;
        match_animation();
    }

    make_some_movement();

    if (stunned) {
        stunned_timer += *global::timer;
        if (stunned_timer > SHOPKEEPER_STUN_TIME) {
            stunned = false;
            stunned_timer = 0;
        }
    }

    if (triggered) {
        if (global::main_dude->dead) {
            triggered = false;
            standby = true;
        }
    }
}

void Shopkeeper::init() {

    if (global::game_state->robbed_killed_shopkeeper && !no_shotgun) {
        standby = true;
        spawn_shotgun();
    }

    activated = true;
    initSprite();
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

    MapTile *t[9] = {};
    Collisions::getNeighboringTiles(global::current_level->map_tiles,
                                    x_current_pos_in_tiles, y_current_pos_in_tiles, t);

    bool bounce = (stunned || killed);

    standingOnLeftEdge = Collisions::isStandingOnLeftEdge(t, x, physical_width, x_current_pos_in_tiles);
    standingOnRightEdge = Collisions::isStandingOnRightEdge(t, x, physical_width, x_current_pos_in_tiles);
    upperCollision = Collisions::checkUpperCollision(t, &x, &y, &ySpeed, physical_width, false, 0);
    bottomCollision = Collisions::checkBottomCollision(t, &x, &y, &ySpeed, physical_width,
                                                       physical_height, bounce, BOUNCING_FACTOR_Y);
    leftCollision = Collisions::checkLeftCollision(t, &x, &y, &xSpeed, physical_width,
                                                   physical_height, bounce, BOUNCING_FACTOR_X);
    rightCollision = Collisions::checkRightCollision(t, &x, &y, &xSpeed, physical_width,
                                                     physical_height, bounce, BOUNCING_FACTOR_X);


    if (bounce)
        return;

    if (bottomCollision && t[TileOrientation::RIGHT_MIDDLE] != nullptr &&
        t[TileOrientation::RIGHT_MIDDLE]->collidable &&
        t[TileOrientation::LEFT_MIDDLE] != nullptr &&
        t[TileOrientation::LEFT_MIDDLE]->collidable) {
        //high jump if shopkeeper's surrounded by tiles
        ySpeed = -5 - ((rand() % 10) / 5);
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

void Shopkeeper::apply_dmg(int dmg_to_apply) {

    if (dmg_to_apply == 0) {
        return;
    }

    hitpoints -= dmg_to_apply;
    if (blood_spawn_timer > 1000) {
        blood_spawn_timer = 0;
        ySpeed = -2.5; //jump a little bit on receiving dmg, even if dead
        if (!killed)
            spawn_blood();
    }

    if (!killed && hitpoints <= 0) {
        global::hud->disable_all_prompts();
        de_shopify_all_items();
        global::game_state->robbed_killed_shopkeeper = true;
        global::killed_npcs.push_back(SpriteType::S_SHOPKEEPER);
        killed = true;
        stunned = false;
    }

}

void Shopkeeper::initSprite() {

    delete mainSpriteInfo;
    delete subSpriteInfo;

    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_shopkeeperPal, gfx_shopkeeperPalLen,
                                                        nullptr, SHOPKEEPER_SPRITE_SIZE, 16, SHOPKEEPER,
                                                        true, false, LAYER_LEVEL::MIDDLE_BOT);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_shopkeeperPal, gfx_shopkeeperPalLen,
                                                          nullptr, SHOPKEEPER_SPRITE_SIZE, 16, SHOPKEEPER,
                                                          true, false, LAYER_LEVEL::MIDDLE_BOT);

    set_position();
    match_animation();
    sprite_utils::set_horizontal_flip(sprite_state == SpriteState::W_RIGHT, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_visibility(true, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
}

void Shopkeeper::set_position() {
    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);

    if (killed && ySpeed == 0 && xSpeed == 0) {
        main_y += 4;
        sub_y += 4;
    }

    sprite_utils::set_entry_xy(mainSpriteInfo, main_x, main_y);
    sprite_utils::set_entry_xy(subSpriteInfo, sub_x, sub_y);

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


//!> after calling this function, call sprite_utils::update_frame to update OAM with current frameGfx
void Shopkeeper::apply_stunned_carried_sprites() {
    anim_frame = 0;
    frameGfx = sprite_utils::get_frame((u8 *) gfx_shopkeeperTiles, SHOPKEEPER_SPRITE_SIZE,
                                       SHOPKEEPER_SPRITESHEET_OFFSET + 4);
}

//!> after calling this function, call sprite_utils::update_frame to update OAM with current frameGfx
void Shopkeeper::apply_dead_carried_sprites() {
    anim_frame = 0;
    frameGfx = sprite_utils::get_frame((u8 *) gfx_shopkeeperTiles, SHOPKEEPER_SPRITE_SIZE,
                                       SHOPKEEPER_SPRITESHEET_OFFSET);
}

//!> after calling this function, call sprite_utils::update_frame to update OAM with current frameGfx
void Shopkeeper::apply_dead_sprites() {
    anim_frame = 0;

    if (ySpeed == 0)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_shopkeeperTiles, SHOPKEEPER_SPRITE_SIZE,
                                           SHOPKEEPER_SPRITESHEET_OFFSET);
    else
        frameGfx = sprite_utils::get_frame((u8 *) gfx_shopkeeperTiles, SHOPKEEPER_SPRITE_SIZE,
                                           SHOPKEEPER_SPRITESHEET_OFFSET + 3);
}


//!> after calling this function, call sprite_utils::update_frame to update OAM with current frameGfx
void Shopkeeper::apply_walking_sprites() {

    if (anim_frame >= 6)
        anim_frame = 0;

    if (xSpeed == 0 && ySpeed == 0)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_shopkeeperTiles, SHOPKEEPER_SPRITE_SIZE,
                                           SHOPKEEPER_SPRITESHEET_OFFSET + 24);
    else
        frameGfx = sprite_utils::get_frame((u8 *) gfx_shopkeeperTiles, SHOPKEEPER_SPRITE_SIZE,
                                           SHOPKEEPER_SPRITESHEET_OFFSET + 12 + anim_frame);

}

//!> after calling this function, call sprite_utils::update_frame to update OAM with current frameGfx
void Shopkeeper::apply_stunned_sprites() {

    if (anim_frame >= 6)
        anim_frame = 0;

    if (ySpeed == 0)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_shopkeeperTiles, SHOPKEEPER_SPRITE_SIZE,
                                           SHOPKEEPER_SPRITESHEET_OFFSET + 18 + anim_frame);
    else if (ySpeed > 0)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_shopkeeperTiles, SHOPKEEPER_SPRITE_SIZE,
                                           SHOPKEEPER_SPRITESHEET_OFFSET + 4);
    else
        frameGfx = sprite_utils::get_frame((u8 *) gfx_shopkeeperTiles, SHOPKEEPER_SPRITE_SIZE,
                                           SHOPKEEPER_SPRITESHEET_OFFSET + 2);
}


void Shopkeeper::match_animation() {

    if (stunned && !hold_by_main_dude)
        apply_stunned_sprites();
    else if (hold_by_main_dude)
        apply_dead_carried_sprites();
    else if (killed)
        apply_dead_sprites();
    else
        apply_walking_sprites();

    sprite_utils::update_frame(frameGfx, SHOPKEEPER_SPRITE_SIZE, mainSpriteInfo, subSpriteInfo);
}

void Shopkeeper::set_shop_bounds() {

    //based on init position and constant shop layout defined in shops.h
    //right: X + 3*TILE_W
    //left: X - 7*TILE_W
    //up: y - 8*TILE_H
    //down: y + 2*TILE_H


    MapTile *shop_sign = nullptr;
    global::current_level->get_first_tile_of_given_type(MapTileType::SHOP_SIGN_RARE, shop_sign);

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
            global::hud->draw_level_hud();
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
            (!shopkeeper_in_shop && jumping_timer > staying_on_ground && bottomCollision && !(killed || stunned))) {
            ySpeed = -5 - ((rand() % 10) / 5);
            jumping_timer = 0;
            staying_on_ground = 250 + (rand() % 1250);
        }
    }
}

void Shopkeeper::de_shopify_all_items() {
    for (int a = 0; a < 4; a++) {

        if (shop_items[a] == nullptr)
            continue;

        shop_items[a]->bought = true;
        shop_items[a]->shopping_icon->set_ready_to_dispose();

    }
    global::game_state->robbed_killed_shopkeeper = true;
}

void Shopkeeper::spawn_shotgun() {
    shotgun = new Shotgun();
    shotgun->hold_by_main_dude = false;
    shotgun->bought = true;
    shotgun->init();
    shotgun->cooldown = 250;
    global::sprites_to_add.push_back(shotgun);
    holding_shotgun = true;

}

void Shopkeeper::trigger() {

    global::hud->disable_all_prompts();
    standby = false;
    triggered = true;
    global::hud->thief = true;
    global::hud->draw_level_hud();
    de_shopify_all_items();
    if (!holding_shotgun) {
        spawn_shotgun();
    }
}


Shopkeeper::Shopkeeper(int x, int y) : Shopkeeper() {
    this->x = x;
    this->y = y;
}

void Shopkeeper::deleteSprite() {
    delete mainSpriteInfo;
    delete subSpriteInfo;
    mainSpriteInfo = nullptr;
    subSpriteInfo = nullptr;
}
