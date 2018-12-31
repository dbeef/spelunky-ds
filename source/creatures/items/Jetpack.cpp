//
// Created by xdbeef on 02.06.18.
//

#include "Jetpack.hpp"
#include "../../../build/gfx_bat_snake_jetpack.h"
#include "../../GlobalsDeclarations.hpp"
#include "../../decorations/GotCollectible.hpp"
#include "../../collisions/Collisions.hpp"
#include "../animations/FallPoof.hpp"
#include "../SpriteUtils.hpp"

void Jetpack::update_creature_specific() {

    if (global::main_dude->carrying_cape && collected) {
        global::main_dude->carrying_jetpack = false;
        _ready_to_dispose = true;
        sprite_utils::set_visibility(false, _main_sprite_info, _sub_sprite_info);
    }

    if (_ready_to_dispose)
        return;

    if (_bought && !collected && check_if_can_be_equipped()) {
        equip();
    } else if (!_bought && !hold_by_main_dude) {
        check_if_can_be_pickuped();
    }

    if (hold_by_main_dude) {
        set_pickuped_position(4, -4);
        if (shopping_transaction(this)) {
            collected = true;
            equip();
        }
    }

    if (collected) {

        if (global::main_dude->climbing || global::main_dude->exiting_level) {
            sprite_utils::set_priority(OBJPRIORITY_0, _main_sprite_info, _sub_sprite_info);
            set_pickuped_position_not_checking(-3, 2);
            sprite_utils::set_horizontal_flip(false, _main_sprite_info, _sub_sprite_info);
        } else if (global::main_dude->sprite_state == SpriteState::W_LEFT) {
            sprite_utils::set_priority(OBJPRIORITY_1, _main_sprite_info, _sub_sprite_info);
            set_pickuped_position_not_checking(-6, 0);
            sprite_utils::set_horizontal_flip(false, _main_sprite_info, _sub_sprite_info);
        } else if (global::main_dude->sprite_state == SpriteState::W_RIGHT) {
            sprite_utils::set_priority(OBJPRIORITY_1, _main_sprite_info, _sub_sprite_info);
            set_pickuped_position_not_checking(-3, 0);
            sprite_utils::set_horizontal_flip(true, _main_sprite_info, _sub_sprite_info);
        }

        if (global::main_dude->using_jetpack) {
            _poof_spawn_timer += *global::timer;

            if (global::main_dude->jetpack_fuel_counter <= 0) {
                global::main_dude->using_jetpack = false;
            }

            if (_poof_spawn_timer > 130) {

                for (int a = 0; a < 2; a++) {
                    if (_poofs[a] != nullptr) {
                        if (_poofs[a]->_ready_to_dispose) {

                            if (_poofs[a]->mainSpriteInfo == nullptr || _poofs[a]->subSpriteInfo == nullptr)
                                _poofs[a]->init_sprites();
                            else
                                sprite_utils::set_visibility(true, _poofs[a]->mainSpriteInfo, _poofs[a]->subSpriteInfo);

                            _poofs[a]->animFrame = 0;
                            _poofs[a]->animFrameTimer = 0;
                            _poofs[a]->_x = _x + 4;
                            _poofs[a]->_y = _y + 4;
                            _poofs[a]->_ready_to_dispose = false;

                            break;

                        }
                    } else {

                        auto *f = new FallPoof(_x + 4, _y + 4);
                        f->gravity = true;
                        _poofs[a] = f;
                        break;
                    }
                }

                _poof_spawn_timer = 0;

            }

        }

        _frame_gfx = sprite_utils::get_frame((u8 *) gfx_bat_snake_jetpackTiles, _sprite_size, 8);
    } else {
        _frame_gfx = sprite_utils::get_frame((u8 *) gfx_bat_snake_jetpackTiles, _sprite_size, 7);
    }

    sprite_utils::update_frame(_frame_gfx, _sprite_size, _main_sprite_info, _sub_sprite_info);
    sprite_utils::set_visibility(true, _main_sprite_info, _sub_sprite_info);
    sprite_utils::set_vertical_flip(false, _main_sprite_info, _sub_sprite_info);

    if (global::main_dude->_bottom_collision || global::main_dude->hanging_on_tile_left ||
        global::main_dude->hanging_on_tile_right) {
        global::main_dude->using_jetpack = false;
        global::main_dude->jetpack_fuel_counter = 15;
    }

    update_sprites_position();

    for (auto &_poof : _poofs) {
        if (_poof == nullptr)
            continue;
        _poof->update();
    }
}

void Jetpack::init_sprites() {

    delete_sprites();

    _sub_sprite_info = global::sub_oam_manager->initSprite(gfx_bat_snake_jetpackPal, gfx_bat_snake_jetpackPalLen,
                                                        nullptr, _sprite_size, _sprite_width,
                                                        _spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
    _main_sprite_info = global::main_oam_manager->initSprite(gfx_bat_snake_jetpackPal, gfx_bat_snake_jetpackPalLen,
                                                          nullptr, _sprite_size, _sprite_width,
                                                          _spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
    if (collected)
        _frame_gfx = sprite_utils::get_frame((u8 *) gfx_bat_snake_jetpackTiles, _sprite_size, 8);
    else
        _frame_gfx = sprite_utils::get_frame((u8 *) gfx_bat_snake_jetpackTiles, _sprite_size, 7);

    sprite_utils::update_frame(_frame_gfx, _sprite_size, _main_sprite_info, _sub_sprite_info);
}

void Jetpack::update_sprites_position() {

    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);

    sprite_utils::set_entry_xy(_main_sprite_info, static_cast<u16>(main_x), static_cast<u16>(main_y));
    sprite_utils::set_entry_xy(_sub_sprite_info, static_cast<u16>(sub_x), static_cast<u16>(sub_y));

    sprite_utils::set_vertical_flip(false, _main_sprite_info, _sub_sprite_info);
    sprite_utils::set_horizontal_flip(false, _main_sprite_info, _sub_sprite_info);

    update_anim_icon(_x, _y, _physical_width);

}

void Jetpack::equip() {

    auto *g = new GotCollectible(_x - 12, _y - 20, GotCollectible::Type::ITEM);
    global::decorations_to_add.push_back(g);

    if (global::main_dude->carrying_cape) {
        global::main_dude->carrying_cape = false;
    }

    if (!global::main_dude->carrying_jetpack) {
        global::main_dude->carrying_jetpack = true;
        update_sprites_position();
        collected = true;
    } else {
        //we've collected an item that is already in inventory, dispose
        _ready_to_dispose = true;
        sprite_utils::set_visibility(false, _main_sprite_info, _sub_sprite_info);
    }
}

void Jetpack::delete_sprites() {

    delete _main_sprite_info;
    delete _sub_sprite_info;

    _main_sprite_info = nullptr;
    _sub_sprite_info = nullptr;

    for (auto &_poof : _poofs) {
        if (_poof == nullptr) continue;
        _poof->delete_sprites();
    }
}

Jetpack::~Jetpack() {
    for (auto &poof : _poofs)
        delete poof;
}
