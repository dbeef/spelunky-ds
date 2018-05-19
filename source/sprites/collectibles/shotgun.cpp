//
// Created by xdbeef on 17.05.18.
//

#include <maxmod9.h>
#include "../../../build/gfx_spike_collectibles.h"
#include "chest.h"
#include "../../collisions/collisions.h"
#include "../../globals_declarations.h"
#include "../../../build/soundbank.h"
#include "shotgun.h"
#include "bullet.h"


void Shotgun::draw() {

    if (ready_to_dispose)
        return;

    //todo fixme picking up other items, like here
    if (hold_by_main_dude && global::input_handler->y_key_down && global::input_handler->down_key_held) {
        hold_by_main_dude = false;
        global::main_dude->holding_item = false;
        global::input_handler->y_key_down = false;
    } else if (global::input_handler->y_key_down && global::input_handler->down_key_held &&
               !global::main_dude->holding_item) {
        if (Collisions::checkCollisionWithMainDude(x, y, 16, 8)) {
            hold_by_main_dude = true;
            global::main_dude->holding_item = true;
            global::main_dude->carrying_shotgun = true;

//            std::cout << "TOOK ITEM";
            global::input_handler->y_key_down = false;
            global::input_handler->y_key_held = false;
        }
    }

    if (hold_by_main_dude) {
        y = global::main_dude->y + 7;

        if (global::main_dude->state == 1) {
            frameGfx = (u8 *) gfx_spike_collectiblesTiles + (sprite_width * sprite_height * (12) / 2);
            x = global::main_dude->x - 6;
        } else {
            frameGfx = (u8 *) gfx_spike_collectiblesTiles + (sprite_width * sprite_height * (11) / 2);
            x = global::main_dude->x + 9;
        }


        subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
        mainSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
    } else
        global::main_dude->carrying_shotgun = false;


    if (activated_by_main_dude && !firing && cooldown > 750) {


        if (!global::main_dude->climbing && !global::main_dude->hanging_on_tile_left &&
            !global::main_dude->hanging_on_tile_right) {
            if (global::main_dude->state == SpriteState::W_LEFT) {
                global::main_dude->xSpeed = MAIN_DUDE_MAX_X_SPEED_RUNNING;
            } else if (global::main_dude->state == SpriteState::W_RIGHT) {
                global::main_dude->xSpeed = -MAIN_DUDE_MAX_X_SPEED_RUNNING;
            }
        }


        mmEffect(SFX_XSHOTGUN);
        firing = true;
        cooldown = 0;
        activated_by_main_dude = false;

        for (int a = 0; a < 4; a++) {
            Bullet *b = new Bullet();
            b->x = x;
            b->y = y;

            if (global::main_dude->state == SpriteState::W_LEFT)
                b->xSpeed = -4.0 - ((rand() % 20) / 10.0);
            else
                b->xSpeed = 4.0 + ((rand() % 20) / 10.0);

            b->init();
            global::sprites.push_back(b);

            if(a == 0)
                b->ySpeed = 1;
            if(a == 1)
                b->ySpeed = -1;
            if(a == 3)
                b->ySpeed = 0;
            if(a == 4)
                b->ySpeed = 2;

        }

    } else {
        activated_by_main_dude = false;
    }

    if (firing) {

        animFrameTimer += *global::timer;

        if (animFrameTimer > 30) {
            animFrameTimer = 0;
            animFrame++;
        }

        frameGfx = (u8 *) gfx_spike_collectiblesTiles + (sprite_width * sprite_height * (13 + animFrame) / 2);
        blast_subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
        blast_mainSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);

        if (global::main_dude->state == SpriteState::W_LEFT) {
            blast_mainSpriteInfo->entry->hFlip = true;
            blast_subSpriteInfo->entry->hFlip = true;
        } else {
            blast_mainSpriteInfo->entry->hFlip = false;
            blast_subSpriteInfo->entry->hFlip = true;
        }

        blast_mainSpriteInfo->entry->vFlip = false;
        blast_subSpriteInfo->entry->vFlip = false;

        if (animFrame >= 9) {
            firing = false;
            animFrame = 0;
        }

        blast_mainSpriteInfo->entry->isHidden = false;
        blast_subSpriteInfo->entry->isHidden = false;
    } else {

        cooldown += *global::timer;

        blast_mainSpriteInfo->entry->isHidden = true;
        blast_subSpriteInfo->entry->isHidden = true;

    }

    if (xSpeed > 0 || ySpeed > 0) {
        for (int a = 0; a < global::sprites.size(); a++) {
            if ((global::sprites.at(a)->spriteType == SpritesheetType::SNAKE ||
                 global::sprites.at(a)->spriteType == SpritesheetType::BAT ||
                 global::sprites.at(a)->spriteType == SpritesheetType::SPIDER)
                && !global::sprites.at(a)->killed) {
                if (Collisions::checkCollisionBodies(x, y, 8, 8, global::sprites.at(a)->x, global::sprites.at(a)->y, 16,
                                                     16)) {
                    global::sprites.at(a)->kill();
                }
            }
        }
    }


    set_position();
}


