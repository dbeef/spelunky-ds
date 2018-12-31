//
// Created by xdbeef on 27.05.18.
//

#include <maxmod9.h>
#include <cstdlib>
#include <cmath>
#include <cstdio>
#include "../../globals_declarations.hpp"
#include "../collectibles/shotgun.hpp"
#include "../collectibles/mitt.hpp"
#include "../collectibles/glove.hpp"
#include "../collectibles/compass.hpp"
#include "../collectibles/spring_shoes.hpp"
#include "../items/rope.hpp"
#include "../../collisions/collisions.hpp"
#include "../../tiles/level_rendering_utils.hpp"
#include "../../../build/soundbank.h"
#include "../../tiles/tile_orientation.hpp"
#include "../items/spike_shoes.hpp"
#include "../items/cape.hpp"
#include "../items/jetpack.hpp"
#include "../sprite_utils.hpp"
#include "../../../build/gfx_spelunker.h"
#include "../items/pistol.hpp"
#include "main_dude.hpp"
#include "../../sound/sound_utils.hpp"
#include "../../tiles/map_tile.hpp"
#include "../../tiles/level.hpp"
#include "../../camera/camera.hpp"
#include "../sprite_info.h"
#include "../items/bomb.hpp"
#include "../../input/input_handler.hpp"

void MainDude::throw_item() {

    //throw holding item
    //if holding bomb, arm it only

    for (unsigned long a = 0; a < global::creatures.size(); a++) {
        if (global::creatures.at(a)) {
            if ((*global::creatures.at(a)).hold_by_main_dude) {

                if ((*global::creatures.at(a)).activated /*&& !carrying_pistol && !carrying_shotgun*/) {

                    if (!global::input_handler->down_key_held) {

                        if (carrying_mitt) {
                            if (sprite_state == SpriteState::W_LEFT)
                                (*global::creatures.at(a))._x_speed = -6 - abs(_x_speed);
                            else
                                (*global::creatures.at(a))._x_speed = 6 + abs(_x_speed);

                        } else {

//                            if (global::creatures.at(a)->speed_of_throwing_x != 0) {
//                                if (sprite_state == SpriteState::W_LEFT)
//                                    (*global::creatures.at(a))._x_speed =
//                                            -global::creatures.at(a)->speed_of_throwing_x - abs(_x_speed);
//                                else
//                                    (*global::creatures.at(a))._x_speed =
//                                            global::creatures.at(a)->speed_of_throwing_x + abs(_x_speed);
//                            } else {
                                if (sprite_state == SpriteState::W_LEFT)
                                    (*global::creatures.at(a))._x_speed = -4 - abs(_x_speed);
                                else
                                    (*global::creatures.at(a))._x_speed = 4 + abs(_x_speed);
//                            }


                        }

                    } else {

                        if (sprite_state == SpriteState::W_LEFT)
                            (*global::creatures.at(a))._x_speed = -0.04f;
                        else
                            (*global::creatures.at(a))._x_speed = 0.04f;

                    }


                    if(_neighboring_tiles[TileOrientation::UP_MIDDLE]->exists &&
                            _neighboring_tiles[TileOrientation::UP_MIDDLE]->collidable) {
                        (*global::creatures.at(a))._y_speed = 0.0f;
                        (*global::creatures.at(a))._x_speed *= 2;
                    }
                    else
                    {

                        if (global::input_handler->up_key_held)
                            (*global::creatures.at(a))._y_speed = -2.55 - abs(_y_speed);
                        else {

//                        if (global::creatures.at(a)->speed_of_throwing_y != 0) {
//                            (*global::creatures.at(a))._y_speed = -global::creatures.at(a)->speed_of_throwing_y;
//                        } else
                            (*global::creatures.at(a))._y_speed = -1;

                        }

                    }

//                    if (global::creatures.at(a)->_sprite_width >= 8) {
//
//                        int xx = floor_div(global::creatures.at(a)->_x + 0.5 * global::creatures.at(a)->_physical_width,
//                                           TILE_W);
//                        int yy = floor_div(global::creatures.at(a)->_y + 0.5 * global::creatures.at(a)->_physical_height,
//                                           TILE_H);
//
//                        MapTile *t = nullptr;
//                        if (xx >= 0 && xx <= 31 && yy > 0 && yy <= 31) {
//                            t = global::current_level->map_tiles[xx][yy];
//                            if (t != nullptr && !t->collidable) {
//                                global::creatures.at(a)->_x = xx * TILE_W;
//                                global::creatures.at(a)->_y = yy * TILE_H;
//                                global::creatures.at(a)->update_collisions_with_map(xx, yy);
//                            }
//                        }
//
//                    }

                    (*global::creatures.at(a)).hold_by_main_dude = false;
                    holding_item = false;

                    global::hud->disable_all_prompts();
                    global::hud->draw_level_hud();

                    mmEffect(SFX_XTHROW);

                } else {
                    (*global::creatures.at(a)).activated = true;
                }

            }
        }
    }
}

