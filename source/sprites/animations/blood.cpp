//
// Created by xdbeef on 28.04.18.
//

#include "blood.hpp"
#include "../../globals_declarations.hpp"
#include "../../../build/gfx_blood_rock_rope_poof.h"
#include "../../collisions/collisions.hpp"
#include "../../tiles/level_rendering_utils.hpp"
#include "../sprite_utils.hpp"

void Blood::draw() {

    //update all trail elements
    for (int a = 0; a < bloodTrail.size(); a++) {
        bloodTrail.at(a)->draw();
        bloodTrail.at(a)->update();
        if (bloodTrail.at(a)->finished) {
            delete (bloodTrail.at(a));
            bloodTrail.erase(bloodTrail.begin() + a);
        }
    }

    if (ready_to_dispose) {
        sprite_utils::set_visibility(false, mainSpriteInfo, subSpriteInfo);
        return;
    }

    time_since_last_spawn += *global::timer;

    if (bottomCollision)
        //dispose blood droplet faster if touching ground
        living_timer += 4 * (*global::timer);
    else
        living_timer += *global::timer;

    if (finished) {
        //if main blood droplet animation is finished,
        //check if all animations in blood trail finished, if so, main blood droplet can be disposed

        bool chain_finished = true;

        for (auto &a : bloodTrail) {
            if (!a->finished) {
                chain_finished = false;
                break;
            }
        }

        if (chain_finished)
            ready_to_dispose = true;

    } else {
        //animation not finished on main droplet

        animFrameTimer += *global::timer;

        if (animFrameTimer > BLOOD_ANIM_FRAME_DELTA) {
            animFrameTimer = 0;
            currentFrame++;
            match_animation();
        }

        if (!finished && bloodTrail.size() <= 2 && time_since_last_spawn > 120) {
            time_since_last_spawn = 0;
            spawn_blood();
        }

        sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
        sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);
        set_position();
    }

}


void Blood::init() {
    initSprite();
}

void Blood::updateSpeed() {

    pos_inc_timer += *global::timer;
    limit_speed(MAX_X_SPEED_BLOOD, MAX_Y_SPEED_BLOOD);

    bool change_pos = (pos_inc_timer > BLOOD_CHANGE_POS_DELTA) && !finished;

    if (change_pos) {
        apply_gravity(GRAVITY_DELTA_SPEED * 0.2f);
        pos_inc_timer = 0;
        update_position();
    }

}

void Blood::updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) {
    MapTile *t[9];
    Collisions::getNeighboringTiles(global::current_level->map_tiles,
                                    x_current_pos_in_tiles, y_current_pos_in_tiles, t);
    upperCollision = Collisions::checkUpperCollision(t, &x, &y, &ySpeed, physical_width, true, 0.5f);
    bottomCollision = Collisions::checkBottomCollision(t, &x, &y, &ySpeed, physical_width, physical_height, true, 0.7f);
    leftCollision = Collisions::checkLeftCollision(t, &x, &y, &xSpeed, physical_width, physical_height, true, 0.5f);
    rightCollision = Collisions::checkRightCollision(t, &x, &y, &xSpeed, physical_width, physical_height, true, 0.5f);

}

void Blood::initSprite() {

    delete mainSpriteInfo;
    delete subSpriteInfo;

    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_blood_rock_rope_poofPal, gfx_blood_rock_rope_poofPalLen,
                                                        nullptr, BLOOD_SPRITE_SIZE, 8, BLOOD_ROCK_ROPE_POOF,
                                                        true, false, LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_blood_rock_rope_poofPal, gfx_blood_rock_rope_poofPalLen,
                                                          nullptr, BLOOD_SPRITE_SIZE, 8, BLOOD_ROCK_ROPE_POOF,
                                                          true, false, LAYER_LEVEL::MIDDLE_TOP);
    match_animation();
    set_position();
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);

}

Blood::Blood() {
    physical_width = BLOOD_PHYSICAL_WIDTH;
    physical_height = BLOOD_PHYSICAL_HEIGHT;
    sprite_width = BLOOD_SPRITE_WIDTH;
    sprite_height = BLOOD_SPRITE_HEIGHT;
}

void Blood::spawn_blood() {
    auto *element = new BloodElement();
    element->x = x;
    element->y = y;
    element->xSpeed = xSpeed;
    element->ySpeed = ySpeed;
    element->currentFrame = currentFrame;
    element->init();
    element->draw();
    bloodTrail.push_back(element);

}

void Blood::match_animation() {

    if (living_timer <= 1200) {
        //while living_timer is less than 1.2 s, repeat the same animation

        if (currentFrame >= 3)
            currentFrame = 0;

        frameGfx = sprite_utils::get_frame((u8 *) gfx_blood_rock_rope_poofTiles, BLOOD_SPRITE_SIZE, 11 + currentFrame);

    } else if (living_timer > 1200) {
        //if living_timer greater than 1.2 s, finish animation with the last frames

        if (currentFrame >= 6) {
            finished = true;
            sprite_utils::set_visibility(false, mainSpriteInfo, subSpriteInfo);
        } else
            frameGfx = sprite_utils::get_frame((u8 *) gfx_blood_rock_rope_poofTiles, BLOOD_SPRITE_SIZE, currentFrame);

    }

    sprite_utils::update_frame(frameGfx, BLOOD_SPRITE_SIZE, mainSpriteInfo, subSpriteInfo);

}

void Blood::set_position() {
    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);
    sprite_utils::set_entry_xy(mainSpriteInfo, main_x, main_y);
    sprite_utils::set_entry_xy(subSpriteInfo, sub_x, sub_y);
}