void Shotgun::init() {
    initSprite();
}

void Shotgun::updateSpeed() {

    if (xSpeed > MAX_X_SPEED_SHOTGUN)
        xSpeed = MAX_X_SPEED_SHOTGUN;
    if (xSpeed < -MAX_X_SPEED_SHOTGUN)
        xSpeed = -MAX_X_SPEED_SHOTGUN;

    if (ySpeed > MAX_Y_SPEED_SHOTGUN)
        ySpeed = MAX_Y_SPEED_SHOTGUN;
    if (ySpeed < -MAX_Y_SPEED_SHOTGUN)
        ySpeed = -MAX_Y_SPEED_SHOTGUN;

    pos_inc_timer += *global::timer;

    bool change_pos = (pos_inc_timer > 15) && !hold_by_main_dude;

    if (change_pos) {
        updatePosition();

        if (bottomCollision && xSpeed > 0) {
            xSpeed -= 0.055;
            if (xSpeed < 0)
                xSpeed = 0;
        }
        if (bottomCollision && xSpeed < 0) {
            xSpeed += 0.055;
            if (xSpeed > 0)
                xSpeed = 0;
        }
        if (!bottomCollision)
            ySpeed += GRAVITY_DELTA_SPEED;

        pos_inc_timer = 0;

    }

}

void Shotgun::updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) {
    MapTile *tiles[9];
    Collisions::getNeighboringTiles(global::level_generator->map_tiles, x_current_pos_in_tiles, y_current_pos_in_tiles,
                                    tiles);

    bottomCollision = Collisions::checkBottomCollision(tiles, &x, &y, &ySpeed, physical_width, physical_height, true);
    leftCollision = Collisions::checkLeftCollision(tiles, &x, &y, &xSpeed, physical_width, physical_height, true);
    rightCollision = Collisions::checkRightCollision(tiles, &x, &y, &xSpeed, physical_width, physical_height, true);
    upperCollision = Collisions::checkUpperCollision(tiles, &x, &y, &ySpeed, physical_width, true);

    if (leftCollision || rightCollision || upperCollision || bottomCollision) {
    }

}

void Shotgun::initSprite() {


    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_spike_collectiblesPal, gfx_spike_collectiblesPalLen,
                                                        nullptr, sprite_width * sprite_height, sprite_width,
                                                        spriteType, true, false);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_spike_collectiblesPal, gfx_spike_collectiblesPalLen,
                                                          nullptr, sprite_width * sprite_height, sprite_width,
                                                          spriteType, true, false);

    blast_subSpriteInfo = global::sub_oam_manager->initSprite(gfx_spike_collectiblesPal, gfx_spike_collectiblesPalLen,
                                                              nullptr, sprite_width * sprite_height, sprite_width,
                                                              spriteType, true, false);
    blast_mainSpriteInfo = global::main_oam_manager->initSprite(gfx_spike_collectiblesPal, gfx_spike_collectiblesPalLen,
                                                                nullptr, sprite_width * sprite_height, sprite_width,
                                                                spriteType, true, false);

    if (spriteState == SpriteState::W_LEFT)
        frameGfx = (u8 *) gfx_spike_collectiblesTiles + (sprite_width * sprite_height * (12) / 2);
    else
        frameGfx = (u8 *) gfx_spike_collectiblesTiles + (sprite_width * sprite_height * (11) / 2);

    subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
    mainSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
}

void Shotgun::set_position() {

    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);

    mainSpriteInfo->entry->x = main_x;
    mainSpriteInfo->entry->y = main_y;

    subSpriteInfo->entry->x = sub_x;
    subSpriteInfo->entry->y = sub_y;

    if (firing) {
        int offset;
        if (global::main_dude->state == SpriteState::W_LEFT)
            offset = -14;
        else
            offset = 14;


        blast_mainSpriteInfo->entry->x = main_x + offset;
        blast_mainSpriteInfo->entry->y = main_y - 6;

        blast_subSpriteInfo->entry->x = sub_x + offset;
        blast_subSpriteInfo->entry->y = sub_y - 6;
    }

    mainSpriteInfo->entry->vFlip = false;
    mainSpriteInfo->entry->hFlip = false;

    subSpriteInfo->entry->vFlip = false;
    subSpriteInfo->entry->hFlip = false;

}

Shotgun::Shotgun() {
    physical_height = SHOTGUN_PHYSICAL_HEIGHT;
    physical_width = SHOTGUN_PHYSICAL_WIDTH;
    sprite_height = SHOTGUN_SPRITE_HEIGHT;
    sprite_width = SHOTGUN_SPRITE_WIDTH;
    spriteType = SpritesheetType::SPIKES_COLLECTIBLES;
}

