//
// Created by xdbeef on 27.05.18.
//

#include <maxmod9.h>
#include <cstdlib>
#include "../../globals_declarations.h"
#include "../collectibles/pistol.h"
#include "../collectibles/shotgun.h"
#include "../collectibles/mitt.h"
#include "../collectibles/glove.h"
#include "../collectibles/compass.h"
#include "../collectibles/spring_shoes.h"
#include "../items/rope.h"
#include "../../collisions/collisions.h"
#include "../../tiles/map_utils.h"
#include "../../../build/soundbank.h"
#include "../../tiles/tile_orientation.h"
#include "../items/spike_shoes.h"
#include "../items/cape.h"
#include "../items/jetpack.h"

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
                            if (sprite_state == SpriteState::W_LEFT)
                                (*global::sprites.at(a)).xSpeed = -4 - abs(xSpeed);
                            else
                                (*global::sprites.at(a)).xSpeed = 4 + abs(xSpeed);
                        }

                    } else {

                        if (sprite_state == SpriteState::W_LEFT)
                            (*global::sprites.at(a)).xSpeed = -0.04;
                        else
                            (*global::sprites.at(a)).xSpeed = 0.04;

                    }

                    if (global::input_handler->up_key_held)
                        (*global::sprites.at(a)).ySpeed = -3 - abs(ySpeed);
                    else
                        (*global::sprites.at(a)).ySpeed = -1;


                    if (global::sprites.at(a)->sprite_width >= 8) {

                        int xx = floor_div(global::sprites.at(a)->x + 0.5 * global::sprites.at(a)->physical_width, TILE_W);
                        int yy = floor_div(global::sprites.at(a)->y + 0.5 * global::sprites.at(a)->physical_height, TILE_H);

                        global::sprites.at(a)->x = xx * TILE_W;
                        global::sprites.at(a)->y = yy * TILE_H;
                    }

                    (*global::sprites.at(a)).hold_by_main_dude = false;
                    holding_item = false;

                    global::hud->disable_all_prompts();
                    global::hud->draw();

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
    global::hud->draw();

    Bomb *bomb = new Bomb();
    bomb->init();
    bomb->hold_by_main_dude = true;

    global::sprites.push_back(bomb);
    holding_item = true;
}

void MainDude::throw_rope() {

    global::hud->ropes--;
    global::hud->draw();

    Rope *rope = new Rope();
    rope->init();
    rope->activated = true;

    rope->y = y + 6;
    rope->x = floor_div(x + 0.5 * MAIN_DUDE_PHYSICAL_WIDTH, TILE_W) * TILE_W + ROPE_PHYSICAL_WIDTH * 0.5;
    rope->ySpeed = -4;

    global::sprites.push_back(rope);

}

void MainDude::spawn_carried_items() {

    if (carrying_spring_shoes) {
        SpringShoes *springShoes = new SpringShoes();
        springShoes->x = HUD_ITEMS_ROW_X;
        springShoes->y = global::hud->items_offset_y;
        springShoes->collected = true;
        springShoes->bought = true;
        springShoes->init();
        global::sprites.push_back(springShoes);
        global::hud->next_item();
    }

    if (carrying_spike_shoes) {
        SpikeShoes *spikeShoes= new SpikeShoes();
        spikeShoes->x = HUD_ITEMS_ROW_X;
        spikeShoes->y = global::hud->items_offset_y;
        spikeShoes->collected = true;
        spikeShoes->bought= true;
        spikeShoes->init();
        global::sprites.push_back(spikeShoes);
        global::hud->next_item();
    }

    if (carrying_compass) {
        Compass *compass = new Compass();
        compass->x = HUD_ITEMS_ROW_X;
        compass->y = global::hud->items_offset_y;
        compass->collected = true;
        compass->bought = true;
        compass->init();
        global::sprites.push_back(compass);
        global::hud->next_item();
    }
    if (carrying_glove) {
        Glove *glove = new Glove();
        glove->x = HUD_ITEMS_ROW_X;
        glove->y = global::hud->items_offset_y;
        glove->collected = true;
        glove->bought = true;
        glove->init();
        global::sprites.push_back(glove);
        global::hud->next_item();
    }
    if (carrying_cape) {
        Cape *cape = new Cape();
        cape->x = HUD_ITEMS_ROW_X;
        cape->y = global::hud->items_offset_y;
        cape->collected = true;
        cape->bought= true;
        cape->init();
        global::sprites.push_back(cape);
        global::hud->next_item();
    }
    if (carrying_jetpack) {
        Jetpack *jetpack = new Jetpack();
        jetpack->x = HUD_ITEMS_ROW_X;
        jetpack->y = global::hud->items_offset_y;
        jetpack->collected = true;
        jetpack->bought = true;
        jetpack->init();
        global::sprites.push_back(jetpack);
        global::hud->next_item();
    }
    if (carrying_mitt) {
        Mitt *mitt = new Mitt();
        mitt->x = HUD_ITEMS_ROW_X;
        mitt->y = global::hud->items_offset_y;
        mitt->collected = true;
        mitt->bought = true;
        mitt->init();
        global::sprites.push_back(mitt);
        global::hud->next_item();
    }
    if (carrying_shotgun) {
        Shotgun *shotgun = new Shotgun();
        shotgun->hold_by_main_dude = true;
        shotgun->init();
        shotgun->bought = true;
        global::sprites.push_back(shotgun);
        holding_item = true;
    }
    if (carrying_pistol) {
        Pistol *pistol = new Pistol();
        pistol->bought = true;
        pistol->hold_by_main_dude = true;
        pistol->init();
        global::sprites.push_back(pistol);
        holding_item = true;
    }
}


