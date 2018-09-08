//
// Created by xdbeef on 02.06.18.
//


#include <cstdlib>
#include "../../globals_declarations.h"
#include "../../../build/gfx_spike_collectibles_flame.h"
#include "../../collisions/collisions.h"
#include "flame.h"
#include "../sprite_utils.h"

void Flame::draw() {

    for (int a = 0; a < flame_trail.size(); a++) {
        flame_trail.at(a)->draw();
        flame_trail.at(a)->update();
        if (flame_trail.at(a)->finished) {
            //FIXME Possible leaks in global::sprite_infos? Same with blood trails.
            delete (flame_trail.at(a));
            flame_trail.erase(flame_trail.begin() + a);
        }
    }

    if (ready_to_dispose)
        return;

    set_position();
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_visibility(true, mainSpriteInfo, subSpriteInfo);

    time_since_last_spawn += *global::timer;

    if (bottomCollision)
        living_timer += 4 * *global::timer;
    else
        living_timer += *global::timer;

    if (finished) {

        //main flame animation finished, check if other spawned flames finished

        bool chain_finished = true;

        for (auto &a : flame_trail) {
            if (!a->finished) {
                chain_finished = false;
                break;
            }
        }

        if (chain_finished) {
            ready_to_dispose = true;
            sprite_utils::set_visibility(false, mainSpriteInfo, subSpriteInfo);
        }


    } else {
        //main flame animation not finished, continue and spawn other flames if possible

        animFrameTimer += *global::timer;

        if (animFrameTimer > FLAME_ANIM_FRAME_DELTA) {
            animFrameTimer = 0;
            match_animation();
        }

        if (!finished && flame_trail.size() <= 2 && time_since_last_spawn > 120) {
            time_since_last_spawn = 0;
            spawn_flame();
        }

    }
}


void Flame::init() {
    initSprite();
}

void Flame::updateSpeed() {

    pos_inc_timer += *global::timer;
    limit_speed(MAX_X_SPEED_FLAME, MAX_Y_SPEED_FLAME);

    bool change_pos = (pos_inc_timer > change_pos_delta_offset) && !finished;

    if (change_pos) {
        apply_gravity(GRAVITY_DELTA_SPEED * 0.15f);
        pos_inc_timer = 0;
        update_position();
    }

}

void Flame::updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) {
    MapTile *t[9];
    Collisions::getNeighboringTiles(global::level_generator->map_tiles,
                                    x_current_pos_in_tiles, y_current_pos_in_tiles, t);
    upperCollision = Collisions::checkUpperCollision(t, &x, &y, &ySpeed, physical_width, true, 0.55);
    bottomCollision = Collisions::checkBottomCollision(t, &x, &y, &ySpeed, physical_width, physical_height, true, 0.55);
    leftCollision = Collisions::checkLeftCollision(t, &x, &y, &xSpeed, physical_width, physical_height, true, 0.55);
    rightCollision = Collisions::checkRightCollision(t, &x, &y, &xSpeed, physical_width, physical_height, true, 0.55);

}

void Flame::initSprite() {
    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_spike_collectibles_flamePal,
                                                        gfx_spike_collectibles_flamePalLen, nullptr, FLAME_SPRITE_SIZE,
                                                        8, SPIKES_COLLECTIBLES, true, false, LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_spike_collectibles_flamePal,
                                                          gfx_spike_collectibles_flamePalLen, nullptr,
                                                          FLAME_SPRITE_SIZE, 8, SPIKES_COLLECTIBLES, true, false,
                                                          LAYER_LEVEL::MIDDLE_TOP);
    match_animation();
    set_position();
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_visibility(true, mainSpriteInfo, subSpriteInfo);
}

Flame::Flame() {
    physical_width = FLAME_PHYSICAL_WIDTH;
    physical_height = FLAME_PHYSICAL_HEIGHT;
    sprite_width = FLAME_SPRITE_WIDTH;
    sprite_height = FLAME_SPRITE_HEIGHT;
    change_pos_delta_offset = FLAME_CHANGE_POS_DELTA + (rand() % 5);
}

void Flame::spawn_flame() {
    auto *element = new FlameElement();
    element->x = x;
    element->y = y;
    element->xSpeed = xSpeed;
    element->ySpeed = ySpeed;
    element->currentFrame = currentFrame;
    element->init();
    element->draw();
    element->pos_inc_delta_offset = change_pos_delta_offset;
    flame_trail.push_back(element);
}

void Flame::set_position() {
    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);
    sprite_utils::set_entry_xy(mainSpriteInfo, main_x, main_y);
    sprite_utils::set_entry_xy(subSpriteInfo, sub_x, sub_y);
}

void Flame::match_animation() {

    if (living_timer <= 1200)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spike_collectibles_flameTiles, FLAME_SPRITE_SIZE, 39);
    else if (living_timer > 1200) {

        currentFrame++;

        if (currentFrame >= 5) {
            finished = true;
            sprite_utils::set_visibility(false, mainSpriteInfo, subSpriteInfo);
        } else
            frameGfx = sprite_utils::get_frame((u8 *) gfx_spike_collectibles_flameTiles, FLAME_SPRITE_SIZE,
                                               currentFrame + 34);

    }

    sprite_utils::update_frame(frameGfx, FLAME_SPRITE_SIZE, mainSpriteInfo, subSpriteInfo);
}
