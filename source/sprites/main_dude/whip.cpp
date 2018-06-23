//
// Created by xdbeef on 07.05.18.
//

#include "whip.h"
#include "../../../build/gfx_spike_collectibles_flame.h"
#include "../../globals_declarations.h"

void Whip::draw() {

    if (global::main_dude->using_whip) {

        update_position();

        whip_timer += *global::timer;
        if (whip_timer > 420 + 0 * 70) {
            whip_timer = 0;
            global::main_dude->using_whip = false;
            hide();
        }

    } else {
        hide();
    }

}


void Whip::init() {
    initSprite();
}

void Whip::update_position() {

    x = global::main_dude->x;
    y = global::main_dude->y;

    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);

    main_sprite_info->entry->isHidden = main_x < -sprite_width || main_y < -sprite_height;
    sub_sprite_info->entry->isHidden = sub_x < -sprite_width || sub_y < -sprite_height;

    if (whip_timer > 100 && whip_timer < 180) {
        assign_pre_whip_sprite();

        if (global::main_dude->sprite_state == SpriteState::W_LEFT) {
            main_sprite_info->entry->x = main_x + 8;
            sub_sprite_info->entry->x = sub_x + 8;
            main_sprite_info->entry->hFlip = true;
            sub_sprite_info->entry->hFlip = true;
        } else {
            main_sprite_info->entry->x = main_x - 8;
            sub_sprite_info->entry->x = sub_x - 8;
            main_sprite_info->entry->hFlip = false;
            sub_sprite_info->entry->hFlip = false;
        }

    } else if (whip_timer >= 220) {
        assign_whip_sprite();

        if (global::main_dude->sprite_state == SpriteState::W_LEFT) {
            main_sprite_info->entry->x = main_x - 16;
            sub_sprite_info->entry->x = sub_x - 16;
            main_sprite_info->entry->hFlip = false;
            sub_sprite_info->entry->hFlip = false;
        } else {
            main_sprite_info->entry->x = main_x + 16;
            sub_sprite_info->entry->x = sub_x + 16;
            main_sprite_info->entry->hFlip = true;
            sub_sprite_info->entry->hFlip = true;
        }

    } else {
        hide();
    }

    main_sprite_info->entry->y = main_y - 1;
    sub_sprite_info->entry->y = sub_y - 1;

}

void Whip::initSprite() {

    //FIXME TYPE SHOULD BE SPIKE_COLLECTIBLES BUT MAKES PALETTE BUGS
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

void Whip::assign_pre_whip_sprite() {
    frameGfx = (u8 *) gfx_spike_collectibles_flameTiles + (41 * sprite_height * sprite_width / 2);
    main_sprite_info->updateFrame(frameGfx, sprite_height * sprite_width);
    sub_sprite_info->updateFrame(frameGfx, sprite_height * sprite_width);
}

void Whip::assign_whip_sprite() {
    frameGfx = (u8 *) gfx_spike_collectibles_flameTiles + (40 * sprite_width * sprite_height / 2);
    main_sprite_info->updateFrame(frameGfx, sprite_width * sprite_height);
    sub_sprite_info->updateFrame(frameGfx, sprite_width * sprite_height);
}

void Whip::hide() {
    main_sprite_info->entry->isHidden = true;
    sub_sprite_info->entry->isHidden = true;
}

Whip::Whip() {
    sprite_width = 16;
    sprite_height = 16;
    physical_width = 16;
    physical_height = 16;
    spritesheet_type = SPIKES_COLLECTIBLES;
}
