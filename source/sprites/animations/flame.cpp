//
// Created by xdbeef on 02.06.18.
//


#include "../../globals_declarations.h"
#include "../../../build/gfx_spike_collectibles_flame.h"
#include "../../collisions/collisions.h"
#include "flame.h"

void Flame::draw() {


    for (int a = 0; a < flame_trail.size(); a++) {
        flame_trail.at(a)->draw();
        flame_trail.at(a)->update();
        if(flame_trail.at(a)->finished) {
            delete(flame_trail.at(a));
            flame_trail.erase(flame_trail.begin() + a);
        }
    }

    if (ready_to_dispose) {
        mainSpriteInfo->entry->isHidden = true;
        subSpriteInfo->entry->isHidden = true;
        return;
    }

    time_since_last_spawn += *global::timer;

    if (bottomCollision)
        living_timer += 4 * *global::timer;
    else
        living_timer += *global::timer;

    if (finished) {

        bool chain_finished = true;

        for (int a = 0; a < flame_trail.size(); a++) {
            if (!flame_trail.at(a)->finished) {
                chain_finished = false;
                break;
            }
        }

        if (chain_finished) {
            ready_to_dispose = true;
        }


    } else {
        animFrameTimer += *global::timer;

        if (animFrameTimer > FLAME_ANIM_FRAME_DELTA) {
            animFrameTimer = 0;

            if (living_timer <= 1200) {

                frameGfx = (u8 *) gfx_spike_collectibles_flameTiles + ((39) * sprite_width * sprite_height / 2);

            } else if (living_timer > 1200) {

            currentFrame++;
                if (currentFrame >= 5) {
                    finished = true;
                    mainSpriteInfo->entry->isHidden = true;
                    subSpriteInfo->entry->isHidden = true;
                } else
                    frameGfx = (u8 *) gfx_spike_collectibles_flameTiles + ((34 + currentFrame) * sprite_width * sprite_height / 2);

            }

            subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
            mainSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);

        }


        if (!finished && flame_trail.size() <= 2 && time_since_last_spawn > 120) {
            time_since_last_spawn = 0;
            spawn_flame();
        }

        int main_x, main_y, sub_x, sub_y;
        get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);

        mainSpriteInfo->entry->x = main_x;
        mainSpriteInfo->entry->y = main_y;

        subSpriteInfo->entry->x = sub_x;
        subSpriteInfo->entry->y = sub_y;


    }

}


void Flame::init() {
    initSprite();
}

void Flame::updateSpeed() {

    limit_speed(MAX_X_SPEED_FLAME, MAX_Y_SPEED_FLAME);

    pos_inc_timer += *global::timer;

    bool change_pos = (pos_inc_timer > change_pos_delta_offset) && !finished;

    if (change_pos) {
//        apply_friction(0.01);

        apply_gravity(GRAVITY_DELTA_SPEED * 0.15f);
        pos_inc_timer = 0;
        update_position();
    }

}

void Flame::updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) {

    MapTile *tiles[9];
    Collisions::getNeighboringTiles(global::level_generator->map_tiles, x_current_pos_in_tiles, y_current_pos_in_tiles,
                                    tiles);

    bottomCollision = Collisions::checkBottomCollision(tiles, &x, &y, &ySpeed, physical_width, physical_height, true,
                                                       0.55f);
    leftCollision = Collisions::checkLeftCollision(tiles, &x, &y, &xSpeed, physical_width, physical_height, true, 0.55f);
    rightCollision = Collisions::checkRightCollision(tiles, &x, &y, &xSpeed, physical_width, physical_height, true,
                                                     0.55f);
    upperCollision = Collisions::checkUpperCollision(tiles, &x, &y, &ySpeed, physical_width, true, 0.55f);

}

void Flame::initSprite() {
    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_spike_collectibles_flamePal, gfx_spike_collectibles_flamePalLen,
                                                        nullptr, sprite_width * sprite_height, 8, SPIKES_COLLECTIBLES, true,
                                                        false,
                                                        LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_spike_collectibles_flamePal, gfx_spike_collectibles_flamePalLen,
                                                          nullptr, sprite_width * sprite_height, 8, SPIKES_COLLECTIBLES,
                                                          true, false,
                                                          LAYER_LEVEL::MIDDLE_TOP);

    frameGfx = (u8 *) gfx_spike_collectibles_flameTiles + (0 * sprite_width * sprite_height / 2);
    subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
    mainSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
}

Flame::Flame() {
    physical_width = FLAME_PHYSICAL_WIDTH;
    physical_height = FLAME_PHYSICAL_HEIGHT;
    sprite_width = FLAME_SPRITE_WIDTH;
    sprite_height = FLAME_SPRITE_HEIGHT;
    change_pos_delta_offset = FLAME_CHANGE_POS_DELTA  + (rand() % 5);
}

void Flame::spawn_flame() {
    FlameElement *element = new FlameElement();

//    if (xSpeed < 0)
//        element->x = x + 8;
//    else
//        element->x = x - 8;
//
//    if (ySpeed > 0)
//        element->y = y - 8;
//    else
//        element->y = y + 8;

    element->x = x;
    element->y = y;


    element->xSpeed = xSpeed /** 0.95f*/;
    element->ySpeed = ySpeed /** 0.95f*/;
    element->currentFrame = currentFrame;
    element->init();
    element->draw();
    element->pos_inc_delta_offset = change_pos_delta_offset;
    flame_trail.push_back(element);
/*
    element->living_timer += 400 * bloodTrail.size();*/
}
