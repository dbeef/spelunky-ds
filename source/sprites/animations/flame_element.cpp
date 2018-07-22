//
// Created by xdbeef on 02.06.18.
//

#include "../../../build/gfx_spike_collectibles_flame.h"
#include "../../globals_declarations.h"
#include "../../collisions/collisions.h"
#include "flame_element.h"
#include "flame.h"

void FlameElement::draw() {

    if (ready_to_dispose || finished) {
        return;
    }


    inactive_delay += *global::timer;

//    if (bottomCollision)
//        living_timer += 4 * *global::timer;
//    else
//        living_timer += *global::timer;


    if (!finished) {
        frameTimer += *global::timer;

        if (frameTimer > FLAME_ANIM_FRAME_DELTA) {
            frameTimer = 0;
            currentFrame++;

            if (currentFrame >= 5) {
                currentFrame = 0;
                finished = true;
                ready_to_dispose = true;
                mainSpriteInfo->entry->isHidden = true;
                subSpriteInfo->entry->isHidden = true;
//                if(living_timer > 1000) {
//                }

            } else {


//                if(living_timer < 1000 && currentFrame >= 5)
//                    currentFrame = 0;

                frameGfx = (u8 *) gfx_spike_collectibles_flameTiles + ((34 + currentFrame) * sprite_width * sprite_height / 2);
                subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
                mainSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);

            }
        }

        int main_x, main_y, sub_x, sub_y;
        get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);

        mainSpriteInfo->entry->x = main_x;
        mainSpriteInfo->entry->y = main_y;

        subSpriteInfo->entry->x = sub_x;
        subSpriteInfo->entry->y = sub_y;

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
//        apply_friction(0.01);
        update_position();
        apply_gravity(GRAVITY_DELTA_SPEED * 0.15f);
        pos_inc_timer = 0;
    }

}

void FlameElement::updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) {

    MapTile *tiles[9];
    Collisions::getNeighboringTiles(global::level_generator->map_tiles, x_current_pos_in_tiles, y_current_pos_in_tiles,
                                    tiles);

    upperCollision = Collisions::checkUpperCollision(tiles, &x, &y, &ySpeed, physical_width, true, 0.55f);
    bottomCollision = Collisions::checkBottomCollision(tiles, &x, &y, &ySpeed, physical_width, physical_height, true, 0.55f);
    leftCollision = Collisions::checkLeftCollision(tiles, &x, &y, &xSpeed, physical_width, physical_height, true, 0.55f);
    rightCollision = Collisions::checkRightCollision(tiles, &x, &y, &xSpeed, physical_width, physical_height, true, 0.55f);


}

void FlameElement::initSprite() {
    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_spike_collectibles_flamePal, gfx_spike_collectibles_flamePalLen,
                                                        nullptr, sprite_width * sprite_height, 8, SPIKES_COLLECTIBLES, true,
                                                        false, LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_spike_collectibles_flamePal, gfx_spike_collectibles_flamePalLen,
                                                          nullptr, sprite_width * sprite_height, 8, SPIKES_COLLECTIBLES,
                                                          true, false, LAYER_LEVEL::MIDDLE_TOP);

    frameGfx = (u8 *) gfx_spike_collectibles_flameTiles + (( 34 + currentFrame )* sprite_width * sprite_height / 2);
    subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
    mainSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);

}

FlameElement::FlameElement() {
    physical_width = FLAME_PHYSICAL_WIDTH;
    physical_height = FLAME_PHYSICAL_HEIGHT;
    sprite_width = FLAME_SPRITE_WIDTH;
    sprite_height = FLAME_SPRITE_HEIGHT;
}