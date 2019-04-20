//
// Created by dbeef on 2/17/19.
//

#include "GoldenIdol.h"
#include "../../GlobalsDeclarations.hpp"
#include "../main_dude/MainDude.hpp"
#include "../../../build/gfx_goldbars.h"
#include "../../collisions/Collisions.hpp"
#include "../../tiles/LevelRenderingUtils.hpp"
#include "../../memory/SpriteUtils.hpp"
#include "../../tiles/TileOrientation.hpp"
#include "../../../build/soundbank.h"
#include "../creatures/Boulder.h"

void GoldenIdol::update_item_specific() {

    if (_ready_to_dispose) return;

    kill_creatures_if_have_speed(1);
    update_sprites_position();

    //search for an exit
    if (!collected && _hold_by_main_dude) {

        if (never_pickuped) {
            auto boulder = new Boulder(_x, _y - 5 * 16);
            global::creatures.push_back(boulder);
            never_pickuped = false;
        }

        MapTile *tiles[9] = {};
        Collisions::getNeighboringTiles(global::current_level->map_tiles, global::main_dude->current_x_in_tiles,
                                        global::main_dude->current_y_in_tiles, tiles);

        collected = tiles[CENTER] != nullptr && tiles[CENTER]->mapTileType == MapTileType::EXIT;
        if (collected) {
            _x_speed = 0;
            _y_speed = 0;
            _x = tiles[CENTER]->x * 16;
            _y = tiles[CENTER]->y * 16;
            if (_hold_by_main_dude) {
                global::main_dude->holding_item = false;
                global::hud->money += 5000;
                global::hud->update();
                _hold_by_main_dude = false;
                _physical_height = 0;
                _physical_width = 0;
            }
            match_frame();
        }
    } else if(collected) {
        animation_timer += *global::timer;
        up_timer += *global::timer;
        if (up_timer > 50) {
            up_timer = 0;
            _y--;
        }
        if (animation_timer > 1000) {
            _ready_to_dispose = true;
            sprite_utils::set_visibility(false, _main_sprite_info, _sub_sprite_info);
        }
    }

}

void GoldenIdol::init_sprites() {

    delete_sprites();

    _sub_sprite_info = global::sub_oam_manager->initSprite(gfx_goldbarsPal, gfx_goldbarsPalLen,
                                                           nullptr, _sprite_size, ObjSize::OBJSIZE_16,
                                                           _spritesheet_type, true, false,
                                                           LAYER_LEVEL::MIDDLE_TOP);
    _main_sprite_info = global::main_oam_manager->initSprite(gfx_goldbarsPal, gfx_goldbarsPalLen,
                                                             nullptr, _sprite_size, ObjSize::OBJSIZE_16,
                                                             _spritesheet_type, true,
                                                             false, LAYER_LEVEL::MIDDLE_TOP);

    match_frame();

    update_sprites_position();

    sprite_utils::set_priority(OBJPRIORITY_0, _main_sprite_info, _sub_sprite_info);
    sprite_utils::set_horizontal_flip(false, _main_sprite_info, _sub_sprite_info);
    sprite_utils::set_vertical_flip(false, _main_sprite_info, _sub_sprite_info);
    sprite_utils::set_visibility(true, _main_sprite_info, _sub_sprite_info);
}

void GoldenIdol::match_frame() {
    u8 *frame_gfx;

    if (collected) {
        frame_gfx = sprite_utils::get_frame((u8 *) gfx_goldbarsTiles, _sprite_size, 45);
    } else
        frame_gfx = sprite_utils::get_frame((u8 *) gfx_goldbarsTiles, _sprite_size, 44);

    sprite_utils::update_frame(frame_gfx, _sprite_size, _main_sprite_info, _sub_sprite_info);
}
