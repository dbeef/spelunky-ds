//
// Created by xdbeef on 02.05.18.
//

#include <maxmod9.h>
#include <cstdlib>
#include "Bat.hpp"
#include "../../../build/gfx_bat_snake_jetpack.h"
#include "../items/Rock.hpp"
#include "../animations/Blood.hpp"
#include "../../collisions/Collisions.hpp"
#include "../../tiles/LevelRenderingUtils.hpp"
#include "../../../build/soundbank.h"

#include "../../GameState.hpp"
#include "../../time/Timer.h"
#include "../../graphics/SpriteUtils.hpp"

#define BAT_ANIM_FRAME_DELTA 100
#define BAT_POS_INC_DELTA 30

void Bat::update_creature_specific() {

    if (_ready_to_dispose)
        return;

//    printf("1");
    update_sprites_position();
//    printf("2");

    sprite_utils::set_horizontal_flip(true, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_visibility(true, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
//    printf("3");

    if (!hunting) {
        //check if main dude is in bat's triggering scope
        hunting = abs(_x - GameState::instance().main_dude->_x) < 7 * 16 &&
                  abs(_y - GameState::instance().main_dude->_y) < 7 * 16 &&
                  GameState::instance().main_dude->_y > _y;

        if (hunting)
            mmEffect(SFX_XBAT);

    } else
        //checking if main dude is still in bat's triggering scope
        hunting = abs(_x - GameState::instance().main_dude->_x) < 9 * 16 && abs(_y - GameState::instance().main_dude->_y) < 9 * 16;

    animFrameTimer += Timer::getDeltaTime();

    if (animFrameTimer > BAT_ANIM_FRAME_DELTA) {

        animFrameTimer = 0;
        animFrame++;

        if (animFrame >= 3)
            animFrame = 0;

//        printf("4");
        match_animation();
//        printf("5");
    }

    if (hunting) {
        follow_main_dude();
    } else {
        //fly upwards, to seek something to hang on
        _x_speed = 0;
        _y_speed = -1;
    }

//    printf("6");
//here

    kill_if_whip(1);
    kill_if_main_dude_jumped_on_you(1);
    deal_damage_main_dude_on_collision(1);

    if (_map_collisions_checked) {
        standingOnLeftEdge = Collisions::isStandingOnLeftEdge(_neighboring_tiles, _x, _physical_width,
                                                              _current_x_in_tiles);
        standingOnRightEdge = Collisions::isStandingOnRightEdge(_neighboring_tiles, _x, _physical_width,
                                                                _current_x_in_tiles);
        hanging = _upper_collision && !hunting;
        _map_collisions_checked = false;
    }

}

//!>bat has only 1 dmg point, always kill if any dmg_apply
void Bat::apply_dmg(int dmg_to_apply) {

    sprite_utils::set_visibility(false, mainSpriteInfo, subSpriteInfo);
    killed = true;
    _ready_to_dispose = true;
    spawn_blood();
    GameState::instance().killed_npcs.push_back(_creature_type);

}

void Bat::init_sprites() {

    delete_sprites();

    subSpriteInfo = GameState::instance().sub_oam_manager->initSprite(gfx_bat_snake_jetpackPal, gfx_bat_snake_jetpackPalLen,
                                                        nullptr, BAT_SPRITE_SIZE, ObjSize::OBJSIZE_16, SpritesheetType::BAT_JETPACK,
                                                        true,
                                                        false, LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = GameState::instance().main_oam_manager->initSprite(gfx_bat_snake_jetpackPal, gfx_bat_snake_jetpackPalLen,
                                                          nullptr, BAT_SPRITE_SIZE, ObjSize::OBJSIZE_16, SpritesheetType::BAT_JETPACK,
                                                          true,
                                                          false, LAYER_LEVEL::MIDDLE_TOP);

    match_animation();

    sprite_utils::set_horizontal_flip(true, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_visibility(true, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);

}

//!> after calling this function, call sprite_utils::update_frame to update OAM with current frameGfx
void Bat::set_sprite_hanging() {
    frameGfx = sprite_utils::get_frame((u8 *) gfx_bat_snake_jetpackTiles, BAT_SPRITE_SIZE, 0);
}

//!> after calling this function, call sprite_utils::update_frame to update OAM with current frameGfx
void Bat::set_sprite_flying_right() {
    frameGfx = sprite_utils::get_frame((u8 *) gfx_bat_snake_jetpackTiles, BAT_SPRITE_SIZE, animFrame + 1);
}

//!> after calling this function, call sprite_utils::update_frame to update OAM with current frameGfx
void Bat::set_sprite_flying_left() {
    frameGfx = sprite_utils::get_frame((u8 *) gfx_bat_snake_jetpackTiles, BAT_SPRITE_SIZE, animFrame + 4);
}

void Bat::update_sprites_position() {
    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);
    sprite_utils::set_entry_xy(mainSpriteInfo, main_x, main_y);
    sprite_utils::set_entry_xy(subSpriteInfo, sub_x, sub_y);
}

//!> naive approach to following main dude, disregarding obstacles just like in the original game
void Bat::follow_main_dude() {
    if (GameState::instance().main_dude->_x > _x)
        _x_speed = 1;
    else
        _x_speed = -1;

    if (GameState::instance().main_dude->_y == _y)
        _y_speed = 0;
    else if (GameState::instance().main_dude->_y > _y)
        _y_speed = 1;
    else
        _y_speed = -1;
}

void Bat::match_animation() {
    if (hanging)
        set_sprite_hanging();
    else if (_x_speed >= 0)
        set_sprite_flying_right();
    else if (_x_speed <= 0)
        set_sprite_flying_left();

//    printf("A");
    sprite_utils::update_frame(frameGfx, BAT_SPRITE_SIZE, mainSpriteInfo, subSpriteInfo);
}

void Bat::delete_sprites() {
    delete mainSpriteInfo;
    delete subSpriteInfo;
    mainSpriteInfo = nullptr;
    subSpriteInfo = nullptr;
}
