//
// Created by xdbeef on 27.05.18.
//

#include <maxmod9.h>
#include <cstdlib>
#include <cmath>
#include <cstdio>
#include "../../globals_declarations.h"
#include "../collectibles/shotgun.h"
#include "../collectibles/mitt.h"
#include "../collectibles/glove.h"
#include "../collectibles/compass.h"
#include "../collectibles/spring_shoes.h"
#include "../items/rope.h"
#include "../../collisions/collisions.h"
#include "../../tiles/map_utils.h"
#include "../../../build/soundbank.h"
#include "../../tiles/tile_orientation.hpp"
#include "../items/spike_shoes.h"
#include "../items/cape.h"
#include "../items/jetpack.h"
#include "../sprite_utils.h"
#include "../../../build/gfx_spelunker.h"
#include "../items/pistol.h"


void MainDude::throw_item() {

    //throw holding item
    //if holding bomb, arm it only

    for (int a = 0; a < global::sprites.size(); a++) {
        if (global::sprites.at(a)) {
            if ((*global::sprites.at(a)).hold_by_main_dude) {

                if ((*global::sprites.at(a)).activated /*&& !carrying_pistol && !carrying_shotgun*/) {

                    if (!global::input_handler->down_key_held) {

                        if (carrying_mitt) {
                            if (sprite_state == SpriteState::W_LEFT)
                                (*global::sprites.at(a)).xSpeed = -6 - abs(xSpeed);
                            else
                                (*global::sprites.at(a)).xSpeed = 6 + abs(xSpeed);

                        } else {

                            if (global::sprites.at(a)->speed_of_throwing_x != 0) {
                                if (sprite_state == SpriteState::W_LEFT)
                                    (*global::sprites.at(a)).xSpeed =
                                            -global::sprites.at(a)->speed_of_throwing_x - abs(xSpeed);
                                else
                                    (*global::sprites.at(a)).xSpeed =
                                            global::sprites.at(a)->speed_of_throwing_x + abs(xSpeed);
                            } else {
                                if (sprite_state == SpriteState::W_LEFT)
                                    (*global::sprites.at(a)).xSpeed = -4 - abs(xSpeed);
                                else
                                    (*global::sprites.at(a)).xSpeed = 4 + abs(xSpeed);
                            }


                        }

                    } else {

                        if (sprite_state == SpriteState::W_LEFT)
                            (*global::sprites.at(a)).xSpeed = -0.04;
                        else
                            (*global::sprites.at(a)).xSpeed = 0.04;

                    }

                    if (global::input_handler->up_key_held)
                        (*global::sprites.at(a)).ySpeed = -3 - abs(ySpeed);
                    else {

                        if (global::sprites.at(a)->speed_of_throwing_y != 0) {
                            (*global::sprites.at(a)).ySpeed = -global::sprites.at(a)->speed_of_throwing_y;
                        } else
                            (*global::sprites.at(a)).ySpeed = -1;


                    }

                    if (global::sprites.at(a)->sprite_width >= 8) {

                        int xx = floor_div(global::sprites.at(a)->x + 0.5 * global::sprites.at(a)->physical_width,
                                           TILE_W);
                        int yy = floor_div(global::sprites.at(a)->y + 0.5 * global::sprites.at(a)->physical_height,
                                           TILE_H);

                        MapTile *t = nullptr;
                        if (xx >= 0 && xx <= 31 && yy > 0 && yy <= 31) {
                            t = global::level_generator->map_tiles[xx][yy];
                            if (t != nullptr && !t->collidable) {
                                global::sprites.at(a)->x = xx * TILE_W;
                                global::sprites.at(a)->y = yy * TILE_H;
                                global::sprites.at(a)->updateCollisionsMap(xx, yy);
                            }
                        }

                    }

                    (*global::sprites.at(a)).hold_by_main_dude = false;
                    holding_item = false;

                    global::hud->disable_all_prompts();
                    global::hud->draw_level_hud();

                    mmEffect(SFX_XTHROW);

                } else {
                    (*global::sprites.at(a)).activated = true;
                }

            }
        }
    }
}

void MainDude::take_out_bomb() {
    global::hud->bombs--;
    global::hud->draw_level_hud();

    Bomb *bomb = new Bomb();
    bomb->init();
    bomb->hold_by_main_dude = true;

    global::sprites_to_add.push_back(bomb);
    holding_item = true;
}

void MainDude::throw_rope() {

    global::hud->ropes--;
    global::hud->draw_level_hud();

    Rope *rope = new Rope();
    rope->init();
    rope->activated = true;

    rope->y = y + 6;
    rope->x = floor_div(x + 0.5 * MAIN_DUDE_PHYSICAL_WIDTH, TILE_W) * TILE_W + ROPE_PHYSICAL_WIDTH * 0.5;
    rope->ySpeed = -4;

    global::sprites_to_add.push_back(rope);

}

