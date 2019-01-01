//
// Created by xdbeef on 31.05.18.
//

#include <maxmod9.h>
#include <cstdlib>
#include "Caveman.hpp"
#include "../../GlobalsDeclarations.hpp"
#include "../../collisions/Collisions.hpp"
#include "../../../build/gfx_caveman_damsel.h"
#include "../../tiles/TileOrientation.hpp"
#include "Damsel.hpp"
#include "../../../build/soundbank.h"
#include "../SpriteUtils.hpp"

#define DAMSEL_SPRITESHEET_OFFSET 25
#define DAMSEL_POS_INC_DELTA 18
#define DAMSEL_TRIGGERED_SPEED 1.2
#define DAMSEL_ANIM_FRAME_DELTA 65
#define DAMSEL_INVERT_SPEED_DELTA 400

void Damsel::update_creature_specific() {

    if (_ready_to_dispose)
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

    sprite_utils::set_horizontal_flip(sprite_state == Orientation::RIGHT, mainSpriteInfo, subSpriteInfo);
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
            stunned = true;
            _bouncing_factor_x = ICollidable::default_bouncing_factor_x;
            _bouncing_factor_y = ICollidable::default_bouncing_factor_y;
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
        kill_mobs_if_thrown(1);

    update_sprites_position();

    if (!hold_by_main_dude && global::main_dude->carrying_damsel) {
        global::main_dude->carrying_damsel = false;
        stunned = true;
        stunned_timer = 0;
        triggered = true;
        stunned = true;
        _bouncing_factor_x = ICollidable::default_bouncing_factor_x;
        _bouncing_factor_y = ICollidable::default_bouncing_factor_y;
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
            _bouncing_factor_x = 0;
            _bouncing_factor_y = 0;
        }
    }

    if (hold_by_main_dude && !killed && !stunned && !exiting_level) {

        //search for an exit
        MapTile *tiles[9] = {};
        Collisions::getNeighboringTiles(global::current_level->map_tiles, global::main_dude->current_x_in_tiles,
                                        global::main_dude->current_y_in_tiles, tiles);

        exiting_level = tiles[CENTER] != nullptr && tiles[CENTER]->mapTileType == MapTileType::EXIT;
        if (exiting_level) {
            _x_speed = 0;
            _y_speed = 0;
            _x = tiles[CENTER]->x * 16;
            _y = tiles[CENTER]->y * 16;
            hold_by_main_dude = false;
            mmEffect(SFX_XSTEPS);
            if (global::main_dude->carrying_damsel) {
                global::main_dude->holding_item = false;
                global::main_dude->carrying_damsel = false;
            }
        }
    }

    if(_map_collisions_checked){

        bool bounce = (stunned || killed);

        standingOnLeftEdge = Collisions::isStandingOnLeftEdge(_neighboring_tiles, _x, _physical_width, _current_x_in_tiles);
        standingOnRightEdge = Collisions::isStandingOnRightEdge(_neighboring_tiles, _x, _physical_width, _current_y_in_tiles);

        if (bounce || exiting_level) return;

        //TODO This piece of code should be shared between shopkeeper/damsel/caveman in some utils file

        if (_bottom_collision && _neighboring_tiles[TileOrientation::RIGHT_MIDDLE] != nullptr &&
            _neighboring_tiles[TileOrientation::RIGHT_MIDDLE]->collidable &&
            _neighboring_tiles[TileOrientation::LEFT_MIDDLE] != nullptr && _neighboring_tiles[TileOrientation::LEFT_MIDDLE]->collidable) {
            //high jump if damsel's surrounded by tiles
            _y_speed = -2.6 - ((rand() % 10) / 5);
            landlocked = true;
        } else
            landlocked = false;

        if (!_bottom_collision) {

            if ((_neighboring_tiles[TileOrientation::RIGHT_MIDDLE] == nullptr || !_neighboring_tiles[TileOrientation::RIGHT_MIDDLE]->collidable) &&
                (_neighboring_tiles[TileOrientation::RIGHT_UP] != nullptr && _neighboring_tiles[TileOrientation::RIGHT_DOWN] != nullptr)) {
                //if there's no collidable tile on right-mid, but there are on right-up and right-down,
                //add extra x-pos to ease going through a hole
                if (_x_speed > 0)
                    _x += 2;
            }

            if ((_neighboring_tiles[TileOrientation::LEFT_MIDDLE] == nullptr || !_neighboring_tiles[TileOrientation::LEFT_MIDDLE]->collidable) &&
                (_neighboring_tiles[TileOrientation::LEFT_UP] != nullptr && _neighboring_tiles[TileOrientation::LEFT_DOWN] != nullptr)) {
                //same but for left side
                if (_x_speed < 0)
                    _x -= 2;
            }

        }


        _map_collisions_checked = false;
    }
    
}

