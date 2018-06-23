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
#include "../animations/got_collectible.h"
#include "../../tiles/map_utils.h"

#define PISTOL_POS_INC_DELTA 15
#define PISTOL_FIRING_OFFSET_X 14
#define PISTOL_FIRING_OFFSET_Y 6
#define PISTOL_COOLDOWN 750

void Pistol::draw() {


    if (ready_to_dispose) {
        mainSpriteInfo->entry->isHidden = true;
        subSpriteInfo->entry->isHidden = true;
        return;
    } else {
        mainSpriteInfo->entry->isHidden = false;
        subSpriteInfo->entry->isHidden = false;
    }

    check_if_can_be_pickuped();

    if (hold_by_main_dude) {

        if (shopping_transaction(this)) {
            equip();
        }

        global::main_dude->carrying_pistol = true;


        y = global::main_dude->y + 7;

        if (global::main_dude->sprite_state == SpriteState::W_LEFT) {
            x = global::main_dude->x - 4;
        } else
            x = global::main_dude->x + 10;

        sprite_state = global::main_dude->sprite_state;

        if (global::main_dude->sprite_state == SpriteState::W_LEFT) {
            sprite_state = SpriteState::W_LEFT;
            frameGfx = (u8 *) gfx_spike_collectibles_flameTiles + (sprite_width * sprite_height * (32) / 2);
        } else {
            sprite_state = SpriteState::W_RIGHT;
            frameGfx = (u8 *) gfx_spike_collectibles_flameTiles + (sprite_width * sprite_height * (33) / 2);
        }

        subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
        mainSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
    } else
        global::main_dude->carrying_pistol = false;


    if (activated && !firing && cooldown > PISTOL_COOLDOWN) {

        if (!global::main_dude->climbing && !global::main_dude->hanging_on_tile_left &&
            !global::main_dude->hanging_on_tile_right) {
            if (global::main_dude->sprite_state == SpriteState::W_LEFT) {
                global::main_dude->xSpeed = MAIN_DUDE_MAX_X_SPEED_RUNNING;
            } else if (global::main_dude->sprite_state == SpriteState::W_RIGHT) {
                global::main_dude->xSpeed = -MAIN_DUDE_MAX_X_SPEED_RUNNING;
            }
        }

        mmEffect(SFX_XSHOTGUN);
        firing = true;
        cooldown = 0;
        activated = false;

        Bullet *b = new Bullet();
        b->x = x;
        b->y = y;

        if (global::main_dude->sprite_state == SpriteState::W_LEFT) {
            b->xSpeed = -5.0;
            b->x-=2;
        }
        else if (global::main_dude->sprite_state == SpriteState::W_RIGHT) {
            b->xSpeed = 5.0;
            b->x+=2;
        }

        b->init();
        global::sprites.push_back(b);
        b->ySpeed = 0;
    } else {
        activated = false;
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

        if (global::main_dude->sprite_state == SpriteState::W_LEFT) {
            blast_mainSpriteInfo->entry->hFlip = true;
            blast_subSpriteInfo->entry->hFlip = true;
        } else {
            blast_mainSpriteInfo->entry->hFlip = false;
            blast_subSpriteInfo->entry->hFlip = false;
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

    kill_mobs_if_thrown(1);

    set_position();
}


void Pistol::init() {
    initSprite();
    init_anim_icon();
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

    bottomCollision = Collisions::checkBottomCollision(tiles, &x, &y, &ySpeed, physical_width, physical_height, true,
                                                       BOUNCING_FACTOR_Y);
    leftCollision = Collisions::checkLeftCollision(tiles, &x, &y, &xSpeed, physical_width, physical_height, true,
                                                   BOUNCING_FACTOR_X);
    rightCollision = Collisions::checkRightCollision(tiles, &x, &y, &xSpeed, physical_width, physical_height, true,
                                                     BOUNCING_FACTOR_X);
    upperCollision = Collisions::checkUpperCollision(tiles, &x, &y, &ySpeed, physical_width, true, BOUNCING_FACTOR_Y);

}

void Pistol::initSprite() {


    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_spike_collectibles_flamePal,
                                                        gfx_spike_collectibles_flamePalLen,
                                                        nullptr, sprite_width * sprite_height, sprite_width,
                                                        spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_spike_collectibles_flamePal,
                                                          gfx_spike_collectibles_flamePalLen,
                                                          nullptr, sprite_width * sprite_height, sprite_width,
                                                          spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);

    blast_subSpriteInfo = global::sub_oam_manager->initSprite(gfx_spike_collectibles_flamePal,
                                                              gfx_spike_collectibles_flamePalLen,
                                                              nullptr, sprite_width * sprite_height, sprite_width,
                                                              spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
    blast_mainSpriteInfo = global::main_oam_manager->initSprite(gfx_spike_collectibles_flamePal,
                                                                gfx_spike_collectibles_flamePalLen,
                                                                nullptr, sprite_width * sprite_height, sprite_width,
                                                                spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);

    if (sprite_state == SpriteState::W_LEFT)
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

    if (!firing) {

        blast_mainSpriteInfo->entry->y = main_y - PISTOL_FIRING_OFFSET_Y;
        blast_subSpriteInfo->entry->y = sub_y - PISTOL_FIRING_OFFSET_Y;

        if (global::main_dude->sprite_state == SpriteState::W_LEFT) {
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

    update_anim_icon(x, y, physical_width);

}

Pistol::Pistol() {
    cost = 10 * 1000;
    name = "PISTOL";
    physical_height = PISTOL_PHYSICAL_HEIGHT;
    physical_width = PISTOL_PHYSICAL_WIDTH;
    sprite_height = PISTOL_SPRITE_HEIGHT;
    sprite_width = PISTOL_SPRITE_WIDTH;
    spritesheet_type = SpritesheetType::SPIKES_COLLECTIBLES;
}

void Pistol::equip() {
    GotCollectible *g = new GotCollectible();
    g->x = x - 12;
    g->y = y - 20;
    g->collectible_type = 0;
    g->init();
    global::sprites.push_back(g);
}

