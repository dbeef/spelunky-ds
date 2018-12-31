//
// Created by xdbeef on 04.04.18.
//

#include <math.h>
#include <maxmod9.h>

#include "bomb.hpp"
#include "../../globals_declarations.hpp"
#include "../../tiles/level_rendering_utils.hpp"
#include "../../collisions/collisions.hpp"
#include "../main_dude/main_dude.hpp"
#include "../../../build/gfx_explosion.h"
#include "../../../build/soundbank.h"
#include "../../tiles/level_rendering_utils.hpp"
#include "../../collisions/collisions.hpp"
#include "../animations/flame.hpp"
#include "../sprite_utils.hpp"

#define BOMB_POS_INC_DELTA 15
#define BOMB_ANIM_FRAME_DELTA 50

void Bomb::update_creature_specific() {

    if (_ready_to_dispose) return;

    if (explosionFrame == 0)
        check_if_can_be_pickuped();

    set_pickuped_position(1, 10, 6);

    if (activated && !armed) {
        set_sprite_armed();
        armed = true;
        armedTimer = 0;
    }

    if (armed) {

        armedTimer += *global::timer;

        if (armedTimer < ARMED_TIME_BLINK_SLOW) {

            if ((armedTimer) % 250 < 125)
                set_sprite_disarmed();
            else
                set_sprite_armed();

        } else if (armedTimer < ARMED_TIME_BLINK_FAST) {

            if ((armedTimer) % 120 < 60)
                set_sprite_disarmed();
            else
                set_sprite_armed();

        } else {

            explosionTimer += *global::timer;
            if (explosionTimer > BOMB_ANIM_FRAME_DELTA && explosionFrame < 10) {

                if (explosionFrame == 0)
                    explode();

                explosionTimer = 0;
                explosionFrame++;

                if (explosionFrame >= 10) {
                    _ready_to_dispose = true;
                    sprite_utils::set_visibility(false, mainSpriteInfo, subSpriteInfo);
                } else {
                    frameGfx = sprite_utils::get_frame((u8 *) gfx_explosionTiles, _sprite_size, 2 + explosionFrame);
                    sprite_utils::update_frame(frameGfx, _sprite_size, mainSpriteInfo, subSpriteInfo);
                }
            }
        }

    }

    update_sprites_position();
    kill_mobs_if_thrown(1);
}


void Bomb::set_sprite_armed() {
    frameGfx = sprite_utils::get_frame((u8 *) gfx_explosionTiles, _sprite_size, 1);
    sprite_utils::update_frame(frameGfx, _sprite_size, mainSpriteInfo, subSpriteInfo);
}

void Bomb::set_sprite_disarmed() {
    frameGfx = sprite_utils::get_frame((u8 *) gfx_explosionTiles, _sprite_size, 0);
    sprite_utils::update_frame(frameGfx, _sprite_size, mainSpriteInfo, subSpriteInfo);
}

void Bomb::init_sprites() {

    delete_sprites();

    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_explosionPal, gfx_explosionPalLen,
                                                        nullptr, _sprite_size, 64, BOMB, true, false,
                                                        LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_explosionPal, gfx_explosionPalLen,
                                                          nullptr, _sprite_size, 64, BOMB, true, false,
                                                          LAYER_LEVEL::MIDDLE_TOP);
    if (armed) {
        if (armedTimer < ARMED_TIME_BLINK_SLOW) {
            if ((armedTimer) % 250 < 125)
                set_sprite_disarmed();
            else
                set_sprite_armed();
        } else if (armedTimer < ARMED_TIME_BLINK_FAST) {
            if ((armedTimer) % 120 < 60)
                set_sprite_disarmed();
            else
                set_sprite_armed();
        }
    } else set_sprite_disarmed();

    update_sprites_position();
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_visibility(true, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_priority(OBJPRIORITY_0, mainSpriteInfo, subSpriteInfo);
}

void Bomb::explode() {

    for (int a = 0; a < 4; a++) {
        auto *flame = new Flame(_x, _y);
        if (a % 2 == 0)
            flame->_x_speed = (1.3 / a);
        else
            flame->_x_speed = (-1.3 / a);

        flame->_y_speed = (-2 / a);
        global::creatures_to_add.push_back(flame);
    }

    mmEffect(SFX_XEXPLOSION);

    int xx = floor_div(this->_x + 0.5 * _sprite_width, TILE_W);
    int yy = floor_div(this->_y + 0.5 * _sprite_height, TILE_H);

    Collisions::bombNeighboringTiles(global::current_level->map_tiles, xx, yy);
    global::game_state->bombed = true;

}

//FIXME This viewporting works buggy, it would be better to split this class into 8x8 bomb and 64x64 explosion animation
void Bomb::update_sprites_position() {

    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);

    //that's because 8x8 bomb sprite is stored in 64x64 sprite, packed with explosion animation
    if (explosionFrame > 0) {
        main_x -= 32;
        main_y -= 32;

        sub_x -= 32;
        sub_y -= 32;

    } else {
        main_x -= 29;
        main_y -= 29;

        sub_x -= 29;
        sub_y -= 29;
    }

    sprite_utils::set_entry_xy(mainSpriteInfo, main_x, main_y);
    sprite_utils::set_entry_xy(subSpriteInfo, sub_x, sub_y);

}

void Bomb::delete_sprites() {
    delete mainSpriteInfo;
    delete subSpriteInfo;
    mainSpriteInfo = nullptr;
    subSpriteInfo = nullptr;
}



