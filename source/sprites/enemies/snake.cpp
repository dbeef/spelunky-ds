//
// Created by xdbeef on 23.04.18.
//

#include <maxmod9.h>
#include "snake.h"
#include "../../globals_declarations.h"
#include "../items/rock.h"
#include "../../../build/gfx_snake.h"
#include "../animations/blood.h"
#include "../../collisions/collisions.h"
#include "../../tiles/map_utils.h"
#include "../../../build/soundbank.h"

#define SNAKE_POS_INC_DELTA 35
#define SNAKE_ANIM_FRAME_DELTA 125
#define SNAKE_HITPOINTS 1

void Snake::draw() {

    if (ready_to_dispose)
        return;

    set_position();

    mainSpriteInfo->entry->hFlip = true;
    subSpriteInfo->entry->hFlip = true;

    mainSpriteInfo->entry->vFlip = false;
    subSpriteInfo->entry->vFlip = false;

    mainSpriteInfo->entry->isHidden = false;
    subSpriteInfo->entry->isHidden = false;

    animFrameTimer += *global::timer;

    if (animFrameTimer > SNAKE_ANIM_FRAME_DELTA) {

        animFrame++;
        if (animFrame >= 4)
            animFrame = 0;

        if (spriteState == SpriteState::W_LEFT) {
            set_sprite_left();
        } else if (spriteState == SpriteState::W_RIGHT) {
            set_sprite_right();
        }
        animFrameTimer = 0;

    }


    if (bottomCollision) {
        if (waitTimer > 0) {
            waitTimer -= *global::timer;
        } else {
            if (goTimer > 0)


                goTimer -= *global::timer;

            if (standingOnLeftEdge && spriteState == 1 || standingOnRightEdge && spriteState == 0) {
                xSpeed = 0;
            } else if (spriteState == 0)
                xSpeed = 0.5;
            else
                xSpeed = -0.5;

            if (goTimer <= 0) {
                randomizeMovement();
                xSpeed = 0;
            }
        }
    }


    kill_if_whip(1);
    kill_if_main_dude_jumped_on_you(1);
    deal_damage_main_dude_on_collision(1);
}


void Snake::init() {

    initSprite();

    frameGfx = (u8 *) gfx_snakeTiles;
    subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
    mainSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);

    spriteType = SpritesheetType::SNAKE;

    randomizeMovement();
}

void Snake::randomizeMovement() {

    int r = rand() % 2;

    if (r == 0) {
        if (spriteState == SpriteState::W_LEFT)
            sameDirectionInRow++;
        else
            sameDirectionInRow = 0;

        if (sameDirectionInRow == 2)
            spriteState = SpriteState::W_RIGHT;
        else
            spriteState = SpriteState::W_LEFT;

    } else if (r == 1) {
        if (spriteState == SpriteState::W_RIGHT)
            sameDirectionInRow++;
        else
            sameDirectionInRow = 0;

        if (sameDirectionInRow == 2)
            spriteState = SpriteState::W_LEFT;
        else
            spriteState = SpriteState::W_RIGHT;
    }

    goTimer = (rand() % 2000) + 1000;
    waitTimer = rand() % 500;
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

    MapTile *tiles[9] = {};
    Collisions::getNeighboringTiles(global::level_generator->map_tiles, x_current_pos_in_tiles,
                                    y_current_pos_in_tiles, tiles);

    standingOnLeftEdge = Collisions::isStandingOnLeftEdge(tiles, x, physical_width, x_current_pos_in_tiles);
    standingOnRightEdge = Collisions::isStandingOnRightEdge(tiles, x, physical_width, x_current_pos_in_tiles);
    bottomCollision = Collisions::checkBottomCollision(tiles, &x, &y, &ySpeed, physical_width, physical_height, false, 0);
    leftCollision = Collisions::checkLeftCollision(tiles, &x, &y, &xSpeed, physical_width, physical_height, false, 0);
    rightCollision = Collisions::checkRightCollision(tiles, &x, &y, &xSpeed, physical_width, physical_height, false, 0);
    upperCollision = Collisions::checkUpperCollision(tiles, &x, &y, &ySpeed, physical_width, false, 0);

}

void Snake::apply_dmg(int dmg_to_apply) {

    //snake has only 1 dmg point, always kill if any dmg_apply

    subSpriteInfo->entry->isHidden = true;
    mainSpriteInfo->entry->isHidden = true;

    subSpriteInfo = nullptr;
    mainSpriteInfo = nullptr;

    spawn_blood();
    killed = true;
    ready_to_dispose = true;
    global::hud->draw();
    global::killed_npcs.push_back(spriteType);

}

void Snake::initSprite() {
    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_snakePal, gfx_snakePalLen,
                                                        nullptr, sprite_width * sprite_height, 16, SNAKE, true, false,
                                                        LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_snakePal, gfx_snakePalLen,
                                                          nullptr, sprite_width * sprite_height, 16, SNAKE, true, false,
                                                          LAYER_LEVEL::MIDDLE_TOP);
    subSpriteInfo->entry->isHidden = false;
    mainSpriteInfo->entry->isHidden = false;

    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);

    mainSpriteInfo->entry->x = main_x;
    mainSpriteInfo->entry->y = main_y;

    subSpriteInfo->entry->x = sub_x;
    subSpriteInfo->entry->y = sub_y;


}

void Snake::set_position() {

    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);

    mainSpriteInfo->entry->x = main_x;
    mainSpriteInfo->entry->y = main_y;

    subSpriteInfo->entry->x = sub_x;
    subSpriteInfo->entry->y = sub_y;
}

void Snake::set_sprite_left() {
    frameGfx = (u8 *) gfx_snakeTiles + ((sprite_width * sprite_height * (animFrame + 4)) / 2);
    subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
    mainSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
    mainSpriteInfo->entry->hFlip = true;
    subSpriteInfo->entry->hFlip = true;

    mainSpriteInfo->entry->isHidden = false;
    subSpriteInfo->entry->isHidden = false;
}

void Snake::set_sprite_right() {
    frameGfx = (u8 *) gfx_snakeTiles + ((sprite_width * sprite_height * animFrame) / 2);
    subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
    mainSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
    mainSpriteInfo->entry->hFlip = true;
    subSpriteInfo->entry->hFlip = true;

    mainSpriteInfo->entry->isHidden = false;
    subSpriteInfo->entry->isHidden = false;
}

Snake::Snake() {
    physical_height = SNAKE_PHYSICAL_HEIGHT;
    physical_width = SNAKE_PHYSICAL_WIDTH;
    sprite_height = SNAKE_SPRITE_HEIGHT;
    sprite_width = SNAKE_SPRITE_WIDTH;
    hitpoints = SNAKE_HITPOINTS;
}

