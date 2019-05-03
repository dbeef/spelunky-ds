//
// Created by xdbeef on 02.06.18.
//

#include <cstdio>
#include "../../../build/gfx_saleable.h"
#include "../../collisions/Collisions.hpp"
#include "../../GameState.hpp"
#include "../../entities/decorations/GotCollectible.hpp"


#include "SpikeShoes.hpp"
#include "../../graphics/SpriteUtils.hpp"

void SpikeShoes::update_item_specific() {

    if (_ready_to_dispose) return;

    update_anim_icon(_x, _y, _physical_width);

    if (_render_in_hud) return;

    if (_bought && check_if_can_be_equipped())
        equip();
    else if (!_bought && !_hold_by_main_dude)
        check_if_can_be_pickuped();

    if (_hold_by_main_dude) {

        if (_bought || shopping_transaction(this)) {
            _render_in_hud = true;
            equip();
        }

    }

    update_sprites_position();
}

void SpikeShoes::init_sprites() {

    delete_sprites();

    _sub_sprite_info = OAMManager::sub().initSprite(gfx_saleablePal, gfx_saleablePalLen,
                                                           nullptr, _sprite_size, ObjSize::OBJSIZE_16,
                                                           _spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
    _main_sprite_info = OAMManager::main().initSprite(gfx_saleablePal, gfx_saleablePalLen,
                                                             nullptr, _sprite_size, ObjSize::OBJSIZE_16,
                                                             _spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);

    u8 *frame_gfx = sprite_utils::get_frame((u8 *) gfx_saleableTiles, _sprite_size, 12);
    sprite_utils::update_frame(frame_gfx, _sprite_size, _main_sprite_info, _sub_sprite_info);

    update_sprites_position();
    sprite_utils::set_vertical_flip(false, _main_sprite_info, _sub_sprite_info);
    sprite_utils::set_horizontal_flip(false, _main_sprite_info, _sub_sprite_info);

    if (_render_in_hud) {
        _main_sprite_info->entry->priority = OBJPRIORITY_0;
        _sub_sprite_info->entry->isHidden = true;
        _main_sprite_info->entry->isHidden = false;
    } else
        sprite_utils::set_visibility(true, _main_sprite_info, _sub_sprite_info);

}

void SpikeShoes::equip() {

    _render_in_hud = true;

    auto *g = new GotCollectible(_x - 12, _y - 20, GotCollectible::Type::ITEM);
    GameState::instance().decorations.push_back(g);

    if (!MainDude::instance().carrying_spike_shoes) {

        MainDude::instance().carrying_spike_shoes = true;

        _x = HUD_ITEMS_ROW_X;
        _y = Hud::instance().items_offset_y;
        Hud::instance().increment_offset_on_grabbed_item();

        _sub_sprite_info->entry->isHidden = true;
        _main_sprite_info->entry->isHidden = false;
        _main_sprite_info->entry->priority = OBJPRIORITY_0;
        update_sprites_position();

    } else {
        sprite_utils::set_visibility(false, _main_sprite_info, _sub_sprite_info);
        _ready_to_dispose = true;
    }

}

void SpikeShoes::print_classname() const {
    printf("SPIKE SHOES\n");
}
