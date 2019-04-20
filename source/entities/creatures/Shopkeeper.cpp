//
// Created by xdbeef on 25.05.18.
//

#include <cstdlib>
#include "Shopkeeper.hpp"
#include "../../GlobalsDeclarations.hpp"
#include "../../collisions/Collisions.hpp"
#include "../../tiles/TileOrientation.hpp"
#include "../../../build/gfx_shopkeeper.h"
#include "../../memory/SpriteUtils.hpp"

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

void Shopkeeper::update_creature_specific() {

    if (_ready_to_dispose)
        return;

    kill_if_whip(1);

    if (!global::main_dude->dead)
        kill_if_main_dude_jumped_on_you(1);

    check_if_dude_in_shop_bounds();

    invert_speed_timer += *global::timer;
    blood_spawn_timer += *global::timer;
    anim_frame_timer += *global::timer;
    jumping_timer += *global::timer;

    sprite_utils::set_horizontal_flip(sprite_state == Orientation::RIGHT, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_visibility(true, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);

    if (stunned || killed) {

        check_if_can_be_pickuped();
        holding_shotgun = false;
        shotgun->_hold_by_anyone = false;

    } else if (triggered && !holding_shotgun) {

        //searching for lost shotgun
        for (auto &creature : global::creatures) {
            if (creature->_creature_type == CreatureType::SHOTGUN) {

                auto *sh = (Shotgun *) creature;
                if (Collisions::checkCollisionBodies(_x, _y, _physical_width, _physical_height, sh->_x, sh->_y,
                                                     sh->_physical_width, sh->_physical_height)) {
                    holding_shotgun = true;
                    shotgun = sh;
                    break;
                }
            }
        }

    }

    set_pickuped_position(9, -4);

    if (shotgun != nullptr && holding_shotgun) {

        shotgun->_hold_by_anyone = true;

        set_pickuped_position_on_another_moving_obj(5, 3, shotgun);

        if (_x_speed > 0) {
            shotgun->_orientation= Orientation::RIGHT;
        } else if (_x_speed < 0) {
            shotgun->_orientation= Orientation::LEFT;
        }

        if (!global::main_dude->dead && triggered) {
            shotgun->_activated = true;
        }
    }

    if (hold_by_main_dude) {

        if (!killed)
            global::main_dude->carrying_damsel = true;

        if (global::main_dude->killed) {
            hold_by_main_dude = false;
            stunned = true;
            _bouncing_factor_x = ICollidable::default_bouncing_factor_x;
            _bouncing_factor_y = ICollidable::default_bouncing_factor_y;
            stunned_timer = 0;
        } else {
            stunned = false;
            _bouncing_factor_x = 0;
            _bouncing_factor_y = 0;
        }

        sprite_state = global::main_dude->sprite_state;
        sprite_utils::set_priority(OBJPRIORITY_0, mainSpriteInfo, subSpriteInfo);

    } else
        sprite_utils::set_priority(OBJPRIORITY_2, mainSpriteInfo, subSpriteInfo);

    if (!hold_by_main_dude)
        kill_creatures_if_have_speed(1);

    update_sprites_position();

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

    if (_map_collisions_checked) {

        if (!killed && !stunned) {

            if (_bottom_collision && _neighboring_tiles[TileOrientation::RIGHT_MIDDLE] != nullptr &&
                _neighboring_tiles[TileOrientation::RIGHT_MIDDLE]->collidable &&
                _neighboring_tiles[TileOrientation::LEFT_MIDDLE] != nullptr &&
                _neighboring_tiles[TileOrientation::LEFT_MIDDLE]->collidable) {
                //high jump if shopkeeper's surrounded by tiles
                _y_speed = -5 - ((rand() % 10) / 5);
                landlocked = true;
            } else
                landlocked = false;

            if (!_bottom_collision) {

                if ((_neighboring_tiles[TileOrientation::RIGHT_MIDDLE] == nullptr ||
                     !_neighboring_tiles[TileOrientation::RIGHT_MIDDLE]->collidable) &&
                    (_neighboring_tiles[TileOrientation::RIGHT_UP] != nullptr &&
                     _neighboring_tiles[TileOrientation::RIGHT_DOWN] != nullptr)) {
                    //if there's no collidable tile on right-mid, but there are on right-up and right-down,
                    //add extra x-pos to ease going through a hole
                    if (_x_speed > 0)
                        _x += 2;
                }

                if ((_neighboring_tiles[TileOrientation::LEFT_MIDDLE] == nullptr ||
                     !_neighboring_tiles[TileOrientation::LEFT_MIDDLE]->collidable) &&
                    (_neighboring_tiles[TileOrientation::LEFT_UP] != nullptr &&
                     _neighboring_tiles[TileOrientation::LEFT_DOWN] != nullptr)) {
                    //same but for left side
                    if (_x_speed < 0)
                        _x -= 2;
                }

            }

        }

        _map_collisions_checked = false;
    }
}


void Shopkeeper::randomizeMovement() {

    int r = rand() % 2;

    if (r == 0) {
        sprite_state = Orientation::LEFT;
        if (shotgun != nullptr)
            shotgun->_orientation= Orientation::LEFT;
    } else if (r == 1) {

        sprite_state = Orientation::RIGHT;

        if (shotgun != nullptr)
            shotgun->_orientation= Orientation::RIGHT;
    }

    if (standby) {
        go_timer = (rand() % 500) + 100;
        waitTimer = (rand() % 1500) + 2000;
    } else if (triggered) {
        go_timer = (rand() % (1 * 2000)) + 1000;
        waitTimer = rand() % 1500;
    }

}

void Shopkeeper::apply_dmg(int dmg_to_apply) {

    if (dmg_to_apply == 0) {
        return;
    }

    hitpoints -= dmg_to_apply;
    if (blood_spawn_timer > 1000) {
        blood_spawn_timer = 0;
        _y_speed = -2.5; //jump a little bit on receiving dmg, even if dead
        if (!killed)
            spawn_blood();
    }

    if (!killed && hitpoints <= 0) {
        global::hud->disable_all_prompts();
        de_shopify_all_items();
        global::game_state->robbed_killed_shopkeeper = true;
        global::killed_npcs.push_back(_creature_type);
        killed = true;
        _bouncing_factor_x = ICollidable::default_bouncing_factor_x;
        _bouncing_factor_y = ICollidable::default_bouncing_factor_y;
        stunned = false;
    }

}

void Shopkeeper::init_sprites() {

    delete_sprites();

    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_shopkeeperPal, gfx_shopkeeperPalLen,
                                                        nullptr, _sprite_size, ObjSize::OBJSIZE_16, SHOPKEEPER,
                                                        true, false, LAYER_LEVEL::MIDDLE_BOT);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_shopkeeperPal, gfx_shopkeeperPalLen,
                                                          nullptr, _sprite_size, ObjSize::OBJSIZE_16, SHOPKEEPER,
                                                          true, false, LAYER_LEVEL::MIDDLE_BOT);

    update_sprites_position();
    match_animation();
    sprite_utils::set_horizontal_flip(sprite_state == Orientation::RIGHT, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_visibility(true, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
}

void Shopkeeper::update_sprites_position() {
    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);

    if (killed && _y_speed == 0 && _x_speed == 0) {
        main_y += 4;
        sub_y += 4;
    }

    sprite_utils::set_entry_xy(mainSpriteInfo, main_x, main_y);
    sprite_utils::set_entry_xy(subSpriteInfo, sub_x, sub_y);

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
            if (sprite_state == Orientation::RIGHT)
                _x_speed = SHOPKEEPER_TRIGGERED_SPEED;
            else
                _x_speed = -SHOPKEEPER_TRIGGERED_SPEED;
        } else if (standby) {
            if (sprite_state == Orientation::RIGHT)
                _x_speed = 1;
            else
                _x_speed = -1;
        }

        if (go_timer <= 0 && (!triggered && standby)) {
            randomizeMovement();
            _x_speed = 0;
        }

        if (invert_speed_timer < SHOPKEEPER_INVERT_SPEED_DELTA)
            return;

        invert_speed_timer = 0;

        if ((standingOnLeftEdge || standingOnRightEdge) && !triggered) {
            if (rand() % 2 == 0) {
                go_timer = 0;
            } else
                _x_speed *= -1;

        }

        if ((_left_collision || _right_collision) && !landlocked) {

            if (sprite_state == Orientation::LEFT)
                sprite_state = Orientation::RIGHT;
            else
                sprite_state = Orientation::LEFT;

            _x_speed *= -1;
            _right_collision = false;
            _left_collision = false;
        }

    }

}