void Damsel::randomizeMovement() {

    int r = rand() % 2;

    if (r == 0) {
        sprite_state = Orientation::LEFT;

    } else if (r == 1) {
        if (sprite_state == Orientation::RIGHT)
            sprite_state = Orientation::RIGHT;
    }

    goTimer = (rand() % (1 * 2000)) + 1000;
    waitTimer = rand() % 1500;
}

void Damsel::apply_dmg(int dmg_to_apply) {

    if (dmg_to_apply == 0 || dmg_to_apply == 1) {
        return;
    }

    //TODO Share this piece of code between damsel/shopkeeper/caveman/main dude
    hitpoints -= dmg_to_apply;
    if (blood_spawn_timer > 1000) {
        blood_spawn_timer = 0;
        _y_speed = -2.5; //jump a little bit on receiving dmg, even if dead
        if (!killed)
            spawn_blood();
    }

    if (killed)
        return;

    if (hitpoints <= 0) {
        killed = true;
        stunned = false;
        global::killed_npcs.push_back(_creature_type);

        _bouncing_factor_x = 0;
        _bouncing_factor_y = 0;
    } else {
        stunned = true;
        _bouncing_factor_x = ICollidable::default_bouncing_factor_x;
        _bouncing_factor_y = ICollidable::default_bouncing_factor_y;
    }

}

void Damsel::init_sprites() {

    delete_sprites();

    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_caveman_damselPal, gfx_caveman_damselPalLen,
                                                        nullptr, _sprite_size, ObjSize::OBJSIZE_16, CAVEMAN_DAMSEL,
                                                        true, false, LAYER_LEVEL::MIDDLE_BOT);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_caveman_damselPal, gfx_caveman_damselPalLen,
                                                          nullptr, _sprite_size, ObjSize::OBJSIZE_16, CAVEMAN_DAMSEL,
                                                          true, false, LAYER_LEVEL::MIDDLE_BOT);
    yell_subSpriteInfo = global::sub_oam_manager->initSprite(gfx_caveman_damselPal, gfx_caveman_damselPalLen,
                                                             nullptr, _sprite_size, ObjSize::OBJSIZE_16, CAVEMAN_DAMSEL,
                                                             true, false, LAYER_LEVEL::MIDDLE_BOT);
    yell_mainSpriteInfo = global::main_oam_manager->initSprite(gfx_caveman_damselPal, gfx_caveman_damselPalLen,
                                                               nullptr, _sprite_size, ObjSize::OBJSIZE_16,
                                                               CAVEMAN_DAMSEL, true, false, LAYER_LEVEL::MIDDLE_BOT);

    match_animation();

    //set yell sprite frame here - it won't change later
    frameGfx = sprite_utils::get_frame((u8 *) gfx_caveman_damselTiles,
                                       _sprite_size, DAMSEL_SPRITESHEET_OFFSET + 31);
    sprite_utils::update_frame(frameGfx, _sprite_size, yell_mainSpriteInfo, yell_subSpriteInfo);

    if (rescued) {
        sprite_utils::set_visibility(false, mainSpriteInfo, subSpriteInfo, yell_mainSpriteInfo, yell_subSpriteInfo);
        return;
    }

    sprite_utils::set_vertical_flip(false, yell_mainSpriteInfo, yell_subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, yell_mainSpriteInfo, yell_subSpriteInfo);
    sprite_utils::set_horizontal_flip(sprite_state == Orientation::RIGHT, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_visibility(true, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);

    update_sprites_position();
}

