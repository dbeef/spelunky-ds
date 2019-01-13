//
// Created by xdbeef on 02.06.18.
//

#include <cstdio>

#include "SpikeShoes.hpp"
#include "../SpritesheetType.hpp"
#include "../../GlobalsDeclarations.hpp"
#include "../../collisions/Collisions.hpp"
#include "../../../build/gfx_saleable.h"
#include "../../decorations/GotCollectible.hpp"
#include "../_BaseCreature.h"
#include "../SpriteUtils.hpp"

#define SPIKE_SHOES_POS_INC_DELTA 15

void SpikeShoes::update_creature_specific() {

    if (_ready_to_dispose) return;

    if (!_collected) {

        if (_bought && check_if_can_be_equipped()) {
            equip();
        } else if (!_bought && !hold_by_main_dude) {
            check_if_can_be_pickuped();
        }

        if (hold_by_main_dude) {
            set_pickuped_position(4, -4);
            if (shopping_transaction(this)) {
                _collected = true;
                _main_sprite_info->entry->priority = OBJPRIORITY_0;
                _sub_sprite_info->entry->isHidden = true;
                _main_sprite_info->entry->isHidden = false;
                equip();
            }
        }
    }

    update_anim_icon(_x, _y, _physical_width);
    update_sprites_position();
}

void SpikeShoes::init_sprites() {

    delete_sprites();

    _sub_sprite_info = global::sub_oam_manager->initSprite(gfx_saleablePal, gfx_saleablePalLen,
                                                           nullptr, _sprite_size, ObjSize::OBJSIZE_16,
                                                           _spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
    _main_sprite_info = global::main_oam_manager->initSprite(gfx_saleablePal, gfx_saleablePalLen,
                                                             nullptr, _sprite_size, ObjSize::OBJSIZE_16,
                                                             _spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);

    u8 *frame_gfx = sprite_utils::get_frame((u8 *) gfx_saleableTiles, _sprite_size, 12);
    sprite_utils::update_frame(frame_gfx, _sprite_size, _main_sprite_info, _sub_sprite_info);

    update_sprites_position();
    sprite_utils::set_vertical_flip(false, _main_sprite_info, _sub_sprite_info);
    sprite_utils::set_horizontal_flip(false, _main_sprite_info, _sub_sprite_info);

    if (_collected) {
        _main_sprite_info->entry->priority = OBJPRIORITY_0;
        _sub_sprite_info->entry->isHidden = true;
        _main_sprite_info->entry->isHidden = false;
    } else
        sprite_utils::set_visibility(true, _main_sprite_info, _sub_sprite_info);

}

void SpikeShoes::update_sprites_position() {

    if (_collected) {
        //no viewporting, draw on HUD
        sprite_utils::set_entry_xy(_main_sprite_info, static_cast<u16>(_x), static_cast<u16>(_y));
    } else {
        int main_x, main_y, sub_x, sub_y;
        get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);
        sprite_utils::set_entry_xy(_main_sprite_info, static_cast<u16>(main_x), static_cast<u16>(main_y));
        sprite_utils::set_entry_xy(_sub_sprite_info, static_cast<u16>(sub_x), static_cast<u16>(sub_y));
    }

}

void SpikeShoes::equip() {

    auto *g = new GotCollectible(_x - 12, _y - 20, GotCollectible::Type::ITEM);
    global::decorations_to_add.push_back(g);

    if (!global::main_dude->carrying_spike_shoes) {
        global::main_dude->carrying_spike_shoes = true;
        update_sprites_position();
        _collected = true;
        _x = HUD_ITEMS_ROW_X;
        _y = global::hud->items_offset_y;
        global::hud->increment_offset_on_grabbed_item();
    } else {
        sprite_utils::set_visibility(false, _main_sprite_info, _sub_sprite_info);
        _ready_to_dispose = true;
    }

}

void SpikeShoes::delete_sprites() {
    delete _main_sprite_info;
    delete _sub_sprite_info;
    _main_sprite_info = nullptr;
    _sub_sprite_info = nullptr;
}