//!> after calling this function, call sprite_utils::update_frame to update OAM with current frameGfx
void Shopkeeper::apply_stunned_carried_sprites() {
    anim_frame = 0;
    frameGfx = sprite_utils::get_frame((u8 *) gfx_shopkeeperTiles, _sprite_size,
                                       SHOPKEEPER_SPRITESHEET_OFFSET + 4);
}

//!> after calling this function, call sprite_utils::update_frame to update OAM with current frameGfx
void Shopkeeper::apply_dead_carried_sprites() {
    anim_frame = 0;
    frameGfx = sprite_utils::get_frame((u8 *) gfx_shopkeeperTiles, _sprite_size,
                                       SHOPKEEPER_SPRITESHEET_OFFSET);
}

//!> after calling this function, call sprite_utils::update_frame to update OAM with current frameGfx
void Shopkeeper::apply_dead_sprites() {
    anim_frame = 0;

    if (_y_speed == 0)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_shopkeeperTiles, _sprite_size,
                                           SHOPKEEPER_SPRITESHEET_OFFSET);
    else
        frameGfx = sprite_utils::get_frame((u8 *) gfx_shopkeeperTiles, _sprite_size,
                                           SHOPKEEPER_SPRITESHEET_OFFSET + 3);
}


//!> after calling this function, call sprite_utils::update_frame to update OAM with current frameGfx
void Shopkeeper::apply_walking_sprites() {

    if (anim_frame >= 6)
        anim_frame = 0;

    if (_x_speed == 0 && _y_speed == 0)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_shopkeeperTiles, _sprite_size,
                                           SHOPKEEPER_SPRITESHEET_OFFSET + 24);
    else
        frameGfx = sprite_utils::get_frame((u8 *) gfx_shopkeeperTiles, _sprite_size,
                                           SHOPKEEPER_SPRITESHEET_OFFSET + 12 + anim_frame);

}

