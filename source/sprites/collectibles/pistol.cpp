//
// Created by xdbeef on 18.05.18.
//


#include <maxmod9.h>
#include "pistol.h"
#include "../../globals_declarations.h"
#include "../../collisions/collisions.h"
#include "../../../build/gfx_spike_collectibles_flame.h"
#include "../../../build/soundbank.h"
#include "bullet.h"

#define PISTOL_POS_INC_DELTA 15
#define PISTOL_FIRING_OFFSET_X 14
#define PISTOL_FIRING_OFFSET_Y 6
#define PISTOL_COOLDOWN 750

void Pistol::draw() {

    if (ready_to_dispose)
        return;

    check_if_can_be_pickuped();

    if (hold_by_main_dude) {

        global::main_dude->carrying_pistol = true;

        y = global::main_dude->y + 7;

        if (global::main_dude->state == 1) {
            frameGfx = (u8 *) gfx_spike_collectibles_flameTiles + (sprite_width * sprite_height * (32) / 2);
            x = global::main_dude->x - 4;
        } else {
            frameGfx = (u8 *) gfx_spike_collectibles_flameTiles + (sprite_width * sprite_height * (33) / 2);
            x = global::main_dude->x + 5;
        }

        subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
        mainSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
    } else
        global::main_dude->carrying_pistol = false;


    if (activated_by_main_dude && !firing && cooldown > PISTOL_COOLDOWN) {


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

        Bullet *b = new Bullet();
        b->x = x;
        b->y = y;

        if (global::main_dude->state == SpriteState::W_LEFT)
            b->xSpeed = -5.0;
        else if (global::main_dude->state == SpriteState::W_RIGHT)
            b->xSpeed = 5.0;

        b->init();
        global::sprites.push_back(b);
        b->ySpeed = 0;
    } else {
        activated_by_main_dude = false;
    }

    if (firing) {

        animFrameTimer += *global::timer;

        if (animFrameTimer > 30) {
            animFrameTimer = 0;
            animFrame++;
        }

        frameGfx = (u8 *) gfx_spike_collectibles_flameTiles + (sprite_width * sprite_height * (13 + animFrame) / 2);
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
                    global::sprites.at(a)->apply_dmg(1);
                }
            }
        }
    }

    set_position();
}


void Pistol::init() {
    initSprite();
}

void Pistol::updateSpeed() {

    limit_speed(MAX_X_SPEED_PISTOL, MAX_Y_SPEED_PISTOL);

    pos_inc_timer += *global::timer;

    bool change_pos = (pos_inc_timer > PISTOL_POS_INC_DELTA) && !hold_by_main_dude;

    if (change_pos) {
        update_position();
        apply_friction(0.055);
        apply_gravity(GRAVITY_DELTA_SPEED);
        pos_inc_timer = 0;
    }

}

void Pistol::updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) {
    MapTile *tiles[9];
    Collisions::getNeighboringTiles(global::level_generator->map_tiles, x_current_pos_in_tiles, y_current_pos_in_tiles,
                                    tiles);

    bottomCollision = Collisions::checkBottomCollision(tiles, &x, &y, &ySpeed, physical_width, physical_height, true, BOUNCING_FACTOR_Y);
    leftCollision = Collisions::checkLeftCollision(tiles, &x, &y, &xSpeed, physical_width, physical_height, true, BOUNCING_FACTOR_X);
    rightCollision = Collisions::checkRightCollision(tiles, &x, &y, &xSpeed, physical_width, physical_height, true, BOUNCING_FACTOR_X);
    upperCollision = Collisions::checkUpperCollision(tiles, &x, &y, &ySpeed, physical_width, true, BOUNCING_FACTOR_Y);

}

void Pistol::initSprite() {


    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_spike_collectibles_flamePal, gfx_spike_collectibles_flamePalLen,
                                                        nullptr, sprite_width * sprite_height, sprite_width,
                                                        spriteType, true, false, LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_spike_collectibles_flamePal, gfx_spike_collectibles_flamePalLen,
                                                          nullptr, sprite_width * sprite_height, sprite_width,
                                                          spriteType, true, false, LAYER_LEVEL::MIDDLE_TOP);

    blast_subSpriteInfo = global::sub_oam_manager->initSprite(gfx_spike_collectibles_flamePal, gfx_spike_collectibles_flamePalLen,
                                                              nullptr, sprite_width * sprite_height, sprite_width,
                                                              spriteType, true, false, LAYER_LEVEL::MIDDLE_TOP);
    blast_mainSpriteInfo = global::main_oam_manager->initSprite(gfx_spike_collectibles_flamePal, gfx_spike_collectibles_flamePalLen,
                                                                nullptr, sprite_width * sprite_height, sprite_width,
                                                                spriteType, true, false, LAYER_LEVEL::MIDDLE_TOP);

    if (spriteState == SpriteState::W_LEFT)
        frameGfx = (u8 *) gfx_spike_collectibles_flameTiles + (sprite_width * sprite_height * (32) / 2);
    else
        frameGfx = (u8 *) gfx_spike_collectibles_flameTiles + (sprite_width * sprite_height * (33) / 2);

    subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
    mainSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
}

void Pistol::set_position() {

    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);


    mainSpriteInfo->entry->x = main_x;
    mainSpriteInfo->entry->y = main_y;
    subSpriteInfo->entry->x = sub_x;
    subSpriteInfo->entry->y = sub_y;

    if (firing) {

        blast_mainSpriteInfo->entry->y = main_y - PISTOL_FIRING_OFFSET_Y;
        blast_subSpriteInfo->entry->y = sub_y - PISTOL_FIRING_OFFSET_Y;

        if (global::main_dude->state == SpriteState::W_LEFT) {
            blast_mainSpriteInfo->entry->x = main_x - PISTOL_FIRING_OFFSET_X;
            blast_subSpriteInfo->entry->x = sub_x - PISTOL_FIRING_OFFSET_X;
        } else {
            blast_mainSpriteInfo->entry->x = main_x + PISTOL_FIRING_OFFSET_X;
            blast_subSpriteInfo->entry->x = sub_x + PISTOL_FIRING_OFFSET_X;
        }

    }

    mainSpriteInfo->entry->vFlip = false;
    mainSpriteInfo->entry->hFlip = false;

    subSpriteInfo->entry->vFlip = false;
    subSpriteInfo->entry->hFlip = false;

}

Pistol::Pistol() {
    physical_height = PISTOL_PHYSICAL_HEIGHT;
    physical_width = PISTOL_PHYSICAL_WIDTH;
    sprite_height = PISTOL_SPRITE_HEIGHT;
    sprite_width = PISTOL_SPRITE_WIDTH;
    spriteType = SpritesheetType::SPIKES_COLLECTIBLES;
}

