//
// Created by xdbeef on 23.04.18.
//

#include <maxmod9.h>
#include <cstdlib>
#include "Snake.hpp"
#include "../../GlobalsDeclarations.hpp"
#include "../items/Rock.hpp"
#include "../../../build/gfx_bat_snake_jetpack.h"
#include "../animations/Blood.hpp"
#include "../../collisions/Collisions.hpp"
#include "../../tiles/LevelRenderingUtils.hpp"
#include "../../../build/soundbank.h"
#include "../../memory/SpriteUtils.hpp"

#define SNAKE_POS_INC_DELTA 35
#define SNAKE_ANIM_FRAME_DELTA 125
#define SNAKE_HITPOINTS 1

void Snake::update_creature_specific() {

    if (_ready_to_dispose)
        return;

    update_sprites_position();
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_visibility(true, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(true, mainSpriteInfo, subSpriteInfo);

    animFrameTimer += *global::timer;

    if (animFrameTimer > SNAKE_ANIM_FRAME_DELTA) {
        //update animation

        animFrameTimer = 0;
        animFrame++;
        if (animFrame >= 4)
            animFrame = 0;

        match_animation();
    }

    if (_bottom_collision) {
        //update movement

        if (goTimer > 0) {
            goTimer -= *global::timer;

            if ((standingOnLeftEdge && spriteState == Orientation::LEFT) ||
                (standingOnRightEdge && spriteState == Orientation::RIGHT))
                _x_speed = 0; //stop if standing on an edge and facing towards it
            else if (spriteState == Orientation::RIGHT)
                _x_speed = 0.5; //if not standing on an edge and facing towards it, go where you're facing
            else
                _x_speed = -0.5;

        } else {

            _x_speed = 0;

            if (waitTimer > 0)
                waitTimer -= *global::timer;
            else if (waitTimer <= 0)
                randomizeMovement();
        }
    }


    kill_if_whip(1);
    kill_if_main_dude_jumped_on_you(1);
    deal_damage_main_dude_on_collision(1);

    if(_map_collisions_checked){
        standingOnLeftEdge = Collisions::isStandingOnLeftEdge(_neighboring_tiles, _x, _physical_width, _current_x_in_tiles);
        standingOnRightEdge = Collisions::isStandingOnRightEdge(_neighboring_tiles, _x, _physical_width, _current_y_in_tiles);
        _map_collisions_checked = false;
    }
}

//!> sets a random direction and time of travelling based on previously used directions
void Snake::randomizeMovement() {

    goTimer = (rand() % 1000) + 1000;
    waitTimer = (rand() % 500) + 500;
    auto r = static_cast<Orientation>(rand() % 2);

    if (r == Orientation::LEFT) {

        if (spriteState == Orientation::LEFT)
            sameDirectionInRow++;
        else
            sameDirectionInRow = 0;

        if (sameDirectionInRow == 2)
            spriteState = Orientation::RIGHT;
        else
            spriteState = Orientation::LEFT;

    } else if (r == Orientation::RIGHT) {

        if (spriteState == Orientation::RIGHT)
            sameDirectionInRow++;
        else
            sameDirectionInRow = 0;

        if (sameDirectionInRow == 2)
            spriteState = Orientation::LEFT;
        else
            spriteState = Orientation::RIGHT;
    }
}

//!> snake has only 1 dmg point, always kill if any dmg_apply
void Snake::apply_dmg(int dmg_to_apply) {

    //FIXME Leaving pointers like that is not a good idea! Find other classes that do that.
    //    subSpriteInfo = nullptr;
    //    mainSpriteInfo = nullptr;

    spawn_blood();
    global::killed_npcs.push_back(_creature_type);
    sprite_utils::set_visibility(false, mainSpriteInfo, subSpriteInfo);
    killed = true;
    _ready_to_dispose = true;
}

void Snake::init_sprites() {

    delete_sprites();
    
    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_bat_snake_jetpackPal, gfx_bat_snake_jetpackPalLen,
                                                        nullptr, _sprite_size, ObjSize::OBJSIZE_16, SNAKE, true, false,
                                                        LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_bat_snake_jetpackPal, gfx_bat_snake_jetpackPalLen,
                                                          nullptr, _sprite_size, ObjSize::OBJSIZE_16, SNAKE, true, false,
                                                          LAYER_LEVEL::MIDDLE_TOP);
    
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_visibility(true, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(true, mainSpriteInfo, subSpriteInfo);
    update_sprites_position();
    match_animation();

}

void Snake::update_sprites_position() {
    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);
    sprite_utils::set_entry_xy(mainSpriteInfo, main_x, main_y);
    sprite_utils::set_entry_xy(subSpriteInfo, sub_x, sub_y);
}

//!> after calling this function, call sprite_utils::update_frame to update OAM with current frameGfx
void Snake::set_sprite_left() {
    frameGfx = sprite_utils::get_frame((u8 *) gfx_bat_snake_jetpackTiles, _sprite_size, animFrame + 13);
}

//!> after calling this function, call sprite_utils::update_frame to update OAM with current frameGfx
void Snake::set_sprite_right() {
    frameGfx = sprite_utils::get_frame((u8 *) gfx_bat_snake_jetpackTiles, _sprite_size, animFrame + 9);
}

void Snake::match_animation() {
    if (spriteState == Orientation::LEFT)
        set_sprite_left();
    else if (spriteState == Orientation::RIGHT)
        set_sprite_right();

    sprite_utils::update_frame(frameGfx, _sprite_size, mainSpriteInfo, subSpriteInfo);
}

void Snake::delete_sprites() {
    delete mainSpriteInfo;
    delete subSpriteInfo;
    mainSpriteInfo = nullptr;
    subSpriteInfo = nullptr;
}

