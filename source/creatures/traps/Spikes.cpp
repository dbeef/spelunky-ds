//
// Created by xdbeef on 15.05.18.
//

#include <cstdlib>
#include <maxmod9.h>

#include "../../../build/gfx_spike_collectibles_flame.h"
#include "../../../build/soundbank.h"

#include "../../collisions/Collisions.hpp"
#include "../../GlobalsDeclarations.hpp"
#include "../../sound/SoundUtils.hpp"
#include "../animations/Blood.hpp"
#include "../SpriteUtils.hpp"
#include "Spikes.h"

void Spikes::update_creature_specific() {

    if (_ready_to_dispose) return;

    // check whether main dude can be killed by this spikes
    if (!global::main_dude->dead && !global::main_dude->using_cape &&
        Collisions::checkCollisionBodiesLeftLowerCorner(_x, _y + 16, _physical_width, _physical_height,
                                                        global::main_dude->_x, global::main_dude->_y + 16, 16, 16) &&
        !global::main_dude->_bottom_collision && global::main_dude->_y_speed > 1) {

        global::main_dude->set_dead();
        spawn_blood();

        _blood = true;
        _frame_gfx = (u8 *) gfx_spike_collectibles_flameTiles + (_sprite_size * (1) / 2);
        sprite_utils::update_frame(_frame_gfx, _sprite_size, _main_sprite_info, _sub_sprite_info);
    }

    // check whether any creature can be killed by this spikes
    for (unsigned long a = 0; a < global::creatures.size(); a++) {
        if ((global::creatures.at(a)->_creature_type == CreatureType::SPIDER ||
             global::creatures.at(a)->_spritesheet_type == SpritesheetType::CAVEMAN_DAMSEL ||
             global::creatures.at(a)->_spritesheet_type == SpritesheetType::SHOPKEEPER)
            && !global::creatures.at(a)->_ready_to_dispose && !global::creatures.at(a)->killed) {

            if (Collisions::checkCollisionBodies(_x, _y, _physical_width, _physical_height,
                                                 global::creatures.at(a)->_x,
                                                 global::creatures.at(a)->_y,
                                                 global::creatures.at(a)->_physical_width,
                                                 global::creatures.at(a)->_physical_height)) {

                if (!global::creatures.at(a)->_bottom_collision && global::creatures.at(a)->_y_speed > 0 &&
                    !global::creatures.at(a)->hold_by_main_dude) {
                    global::creatures.at(a)->apply_dmg(8);
                    global::creatures.at(a)->_x_speed = 0;
                    _blood = true;
                    _frame_gfx = (u8 *) gfx_spike_collectibles_flameTiles + (_sprite_size * (1) / 2);
                    sprite_utils::update_frame(_frame_gfx, _sprite_size, _main_sprite_info, _sub_sprite_info);
                }
            }
        }
    }

    update_sprites_position();
}

void Spikes::init_sprites() {

    delete_sprites();

    _sub_sprite_info = global::sub_oam_manager->initSprite(gfx_spike_collectibles_flamePal,
                                                           gfx_spike_collectibles_flamePalLen,
                                                           nullptr, _sprite_size, _sprite_width,
                                                           _spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_BOT);
    _main_sprite_info = global::main_oam_manager->initSprite(gfx_spike_collectibles_flamePal,
                                                             gfx_spike_collectibles_flamePalLen,
                                                             nullptr, _sprite_size, _sprite_width,
                                                             _spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_BOT);

    if (_blood)
        _frame_gfx = (u8 *) gfx_spike_collectibles_flameTiles + (_sprite_size * (1) / 2);
    else
        _frame_gfx = (u8 *) gfx_spike_collectibles_flameTiles;
    sprite_utils::update_frame(_frame_gfx, _sprite_size, _main_sprite_info, _sub_sprite_info);

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
