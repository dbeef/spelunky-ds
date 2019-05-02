//
// Created by xdbeef on 19.05.18.
//


#include "Mitt.hpp"
#include "Glove.hpp"

#include "../../GameState.hpp"
#include "../../collisions/Collisions.hpp"
#include "../../../build/gfx_saleable.h"
#include "../../entities/decorations/GotCollectible.hpp"
#include "../../graphics/SpriteUtils.hpp"


void Glove::update_item_specific() {

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

void Glove::init_sprites() {

    delete_sprites();

    _sub_sprite_info = GameState::instance().sub_oam_manager->initSprite(gfx_saleablePal, gfx_saleablePalLen,
                                                        nullptr, _sprite_size, ObjSize::OBJSIZE_16,
                                                        _spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
    _main_sprite_info = GameState::instance().main_oam_manager->initSprite(gfx_saleablePal, gfx_saleablePalLen,
                                                          nullptr, _sprite_size, ObjSize::OBJSIZE_16,
                                                          _spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
    match_animation();
    sprite_utils::set_vertical_flip(false, _main_sprite_info, _sub_sprite_info);
    sprite_utils::set_horizontal_flip(false, _main_sprite_info, _sub_sprite_info);
    update_sprites_position();
}

void Glove::equip() {

    _render_in_hud = true;

    auto *g = new GotCollectible(_x - 12, _y - 20, GotCollectible::Type::ITEM);
    GameState::instance().decorations.push_back(g);

    if (!GameState::instance().main_dude->carrying_glove) {
        GameState::instance().main_dude->carrying_glove = true;
        update_sprites_position();
        _x = HUD_ITEMS_ROW_X;
        _y = GameState::instance().hud->items_offset_y;
        GameState::instance().hud->increment_offset_on_grabbed_item();
    } else {
        sprite_utils::set_visibility(false, _main_sprite_info, _sub_sprite_info);
        _ready_to_dispose = true;
    }
}

void Glove::match_animation() {
    u8 *frame_gfx= sprite_utils::get_frame((u8 *) gfx_saleableTiles, _sprite_size, 1);
    sprite_utils::update_frame(frame_gfx, _sprite_size, _main_sprite_info, _sub_sprite_info);
}

