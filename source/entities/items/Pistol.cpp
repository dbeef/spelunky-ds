//
// Created by xdbeef on 18.05.18.
//


#include <maxmod9.h>
#include "Pistol.hpp"
#include "../../GameState.hpp"
#include "../../collisions/Collisions.hpp"
#include "../../../build/gfx_spike_collectibles_flame.h"
#include "../../../build/soundbank.h"
#include "../../entities/decorations/GotCollectible.hpp"
#include "../../tiles/LevelRenderingUtils.hpp"
#include "../../memory/SpriteUtils.hpp"
#include "../../entities/decorations/Blast.hpp"
#include "../creatures/Bullet.hpp"
#include "../../time/Timer.h"

void Pistol::update_item_specific() {

    if (_ready_to_dispose) return;

    update_anim_icon(_x, _y, _physical_width);

    if (_hold_by_main_dude) {

        if (shopping_transaction(this))
            equip();

        GameState::instance().main_dude->carrying_pistol = true;
        _orientation = GameState::instance().main_dude->sprite_state;
        match_animation();

    } else
        GameState::instance().main_dude->carrying_pistol = false;

    update_sprites_position(); //must be called here, after position is offsetted if pistol carried
    handle_shooting();
    kill_creatures_if_have_speed(1);
}

void Pistol::init_sprites() {

    delete_sprites();
    
    _main_sprite_info = GameState::instance().main_oam_manager->initSprite(gfx_spike_collectibles_flamePal,
                                                          gfx_spike_collectibles_flamePalLen,
                                                          nullptr, _sprite_size, ObjSize::OBJSIZE_16,
                                                          _spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
    _sub_sprite_info = GameState::instance().sub_oam_manager->initSprite(gfx_spike_collectibles_flamePal,
                                                        gfx_spike_collectibles_flamePalLen,
                                                        nullptr, _sprite_size, ObjSize::OBJSIZE_16,
                                                        _spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);

    sprite_utils::set_vertical_flip(false, _main_sprite_info, _sub_sprite_info);
    sprite_utils::set_horizontal_flip(false, _main_sprite_info, _sub_sprite_info);
    sprite_utils::set_visibility(true, _main_sprite_info, _sub_sprite_info);

    match_animation();
    update_sprites_position();
}

void Pistol::equip() {
    auto *g = new GotCollectible(_x - 12, _y - 20, GotCollectible::Type::ITEM);
    GameState::instance().decorations.push_back(g);
}

void Pistol::spawn_bullet() {

    auto *b = new Bullet(_x, _y);

    if (GameState::instance().main_dude->sprite_state == Orientation::LEFT) {
        b->_x_speed = -5.0;
        b->_x -= 2;
    } else if (GameState::instance().main_dude->sprite_state == Orientation::RIGHT) {
        b->_x_speed = 5.0;
        b->_x += 2;
    }

    GameState::instance().creatures.push_back(b);
    b->_y_speed = 0;
}

//TODO Share this function between pistol / shotgun, similiar behaviour besides cooldown time / spawn bullet callback
void Pistol::handle_shooting() {

    if (_activated && !_firing && _cooldown > pistol_cooldown) {

        mmEffect(SFX_XSHOTGUN);
        _firing = true;
        _cooldown = 0;
        _activated = false;
        spawn_bullet();

    } else
        _activated = false;

    if (_blast->_anim_frame_index >= 9) {
        _firing = false;
        _blast->_anim_frame_index = 0;
    }

    _blast->_firing = _firing;

    if (!_firing) {
        _cooldown += Timer::getDeltaTime();
        if (_orientation == Orientation::LEFT)
            _blast->_x = _x - 10;
        else
            _blast->_x = _x + 10;
        _blast->_y = _y;
        _blast->_orientation = _orientation;
    }

}

void Pistol::match_animation() {

    u8 *frame_gfx;

    if (_orientation == Orientation::LEFT)
        frame_gfx = sprite_utils::get_frame((u8 *) gfx_spike_collectibles_flameTiles, _sprite_size, 32);
    else
        frame_gfx = sprite_utils::get_frame((u8 *) gfx_spike_collectibles_flameTiles, _sprite_size, 33);

    sprite_utils::update_frame(frame_gfx, _sprite_size, _main_sprite_info, _sub_sprite_info);
}