//!> after calling this function, call sprite_utils::update_frame to update OAM with current frameGfx
void Shopkeeper::apply_stunned_sprites() {

    if (anim_frame >= 6)
        anim_frame = 0;

    if (_y_speed == 0)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_shopkeeperTiles, _sprite_size,
                                           SHOPKEEPER_SPRITESHEET_OFFSET + 18 + anim_frame);
    else if (_y_speed > 0)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_shopkeeperTiles, _sprite_size,
                                           SHOPKEEPER_SPRITESHEET_OFFSET + 4);
    else
        frameGfx = sprite_utils::get_frame((u8 *) gfx_shopkeeperTiles, _sprite_size,
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

    sprite_utils::update_frame(frameGfx, _sprite_size, mainSpriteInfo, subSpriteInfo);
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

    shop_bounds_up_y_px = _y - (8 * TILE_H);
    shop_bounds_down_y_px = _y + (2 * TILE_H);

    if (_x - tile_x > 0) {
        //left oriented shop (exit/entrance is on the left side)
        sprite_state = Orientation::LEFT;
        shop_bounds_right_x_px = _x + (3 * TILE_W);
        if (standby)
            shop_bounds_left_x_px = _x - (9 * TILE_W);
        else
            shop_bounds_left_x_px = _x - (6 * TILE_W);

    } else {
        sprite_state = Orientation::RIGHT;
        shop_bounds_left_x_px = _x - (3 * TILE_W);
        if (standby)
            shop_bounds_right_x_px = _x + (9 * TILE_W);
        else
            shop_bounds_right_x_px = _x + (6 * TILE_W);
    }

}


//done
void Shopkeeper::check_if_dude_in_shop_bounds() {

    if (killed)
        return;

    if (global::main_dude->_x < shop_bounds_right_x_px && global::main_dude->_x > shop_bounds_left_x_px &&
        global::main_dude->_y > shop_bounds_up_y_px && global::main_dude->_y < shop_bounds_down_y_px) {

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

        int diff = _x - global::main_dude->_x;

        if (!triggered) {

            if (diff > 0)
                sprite_state = Orientation::LEFT;
            else
                sprite_state = Orientation::RIGHT;

            if (global::hud->holding_item_shopping) {

                int abs_diff = abs(diff);

                if (abs_diff > 1.5 * TILE_W) {

                    if (diff > 0)
                        _x_speed = -2.5f;
                    else
                        _x_speed = 2.5f;

                }

            }

        }


    } else if (global::hud->holding_item_shopping && !triggered) {
        trigger();
    } else {

        bool shopkeeper_in_shop = _x < shop_bounds_right_x_px && _x > shop_bounds_left_x_px &&
                                  _y > shop_bounds_up_y_px && _y < shop_bounds_down_y_px;

        if ((_bottom_collision && (_left_collision || _right_collision)) ||
            (!shopkeeper_in_shop && jumping_timer > staying_on_ground && _bottom_collision && !(killed || stunned))) {
            _y_speed = -5 - ((rand() % 10) / 5);
            jumping_timer = 0;
            staying_on_ground = 250 + (rand() % 1250);
        }
    }
}

void Shopkeeper::de_shopify_all_items() {
    for (auto &shop_item : shop_items) {

        if (shop_item == nullptr)
            continue;

        shop_item->_bought = true;
        if(shop_item->_shopping_icon)
            shop_item->_shopping_icon->set_ready_to_dispose();
    }
    global::game_state->robbed_killed_shopkeeper = true;
}

void Shopkeeper::spawn_shotgun() {
    shotgun = new Shotgun(0, 0);
    shotgun->_hold_by_main_dude = false;
    shotgun->_bought = true;
    shotgun->cooldown = 250;
    global::items.push_back(shotgun);
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

void Shopkeeper::delete_sprites() {
    delete mainSpriteInfo;
    delete subSpriteInfo;
    mainSpriteInfo = nullptr;
    subSpriteInfo = nullptr;
}

Shopkeeper::Shopkeeper(int x, int y) : BaseCreature(
        x,
        y,
        shopkeeper_sprite_width,
        shopkeeper_sprite_height,
        shopkeeper_spritesheet_type,
        shopkeeper_physical_width,
        shopkeeper_physical_height,
        CreatureType::SHOPKEEPER
    ) {
    hitpoints = 3;

    if (global::game_state->robbed_killed_shopkeeper && !no_shotgun) {
        standby = true;
        spawn_shotgun();
    }
    init_sprites();
    activated = true;
    randomizeMovement();
    _pos_update_delta = 18;
    _friction = 0.5f;
}