void MainDude::apply_crawling_sprite() {

    int frame;
    u8 *offset;

    if (sprite_state == SpriteState::W_LEFT)
        frame = animFrame + (4) * SPRITESHEET_ROW_WIDTH;
    else
        frame = animFrame + (5) * SPRITESHEET_ROW_WIDTH + 3;

    offset = frameGfx + frame * MAIN_DUDE_SPRITE_WIDTH * MAIN_DUDE_SPRITE_HEIGHT / 2;
    main_spelunker->updateFrame(offset, sprite_width * sprite_height);
    sub_spelunker->updateFrame(offset, sprite_width * sprite_height);
}

void MainDude::apply_hanging_on_tile_sprite() {

    int frame;
    u8 *offset;

    if (hanging_on_tile_right) {
        frame = (2 * SPRITESHEET_ROW_WIDTH) + 1;
        offset = frameGfx + frame * MAIN_DUDE_SPRITE_WIDTH * MAIN_DUDE_SPRITE_HEIGHT / 2;
    } else if (hanging_on_tile_left) {
        frame = (2 * SPRITESHEET_ROW_WIDTH);
        offset = frameGfx + frame * MAIN_DUDE_SPRITE_WIDTH * MAIN_DUDE_SPRITE_HEIGHT / 2;
    }

    main_spelunker->updateFrame(offset, sprite_width * sprite_height);
    sub_spelunker->updateFrame(offset, sprite_width * sprite_height);
}

void MainDude::apply_whip_sprite() {

    int frame;
    u8 *offset;

    if (sprite_state == SpriteState::W_LEFT) {
        frame = (9 * SPRITESHEET_ROW_WIDTH) + 2 + animFrame;
        offset = frameGfx + frame * MAIN_DUDE_SPRITE_WIDTH * MAIN_DUDE_SPRITE_HEIGHT / 2;
    } else if (sprite_state == 0) {
        frame = (10 * SPRITESHEET_ROW_WIDTH) + 2 + animFrame;
        offset = frameGfx + frame * MAIN_DUDE_SPRITE_WIDTH * MAIN_DUDE_SPRITE_HEIGHT / 2;
    }

    main_spelunker->updateFrame(offset, sprite_width * sprite_height);
    sub_spelunker->updateFrame(offset, sprite_width * sprite_height);

}

void MainDude::apply_pushing_sprite() {

    int frame;
    u8 *offset;

    if (pushing_left)
        frame = animFrame + (7) * SPRITESHEET_ROW_WIDTH;
    else
        frame = animFrame + (8) * SPRITESHEET_ROW_WIDTH + 1;


    offset = frameGfx + frame * MAIN_DUDE_SPRITE_WIDTH * MAIN_DUDE_SPRITE_HEIGHT / 2;
    main_spelunker->updateFrame(offset, sprite_width * sprite_height);
    sub_spelunker->updateFrame(offset, sprite_width * sprite_height);

}

void MainDude::apply_stunned_sprite() {

    int frame;
    u8 *offset;

    if (animFrame > 4)
        animFrame = 0;

    frame = (3 * SPRITESHEET_ROW_WIDTH) + animFrame;
    offset = frameGfx + frame * MAIN_DUDE_SPRITE_WIDTH * MAIN_DUDE_SPRITE_HEIGHT / 2;
    main_spelunker->updateFrame(offset, sprite_width * sprite_height);
    sub_spelunker->updateFrame(offset, sprite_width * sprite_height);
}

void MainDude::apply_climbing_sprite() {

    int frame;
    u8 *offset;

    if (started_climbing_rope) {
        if (animFrame >= 12)
            animFrame = 0;

        frame = ((12) * SPRITESHEET_ROW_WIDTH) + animFrame + 2;
        offset = frameGfx + frame * MAIN_DUDE_SPRITE_WIDTH * MAIN_DUDE_SPRITE_HEIGHT / 2;
        main_spelunker->updateFrame(offset, sprite_width * sprite_height);
        sub_spelunker->updateFrame(offset, sprite_width * sprite_height);

    } else if (started_climbing_ladder) {

        if (animFrame >= 6)
            animFrame = 0;

        frame = ((16) * SPRITESHEET_ROW_WIDTH) + animFrame;
        offset = frameGfx + frame * MAIN_DUDE_SPRITE_WIDTH * MAIN_DUDE_SPRITE_HEIGHT / 2;
        main_spelunker->updateFrame(offset, sprite_width * sprite_height);
        sub_spelunker->updateFrame(offset, sprite_width * sprite_height);
    }


}