void MainDude::take_out_bomb() {
    global::hud->bombs--;
    global::hud->draw_level_hud();

    Bomb *bomb = new Bomb(_x, _y);
    bomb->hold_by_main_dude = true;

    global::creatures_to_add.push_back(bomb);
    holding_item = true;
}

void MainDude::throw_rope() {

    global::hud->ropes--;
    global::hud->draw_level_hud();

    int ROPE_PHYSICAL_WIDTH = 16;
    Rope *rope = new Rope(_y + 6, floor_div(_x + 0.5 * _physical_width, TILE_W) * TILE_W + ROPE_PHYSICAL_WIDTH * 0.5);
    rope->activated = true;
    rope->_y_speed = -4;
    global::creatures_to_add.push_back(rope);

}

void MainDude::spawn_carried_items() {

    if (carrying_spring_shoes) {
        auto *springShoes = new SpringShoes(HUD_ITEMS_ROW_X, global::hud->items_offset_y);
        springShoes->collected = true;
        springShoes->_bought = true;
        global::creatures_to_add.push_back(springShoes);
        global::hud->increment_offset_on_grabbed_item();
    }

    if (carrying_spike_shoes) {
        auto *spikeShoes = new SpikeShoes(HUD_ITEMS_ROW_X, global::hud->items_offset_y);
        spikeShoes->_collected = true;
        spikeShoes->_bought = true;
        global::creatures_to_add.push_back(spikeShoes);
        global::hud->increment_offset_on_grabbed_item();
    }

    if (carrying_compass) {
        auto *compass = new Compass(HUD_ITEMS_ROW_X, global::hud->items_offset_y);
        compass->collected = true;
        compass->_bought = true;
        global::creatures_to_add.push_back(compass);
        global::hud->increment_offset_on_grabbed_item();
    }
    if (carrying_glove) {
        auto *glove = new Glove(HUD_ITEMS_ROW_X, global::hud->items_offset_y);
        glove->collected = true;
        glove->_bought = true;
        global::creatures_to_add.push_back(glove);
        global::hud->increment_offset_on_grabbed_item();
    }
    if (carrying_cape) {
        auto cape = new Cape(HUD_ITEMS_ROW_X, global::hud->items_offset_y);
        cape->_collected = true;
        cape->_bought = true;
        global::creatures_to_add.push_back(cape);
        global::hud->increment_offset_on_grabbed_item();
    }
    if (carrying_jetpack) {
        auto *jetpack = new Jetpack(HUD_ITEMS_ROW_X, global::hud->items_offset_y);
        jetpack->collected = true;
        jetpack->_bought = true;
        global::creatures_to_add.push_back(jetpack);
        global::hud->increment_offset_on_grabbed_item();
    }
    if (carrying_mitt) {
        auto mitt = new Mitt(HUD_ITEMS_ROW_X, global::hud->items_offset_y);
        mitt->collected = true;
        mitt->_bought = true;
        global::creatures_to_add.push_back(mitt);
        global::hud->increment_offset_on_grabbed_item();
    }
    if (carrying_shotgun) {
        holding_item = true;
        auto *shotgun = new Shotgun(_x, _y);
        shotgun->_bought = true;
        shotgun->hold_by_main_dude = true;
        global::creatures_to_add.push_back(shotgun);
    }

    if (carrying_pistol) {
        holding_item = true;
        auto *pistol = new Pistol(_x, _y);
        pistol->_bought = true;
        pistol->hold_by_main_dude = true;
        global::creatures_to_add.push_back(pistol);
    }
}


