//
// Created by xdbeef on 23.06.18.
//

#include <cstdio>
#include "../../../build/gfx_got_collectible.h"
#include "blast.h"
#include "../../globals_declarations.h"
#include "../../../build/gfx_spike_collectibles_flame.h"

#define SHOTGUN_FIRING_OFFSET_X 14
#define SHOTGUN_FIRING_OFFSET_Y 6
#define SHOTGUN_ANIM_DELTA 30

void Blast::draw() {


    if (ready_to_dispose) {
        mainSpriteInfo->entry->isHidden = true;
        subSpriteInfo->entry->isHidden = true;
        return;
    } else {
        mainSpriteInfo->entry->isHidden = false;
        subSpriteInfo->entry->isHidden = false;
    }

    if (firing) {

        animFrameTimer += *global::timer;

        if (animFrameTimer > SHOTGUN_ANIM_DELTA) {
            animFrameTimer = 0;
            animFrame++;
        }

        frameGfx = (u8 *) gfx_spike_collectibles_flameTiles + (sprite_width * sprite_height * (13 + animFrame) / 2);
        subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
        mainSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);

        mainSpriteInfo->entry->vFlip = false;
        subSpriteInfo->entry->vFlip = false;

        mainSpriteInfo->entry->isHidden = false;
        subSpriteInfo->entry->isHidden = false;
    } else {

        mainSpriteInfo->entry->isHidden = true;
        subSpriteInfo->entry->isHidden = true;

        if (sprite_state == SpriteState::W_LEFT) {
            mainSpriteInfo->entry->hFlip = true;
            subSpriteInfo->entry->hFlip = true;
        } else {
            mainSpriteInfo->entry->hFlip = false;
            subSpriteInfo->entry->hFlip = false;
        }
    }

    set_position();

}


void Blast::init() {
    initSprite();
}

void Blast::initSprite() {


    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_spike_collectibles_flamePal,
                                                              gfx_spike_collectibles_flamePalLen,
                                                              nullptr, sprite_width * sprite_height, sprite_width,
                                                              spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_spike_collectibles_flamePal,
                                                                gfx_spike_collectibles_flamePalLen,
                                                                nullptr, sprite_width * sprite_height, sprite_width,
                                                                spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
    
    subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
    mainSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);


}

void Blast::set_position() {

    if (ready_to_dispose)
        return;

    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);

    mainSpriteInfo->entry->y = main_y - SHOTGUN_FIRING_OFFSET_Y;
    subSpriteInfo->entry->y = sub_y - SHOTGUN_FIRING_OFFSET_Y;

    if (sprite_state == SpriteState::W_LEFT) {
        mainSpriteInfo->entry->x = main_x - SHOTGUN_FIRING_OFFSET_X;
        subSpriteInfo->entry->x = sub_x - SHOTGUN_FIRING_OFFSET_X;
    } else {
        mainSpriteInfo->entry->x = main_x + SHOTGUN_FIRING_OFFSET_X;
        subSpriteInfo->entry->x = sub_x + SHOTGUN_FIRING_OFFSET_X;
    }

}

Blast::Blast() {
    sprite_height = BLAST_SPRITE_HEIGHT;
    sprite_width = BLAST_SPRITE_WIDTH;
    spritesheet_type = SpritesheetType::SPIKES_COLLECTIBLES;
}

