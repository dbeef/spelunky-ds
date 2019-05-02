//
// Created by xdbeef on 02.06.18.
//

#include "Jetpack.hpp"
#include "../../../build/gfx_bat_snake_jetpack.h"
#include "../../GameState.hpp"
#include "../../entities/decorations/GotCollectible.hpp"
#include "../../collisions/Collisions.hpp"
#include "../animations/FallPoof.hpp"
#include "../../memory/SpriteUtils.hpp"

void Jetpack::update_item_specific() {

    if (GameState::instance().main_dude->carrying_cape && collected) {
        GameState::instance().main_dude->carrying_jetpack = false;
        _ready_to_dispose = true;
        sprite_utils::set_visibility(false, _main_sprite_info, _sub_sprite_info);
    }

    if (_ready_to_dispose)
        return;

    if (_bought && !collected && check_if_can_be_equipped()) {
        equip();
    } else if (!_bought && !_hold_by_main_dude) {
        check_if_can_be_pickuped();
    }

    if (_hold_by_main_dude) {
        if (shopping_transaction(this)) {
            collected = true;
            equip();
        }
    }

    if (collected) {

        if (GameState::instance().main_dude->climbing || GameState::instance().main_dude->exiting_level) {
            sprite_utils::set_priority(OBJPRIORITY_0, _main_sprite_info, _sub_sprite_info);
            set_pickuped_position_not_checking(-3, -3, 2);
            sprite_utils::set_horizontal_flip(false, _main_sprite_info, _sub_sprite_info);
        } else if (GameState::instance().main_dude->sprite_state == Orientation::LEFT) {
            sprite_utils::set_priority(OBJPRIORITY_1, _main_sprite_info, _sub_sprite_info);
            set_pickuped_position_not_checking(-6, -6, 0);
            sprite_utils::set_horizontal_flip(false, _main_sprite_info, _sub_sprite_info);
        } else if (GameState::instance().main_dude->sprite_state == Orientation::RIGHT) {
            sprite_utils::set_priority(OBJPRIORITY_1, _main_sprite_info, _sub_sprite_info);
            set_pickuped_position_not_checking(-3, -3, 0);
            sprite_utils::set_horizontal_flip(true, _main_sprite_info, _sub_sprite_info);
        }


        if (GameState::instance().main_dude->using_jetpack) {
            _poof_spawn_timer += *GameState::instance().timer;

            if (GameState::instance().main_dude->jetpack_fuel_counter <= 0) {
                GameState::instance().main_dude->using_jetpack = false;
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
    }

    if (GameState::instance().main_dude->_bottom_collision || GameState::instance().main_dude->hanging_on_tile_left ||
        GameState::instance().main_dude->hanging_on_tile_right) {
        GameState::instance().main_dude->using_jetpack = false;
        GameState::instance().main_dude->jetpack_fuel_counter = 15;
    }

    update_sprites_position();
    update_anim_icon(_x, _y, _physical_width);

    for (auto &_poof : _poofs) {
        if (_poof == nullptr)
            continue;
        _poof->update();
    }
}

void Jetpack::init_sprites() {

    delete_sprites();

    _sub_sprite_info = GameState::instance().sub_oam_manager->initSprite(gfx_bat_snake_jetpackPal, gfx_bat_snake_jetpackPalLen,
                                                           nullptr, _sprite_size, ObjSize::OBJSIZE_16,
                                                           _spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
    _main_sprite_info = GameState::instance().main_oam_manager->initSprite(gfx_bat_snake_jetpackPal, gfx_bat_snake_jetpackPalLen,
                                                             nullptr, _sprite_size, ObjSize::OBJSIZE_16,
                                                             _spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);

    update_anim_icon(_x, _y, _physical_width);

    sprite_utils::set_visibility(true, _main_sprite_info, _sub_sprite_info);
    sprite_utils::set_vertical_flip(false, _main_sprite_info, _sub_sprite_info);

    u8 *frame_gfx;

    if (collected)
        frame_gfx = sprite_utils::get_frame((u8 *) gfx_bat_snake_jetpackTiles, _sprite_size, 8);
    else
        frame_gfx = sprite_utils::get_frame((u8 *) gfx_bat_snake_jetpackTiles, _sprite_size, 7);

    sprite_utils::update_frame(frame_gfx, _sprite_size, _main_sprite_info, _sub_sprite_info);
}

void Jetpack::equip() {

    auto *g = new GotCollectible(_x - 12, _y - 20, GotCollectible::Type::ITEM);
    GameState::instance().decorations.push_back(g);

    if (GameState::instance().main_dude->carrying_cape) {
        GameState::instance().main_dude->carrying_cape = false;
    }

    if (!GameState::instance().main_dude->carrying_jetpack) {
        GameState::instance().main_dude->carrying_jetpack = true;
        update_sprites_position();
        collected = true;
        u8 *frame_gfx = sprite_utils::get_frame((u8 *) gfx_bat_snake_jetpackTiles, _sprite_size, 8);
        sprite_utils::update_frame(frame_gfx, _sprite_size, _main_sprite_info, _sub_sprite_info);
    } else {
        //we've collected an item that is already in inventory, dispose
        _ready_to_dispose = true;
        sprite_utils::set_visibility(false, _main_sprite_info, _sub_sprite_info);
    }
}


Jetpack::~Jetpack() {
    for (auto &poof : _poofs)
        delete poof;
}
