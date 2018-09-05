//
// Created by xdbeef on 15.05.18.
//

#include "../../../build/gfx_spike_collectibles_flame.h"
#include <cstdlib>
#include <maxmod9.h>
#include "spikes.h"
#include "../../collisions/collisions.h"
#include "../../globals_declarations.h"
#include "../../../build/soundbank.h"
#include "../animations/blood.h"
#include "../../sound/sound_utils.h"

void Spikes::draw() {

    if (ready_to_dispose)
        return;

    if (!global::main_dude->dead && !global::main_dude->using_cape
        && Collisions::checkCollisionBodiesLeftLowerCorner
                (x, y + 16, physical_width, physical_height, global::main_dude->x, global::main_dude->y + 16, 16, 16) &&
        /*global::main_dude->time_since_last_damage > 1000 &&*/
        !global::main_dude->bottomCollision && global::main_dude->ySpeed > 1) {

        kill_main_dude();
        spawn_blood();
        blood = true;
        frameGfx = (u8 *) gfx_spike_collectibles_flameTiles + (sprite_width * sprite_height * (1) / 2);
        subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
        mainSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
    }

    for (int a = 0; a < global::sprites.size(); a++) {
        if ((global::sprites.at(a)->sprite_type == SpriteType ::S_SPIDER ||
             global::sprites.at(a)->spritesheet_type == SpritesheetType::CAVEMAN_DAMSEL ||
             global::sprites.at(a)->spritesheet_type == SpritesheetType::SHOPKEEPER)
            && !global::sprites.at(a)->ready_to_dispose && !global::sprites.at(a)->killed) {

            if (Collisions::checkCollisionBodies(x, y, physical_width, physical_height,
                                                 global::sprites.at(a)->x,
                                                 global::sprites.at(a)->y,
                                                 global::sprites.at(a)->physical_width,
                                                 global::sprites.at(a)->physical_height)) {

                if (!global::sprites.at(a)->bottomCollision && global::sprites.at(a)->ySpeed > 0 && !global::sprites.at(a)->hold_by_main_dude) {
                    global::sprites.at(a)->apply_dmg(8);
                    global::sprites.at(a)->xSpeed = 0;


                    blood = true;
                    frameGfx = (u8 *) gfx_spike_collectibles_flameTiles + (sprite_width * sprite_height * (1) / 2);
                    subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
                    mainSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
                }
            }

        }
    }


    set_position();
}


void Spikes::init() {
    initSprite();
    activated = true;
}

void Spikes::initSprite() {

    if (blood)
        frameGfx = (u8 *) gfx_spike_collectibles_flameTiles + (sprite_width * sprite_height * (1) / 2);
    else
        frameGfx = (u8 *) gfx_spike_collectibles_flameTiles;

    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_spike_collectibles_flamePal, gfx_spike_collectibles_flamePalLen,
                                                        nullptr, sprite_width * sprite_height, sprite_width,
                                                        spriteType, true, false, LAYER_LEVEL::MIDDLE_BOT);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_spike_collectibles_flamePal, gfx_spike_collectibles_flamePalLen,
                                                          nullptr, sprite_width * sprite_height, sprite_width,
                                                          spriteType, true, false, LAYER_LEVEL::MIDDLE_BOT);

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

void Spikes::kill_main_dude() {

    mmEffect(SFX_XDIE);
    sound::stop_cave_music();

    global::main_dude->time_since_last_damage = 0;
    global::hud->hearts = 0;
    global::hud->draw_level_hud();

    global::main_dude->ySpeed = -MAIN_DUDE_JUMP_SPEED * 0.25;
    global::main_dude->dead = true;
    consoleClear();

}

