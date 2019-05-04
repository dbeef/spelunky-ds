//
// Created by xdbeef on 24.02.18.
//

#include <nds.h>
#include <nds/arm9/sprite.h>
#include "../../math/Math.hpp"
#include <cstdlib>
#include <cstring>

#include <cstdlib>
#include <cmath>
#include <cstdio>
#include "../../GameState.hpp"
#include "../items/Shotgun.hpp"
#include "../items/Mitt.hpp"
#include "../items/Glove.hpp"
#include "../items/Compass.hpp"
#include "../items/SpringShoes.hpp"
#include "../items/Rope.hpp"
#include "../../collisions/Collisions.hpp"
#include "../../../build/soundbank.h"
#include "../../tiles/TileOrientation.hpp"
#include "../items/SpikeShoes.hpp"
#include "../items/Cape.hpp"
#include "../items/Jetpack.hpp"

#include "../../../build/gfx_spelunker.h"
#include "../items/Pistol.hpp"
#include "MainDude.hpp"
#include "../../sound/Sound.hpp"
#include "../../tiles/MapTile.hpp"
#include "../../tiles/Level.hpp"
#include "../../camera/Camera.hpp"

#include "../items/Bomb.hpp"
#include "../../input/InputHandler.hpp"
#include "../../GameState.hpp"
#include "../../graphics/SpriteUtils.hpp"
#include "../../math/Math.hpp"

#include "../../graphics/SpriteUtils.hpp"
#include "../../GameState.hpp"
#include "MainDude.hpp"
#include "../../GameState.hpp"
#include "../../collisions/Collisions.hpp"
#include "../../../build/gfx_spelunker.h"
#include "../../../build/soundbank.h"
#include "../../tiles/TileOrientation.hpp"
#include "../animations/FallPoof.hpp"
#include "../../sound/Sound.hpp"

#include "../../tiles/MapTile.hpp"
#include "../../tiles/Level.hpp"
#include "../../camera/Camera.hpp"

#include "../items/Bomb.hpp"
#include "../../input/InputHandler.hpp"
#include "../items/_BaseItem.h"
#include "../_interfaces/IPickupable.h"
#include "../../time/Timer.h"

#include "states/_DudeStateHandler.hpp"
#include "states/Climbing.hpp"
#include "states/Dead.hpp"
#include "states/ExitingLevel.hpp"
#include "states/HangingOnEdge.hpp"
#include "states/Stunned.hpp"
#include "states/Walking.hpp"

MainDude *MainDude::_instance = nullptr;

void MainDude::init() {
    SPELUNKYDS_BREAKING_ASSERT(!_instance);
    _instance = new MainDude(MAIN_DUDE_MENU_START_POSITION_X, MAIN_DUDE_MENU_START_POSITION_Y);
    SPELUNKYDS_BREAKING_ASSERT(_instance);
}

void MainDude::dispose() {
    SPELUNKYDS_BREAKING_ASSERT(_instance);
    delete _instance;
}

void MainDude::reset_state() {
    reset_carried_items();
    _current_state = _walking;
}

void MainDude::update_creature_specific() {
    SPELUNKYDS_BREAKING_ASSERT(_current_state);
    _current_state->handle_input();
    _current_state->update();
}

void MainDude::init_sprites() {

    delete_sprites();

    main_sprite_info = OAMManager::main().initSprite(gfx_spelunkerPal, gfx_spelunkerPalLen, nullptr,
                                                     _sprite_size, ObjSize::OBJSIZE_16, _spritesheet_type, true,
                                                     false,
                                                     LAYER_LEVEL::MIDDLE_TOP);


    sub_sprite_info = OAMManager::sub().initSprite(gfx_spelunkerPal, gfx_spelunkerPalLen, nullptr,
                                                   _sprite_size, ObjSize::OBJSIZE_16, _spritesheet_type, true,
                                                   false,
                                                   LAYER_LEVEL::MIDDLE_TOP);

    sprite_utils::set_vertical_flip(false, main_sprite_info, sub_sprite_info);
    sprite_utils::set_horizontal_flip(false, main_sprite_info, sub_sprite_info);

    update_sprites_position();
    match_animation();

}

void MainDude::update_sprites_position() {
    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);
    sprite_utils::set_entry_xy(main_sprite_info, static_cast<u16>(main_x), static_cast<u16>(main_y));
    sprite_utils::set_entry_xy(sub_sprite_info, static_cast<u16>(sub_x), static_cast<u16>(sub_y));
}

bool MainDude::can_apply_gravity() const {
    return !_bottom_collision &&
           _current_state->_state != _MainDudeState::CLIMBING &&
           _current_state->_state != _MainDudeState::HANGING_ON_EDGE;
}

void MainDude::delete_sprites() {
    delete main_sprite_info;
    delete sub_sprite_info;
    main_sprite_info = nullptr;
    sub_sprite_info = nullptr;
}

MainDude::~MainDude() {
    delete _climbing;
    delete _dead;
    delete _exiting_level;
    delete _hanging_on_edge;
    delete _stunned;
    delete _walking;
}

MainDude::MainDude(int x, int y) : BaseCreature(
        x,
        y,
        main_dude_sprite_width,
        main_dude_sprite_height,
        main_dude_spritesheet_type,
        main_dude_physical_width,
        main_dude_physical_height,
        CreatureType::MAIN_DUDE
) {

    // main dude specific characteristics
    time_since_last_damage = MAIN_DUDE_DAMAGE_PROTECTION_TIME + 1;
    _friction = ICollidable::default_friction * 5.0f;
    _bouncing_factor_y = 0;
    _bouncing_factor_x = 0;
    _gravity = ICollidable::default_gravity * 1.15f;

    // allocate possible states and start with walking
    _climbing = new Climbing();
    _dead = new Dead();
    _exiting_level = new ExitingLevel();
    _hanging_on_edge = new HangingOnEdge();
    _stunned = new Stunned();
    _walking = new Walking();

    _current_state = _walking;
}

void MainDude::print_classname() const {
    printf("MAIN_DUDE\n");
}

void MainDude::reset_carried_items() {
    carried_items = {};
}

void MainDude::set_stunned() {
    _current_state = _stunned;
    _current_state->enter();
}

void MainDude::set_exiting_level() {
    _current_state = _exiting_level;
    _current_state->enter();
}

void MainDude::set_walking() {
    _current_state = _hanging_on_edge;
    _current_state->enter();
}

void MainDude::set_climbing() {
    _current_state = _climbing;
    _current_state->enter();
}

void MainDude::set_hanging_on_edge() {
    _current_state = _hanging_on_edge;
    _current_state->enter();
};

void MainDude::set_dead() {
    _current_state = _dead;
    _current_state->enter();
}