void MainDude::apply_dead_sprite() {

    int frame;
    u8 *offset;

    if (bottomCollision) {
        frame = ((2) * SPRITESHEET_ROW_WIDTH) + 5;
    } else
        frame = ((2) * SPRITESHEET_ROW_WIDTH) + 4;

    offset = frameGfx + frame * MAIN_DUDE_SPRITE_WIDTH * MAIN_DUDE_SPRITE_HEIGHT / 2;
    main_spelunker->updateFrame(offset, sprite_width * sprite_height);
    sub_spelunker->updateFrame(offset, sprite_width * sprite_height);
}

void MainDude::apply_walking_sprite() {

    int frame;
    u8 *offset;

    if (sprite_state == SpriteState::W_LEFT) {
        frame = sprite_state * FRAMES_PER_ANIMATION;
        offset = frameGfx + frame * MAIN_DUDE_SPRITE_WIDTH * MAIN_DUDE_SPRITE_HEIGHT / 2;
    } else if (sprite_state == SpriteState::W_RIGHT) {
        frame = sprite_state * FRAMES_PER_ANIMATION;
        offset = frameGfx + frame * MAIN_DUDE_SPRITE_WIDTH * MAIN_DUDE_SPRITE_HEIGHT / 2;
    }
    main_spelunker->updateFrame(offset, sprite_width * sprite_height);
    sub_spelunker->updateFrame(offset, sprite_width * sprite_height);

}

void MainDude::apply_falling_sprite() {

    int frame;
    u8 *offset;

    if (abs(xSpeed) != 0) {
        frame = animFrame + sprite_state * FRAMES_PER_ANIMATION;
        offset = frameGfx + frame * MAIN_DUDE_SPRITE_WIDTH * MAIN_DUDE_SPRITE_HEIGHT / 2;
    } else {
        if (sprite_state== SpriteState::W_LEFT) {
            frame = (2 * SPRITESHEET_ROW_WIDTH) + 2;
            offset = frameGfx + frame * MAIN_DUDE_SPRITE_WIDTH * MAIN_DUDE_SPRITE_HEIGHT / 2;
        } else if (sprite_state == SpriteState::W_RIGHT) {
            frame = (2 * SPRITESHEET_ROW_WIDTH) + 3;
            offset = frameGfx + frame * MAIN_DUDE_SPRITE_WIDTH * MAIN_DUDE_SPRITE_HEIGHT / 2;
        }

    }
    main_spelunker->updateFrame(offset, sprite_width * sprite_height);
    sub_spelunker->updateFrame(offset, sprite_width * sprite_height);
}

void MainDude::apply_exiting_level_sprite() {

    int frame;
    u8 *offset;

    frame = ((13) * SPRITESHEET_ROW_WIDTH) + animFrame + 2;
    offset = frameGfx + frame * MAIN_DUDE_SPRITE_WIDTH * MAIN_DUDE_SPRITE_HEIGHT / 2;
    main_spelunker->updateFrame(offset, sprite_width * sprite_height);
    sub_spelunker->updateFrame(offset, sprite_width * sprite_height);

}

void MainDude::apply_blinking_on_damage() {

    if (!global::game_state->levels_transition_screen) {
        if (time_since_last_damage < DAMAGE_PROTECTION_TIME) {
            if (time_since_last_damage % 100 < 50) {
                main_spelunker->entry->isHidden = true;
                sub_spelunker->entry->isHidden = true;
            } else {
                main_spelunker->entry->isHidden = false;
                sub_spelunker->entry->isHidden = false;
            }
        } else {
            main_spelunker->entry->isHidden = false;
            sub_spelunker->entry->isHidden = false;
        }
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

    if ((y_bound && x_bound) && hanging_timer > MIN_HANGING_TIME) {

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
    if(dmg_to_apply == 4){

        //fixme some enum that would indicate 'instant death, no matter for hp quantity' or a function kill_instantly
        //to differentiate

        global::hud->hide();
        global::main_dude->ySpeed = -MAIN_DUDE_JUMP_SPEED * 0.25;
        global::main_dude->dead = true;
        global::main_dude->climbing = false;
        global::main_dude->can_climb_rope= false;
        global::main_dude->can_climb_ladder= false;
        global::main_dude->hanging_on_tile_left= false;
        global::main_dude->hanging_on_tile_right= false;
        global::main_dude->pushing_left= false;
        global::main_dude->pushing_right= false;

        mmEffect(SFX_XDIE);
    }
}
