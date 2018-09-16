//
// Created by xdbeef on 23.04.18.
//

#include <cmath>
#include "jar.hpp"
#include "../../globals_declarations.hpp"
#include "../../../build/gfx_spike_collectibles_flame.h"
#include "../../collisions/collisions.hpp"
#include "../sprite_utils.hpp"

#define JAR_POS_INC_DELTA 15
#define JAR_HITPOINTS 1

void Jar::draw() {

    if (ready_to_dispose) return;

    if (frame >= 7) {
        //last frame of 'poof' animation that is played on destroying jar
        sprite_utils::set_visibility(false, mainSpriteInfo, subSpriteInfo);
        ready_to_dispose = true;
        return;
    }

    check_if_can_be_pickuped();
    set_pickuped_position(2, 8, 6);

    if (killed && frame < 7) {

        frameTimer += *global::timer;
        if (frameTimer > 50) {
            frame++;
            match_animation();
            frameTimer = 0;
        }

    }

    if (kill_mobs_if_thrown(1))
        apply_dmg(1);

    set_sprite_attributes();
    kill_if_whip(1);
}


void Jar::init() {
    initSprite();
    activated = true;
}

void Jar::updateSpeed() {

    if (killed) return;

    limit_speed(MAX_X_SPEED_JAR, MAX_Y_SPEED_JAR);

    pos_inc_timer += *global::timer;

    bool change_pos = (pos_inc_timer > JAR_POS_INC_DELTA) && !hold_by_main_dude;

    if (change_pos) {
        apply_friction(0.055);
        update_position();
        apply_gravity(GRAVITY_DELTA_SPEED * 0.8);
        pos_inc_timer = 0;

    }
}


void Jar::updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) {

    MapTile *t[9];
    Collisions::getNeighboringTiles(global::current_level->map_tiles,
                                    x_current_pos_in_tiles, y_current_pos_in_tiles, t);

    upperCollision = Collisions::checkUpperCollision(t, &x, &y, &ySpeed, physical_width, true, 0.35);
    bottomCollision = Collisions::checkBottomCollision(t, &x, &y, &ySpeed, physical_width, physical_height, true, 0.35);
    leftCollision = Collisions::checkLeftCollision(t, &x, &y, &xSpeed, physical_width, physical_height, true, 0.15);
    rightCollision = Collisions::checkRightCollision(t, &x, &y, &xSpeed, physical_width, physical_height, true, 0.15);

    if ((fabs(xSpeed) > 0.5 || fabs(ySpeed) > 0.5) &&
        (bottomCollision || leftCollision || rightCollision || upperCollision)) {
        //destroy jar on colliding with map tiles with enough speed
        apply_dmg(1);
    }

}

void Jar::initSprite() {

    delete mainSpriteInfo;
    delete subSpriteInfo;

    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_spike_collectibles_flamePal,
                                                        gfx_spike_collectibles_flamePalLen, nullptr, JAR_SPRITE_SIZE,
                                                        16, JAR, true, false, LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_spike_collectibles_flamePal,
                                                          gfx_spike_collectibles_flamePalLen, nullptr, JAR_SPRITE_SIZE,
                                                          16, JAR, true, false, LAYER_LEVEL::MIDDLE_TOP);
    match_animation();
    set_sprite_attributes();
}

Jar::Jar() {
    physical_height = JAR_PHYSICAL_HEIGHT;
    physical_width = JAR_PHYSICAL_WIDTH;
    sprite_height = JAR_SPRITE_HEIGHT;
    sprite_width = JAR_SPRITE_WIDTH;
    hitpoints = JAR_HITPOINTS;
    spritesheet_type = SpritesheetType::JAR;
}

void Jar::apply_dmg(int dmg_to_apply) {
    //jar has only 1 dmg point, always kill if any dmg_apply
    killed = true;
}

void Jar::match_animation() {

    if (killed)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spike_collectibles_flameTiles, JAR_SPRITE_SIZE, frame + 24);
    else
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spike_collectibles_flameTiles, JAR_SPRITE_SIZE, 24);

    sprite_utils::update_frame(frameGfx, JAR_SPRITE_SIZE, mainSpriteInfo, subSpriteInfo);
}

void Jar::set_position() {
    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);
    sprite_utils::set_entry_xy(mainSpriteInfo, main_x, main_y);
    sprite_utils::set_entry_xy(subSpriteInfo, sub_x, sub_y);
}

void Jar::set_sprite_attributes() {
    set_position();
    sprite_utils::set_visibility(true, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_priority(OBJPRIORITY_0, mainSpriteInfo, subSpriteInfo);
}
