//
// Created by xdbeef on 15.05.18.
//

#include "../../../build/gfx_spike_collectibles.h"
#include <cstdlib>
#include <maxmod9.h>
#include "spikes.h"
#include "../../collisions/collisions.h"
#include "../../globals_declarations.h"
#include "../../../build/soundbank.h"
#include "../animations/blood.h"

void Spikes::draw() {

    if (ready_to_dispose)
        return;

    if (!global::main_dude->dead
        && Collisions::checkCollisionBodiesLeftLowerCorner
                           (x , y + 16, physical_width, physical_height, global::main_dude->x, global::main_dude->y + 16, 16, 16) &&
        global::main_dude->time_since_last_damage > 1000 &&
        !global::main_dude->bottomCollision && global::main_dude->ySpeed > 0) {

        mmEffect(SFX_XDIE);

        global::main_dude->time_since_last_damage = 0;
        global::hud->hearts = 0;
        global::hud->draw();

        global::hud->hide();
        global::main_dude->ySpeed = -MAIN_DUDE_JUMP_SPEED * 0.25;
        global::main_dude->dead = true;


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


        blood = true;

        frameGfx = (u8 *) gfx_spike_collectiblesTiles + (sprite_width * sprite_height * (1) / 2);
        subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
        mainSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
    }

    set_position();
}


void Spikes::init() {
    initSprite();
    activated_by_main_dude = true;
}

void Spikes::updateSpeed() {
}

void Spikes::updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) {

}

void Spikes::initSprite() {

    if (blood)
        frameGfx = (u8 *) gfx_spike_collectiblesTiles + (sprite_width * sprite_height * (1) / 2);
    else
        frameGfx = (u8 *) gfx_spike_collectiblesTiles;

    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_spike_collectiblesPal, gfx_spike_collectiblesPalLen,
                                                        nullptr, sprite_width * sprite_height, sprite_width,
                                                        spriteType, true, false,LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_spike_collectiblesPal, gfx_spike_collectiblesPalLen,
                                                          nullptr, sprite_width * sprite_height, sprite_width,
                                                          spriteType, true, false,LAYER_LEVEL::MIDDLE_TOP);

    subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
    mainSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
}

void Spikes::set_position() {

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

}

Spikes::Spikes() {
    this->sprite_height = SPIKES_SPRITE_HEIGHT;
    this->sprite_width = SPIKES_SPRITE_WIDTH;
    this->physical_height = SPIKES_PHYSICAL_HEIGHT;
    this->physical_width = SPIKES_PHYSICAL_WIDTH;
    spriteType = SpritesheetType::SPIKES_COLLECTIBLES;
}

