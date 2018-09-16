//
// Created by xdbeef on 02.06.18.
//

#include "../../../build/gfx_spike_collectibles_flame.h"
#include "../../globals_declarations.hpp"
#include "../../collisions/collisions.hpp"
#include "flame_element.hpp"
#include "flame.hpp"
#include "../sprite_utils.hpp"

//TODO remove 'finished' flag - duplicates ready_to_dispose - same for blood element
//TODO Give bombs a bigger rendering priority than other things - but no bigger than hud
void FlameElement::draw() {

    if (ready_to_dispose || finished)
        return;

    set_position();
    sprite_utils::set_visibility(true, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);

    inactive_delay += *global::timer;
    frameTimer += *global::timer;

    if (frameTimer > FLAME_ANIM_FRAME_DELTA) {

        frameTimer = 0;
        currentFrame++;

        if (currentFrame >= 5) {
            finished = true;
            ready_to_dispose = true;
            sprite_utils::set_visibility(false, mainSpriteInfo, subSpriteInfo);
        } else
            match_animation();

    }

}


void FlameElement::init() {
    initSprite();
}

void FlameElement::updateSpeed() {

    limit_speed(MAX_X_SPEED_FLAME, MAX_Y_SPEED_FLAME);

    if (inactive_delay < 90)
        return;

    pos_inc_timer += *global::timer;

    bool change_pos = (pos_inc_timer > pos_inc_delta_offset) && !finished;

    if (change_pos) {
        update_position();
        apply_gravity(GRAVITY_DELTA_SPEED * 0.15f);
        pos_inc_timer = 0;
    }

}

void FlameElement::updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) {
    MapTile *t[9];
    Collisions::getNeighboringTiles(global::current_level->map_tiles,
                                    x_current_pos_in_tiles, y_current_pos_in_tiles, t);
    upperCollision = Collisions::checkUpperCollision(t, &x, &y, &ySpeed, physical_width, true, 0.55);
    bottomCollision = Collisions::checkBottomCollision(t, &x, &y, &ySpeed, physical_width, physical_height, true, 0.55);
    leftCollision = Collisions::checkLeftCollision(t, &x, &y, &xSpeed, physical_width, physical_height, true, 0.55);
    rightCollision = Collisions::checkRightCollision(t, &x, &y, &xSpeed, physical_width, physical_height, true, 0.55);
}

void FlameElement::initSprite() {

    delete mainSpriteInfo;
    delete subSpriteInfo;

    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_spike_collectibles_flamePal,
                                                        gfx_spike_collectibles_flamePalLen, nullptr,
                                                        FLAME_SPRITE_SIZE, 8, SPIKES_COLLECTIBLES, true,
                                                        false, LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_spike_collectibles_flamePal,
                                                          gfx_spike_collectibles_flamePalLen,
                                                          nullptr, FLAME_SPRITE_SIZE, 8, SPIKES_COLLECTIBLES,
                                                          true, false, LAYER_LEVEL::MIDDLE_TOP);
    match_animation();
    set_position();
    sprite_utils::set_visibility(true, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);
}

FlameElement::FlameElement() {
    physical_width = FLAME_PHYSICAL_WIDTH;
    physical_height = FLAME_PHYSICAL_HEIGHT;
    sprite_width = FLAME_SPRITE_WIDTH;
    sprite_height = FLAME_SPRITE_HEIGHT;
}

void FlameElement::set_position() {
    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);
    sprite_utils::set_entry_xy(mainSpriteInfo, main_x, main_y);
    sprite_utils::set_entry_xy(subSpriteInfo, sub_x, sub_y);
}

void FlameElement::match_animation() {
    frameGfx = sprite_utils::get_frame((u8 *) gfx_spike_collectibles_flameTiles, FLAME_SPRITE_SIZE, 34 + currentFrame);
    sprite_utils::update_frame(frameGfx, FLAME_SPRITE_SIZE, mainSpriteInfo, subSpriteInfo);
}