void MainDude::set_sprite_crawling() {

    if (sprite_state == SpriteState::W_LEFT)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, _sprite_size,
                                           animFrame + 24);
    else
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, _sprite_size,
                                           animFrame + 33);

}

void MainDude::set_sprite_hanging_on_tile() {

    if (hanging_on_tile_right)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, _sprite_size, 13);
    else if (hanging_on_tile_left)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, _sprite_size, 12);
}

void MainDude::set_sprite_whiping() {

    if (sprite_state == SpriteState::W_LEFT)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, _sprite_size, 56 + animFrame);
    else if (sprite_state == 0)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, _sprite_size, 62 + animFrame);

}

void MainDude::set_sprite_pushing() {

    if (pushing_left)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, _sprite_size, 42 + animFrame);
    else
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, _sprite_size, 49 + animFrame);
}

void MainDude::set_sprite_stunned() {

    if (animFrame > 4)
        animFrame = 0;

    frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, _sprite_size, 18 + animFrame);
}

void MainDude::set_sprite_climbing() {

    if (started_climbing_rope) {

        if (animFrame >= 12)
            animFrame = 0;

        frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, _sprite_size, 74 + animFrame);
    } else if (started_climbing_ladder) {

        if (animFrame >= 6)
            animFrame = 0;

        frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, _sprite_size, 96 + animFrame);
    }

}

void MainDude::set_sprite_dead() {
    if (_bottom_collision)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, _sprite_size, 17);
    else
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, _sprite_size, 16);
}

void MainDude::set_sprite_walking_when_in_air() {
    frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, _sprite_size, sprite_state * 6);
}

void MainDude::set_sprite_falling() {

    if (fabs(_x_speed) != 0)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, _sprite_size,
                                           animFrame + (sprite_state * MAIN_DUDE_FRAMES_PER_ANIMATION));
    else if (sprite_state == SpriteState::W_LEFT)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, _sprite_size, 14);
    else if (sprite_state == SpriteState::W_RIGHT)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, _sprite_size, 15);
}

void MainDude::set_sprite_exiting_level() {
    frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, _sprite_size, 80 + animFrame);
}

void MainDude::apply_blinking_on_damage() {

    if (!global::game_state->levels_transition_screen) {

        if (time_since_last_damage < MAIN_DUDE_DAMAGE_PROTECTION_TIME) {
            sprite_utils::set_visibility(((int) time_since_last_damage % 100) >= 50, main_sprite_info, sub_sprite_info);
        } else
            sprite_utils::set_visibility(true, main_sprite_info, sub_sprite_info);

    }
}


