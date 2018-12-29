//
// Created by xdbeef on 23.06.18.
//

#include <cstdio>
#include "../../../build/gfx_got_collectible.h"
#include "blast.hpp"
#include "../../globals_declarations.hpp"
#include "../../../build/gfx_spike_collectibles_flame.h"
#include "../sprite_utils.hpp"

#define SHOTGUN_FIRING_OFFSET_X 14
#define SHOTGUN_FIRING_OFFSET_Y 6
#define SHOTGUN_ANIM_DELTA 30

void Blast::draw() {

    if (ready_to_dispose)
        return;

    //'firing' property is set by the owner of a blast instance, by default a pistol/shotgun
    if (firing) {

        animFrameTimer += *global::timer;

        if (animFrameTimer > SHOTGUN_ANIM_DELTA) {
            animFrameTimer = 0;
            animFrame++;
        }

        match_animation();

        sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
        sprite_utils::set_visibility(true, mainSpriteInfo, subSpriteInfo);

    } else {
        sprite_utils::set_visibility(false, mainSpriteInfo, subSpriteInfo);
        sprite_utils::set_horizontal_flip(sprite_state == SpriteState::W_LEFT, mainSpriteInfo, subSpriteInfo);
    }

    set_position();

}


void Blast::init() {
    initSprite();
}

void Blast::initSprite() {

    delete mainSpriteInfo;
    delete subSpriteInfo;

    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_spike_collectibles_flamePal,
                                                        gfx_spike_collectibles_flamePalLen,
                                                        nullptr, BLAST_SPRITE_SIZE, sprite_width,
                                                        spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_spike_collectibles_flamePal,
                                                          gfx_spike_collectibles_flamePalLen,
                                                          nullptr, BLAST_SPRITE_SIZE, sprite_width,
                                                          spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);

    sprite_utils::set_visibility(true, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    //Make sure sprite_state is not updated even when firing == false! Otherwise blast may change direction
    //on sprite reload via initSprite
    sprite_utils::set_horizontal_flip(sprite_state == SpriteState::W_LEFT, mainSpriteInfo, subSpriteInfo);
    match_animation();
    set_position();
    sprite_utils::set_priority(OBJPRIORITY_0, mainSpriteInfo, subSpriteInfo);

}

void Blast::set_position() {

    //TODO Overload get_x_y_viewported with version that allows passing offsets, so temp variables wouldn't be needed

    int temp_y = y, temp_x = x;

    y -= SHOTGUN_FIRING_OFFSET_Y;

    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);

    if (sprite_state == SpriteState::W_LEFT)
        x -= SHOTGUN_FIRING_OFFSET_X;
    else
        x += SHOTGUN_FIRING_OFFSET_X;

    sprite_utils::set_entry_xy(mainSpriteInfo, main_x, main_y);
    sprite_utils::set_entry_xy(subSpriteInfo, sub_x, sub_y);

    y = temp_y;
    x = temp_x;
}

Blast::Blast() {
    sprite_height = BLAST_SPRITE_HEIGHT;
    sprite_width = BLAST_SPRITE_WIDTH;
    spritesheet_type = SpritesheetType::SPIKES_COLLECTIBLES;
}

void Blast::match_animation() {
    frameGfx = sprite_utils::get_frame((u8 *) gfx_spike_collectibles_flameTiles, BLAST_SPRITE_SIZE, 13 + animFrame);
    sprite_utils::update_frame(frameGfx, BLAST_SPRITE_SIZE, mainSpriteInfo, subSpriteInfo);
}

void Blast::deleteSprite() {
    delete mainSpriteInfo;
    delete subSpriteInfo;
    mainSpriteInfo = nullptr;
    subSpriteInfo = nullptr;
}