void Damsel::update_sprites_position() {

    int main_x, main_y, sub_x, sub_y;

    //viewporting damsel
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);

    sprite_utils::set_entry_xy(mainSpriteInfo, main_x, main_y);
    sprite_utils::set_entry_xy(subSpriteInfo, sub_x, sub_y);

    //viewporting 'help' prompt
    int temp_y = _y;
    _y -= 13 + (animFrame * 0.5);

    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);

    sprite_utils::set_entry_xy(yell_mainSpriteInfo, main_x, main_y);
    sprite_utils::set_entry_xy(yell_subSpriteInfo, sub_x, sub_y);

    _y = temp_y;
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
            if (sprite_state == Orientation::RIGHT)
                _x_speed = DAMSEL_TRIGGERED_SPEED;
            else
                _x_speed = -DAMSEL_TRIGGERED_SPEED;
        }

        if (goTimer <= 0 && !triggered) {
            randomizeMovement();
            _x_speed = 0;
        }

        if (invert_speed_timer < DAMSEL_INVERT_SPEED_DELTA)
            return;

        invert_speed_timer = 0;

        if ((standingOnLeftEdge || standingOnRightEdge) && !triggered) {
            if (rand() % 2 == 0) {
                goTimer = 0;
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
void Damsel::apply_stunned_carried_sprites() {
    animFrame = 0;
    frameGfx = sprite_utils::get_frame((u8 *) gfx_caveman_damselTiles, _sprite_size,
                                       DAMSEL_SPRITESHEET_OFFSET + 4);
}

//!> after calling this function, call sprite_utils::update_frame to update OAM with current frameGfx
void Damsel::apply_dead_carried_sprites() {
    animFrame = 0;
    frameGfx = sprite_utils::get_frame((u8 *) gfx_caveman_damselTiles, _sprite_size,
                                       DAMSEL_SPRITESHEET_OFFSET + 4);
}

//!> after calling this function, call sprite_utils::update_frame to update OAM with current frameGfx
void Damsel::apply_dead_sprites() {
    animFrame = 0;
    if (_y_speed == 0)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_caveman_damselTiles, _sprite_size,
                                           DAMSEL_SPRITESHEET_OFFSET);
    else if (_y_speed > 0)
        frameGfx =
                sprite_utils::get_frame((u8 *) gfx_caveman_damselTiles, _sprite_size,
                                        DAMSEL_SPRITESHEET_OFFSET + 3);
    else
        frameGfx = sprite_utils::get_frame((u8 *) gfx_caveman_damselTiles, _sprite_size,
                                           DAMSEL_SPRITESHEET_OFFSET + 1);

}

//!> after calling this function, call sprite_utils::update_frame to update OAM with current frameGfx
void Damsel::apply_walking_sprites() {

    if (animFrame >= 4)
        animFrame = 0;

    if (_x_speed == 0 && _y_speed == 0)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_caveman_damselTiles, _sprite_size,
                                           DAMSEL_SPRITESHEET_OFFSET + 5);
    else if (_x_speed != 0 && _y_speed == 0)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_caveman_damselTiles, _sprite_size,
                                           DAMSEL_SPRITESHEET_OFFSET + 6 + animFrame);
    else
        frameGfx = sprite_utils::get_frame((u8 *) gfx_caveman_damselTiles, _sprite_size,
                                           DAMSEL_SPRITESHEET_OFFSET + 3);
}

//!> after calling this function, call sprite_utils::update_frame to update OAM with current frameGfx
void Damsel::apply_stunned_sprites() {
    if (animFrame >= 5)
        animFrame = 0;

    if (_y_speed == 0)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_caveman_damselTiles, _sprite_size,
                                           DAMSEL_SPRITESHEET_OFFSET + 10 + animFrame);
    else if (_y_speed > 0)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_caveman_damselTiles, _sprite_size,
                                           DAMSEL_SPRITESHEET_OFFSET + 4);
    else
        frameGfx = sprite_utils::get_frame((u8 *) gfx_caveman_damselTiles, _sprite_size,
                                           DAMSEL_SPRITESHEET_OFFSET + 2);
}

//!> after calling this function, call sprite_utils::update_frame to update OAM with current frameGfx
void Damsel::apply_exiting_level_sprites() {
    if (animFrame >= 16) {
        rescued = true;
        global::game_state->damsels_rescued_this_level++;
    } else
        frameGfx = sprite_utils::get_frame((u8 *) gfx_caveman_damselTiles, _sprite_size,
                                           DAMSEL_SPRITESHEET_OFFSET + 14 + animFrame);
}

//!> after calling this function, call sprite_utils::update_frame to update OAM with current frameGfx
void Damsel::apply_yelling_sprites() {

    if (animFrame >= 10) {
        yelling = false;
        yell_mainSpriteInfo->entry->isHidden = true;
        yell_subSpriteInfo->entry->isHidden = true;
    } else
        frameGfx = sprite_utils::get_frame((u8 *) gfx_caveman_damselTiles, _sprite_size,
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

    sprite_utils::update_frame(frameGfx, _sprite_size, mainSpriteInfo, subSpriteInfo);
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
                   ((_sprite_size * (DAMSEL_SPRITESHEET_OFFSET + animFrame + 43)) / 2);
}

void Damsel::delete_sprites() {
    delete mainSpriteInfo;
    delete subSpriteInfo;
    delete yell_mainSpriteInfo;
    delete yell_subSpriteInfo;
    mainSpriteInfo = nullptr;
    subSpriteInfo = nullptr;
    yell_mainSpriteInfo= nullptr;
    yell_subSpriteInfo = nullptr;
}
