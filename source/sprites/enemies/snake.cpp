//
// Created by xdbeef on 23.04.18.
//

#include "snake.h"
#include "../../globals_declarations.h"
#include "../items/rock.h"
#include "../../../build/gfx_snake.h"
#include "../animations/blood.h"
#include "../../collisions/collisions.h"
#include "../../tiles/map_utils.h"


void Snake::draw() {

    if (ready_to_dispose)
        return;

    set_position();


    //idk why do i have to do that, if it is already flipped in image
    mainSpriteInfo->entry->hFlip = true;
    subSpriteInfo->entry->hFlip = true;

    mainSpriteInfo->entry->vFlip = false;
    subSpriteInfo->entry->vFlip = false;

    mainSpriteInfo->entry->isHidden = false;
    subSpriteInfo->entry->isHidden = false;

    animFrameTimer += *global::timer;

    if (animFrameTimer > 125) {
//        std::cout<< animFrame << '\n';
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

//            std::cout << goTimer << '\n';

            if (standingOnLeftEdge && spriteState == 1 || standingOnRightEdge && spriteState == 0) {
                xSpeed = 0;
            } else if (spriteState == 0)
                xSpeed = 0.5;
            else
                xSpeed = -0.5;

            if (goTimer <= 0) {
//                std::cout << "RAND" << '\n';
                randomizeMovement();
                xSpeed = 0;
            }
        }
    }


    if (global::main_dude->using_whip && !killed && global::main_dude->whip->whip_timer > 120) {
        if (Collisions::checkCollisionWithMainDudeWhip(x, y, 16, 16)) {
            kill();
        }
    }


    if (!killed && Collisions::checkCollisionWithMainDude(x, y, 16, 16) && global::main_dude->ySpeed > 0 &&
        global::main_dude->y - 4 < y) {
        kill();
        global::main_dude->ySpeed = -2;
        global::main_dude->jumping_timer = 0;
    }

    if (!killed && !global::main_dude->dead && Collisions::checkCollisionWithMainDude(x, y, 16, 16) &&
        global::main_dude->time_since_last_damage > 1000) {

        global::main_dude->time_since_last_damage = 0;
        global::hud->hearts--;
        global::hud->draw();

        if (global::hud->hearts == 0) {
            global::hud->hide();
            global::main_dude->ySpeed = -MAIN_DUDE_JUMP_SPEED * 0.25;
            global::main_dude->dead = true;
        }

    }


}


void Snake::init() {

    initSprite();

    sameDirectionInRow = 0;
    frameGfx = (u8 *) gfx_snakeTiles;
    subSpriteInfo->updateFrame(frameGfx, 16 * 16);
    mainSpriteInfo->updateFrame(frameGfx, 16 * 16);

    spriteType = SpritesheetType::SNAKE;

    randomizeMovement();
}

void Snake::randomizeMovement() {
//    srand(*global::timer + x + y + global::main_dude->x + +global::main_dude->y);

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

    if (xSpeed > MAX_X_SPEED_ROCK)
        xSpeed = MAX_X_SPEED_ROCK;
    if (xSpeed < -MAX_X_SPEED_ROCK)
        xSpeed = -MAX_X_SPEED_ROCK;

    if (ySpeed > MAX_Y_SPEED_ROCK)
        ySpeed = MAX_Y_SPEED_ROCK;
    if (ySpeed < -MAX_Y_SPEED_ROCK)
        ySpeed = -MAX_Y_SPEED_ROCK;

    pos_inc_timer += *global::timer;

    if (pos_inc_timer > 35) {
        updatePosition();
        if (!bottomCollision)
            ySpeed += GRAVITY_DELTA_SPEED;
        pos_inc_timer = 0;
    }

}

void Snake::updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) {

    MapTile *tiles[9] = {};
    Collisions::getNeighboringTiles(global::level_generator->map_tiles, x_current_pos_in_tiles,
                                    y_current_pos_in_tiles, tiles);

    standingOnLeftEdge = Collisions::isStandingOnLeftEdge(tiles, x, 16, x_current_pos_in_tiles);
    standingOnRightEdge = Collisions::isStandingOnRightEdge(tiles, x, 16, x_current_pos_in_tiles);
    bottomCollision = Collisions::checkBottomCollision(tiles, &x, &y, &ySpeed, 16, 16, false);
    leftCollision = Collisions::checkLeftCollision(tiles, &x, &y, &xSpeed, 16, 16, false);
    rightCollision = Collisions::checkRightCollision(tiles, &x, &y, &xSpeed, 16, 16, false);
    upperCollision = Collisions::checkUpperCollision(tiles, &x, &y, &ySpeed, 16, false);

    if (bottomCollision) {
        //nothing
    }


}

void Snake::kill() {
    subSpriteInfo->entry->isHidden = true;
    mainSpriteInfo->entry->isHidden = true;

    subSpriteInfo = nullptr;
    mainSpriteInfo = nullptr;

    for (int a = 0; a < 4; a++) {
        Blood *blood = new Blood();
        blood->init();
        blood->x = x;
        blood->y = y;

        if (a % 2 == 0)
            blood->xSpeed = (1.3 / a);
        else
            blood->xSpeed = (-1.3 / a);

        blood->ySpeed = -2 / a;
        global::sprites.push_back(blood);
    }
    killed = true;
    ready_to_dispose = true;
    global::hud->draw();
    global::killed_npcs.push_back(spriteType);

}

void Snake::initSprite() {
    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_snakePal, gfx_snakePalLen,
                                                        nullptr, 16 * 16, 16, SNAKE, true, false);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_snakePal, gfx_snakePalLen,
                                                          nullptr, 16 * 16, 16, SNAKE, true, false);
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
    frameGfx = (u8 *) gfx_snakeTiles + ((16 * 16 * (animFrame + 4)) / 2);
    subSpriteInfo->updateFrame(frameGfx, 16 * 16);
    mainSpriteInfo->updateFrame(frameGfx, 16 * 16);
    mainSpriteInfo->entry->hFlip = true;
    subSpriteInfo->entry->hFlip = true;

    mainSpriteInfo->entry->isHidden = false;
    subSpriteInfo->entry->isHidden = false;
}

void Snake::set_sprite_right() {
    frameGfx = (u8 *) gfx_snakeTiles + ((16 * 16 * animFrame) / 2);
    subSpriteInfo->updateFrame(frameGfx, 16 * 16);
    mainSpriteInfo->updateFrame(frameGfx, 16 * 16);
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
}