void MainDude::spawn_carried_items() {

    if (carrying_spring_shoes) {
        auto *springShoes = new SpringShoes();
        springShoes->x = HUD_ITEMS_ROW_X;
        springShoes->y = global::hud->items_offset_y;
        springShoes->collected = true;
        springShoes->bought = true;
        springShoes->init();
        global::sprites_to_add.push_back(springShoes);
        global::hud->increment_offset_on_grabbed_item();
    }

    if (carrying_spike_shoes) {
        auto *spikeShoes = new SpikeShoes();
        spikeShoes->x = HUD_ITEMS_ROW_X;
        spikeShoes->y = global::hud->items_offset_y;
        spikeShoes->collected = true;
        spikeShoes->bought = true;
        spikeShoes->init();
        global::sprites_to_add.push_back(spikeShoes);
        global::hud->increment_offset_on_grabbed_item();
    }

    if (carrying_compass) {
        auto *compass = new Compass();
        compass->x = HUD_ITEMS_ROW_X;
        compass->y = global::hud->items_offset_y;
        compass->collected = true;
        compass->bought = true;
        compass->init();
        global::sprites_to_add.push_back(compass);
        global::hud->increment_offset_on_grabbed_item();
    }
    if (carrying_glove) {
        auto *glove = new Glove();
        glove->x = HUD_ITEMS_ROW_X;
        glove->y = global::hud->items_offset_y;
        glove->collected = true;
        glove->bought = true;
        glove->init();
        global::sprites_to_add.push_back(glove);
        global::hud->increment_offset_on_grabbed_item();
    }
    if (carrying_cape) {
        auto cape = new Cape();
        cape->x = HUD_ITEMS_ROW_X;
        cape->y = global::hud->items_offset_y;
        cape->collected = true;
        cape->bought = true;
        cape->init();
        global::sprites_to_add.push_back(cape);
        global::hud->increment_offset_on_grabbed_item();
    }
    if (carrying_jetpack) {
        auto *jetpack = new Jetpack();
        jetpack->x = HUD_ITEMS_ROW_X;
        jetpack->y = global::hud->items_offset_y;
        jetpack->collected = true;
        jetpack->bought = true;
        jetpack->init();
        global::sprites_to_add.push_back(jetpack);
        global::hud->increment_offset_on_grabbed_item();
    }
    if (carrying_mitt) {
        auto mitt = new Mitt();
        mitt->x = HUD_ITEMS_ROW_X;
        mitt->y = global::hud->items_offset_y;
        mitt->collected = true;
        mitt->bought = true;
        mitt->init();
        global::sprites_to_add.push_back(mitt);
        global::hud->increment_offset_on_grabbed_item();
    }
    if (carrying_shotgun) {
        holding_item = true;
        auto *shotgun = new Shotgun();
        shotgun->bought = true;
        shotgun->hold_by_main_dude = true;
        shotgun->init();
        global::sprites_to_add.push_back(shotgun);
    }

    if (carrying_pistol) {
        holding_item = true;
        auto *pistol = new Pistol();
        pistol->bought = true;
        pistol->hold_by_main_dude = true;
        pistol->init();
        global::sprites_to_add.push_back(pistol);
    }
}


void MainDude::set_sprite_crawling() {

    if (sprite_state == SpriteState::W_LEFT)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, MAIN_DUDE_SPRITE_SIZE,
                                           animFrame + 24);
    else
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, MAIN_DUDE_SPRITE_SIZE,
                                           animFrame + 33);

}

void MainDude::set_sprite_hanging_on_tile() {

    if (hanging_on_tile_right)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, MAIN_DUDE_SPRITE_SIZE, 13);
    else if (hanging_on_tile_left)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, MAIN_DUDE_SPRITE_SIZE, 12);
}

void MainDude::set_sprite_whiping() {

    if (sprite_state == SpriteState::W_LEFT)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, MAIN_DUDE_SPRITE_SIZE, 56 + animFrame);
    else if (sprite_state == 0)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, MAIN_DUDE_SPRITE_SIZE, 62 + animFrame);

}

void MainDude::set_sprite_pushing() {

    if (pushing_left)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, MAIN_DUDE_SPRITE_SIZE, 42 + animFrame);
    else
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, MAIN_DUDE_SPRITE_SIZE, 49 + animFrame);
}

void MainDude::set_sprite_stunned() {

    if (animFrame > 4)
        animFrame = 0;

    frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, MAIN_DUDE_SPRITE_SIZE, 18 + animFrame);
}

void MainDude::set_sprite_climbing() {

    if (started_climbing_rope) {

        if (animFrame >= 12)
            animFrame = 0;

        frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, MAIN_DUDE_SPRITE_SIZE, 74 + animFrame);
    } else if (started_climbing_ladder) {

        if (animFrame >= 6)
            animFrame = 0;

        frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, MAIN_DUDE_SPRITE_SIZE, 96 + animFrame);
    }

}

