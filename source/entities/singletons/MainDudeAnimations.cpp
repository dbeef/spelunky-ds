//
// Created by dbeef on 5/4/19.
//

#include <cmath>
#include "MainDude.hpp"
#include "../../graphics/SpriteUtils.hpp"
#include "../../../build/gfx_spelunker.h"
#include "states/Climbing.hpp"

// TODO: Separate class for this, encapsulating current animation frame

void MainDude::set_sprite_crawling() {
    if (animFrame >= 9) animFrame = 0;
    if (sprite_state == Orientation::LEFT)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, _sprite_size,
                                           animFrame + 24);
    else
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, _sprite_size,
                                           animFrame + 33);
}

void MainDude::set_sprite_hanging_on_tile() {
    if (_orientation == Orientation::RIGHT)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, _sprite_size, 13);
    else if (_orientation == Orientation::LEFT)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, _sprite_size, 12);
}

void MainDude::set_sprite_whiping() {
    if (animFrame >= 5) animFrame = 0;
    if (sprite_state == Orientation::LEFT)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, _sprite_size, 56 + animFrame);
    else if (sprite_state == Orientation::RIGHT)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, _sprite_size, 62 + animFrame);
}

void MainDude::set_sprite_pushing() {
    if (animFrame >= 7) animFrame = 0;
    if (_orientation == Orientation::LEFT)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, _sprite_size, 42 + animFrame);
    else
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, _sprite_size, 49 + animFrame);
}

void MainDude::set_sprite_stunned() {
    if (animFrame > 4) animFrame = 0;
    frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, _sprite_size, 18 + animFrame);
}

void MainDude::set_sprite_climbing_ladder() {
    if (animFrame >= 6) animFrame = 0;
    frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, _sprite_size, 96 + animFrame);
}

void MainDude::set_sprite_climbing_rope() {
    if (animFrame >= 12) animFrame = 0;
    frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, _sprite_size, 74 + animFrame);
}

void MainDude::set_sprite_dead() {
    if (_bottom_collision)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, _sprite_size, 17);
    else
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, _sprite_size, 16);
}

void MainDude::set_sprite_walking() {
    if (animFrame >= 7) animFrame = 0;
    frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, _sprite_size, static_cast<u16>(sprite_state) * 6);
}

void MainDude::set_sprite_falling() {
    if (fabs(_x_speed) != 0)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, _sprite_size,
                                           animFrame + (static_cast<u16>(sprite_state) * 7));

    else if (sprite_state == Orientation::LEFT)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, _sprite_size, 14);
    else if (sprite_state == Orientation::RIGHT)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, _sprite_size, 15);
}

void MainDude::set_sprite_exiting_level() {
    if (animFrame >= 16) animFrame = 0;
    frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, _sprite_size, 80 + animFrame);
}

void MainDude::match_animation() {
    switch (_current_state->_state) {
        case _MainDudeState::WALKING: {
            if (using_whip)
                set_sprite_whiping();
            else
                set_sprite_walking();
            break;
        }
        case _MainDudeState::CLIMBING: {
            if (using_whip)
                set_sprite_whiping();
            else {
                switch(_climbing->_type){
                    case Climbing::LADDER:{
                        set_sprite_climbing_ladder();
                        break;
                    }
                    case Climbing::ROPE:{
                        set_sprite_climbing_rope();
                        break;
                    }
                }
            }
            break;
        }
        case _MainDudeState::HANGING_ON_EDGE: {
            if (using_whip)
                set_sprite_whiping();
            else
                set_sprite_hanging_on_tile();
            break;
        }
        case _MainDudeState::STUNNED: {
            set_sprite_stunned();
            break;
        }
        case _MainDudeState::DEAD: {
            set_sprite_dead();
            break;
        }
        case _MainDudeState::EXITING_LEVEL: {
            set_sprite_exiting_level();
            break;
        }
    }
}
