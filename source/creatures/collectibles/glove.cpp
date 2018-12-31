//
// Created by xdbeef on 19.05.18.
//


#include "mitt.hpp"
#include "glove.hpp"
#include "../spritesheet_type.hpp"
#include "../../globals_declarations.hpp"
#include "../../collisions/collisions.hpp"
#include "../../../build/gfx_saleable.h"
#include "../../decorations/got_collectible.hpp"
#include "../sprite_utils.hpp"

void Glove::update_creature_specific() {

    if (_ready_to_dispose) return;

    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);
    update_anim_icon(_x, _y, _physical_width);

    if (collected) return;

    if (_bought && check_if_can_be_equipped())
        equip();
    else if (!_bought && !hold_by_main_dude)
        check_if_can_be_pickuped();

    if (hold_by_main_dude) {
        set_pickuped_position(4, -4);
        if (shopping_transaction(this)) {
            collected = true;
            equip();
        }
    }

    update_sprites_position();
}

void Glove::init_sprites() {

    delete_sprites();

    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_saleablePal, gfx_saleablePalLen,
                                                        nullptr, _sprite_size, _sprite_width,
                                                        _spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_saleablePal, gfx_saleablePalLen,
                                                          nullptr, _sprite_size, _sprite_width,
                                                          _spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
    match_animation();
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);
    update_sprites_position();
}

void Glove::update_sprites_position() {
    if (collected) {
        mainSpriteInfo->entry->priority = OBJPRIORITY_0;
        sprite_utils::set_entry_xy(mainSpriteInfo, _x, _y);
        subSpriteInfo->entry->isHidden = true;
        mainSpriteInfo->entry->isHidden = false;
    } else {
        int main_x, main_y, sub_x, sub_y;
        get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);
        sprite_utils::set_entry_xy(mainSpriteInfo, main_x, main_y);
        sprite_utils::set_entry_xy(subSpriteInfo, sub_x, sub_y);
    }
}

void Glove::equip() {
    collected = true;

    auto *g = new GotCollectible(_x - 12, _y - 20, GotCollectible::Type::ITEM);
    global::decorations_to_add.push_back(g);

    if (!global::main_dude->carrying_glove) {
        global::main_dude->carrying_glove = true;
        update_sprites_position();
        _x = HUD_ITEMS_ROW_X;
        _y = global::hud->items_offset_y;
        global::hud->increment_offset_on_grabbed_item();
    } else {
        sprite_utils::set_visibility(false, mainSpriteInfo, subSpriteInfo);
        _ready_to_dispose = true;
    }
}

void Glove::match_animation() {
    frameGfx = sprite_utils::get_frame((u8 *) gfx_saleableTiles, _sprite_size, 1);
    sprite_utils::update_frame(frameGfx, _sprite_size, mainSpriteInfo, subSpriteInfo);
}

void Glove::delete_sprites() {
    delete mainSpriteInfo;
    delete subSpriteInfo;
    mainSpriteInfo = nullptr;
    subSpriteInfo = nullptr;
}

