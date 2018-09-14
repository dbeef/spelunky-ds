//
// Created by xdbeef on 07.05.18.
//

#include "whip.h"
#include "../../../build/gfx_spike_collectibles_flame.h"
#include "../../globals_declarations.h"
#include "../sprite_utils.h"

void Whip::draw() {

    if (global::main_dude->using_whip) {
        update_position();

        whip_timer += *global::timer;
        if (whip_timer > 420) {
            whip_timer = 0;
            global::main_dude->using_whip = false;
            hide();
        }

    } else
        hide();

}


void Whip::init() {
    initSprite();
}

void Whip::update_position() {

    x = global::main_dude->x;
    y = global::main_dude->y - 1;

    if (whip_timer > 100 && whip_timer < 180) {

        //whiping, phase 1 - whip is behind main dude
        assign_pre_whip_sprite();
        sprite_utils::set_visibility(true, main_sprite_info, sub_sprite_info);

        if (global::main_dude->sprite_state == SpriteState::W_LEFT) {
            x += 8;
            sprite_utils::set_horizontal_flip(true, main_sprite_info, sub_sprite_info);
        } else {
            x -= 8;
            sprite_utils::set_horizontal_flip(false, main_sprite_info, sub_sprite_info);
        }

    } else if (whip_timer >= 220) {

        //whipping, phase 2 - whip is in front of main dude
        assign_whip_sprite();
        sprite_utils::set_visibility(true, main_sprite_info, sub_sprite_info);

        if (global::main_dude->sprite_state == SpriteState::W_LEFT) {
            x -= 16;
            sprite_utils::set_horizontal_flip(false, main_sprite_info, sub_sprite_info);
        } else {
            x += 16;
            sprite_utils::set_horizontal_flip(true, main_sprite_info, sub_sprite_info);
        }

    } else
        hide();

    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);
    sprite_utils::set_entry_xy(main_sprite_info, main_x, main_y);
    sprite_utils::set_entry_xy(sub_sprite_info, sub_x, sub_y);
    sprite_utils::set_vertical_flip(false, main_sprite_info, sub_sprite_info);
    sprite_utils::update_frame(frameGfx, WHIP_SPRITE_SIZE, main_sprite_info, sub_sprite_info);
}

void Whip::initSprite() {

    delete main_sprite_info;
    delete sub_sprite_info;

    main_sprite_info = global::main_oam_manager->initSprite(gfx_spike_collectibles_flamePal,
                                                            gfx_spike_collectibles_flamePalLen,
                                                            nullptr, 16 * 16, 16, WHIP,
                                                            true, false, LAYER_LEVEL::MIDDLE_TOP);

    sub_sprite_info = global::sub_oam_manager->initSprite(gfx_spike_collectibles_flamePal,
                                                          gfx_spike_collectibles_flamePalLen, nullptr,
                                                          16 * 16, 16, WHIP, true, false,
                                                          LAYER_LEVEL::MIDDLE_TOP);
    hide();
}

//!> after calling this function, call sprite_utils::update_frame to update OAM with current frameGfx
void Whip::assign_pre_whip_sprite() {
    frameGfx = sprite_utils::get_frame((u8 *) gfx_spike_collectibles_flameTiles, WHIP_SPRITE_SIZE, 41);
}

//!> after calling this function, call sprite_utils::update_frame to update OAM with current frameGfx
void Whip::assign_whip_sprite() {
    frameGfx = sprite_utils::get_frame((u8 *) gfx_spike_collectibles_flameTiles, WHIP_SPRITE_SIZE, 40);
}

void Whip::hide() {
    sprite_utils::set_visibility(false, main_sprite_info, sub_sprite_info);
}

Whip::Whip() {
    sprite_width = WHIP_SPRITE_WIDTH;
    sprite_height = WHIP_SPRITE_HEIGHT;
    physical_width = WHIP_PHYSICAL_WIDTH;
    physical_height = WHIP_PHYSICAL_HEIGHT;
    spritesheet_type = SPIKES_COLLECTIBLES;
}
