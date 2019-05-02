//
// Created by xdbeef on 18.05.18.
//



#include "SpringShoes.hpp"
#include "../../GameState.hpp"
#include "../../collisions/Collisions.hpp"
#include "../../../build/gfx_saleable.h"
#include "../../entities/decorations/GotCollectible.hpp"
#include "../../graphics/SpriteUtils.hpp"


void SpringShoes::update_item_specific() {

    if (_ready_to_dispose) return;

    sprite_utils::set_vertical_flip(false, _main_sprite_info, _sub_sprite_info);
    sprite_utils::set_horizontal_flip(false, _main_sprite_info, _sub_sprite_info);
    update_anim_icon(_x, _y, _physical_width);

    if (!collected) {

        if (_bought && check_if_can_be_equipped())
            equip();
        else if (!_bought && !_hold_by_main_dude)
            check_if_can_be_pickuped();

        if (_hold_by_main_dude) {
            if (shopping_transaction(this)) {
                collected = true;
                equip();
            }
        }
    }

    update_sprites_position();

}

void SpringShoes::init_sprites() {

    delete_sprites();

    _sub_sprite_info = GameState::instance().sub_oam_manager->initSprite(gfx_saleablePal, gfx_saleablePalLen,
                                                        nullptr, _sprite_size, ObjSize::OBJSIZE_16,
                                                        _spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
    _main_sprite_info = GameState::instance().main_oam_manager->initSprite(gfx_saleablePal, gfx_saleablePalLen,
                                                          nullptr, _sprite_size, ObjSize::OBJSIZE_16,
                                                          _spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);

    u8 *frame_gfx = sprite_utils::get_frame((u8 *) gfx_saleableTiles, _sprite_size, 3);
    sprite_utils::update_frame(frame_gfx, _sprite_size, _main_sprite_info, _sub_sprite_info);

}

void SpringShoes::equip() {

    auto g = new GotCollectible(_x - 12, _y - 20, GotCollectible::Type::ITEM);
    GameState::instance().decorations.push_back(g);

    if (!GameState::instance().main_dude->carrying_spring_shoes) {
        GameState::instance().main_dude->carrying_spring_shoes = true;
        update_sprites_position();
        collected = true;
        _x = HUD_ITEMS_ROW_X;
        _y = Hud::instance().items_offset_y;
        Hud::instance().increment_offset_on_grabbed_item();
    } else {
        sprite_utils::set_visibility(false, _main_sprite_info, _sub_sprite_info);
        _ready_to_dispose = true;
    }
}

