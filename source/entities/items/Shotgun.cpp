//
// Created by xdbeef on 17.05.18.
//


#include <cstdlib>
#include "../../../build/soundbank.h"
#include "../../../build/gfx_spike_collectibles_flame.h"
#include "../../collisions/Collisions.hpp"
#include "../../GameState.hpp"
#include "../creatures/Bullet.hpp"
#include "../../entities/decorations/GotCollectible.hpp"
#include "../../tiles/LevelRenderingUtils.hpp"

#include "Shotgun.hpp"
#include "../../time/Timer.h"
#include "../../sound/Sound.hpp"

void Shotgun::update_item_specific() {

    if (_ready_to_dispose) return;

    update_anim_icon(_x, _y, _physical_width);
    match_animation();

    if (_hold_by_main_dude) {

        if (shopping_transaction(this))
            equip();

        MainDude::instance().carrying_shotgun = true;
        _orientation = MainDude::instance().sprite_state;
        sprite_utils::set_priority(OBJPRIORITY_0, _main_sprite_info, _sub_sprite_info);

        _y = MainDude::instance()._y + 7;
        if (_orientation == Orientation::LEFT)
            _x = MainDude::instance()._x - 4;
        else
            _x = MainDude::instance()._x + 7;

    } else {
        sprite_utils::set_priority(OBJPRIORITY_1, _main_sprite_info, _sub_sprite_info);
        MainDude::instance().carrying_shotgun = false;
    }

    update_sprites_position();

    handle_shooting();
    // TODO All items kill creatures when have speed (i.e falling), automatize calling this in BaseItem's update.
    kill_creatures_if_have_speed(1);
}

void Shotgun::init_sprites() {

    delete_sprites();

    _sub_sprite_info = OAMManager::sub().initSprite(gfx_spike_collectibles_flamePal,
                                                           gfx_spike_collectibles_flamePalLen,
                                                           nullptr, _sprite_size, ObjSize::OBJSIZE_16,
                                                           _spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
    _main_sprite_info = OAMManager::main().initSprite(gfx_spike_collectibles_flamePal,
                                                             gfx_spike_collectibles_flamePalLen,
                                                             nullptr, _sprite_size, ObjSize::OBJSIZE_16,
                                                             _spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
    sprite_utils::set_vertical_flip(false, _main_sprite_info, _sub_sprite_info);
    sprite_utils::set_horizontal_flip(false, _main_sprite_info, _sub_sprite_info);
    match_animation();
    update_sprites_position();
}

void Shotgun::spawn_bullets() {

    for (int a = 0; a < 3; a++) {

        auto *b = new Bullet(_x, _y - 2);

        if (_orientation == Orientation::LEFT) {
            b->_x_speed = -4.3 - ((rand() % 20) / 10.0);
            b->_x -= 5;
        } else {
            b->_x_speed = 4.3 + ((rand() % 20) / 10.0);
            b->_x += 5 + _physical_width;
        }

        GameState::instance().creatures.push_back(b);

        if (a == 0)
            b->_y_speed = 1;
        if (a == 1)
            b->_y_speed = 0;
        if (a == 2)
            b->_y_speed = -1;
    }
}

void Shotgun::equip() {
    auto *g = new GotCollectible(_x - 12, _y - 20, GotCollectible::Type::ITEM);
    GameState::instance().decorations.push_back(g);
}

void Shotgun::match_animation() {

    u8 *_frame_gfx;

    if (_orientation == Orientation::LEFT)
        _frame_gfx = sprite_utils::get_frame((u8 *) gfx_spike_collectibles_flameTiles, _sprite_size, 12);
    else
        _frame_gfx = sprite_utils::get_frame((u8 *) gfx_spike_collectibles_flameTiles, _sprite_size, 11);

    sprite_utils::update_frame(_frame_gfx, _sprite_size, _main_sprite_info, _sub_sprite_info);
}

void Shotgun::handle_shooting() {
    if (_activated && !firing && cooldown > shotgun_cooldown) {

        sound::shotgun();
        firing = true;
        cooldown = 0;
        _activated = false;
        spawn_bullets();

    } else
        _activated = false;

    if (blast->_anim_frame_index >= 9) {
        firing = false;
        blast->_anim_frame_index = 0;
    }

    blast->_firing = firing;

    if (!firing) {
        cooldown += Timer::getDeltaTime();
        if (_orientation == Orientation::LEFT)
            blast->_x = _x - 10;
        else
            blast->_x = _x + 10;
        blast->_y = _y;
        blast->_orientation = _orientation;
    }
}

Shotgun::Shotgun(int x, int y) : BaseItem(
        x,
        y,
        shotgun_sprite_width,
        shotgun_sprite_height,
        shotgun_spritesheet_type,
        shotgun_physical_width,
        shotgun_physical_height,
        shotgun_offset_x_left,
        shotgun_offset_x_right,
        shotgun_offset_y
), ShoppingObject(shotgun_cost, shotgun_name) {

    init_anim_icon();
    update_anim_icon(x, y, _physical_width);

    blast = new Blast(0, 0);
    GameState::instance().decorations.push_back(blast);
    init_sprites();
}

void Shotgun::print_classname() const {
    printf("SHOTGUN\n");
}
