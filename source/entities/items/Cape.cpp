//
// Created by xdbeef on 02.06.18.
//


#include "Cape.hpp"
#include "../../../build/gfx_goldbars.h"
#include "../../GameState.hpp"
#include "../../entities/decorations/GotCollectible.hpp"
#include "../../collisions/Collisions.hpp"

#include "../../time/Timer.h"
#include "../../graphics/SpriteUtils.hpp"

void Cape::update_item_specific() {

    if (MainDude::instance().carrying_jetpack && _collected) {
        MainDude::instance().carrying_cape = false;
        sprite_utils::set_visibility(false, _main_sprite_info, _sub_sprite_info);
        _ready_to_dispose = true;
    }

    if (_ready_to_dispose) return;

    sprite_utils::set_vertical_flip(false, _main_sprite_info, _sub_sprite_info);
    sprite_utils::set_horizontal_flip(false, _main_sprite_info, _sub_sprite_info);
    update_anim_icon(_x, _y, _physical_width);
    sprite_utils::set_visibility(true, _main_sprite_info, _sub_sprite_info);

    if (_bought && !_collected && check_if_can_be_equipped())
        equip();
    else if (!_bought && !_hold_by_main_dude)
        check_if_can_be_pickuped();

    if (_hold_by_main_dude) {
        if (shopping_transaction(this)) {
            _collected = true;
            equip();
        }
    }

    update_sprites_position();

    if (_collected) {

        //match sprite rendering priority and x/y offset to the main dude's sprite state
        if (MainDude::instance().climbing || MainDude::instance().exiting_level) {
            set_pickuped_position_not_checking(0, 0, 4);
            sprite_utils::set_priority(OBJPRIORITY_0, _main_sprite_info, _sub_sprite_info);
        } else if (InputHandler::instance().keys.down_key_held || MainDude::instance().dead || MainDude::instance().stunned) {
            set_pickuped_position_not_checking(0, 0, 4);
            sprite_utils::set_priority(OBJPRIORITY_1, _main_sprite_info, _sub_sprite_info);
        } else {
            sprite_utils::set_priority(OBJPRIORITY_1, _main_sprite_info, _sub_sprite_info);
            set_pickuped_position_not_checking(-3, -3, -1);
        }

        _anim_frame_timer += Timer::getDeltaTime();

        if (_anim_frame_timer > cape_anim_frame_delta) {

            if (MainDude::instance()._x_speed != 0 || MainDude::instance()._y_speed != 0)
                _anim_frame_index++;

            _anim_frame_timer = 0;
            match_animation();
        }

        sprite_utils::set_horizontal_flip(MainDude::instance().sprite_state == Orientation::RIGHT,
                                          _main_sprite_info, _sub_sprite_info);

        if (MainDude::instance()._bottom_collision)
            MainDude::instance().using_cape = false;

    }

}

void Cape::init_sprites() {

    delete_sprites();
    
    _sub_sprite_info = OAMManager::sub().initSprite(gfx_goldbarsPal, gfx_goldbarsPalLen,
                                                        nullptr, _sprite_size, ObjSize::OBJSIZE_16,
                                                        _spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
    _main_sprite_info = OAMManager::main().initSprite(gfx_goldbarsPal, gfx_goldbarsPalLen,
                                                          nullptr, _sprite_size, ObjSize::OBJSIZE_16,
                                                          _spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
    update_sprites_position();
    match_animation();
    sprite_utils::set_vertical_flip(false, _main_sprite_info, _sub_sprite_info);

    if (_collected)
        sprite_utils::set_horizontal_flip(MainDude::instance().sprite_state == Orientation::RIGHT,
                                          _main_sprite_info, _sub_sprite_info);
    else
        sprite_utils::set_horizontal_flip(false, _main_sprite_info, _sub_sprite_info);
}

void Cape::match_animation() {

    u8 *frame_gfx;
    
    if (!_collected) {
        frame_gfx = sprite_utils::get_frame((u8 *) gfx_goldbarsTiles, _sprite_size, 2);
    } else if (MainDude::instance().climbing) {
        //climbing
        frame_gfx = sprite_utils::get_frame((u8 *) gfx_goldbarsTiles, _sprite_size, 3);
    } else if (InputHandler::instance().keys.down_key_held || MainDude::instance().dead || MainDude::instance().stunned ||
               (!MainDude::instance().using_cape && !MainDude::instance().stunned &&
                (MainDude::instance()._x_speed != 0 || MainDude::instance()._y_speed != 0))) {

        if (_anim_frame_index >= 5)
            _anim_frame_index = 0;

        frame_gfx = sprite_utils::get_frame((u8 *) gfx_goldbarsTiles, _sprite_size, 5 + _anim_frame_index);

    } else if (MainDude::instance()._x_speed == 0 && MainDude::instance()._y_speed == 0) {
        //staying still
        frame_gfx = sprite_utils::get_frame((u8 *) gfx_goldbarsTiles, _sprite_size, 4);
    } else {

        //falling
        if (_anim_frame_index >= 2)
            _anim_frame_index = 0;

        frame_gfx = sprite_utils::get_frame((u8 *) gfx_goldbarsTiles, _sprite_size, 10 + _anim_frame_index);
    }

    sprite_utils::update_frame(frame_gfx, _sprite_size, _main_sprite_info, _sub_sprite_info);

}

void Cape::equip() {

    auto g = new GotCollectible(_x - 12, _y - 20, GotCollectible::Type::ITEM);
    GameState::instance().decorations.push_back(g);

    if (MainDude::instance().carrying_jetpack)
        MainDude::instance().carrying_jetpack = false;

    if (!MainDude::instance().carrying_cape) {
        MainDude::instance().carrying_cape = true;
        update_sprites_position();
        _collected = true;
    } else {
        //we've collected an item that is already in inventory, dispose
        _ready_to_dispose = true;
        sprite_utils::set_visibility(false, _main_sprite_info, _sub_sprite_info);
    }

}

void Cape::print_classname() const {
    printf("CAPE\n");
}
