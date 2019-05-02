//
// Created by xdbeef on 15.05.18.
//

#include <cstdlib>
#include <maxmod9.h>

#include "../../../build/gfx_spike_collectibles_flame.h"
#include "../../../build/soundbank.h"

#include "../../collisions/Collisions.hpp"
#include "../../sound/Sound.hpp"
#include "../animations/Blood.hpp"
#include "../../memory/SpriteUtils.hpp"
#include "Spikes.h"
#include "../../GameState.hpp"

void Spikes::update_creature_specific() {

    if (_ready_to_dispose) return;

    // check whether main dude can be killed by this spikes
    if (!GameState::instance().main_dude->dead && !GameState::instance().main_dude->using_cape &&
        Collisions::checkCollisionBodiesLeftLowerCorner(_x, _y + 16, _physical_width, _physical_height,
                                                        GameState::instance().main_dude->_x, GameState::instance().main_dude->_y + 16, 16, 16) &&
        !GameState::instance().main_dude->_bottom_collision && GameState::instance().main_dude->_y_speed > 1) {

        GameState::instance().main_dude->set_dead();
        spawn_blood();

        _blood = true;
        u8 *frame_gfx = (u8 *) gfx_spike_collectibles_flameTiles + (_sprite_size * (1) / 2);
        sprite_utils::update_frame(frame_gfx, _sprite_size, _main_sprite_info, _sub_sprite_info);
    }

    // check whether any creature can be killed by this spikes
    for (unsigned long a = 0; a < GameState::instance().creatures.size(); a++) {
        if ((GameState::instance().creatures.at(a)->_creature_type == CreatureType::SPIDER ||
             GameState::instance().creatures.at(a)->_spritesheet_type == SpritesheetType::CAVEMAN_DAMSEL ||
             GameState::instance().creatures.at(a)->_spritesheet_type == SpritesheetType::SHOPKEEPER)
            && !GameState::instance().creatures.at(a)->_ready_to_dispose && !GameState::instance().creatures.at(a)->killed) {

            if (Collisions::checkCollisionBodies(_x, _y, _physical_width, _physical_height,
                                                 GameState::instance().creatures.at(a)->_x,
                                                 GameState::instance().creatures.at(a)->_y,
                                                 GameState::instance().creatures.at(a)->_physical_width,
                                                 GameState::instance().creatures.at(a)->_physical_height)) {

                if (!GameState::instance().creatures.at(a)->_bottom_collision && GameState::instance().creatures.at(a)->_y_speed > 0 &&
                    !GameState::instance().creatures.at(a)->hold_by_main_dude) {
                    GameState::instance().creatures.at(a)->apply_dmg(8);
                    GameState::instance().creatures.at(a)->_x_speed = 0;
                    _blood = true;
                    u8 *frame_gfx = (u8 *) gfx_spike_collectibles_flameTiles + (_sprite_size * (1) / 2);
                    sprite_utils::update_frame(frame_gfx, _sprite_size, _main_sprite_info, _sub_sprite_info);
                }
            }
        }
    }

    update_sprites_position();
}

void Spikes::init_sprites() {

    delete_sprites();

    _sub_sprite_info = GameState::instance().sub_oam_manager->initSprite(gfx_spike_collectibles_flamePal,
                                                           gfx_spike_collectibles_flamePalLen,
                                                           nullptr, _sprite_size, ObjSize::OBJSIZE_16,
                                                           _spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_BOT);
    _main_sprite_info = GameState::instance().main_oam_manager->initSprite(gfx_spike_collectibles_flamePal,
                                                             gfx_spike_collectibles_flamePalLen,
                                                             nullptr, _sprite_size, ObjSize::OBJSIZE_16,
                                                             _spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_BOT);

    u8 *frame_gfx;

    if (_blood)
        frame_gfx = (u8 *) gfx_spike_collectibles_flameTiles + (_sprite_size * (1) / 2);
    else
        frame_gfx = (u8 *) gfx_spike_collectibles_flameTiles;
    sprite_utils::update_frame(frame_gfx, _sprite_size, _main_sprite_info, _sub_sprite_info);

    sprite_utils::set_vertical_flip(false, _main_sprite_info, _sub_sprite_info);
    sprite_utils::set_horizontal_flip(false, _main_sprite_info, _sub_sprite_info);
    sprite_utils::set_visibility(true, _main_sprite_info, _sub_sprite_info);
}

void Spikes::update_sprites_position() {
    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);
    sprite_utils::set_entry_xy(_main_sprite_info, static_cast<u16>(main_x), static_cast<u16>(main_y));
    sprite_utils::set_entry_xy(_sub_sprite_info, static_cast<u16>(sub_x), static_cast<u16>(sub_y));
}


void Spikes::delete_sprites() {
    delete _main_sprite_info;
    delete _sub_sprite_info;
    _main_sprite_info = nullptr;
    _sub_sprite_info = nullptr;
}
