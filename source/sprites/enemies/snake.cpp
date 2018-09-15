//
// Created by xdbeef on 23.04.18.
//

#include <maxmod9.h>
#include <cstdlib>
#include "snake.h"
#include "../../globals_declarations.h"
#include "../items/rock.h"
#include "../../../build/gfx_bat_snake_jetpack.h"
#include "../animations/blood.h"
#include "../../collisions/collisions.h"
#include "../../tiles/level_rendering_utils.h"
#include "../../../build/soundbank.h"
#include "../sprite_utils.h"

#define SNAKE_POS_INC_DELTA 35
#define SNAKE_ANIM_FRAME_DELTA 125
#define SNAKE_HITPOINTS 1

void Snake::draw() {

    if (ready_to_dispose)
        return;

    set_position();
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_visibility(true, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(true, mainSpriteInfo, subSpriteInfo);

    animFrameTimer += *global::timer;

    if (animFrameTimer > SNAKE_ANIM_FRAME_DELTA) {
        //update animation

        animFrameTimer = 0;
        animFrame++;
        if (animFrame >= 4)
            animFrame = 0;

        match_animation();
    }

    if (bottomCollision) {
        //update movement

        if (goTimer > 0) {
            goTimer -= *global::timer;

            if ((standingOnLeftEdge && spriteState == SpriteState::W_LEFT) ||
                (standingOnRightEdge && spriteState == SpriteState::W_RIGHT))
                xSpeed = 0; //stop if standing on an edge and facing towards it
            else if (spriteState == SpriteState::W_RIGHT)
                xSpeed = 0.5; //if not standing on an edge and facing towards it, go where you're facing
            else
                xSpeed = -0.5;

        } else {

            xSpeed = 0;

            if (waitTimer > 0)
                waitTimer -= *global::timer;
            else if (waitTimer <= 0)
                randomizeMovement();
        }
    }


    kill_if_whip(1);
    kill_if_main_dude_jumped_on_you(1);
    deal_damage_main_dude_on_collision(1);
}


void Snake::init() {
    spritesheet_type = SpritesheetType::SNAKE;
    initSprite();
    randomizeMovement();
}

//!> sets a random direction and time of travelling based on previously used directions
void Snake::randomizeMovement() {

    goTimer = (rand() % 1000) + 1000;
    waitTimer = (rand() % 500) + 500;
    int r = rand() % 2;

    if (r == SpriteState::W_LEFT) {

        if (spriteState == SpriteState::W_LEFT)
            sameDirectionInRow++;
        else
            sameDirectionInRow = 0;

        if (sameDirectionInRow == 2)
            spriteState = SpriteState::W_RIGHT;
        else
            spriteState = SpriteState::W_LEFT;

    } else if (r == SpriteState::W_RIGHT) {

        if (spriteState == SpriteState::W_RIGHT)
            sameDirectionInRow++;
        else
            sameDirectionInRow = 0;

        if (sameDirectionInRow == 2)
            spriteState = SpriteState::W_LEFT;
        else
            spriteState = SpriteState::W_RIGHT;
    }
}

void Snake::updateSpeed() {

    limit_speed(MAX_X_SPEED_SNAKE, MAX_Y_SPEED_SNAKE);
    pos_inc_timer += *global::timer;

    if (pos_inc_timer > SNAKE_POS_INC_DELTA) {
        update_position();
        apply_gravity(GRAVITY_DELTA_SPEED);
        pos_inc_timer = 0;
    }

}

void Snake::updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) {

    MapTile *t[9] = {};
    Collisions::getNeighboringTiles(global::current_level->map_tiles,
                                    x_current_pos_in_tiles,
                                    y_current_pos_in_tiles, t);

    standingOnLeftEdge = Collisions::isStandingOnLeftEdge(t, x, physical_width, x_current_pos_in_tiles);
    standingOnRightEdge = Collisions::isStandingOnRightEdge(t, x, physical_width, x_current_pos_in_tiles);
    bottomCollision = Collisions::checkBottomCollision(t, &x, &y, &ySpeed, physical_width, physical_height, false, 0);
    leftCollision = Collisions::checkLeftCollision(t, &x, &y, &xSpeed, physical_width, physical_height, false, 0);
    rightCollision = Collisions::checkRightCollision(t, &x, &y, &xSpeed, physical_width, physical_height, false, 0);
    upperCollision = Collisions::checkUpperCollision(t, &x, &y, &ySpeed, physical_width, false, 0);

}

//!> snake has only 1 dmg point, always kill if any dmg_apply
void Snake::apply_dmg(int dmg_to_apply) {

    //FIXME Leaving pointers like that is not a good idea! Find other classes that do that.
    //    subSpriteInfo = nullptr;
    //    mainSpriteInfo = nullptr;

    spawn_blood();
    global::killed_npcs.push_back(SpriteType::S_SNAKE);
    sprite_utils::set_visibility(false, mainSpriteInfo, subSpriteInfo);
    killed = true;
    ready_to_dispose = true;
}

void Snake::initSprite() {

    delete subSpriteInfo;
    delete mainSpriteInfo;

    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_bat_snake_jetpackPal, gfx_bat_snake_jetpackPalLen,
                                                        nullptr, SNAKE_SPRITE_SIZE, 16, SNAKE, true, false,
                                                        LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_bat_snake_jetpackPal, gfx_bat_snake_jetpackPalLen,
                                                          nullptr, SNAKE_SPRITE_SIZE, 16, SNAKE, true, false,
                                                          LAYER_LEVEL::MIDDLE_TOP);
    
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_visibility(true, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(true, mainSpriteInfo, subSpriteInfo);
    set_position();
    match_animation();

}

void Snake::set_position() {
    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);
    sprite_utils::set_entry_xy(mainSpriteInfo, main_x, main_y);
    sprite_utils::set_entry_xy(subSpriteInfo, sub_x, sub_y);
}

//!> after calling this function, call sprite_utils::update_frame to update OAM with current frameGfx
void Snake::set_sprite_left() {
    frameGfx = sprite_utils::get_frame((u8 *) gfx_bat_snake_jetpackTiles, SNAKE_SPRITE_SIZE, animFrame + 13);
}

//!> after calling this function, call sprite_utils::update_frame to update OAM with current frameGfx
void Snake::set_sprite_right() {
    frameGfx = sprite_utils::get_frame((u8 *) gfx_bat_snake_jetpackTiles, SNAKE_SPRITE_SIZE, animFrame + 9);
}

Snake::Snake() {
    physical_height = SNAKE_PHYSICAL_HEIGHT;
    physical_width = SNAKE_PHYSICAL_WIDTH;
    sprite_height = SNAKE_SPRITE_HEIGHT;
    sprite_width = SNAKE_SPRITE_WIDTH;
    hitpoints = SNAKE_HITPOINTS;
}

Snake::Snake(int x, int y) : Snake() {
    this->x = x;
    this->y = y;
}

void Snake::match_animation() {
    if (spriteState == SpriteState::W_LEFT)
        set_sprite_left();
    else if (spriteState == SpriteState::W_RIGHT)
        set_sprite_right();

    sprite_utils::update_frame(frameGfx, SNAKE_SPRITE_SIZE, mainSpriteInfo, subSpriteInfo);
}