void MainDude::set_sprite_dead() {
    if (bottomCollision)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, MAIN_DUDE_SPRITE_SIZE, 17);
    else
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, MAIN_DUDE_SPRITE_SIZE, 16);
}

void MainDude::set_sprite_walking_when_in_air() {
    frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, MAIN_DUDE_SPRITE_SIZE, sprite_state * 6);
}

void MainDude::set_sprite_falling() {

    if (fabs(xSpeed) != 0)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, MAIN_DUDE_SPRITE_SIZE,
                                           animFrame + (sprite_state * MAIN_DUDE_FRAMES_PER_ANIMATION));
    else if (sprite_state == SpriteState::W_LEFT)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, MAIN_DUDE_SPRITE_SIZE, 14);
    else if (sprite_state == SpriteState::W_RIGHT)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, MAIN_DUDE_SPRITE_SIZE, 15);
}

void MainDude::set_sprite_exiting_level() {
    frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, MAIN_DUDE_SPRITE_SIZE, 80 + animFrame);
}

void MainDude::apply_blinking_on_damage() {

    if (!global::game_state->levels_transition_screen) {

        if (time_since_last_damage < MAIN_DUDE_DAMAGE_PROTECTION_TIME) {
            sprite_utils::set_visibility(((int) time_since_last_damage % 100) >= 50, main_spelunker, sub_spelunker);
        } else
            sprite_utils::set_visibility(true, main_spelunker, sub_spelunker);

    }
}


void MainDude::can_hang_on_tile(MapTile **neighboringTiles) {

    if (bottomCollision || (!leftCollision && !rightCollision))
        return;

    if ((neighboringTiles[UP_MIDDLE] != nullptr && neighboringTiles[UP_MIDDLE]->collidable) ||
        (neighboringTiles[DOWN_MIDDLE] != nullptr && neighboringTiles[DOWN_MIDDLE]->collidable))
        return;

    bool y_bound = false;
    bool x_bound = false;

    if (rightCollision && sprite_state == W_LEFT) {

        if (!carrying_glove && (neighboringTiles[LEFT_UP] != nullptr && neighboringTiles[LEFT_UP]->collidable))
            return;

        x_bound = (this->x <= (neighboringTiles[LEFT_MIDDLE]->x * 16) + 16 &&
                   (this->x >= (neighboringTiles[LEFT_MIDDLE]->x * 16) + 12));
        y_bound = (this->y > (neighboringTiles[LEFT_MIDDLE]->y * 16) - 2) &&
                  (this->y < (neighboringTiles[LEFT_MIDDLE]->y * 16) + 8);
    } else if (leftCollision && sprite_state == W_RIGHT) {

        if (!carrying_glove && (neighboringTiles[RIGHT_UP] != nullptr && neighboringTiles[RIGHT_UP]->collidable))
            return;

        y_bound = (this->y > (neighboringTiles[RIGHT_MIDDLE]->y * 16) - 2) &&
                  (this->y < (neighboringTiles[RIGHT_MIDDLE]->y * 16) + 8);
        x_bound = (this->x <= (neighboringTiles[RIGHT_MIDDLE]->x * 16) - 12 &&
                   (this->x >= (neighboringTiles[RIGHT_MIDDLE]->x * 16) - 16));
    }

    if ((y_bound && x_bound) && hanging_timer > MAIN_DUDE_MIN_HANGING_TIME) {

        if (rightCollision && neighboringTiles[LEFT_MIDDLE]->collidable) {
            this->y = (neighboringTiles[LEFT_MIDDLE]->y * 16);
            hanging_on_tile_right = true;
            jumping_timer = 0;
            hanging_timer = 0;
            ySpeed = 0;
        }
        if (leftCollision && neighboringTiles[RIGHT_MIDDLE]->collidable) {
            jumping_timer = 0;
            hanging_on_tile_left = true;
            this->y = (neighboringTiles[RIGHT_MIDDLE]->y * 16);
            hanging_timer = 0;
            ySpeed = 0;
        }
    }

}

void MainDude::apply_dmg(int dmg_to_apply) {
    if (dmg_to_apply == 4) {

        //fixme some enum that would indicate 'instant death, no matter for hp quantity' or a function kill_instantly
        //to differentiate

        global::main_dude->ySpeed = -MAIN_DUDE_JUMP_SPEED * 0.25;
        global::main_dude->dead = true;
        global::main_dude->climbing = false;
        global::main_dude->can_climb_rope = false;
        global::main_dude->can_climb_ladder = false;
        global::main_dude->hanging_on_tile_left = false;
        global::main_dude->hanging_on_tile_right = false;
        global::main_dude->pushing_left = false;
        global::main_dude->pushing_right = false;
        consoleClear();

        mmEffect(SFX_XDIE);
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
    } else if (!bottomCollision) {
        set_sprite_walking_when_in_air();
    } else {
        set_sprite_falling();
    }

    sprite_utils::update_frame(frameGfx, MAIN_DUDE_SPRITE_SIZE, main_spelunker, sub_spelunker);
}
