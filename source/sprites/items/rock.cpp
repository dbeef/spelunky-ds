//
// Created by xdbeef on 04.04.18.
//

#include "../../globals_declarations.h"
#include "../main_dude/main_dude.h"
#include "../../../build/gfx_blood_rock_rope_poof.h"
#include "rock.h"
#include "../../collisions/collisions.h"
#include "../../tiles/level_rendering_utils.h"
#include "../sprite_utils.h"

#define ROCK_POS_INC_DELTA 15

void Rock::draw() {

    if (ready_to_dispose) return;

    check_if_can_be_pickuped();
    set_pickuped_position(0, 10, 6);
    set_sprite_attributes();
    kill_mobs_if_thrown(1);
}


void Rock::init() {
    initSprite();
    activated = true;
}

void Rock::updateSpeed() {

    limit_speed(MAX_X_SPEED_ROCK, MAX_Y_SPEED_ROCK);

    pos_inc_timer += *global::timer;

    bool change_pos = (pos_inc_timer > ROCK_POS_INC_DELTA) && !hold_by_main_dude;

    if (change_pos) {
        apply_friction(0.08);
        update_position();
        apply_gravity(GRAVITY_DELTA_SPEED * 0.8);
        pos_inc_timer = 0;
    }

}


void Rock::updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) {

    MapTile *t[9];
    Collisions::getNeighboringTiles(global::current_level->map_tiles,
                                    x_current_pos_in_tiles, y_current_pos_in_tiles, t);

    upperCollision = Collisions::checkUpperCollision(t, &x, &y, &ySpeed, physical_width, true, 0.35);
    bottomCollision = Collisions::checkBottomCollision(t, &x, &y, &ySpeed, physical_width, physical_height, true, 0.35);
    leftCollision = Collisions::checkLeftCollision(t, &x, &y, &xSpeed, physical_width, physical_height, true, 0.15);
    rightCollision = Collisions::checkRightCollision(t, &x, &y, &xSpeed, physical_width, physical_height, true, 0.15);

}

void Rock::initSprite() {

    delete mainSpriteInfo;
    delete subSpriteInfo;

    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_blood_rock_rope_poofPal, gfx_blood_rock_rope_poofPalLen,
                                                        nullptr, ROCK_SPRITE_SIZE, 8, BLOOD_ROCK_ROPE_POOF, true, false,
                                                        LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_blood_rock_rope_poofPal, gfx_blood_rock_rope_poofPalLen,
                                                          nullptr, ROCK_SPRITE_SIZE, 8, BLOOD_ROCK_ROPE_POOF, true,
                                                          false, LAYER_LEVEL::MIDDLE_TOP);

    frameGfx = sprite_utils::get_frame((u8 *) gfx_blood_rock_rope_poofTiles, ROCK_SPRITE_SIZE, 7);
    sprite_utils::update_frame(frameGfx, ROCK_SPRITE_SIZE, mainSpriteInfo, subSpriteInfo);
    set_sprite_attributes();
}

Rock::Rock() {
    physical_height = ROCK_PHYSICAL_HEIGHT;
    physical_width = ROCK_PHYSICAL_WIDTH;
    sprite_height = ROCK_SPRITE_HEIGHT;
    sprite_width = ROCK_SPRITE_WIDTH;
    spritesheet_type = SpritesheetType::BLOOD_ROCK_ROPE_POOF;
}

void Rock::set_position() {
    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);
    sprite_utils::set_entry_xy(mainSpriteInfo, main_x, main_y);
    sprite_utils::set_entry_xy(subSpriteInfo, sub_x, sub_y);
}

void Rock::set_sprite_attributes() {
    set_position();
    sprite_utils::set_priority(OBJPRIORITY_0, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_visibility(true, mainSpriteInfo, subSpriteInfo);
}
