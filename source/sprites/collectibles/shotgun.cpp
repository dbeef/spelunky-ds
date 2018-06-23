//
// Created by xdbeef on 17.05.18.
//

#include <maxmod9.h>
#include <cstdlib>
#include "../../../build/gfx_spike_collectibles_flame.h"
#include "chest.h"
#include "../../collisions/collisions.h"
#include "../../globals_declarations.h"
#include "../../../build/soundbank.h"
#include "shotgun.h"
#include "bullet.h"
#include "../animations/got_collectible.h"
#include "../../tiles/map_utils.h"

#define SHOTGUN_POS_INC_DELTA 15
#define SHOTGUN_COOLDOWN 750

void Shotgun::draw() {

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

        if (global::main_dude->xSpeed > 0)
            sprite_state = SpriteState::W_RIGHT;
        else if (global::main_dude->xSpeed < 0)
            sprite_state = SpriteState::W_LEFT;
        else if (global::main_dude->hanging_on_tile_left)
            sprite_state = SpriteState::W_RIGHT;
        else if (global::main_dude->hanging_on_tile_right)
            sprite_state = SpriteState::W_LEFT;

        if (shopping_transaction(this)) {
            equip();
        }

        mainSpriteInfo->entry->priority = OBJPRIORITY_0;

        global::main_dude->carrying_shotgun = true;

        y = global::main_dude->y + 7;

        if (sprite_state == SpriteState::W_LEFT) {
            x = global::main_dude->x - 4;
        } else {
            x = global::main_dude->x + 7;
        }


    } else {
        mainSpriteInfo->entry->priority = OBJPRIORITY_1;
        global::main_dude->carrying_shotgun = false;
    }


    if (sprite_state == SpriteState::W_LEFT) {
        frameGfx = (u8 *) gfx_spike_collectibles_flameTiles + (sprite_width * sprite_height * (12) / 2);
    } else {
        frameGfx = (u8 *) gfx_spike_collectibles_flameTiles + (sprite_width * sprite_height * (11) / 2);
    }

    subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
    mainSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);


    if (activated && !firing && cooldown > SHOTGUN_COOLDOWN) {

        if (!global::main_dude->climbing && !global::main_dude->hanging_on_tile_left &&
            !global::main_dude->hanging_on_tile_right) {

            //FIXME Wkleić wszędzie gdzie activated = true
//            if (global::main_dude->sprite_state == SpriteState::W_LEFT) {
//                global::main_dude->xSpeed = MAIN_DUDE_MAX_X_SPEED_RUNNING;
//            } else if (global::main_dude->sprite_state == SpriteState::W_RIGHT) {
//                global::main_dude->xSpeed = -MAIN_DUDE_MAX_X_SPEED_RUNNING;
//            }

        }


        mmEffect(SFX_XSHOTGUN);
        firing = true;
        cooldown = 0;
        activated = false;

        int main_x, main_y, sub_x, sub_y;
        get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);

        spawn_bullets();

    } else {
        activated = false;
    }


    if (blast->animFrame >= 9) {
        firing = false;
        blast->animFrame = 0;
    }

    blast->firing = firing;

    if (!firing)
        cooldown += *global::timer;

    if (!firing) {
        blast->x = x;
        blast->y = y;
        blast->sprite_state = sprite_state;
    }

    kill_mobs_if_thrown(1);
    set_position();
}


void Shotgun::init() {
    initSprite();
    init_anim_icon();
    blast = new Blast();
    blast->init();
    global::sprites.push_back(blast);
}

void Shotgun::updateSpeed() {

    limit_speed(MAX_X_SPEED_SHOTGUN, MAX_Y_SPEED_SHOTGUN);

    pos_inc_timer += *global::timer;

    bool change_pos = (pos_inc_timer > SHOTGUN_POS_INC_DELTA) && !hold_by_main_dude;

    if (change_pos) {
        update_position();
        apply_friction(0.055);
        apply_gravity(GRAVITY_DELTA_SPEED);
        pos_inc_timer = 0;
    }

}

void Shotgun::updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) {

    if (hold_by_anyone)
        return;

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

void Shotgun::initSprite() {


    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_spike_collectibles_flamePal,
                                                        gfx_spike_collectibles_flamePalLen,
                                                        nullptr, sprite_width * sprite_height, sprite_width,
                                                        spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_spike_collectibles_flamePal,
                                                          gfx_spike_collectibles_flamePalLen,
                                                          nullptr, sprite_width * sprite_height, sprite_width,
                                                          spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);


    if (sprite_state == SpriteState::W_LEFT)
        frameGfx = (u8 *) gfx_spike_collectibles_flameTiles + (sprite_width * sprite_height * (12) / 2);
    else
        frameGfx = (u8 *) gfx_spike_collectibles_flameTiles + (sprite_width * sprite_height * (11) / 2);

    subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
    mainSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
}

void Shotgun::set_position() {

    //TODO BLAST MUST HAVE ITS SEPARATE CLASS FOR VIEWPORTING

    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);

    mainSpriteInfo->entry->x = main_x;
    mainSpriteInfo->entry->y = main_y;

    subSpriteInfo->entry->x = sub_x;
    subSpriteInfo->entry->y = sub_y;

    mainSpriteInfo->entry->vFlip = false;
    mainSpriteInfo->entry->hFlip = false;

    subSpriteInfo->entry->vFlip = false;
    subSpriteInfo->entry->hFlip = false;

    update_anim_icon(x, y, physical_width);


}

Shotgun::Shotgun() {
    cost = 13500;
    name = "SHOTGUN";
    physical_height = SHOTGUN_PHYSICAL_HEIGHT;
    physical_width = SHOTGUN_PHYSICAL_WIDTH;
    sprite_height = SHOTGUN_SPRITE_HEIGHT;
    sprite_width = SHOTGUN_SPRITE_WIDTH;
    //FIXME Ustawiać to wszędzie
    spritesheet_type = SpritesheetType::SPIKES_COLLECTIBLES;
    sprite_type = SpriteType::S_SHOTGUN;
}

void Shotgun::spawn_bullets() {

    for (int a = 0; a < 4; a++) {
        auto *b = new Bullet();
        b->x = x;
        b->y = y;

        if (sprite_state == SpriteState::W_LEFT) {
            b->xSpeed = -4.3 - ((rand() % 20) / 10.0);
            b->x -= 5;
        } else {
            b->xSpeed = 4.3 + ((rand() % 20) / 10.0);
            b->x += 5 + physical_width;
        }

        b->init();
        global::sprites.push_back(b);

        if (a == 0)
            b->ySpeed = 1;
        if (a == 1)
            b->ySpeed = -1;
        if (a == 3)
            b->ySpeed = 0;
        if (a == 4)
            b->ySpeed = 2;

    }
}

void Shotgun::equip() {
    auto *g = new GotCollectible();
    g->x = x - 12;
    g->y = y - 20;
    g->collectible_type = 0;
    g->init();
    global::sprites.push_back(g);

}