void MainDude::can_hang_on_tile(MapTile **neighboringTiles) {

    if (_bottom_collision || (!_left_collision && !_right_collision))
        return;

    if ((neighboringTiles[UP_MIDDLE] != nullptr && neighboringTiles[UP_MIDDLE]->collidable) ||
        (neighboringTiles[DOWN_MIDDLE] != nullptr && neighboringTiles[DOWN_MIDDLE]->collidable))
        return;

    bool y_bound = false;
    bool x_bound = false;

    if (_right_collision && sprite_state == W_LEFT) {

        if (!carrying_glove && (neighboringTiles[LEFT_UP] != nullptr && neighboringTiles[LEFT_UP]->collidable))
            return;

        x_bound = (this->_x <= (neighboringTiles[LEFT_MIDDLE]->x * 16) + 16 &&
                   (this->_x >= (neighboringTiles[LEFT_MIDDLE]->x * 16) + 12));
        y_bound = (this->_y > (neighboringTiles[LEFT_MIDDLE]->y * 16) - 2) &&
                  (this->_y < (neighboringTiles[LEFT_MIDDLE]->y * 16) + 8);
    } else if (_left_collision && sprite_state == W_RIGHT) {

        if (!carrying_glove && (neighboringTiles[RIGHT_UP] != nullptr && neighboringTiles[RIGHT_UP]->collidable))
            return;

        y_bound = (this->_y > (neighboringTiles[RIGHT_MIDDLE]->y * 16) - 2) &&
                  (this->_y < (neighboringTiles[RIGHT_MIDDLE]->y * 16) + 8);
        x_bound = (this->_x <= (neighboringTiles[RIGHT_MIDDLE]->x * 16) - 12 &&
                   (this->_x >= (neighboringTiles[RIGHT_MIDDLE]->x * 16) - 16));
    }

    if ((y_bound && x_bound) && hanging_timer > MAIN_DUDE_MIN_HANGING_TIME) {

        if (_right_collision && neighboringTiles[LEFT_MIDDLE]->collidable) {
            this->_y = (neighboringTiles[LEFT_MIDDLE]->y * 16);
            hanging_on_tile_right = true;
            jumping_timer = 0;
            hanging_timer = 0;
            _y_speed = 0;
        }
        if (_left_collision && neighboringTiles[RIGHT_MIDDLE]->collidable) {
            jumping_timer = 0;
            hanging_on_tile_left = true;
            this->_y = (neighboringTiles[RIGHT_MIDDLE]->y * 16);
            hanging_timer = 0;
            _y_speed = 0;
        }
    }


    if (hanging_on_tile_right || hanging_on_tile_left) {
        using_cape = false;
        using_jetpack = false;
    }

    if (_upper_collision || _bottom_collision) {
        hanging_on_tile_left = false;
        hanging_on_tile_right = false;
        if (using_cape)
            jumping_timer = 0;
    }


}

void MainDude::apply_dmg(int dmg_to_apply) {
    if (dmg_to_apply == 4) {
        //fixme some enum that would indicate 'instant death, no matter for hp quantity' or a function kill_instantly
        //to differentiate (overloaded function)
        set_dead();
    }
}

void MainDude::match_animation() {

    if (exiting_level || (dead && global::input_handler->y_key_down)) {
        global::game_state->handle_changing_screens();
        set_sprite_exiting_level();
    } else if (using_whip) {
        set_sprite_whiping();
    } else if (dead) {
        set_sprite_dead();
    } else if (climbing) {
        set_sprite_climbing();
    } else if (stunned) {
        set_sprite_stunned();
    } else if (pushing_left || pushing_right) {
        set_sprite_pushing();
    } else if (hanging_on_tile_right || hanging_on_tile_left) {
        set_sprite_hanging_on_tile();
    } else if (crawling) {
        set_sprite_crawling();
    } else if (!_bottom_collision) {
        set_sprite_walking_when_in_air();
    } else {
        set_sprite_falling();
    }

    sprite_utils::update_frame(frameGfx, _sprite_size, main_sprite_info, sub_sprite_info);
}

void MainDude::update_sprites_position() {
    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);
    sprite_utils::set_entry_xy(main_sprite_info, static_cast<u16>(main_x), static_cast<u16>(main_y));
    sprite_utils::set_entry_xy(sub_sprite_info, static_cast<u16>(sub_x), static_cast<u16>(sub_y));
}

bool MainDude::can_apply_gravity() {
    return !_bottom_collision && !climbing && !(hanging_on_tile_left || hanging_on_tile_right);
}

//TODO Enum dead cause?
void MainDude::set_dead() {
    time_since_last_damage = 0;
    global::hud->hearts = 0;
    global::hud->draw_level_hud();
    dead = true;
    _bouncing_factor_y = ICollidable::default_bouncing_factor_y;
    _y_speed = -MAIN_DUDE_JUMP_SPEED * 0.25;
    climbing = false;
    can_climb_rope = false;
    can_climb_ladder = false;
    hanging_on_tile_left = false;
    hanging_on_tile_right = false;
    pushing_left = false;
    pushing_right = false;
    consoleClear();
    sound::stop_cave_music();
    mmEffect(SFX_XDIE);
}

void MainDude::delete_sprites() {
    delete main_sprite_info;
    delete sub_sprite_info;
    main_sprite_info = nullptr;
    sub_sprite_info = nullptr;
}
