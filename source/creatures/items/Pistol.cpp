//
// Created by xdbeef on 18.05.18.
//


#include <maxmod9.h>
#include "Pistol.hpp"
#include "../../GlobalsDeclarations.hpp"
#include "../../collisions/Collisions.hpp"
#include "../../../build/gfx_spike_collectibles_flame.h"
#include "../../../build/soundbank.h"
#include "../../decorations/GotCollectible.hpp"
#include "../../tiles/LevelRenderingUtils.hpp"
#include "../SpriteUtils.hpp"
#include "../../decorations/Blast.hpp"
#include "../collectibles/Bullet.hpp"

#define PISTOL_POS_INC_DELTA 15
#define PISTOL_COOLDOWN 750

void Pistol::update_creature_specific() {

    if (_ready_to_dispose) return;

    sprite_utils::set_vertical_flip(false, _main_sprite_info, _sub_sprite_info);
    sprite_utils::set_horizontal_flip(false, _main_sprite_info, _sub_sprite_info);
    update_anim_icon(_x, _y, _physical_width);
    match_animation();

    check_if_can_be_pickuped();
    if (hold_by_main_dude) {

        if (shopping_transaction(this))
            equip();

        global::main_dude->carrying_pistol = true;
        sprite_state = global::main_dude->sprite_state;

        _y = global::main_dude->_y + 7;
        if (global::main_dude->sprite_state == SpriteState::W_LEFT)
            _x = global::main_dude->_x - 4;
        else
            _x = global::main_dude->_x + 10;

    } else
        global::main_dude->carrying_pistol = false;

    update_sprites_position(); //must be called here, after position is offsetted if pistol carried
    handle_shooting();
    kill_mobs_if_thrown(1);
}

void Pistol::init_sprites() {

    delete_sprites();
    
    _main_sprite_info = global::main_oam_manager->initSprite(gfx_spike_collectibles_flamePal,
                                                          gfx_spike_collectibles_flamePalLen,
                                                          nullptr, _sprite_size, _sprite_width,
                                                          _spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
    _sub_sprite_info = global::sub_oam_manager->initSprite(gfx_spike_collectibles_flamePal,
                                                        gfx_spike_collectibles_flamePalLen,
                                                        nullptr, _sprite_size, _sprite_width,
                                                        _spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);

    sprite_utils::set_vertical_flip(false, _main_sprite_info, _sub_sprite_info);
    sprite_utils::set_horizontal_flip(false, _main_sprite_info, _sub_sprite_info);
    match_animation();
    update_sprites_position();
}

void Pistol::update_sprites_position() {
    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);
    sprite_utils::set_entry_xy(_main_sprite_info, static_cast<u16>(main_x), static_cast<u16>(main_y));
    sprite_utils::set_entry_xy(_sub_sprite_info, static_cast<u16>(sub_x), static_cast<u16>(sub_y));
}

void Pistol::equip() {
    auto *g = new GotCollectible(_x - 12, _y - 20, GotCollectible::Type::ITEM);
    global::decorations_to_add.push_back(g);
}

void Pistol::spawn_bullet() {

    auto *b = new Bullet(_x, _y);

    if (global::main_dude->sprite_state == SpriteState::W_LEFT) {
        b->_x_speed = -5.0;
        b->_x -= 2;
    } else if (global::main_dude->sprite_state == SpriteState::W_RIGHT) {
        b->_x_speed = 5.0;
        b->_x += 2;
    }

    global::creatures_to_add.push_back(b);
    b->_y_speed = 0;
}

//TODO Share this function between pistol / shotgun, similiar behaviour besides cooldown time / spawn bullet callback
void Pistol::handle_shooting() {

    if (activated && !_firing && _cooldown > PISTOL_COOLDOWN) {

        mmEffect(SFX_XSHOTGUN);
        _firing = true;
        _cooldown = 0;
        activated = false;
        spawn_bullet();

    } else
        activated = false;

    if (_blast->_anim_frame_index >= 9) {
        _firing = false;
        _blast->_anim_frame_index = 0;
    }

    _blast->_firing = _firing;

    if (!_firing) {
        _cooldown += *global::timer;
        if (sprite_state == SpriteState::W_LEFT)
            _blast->_x = _x - 10;
        else
            _blast->_x = _x + 10;
        _blast->_y = _y;
        _blast->_sprite_state = sprite_state;
    }

}

void Pistol::match_animation() {

    if (sprite_state == SpriteState::W_LEFT)
        _frame_gfx = sprite_utils::get_frame((u8 *) gfx_spike_collectibles_flameTiles, _sprite_size, 32);
    else
        _frame_gfx = sprite_utils::get_frame((u8 *) gfx_spike_collectibles_flameTiles, _sprite_size, 33);

    sprite_utils::update_frame(_frame_gfx, _sprite_size, _main_sprite_info, _sub_sprite_info);
}

void Pistol::delete_sprites() {
    delete _main_sprite_info;
    delete _sub_sprite_info;
    _main_sprite_info = nullptr;
    _sub_sprite_info = nullptr;
}

