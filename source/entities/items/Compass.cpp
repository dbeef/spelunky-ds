//
// Created by xdbeef on 19.05.18.
//


#include <cstdlib>
#include "Compass.hpp"
#include "../../GameState.hpp"
#include "../../collisions/Collisions.hpp"
#include "../../../build/gfx_saleable.h"
#include "../../entities/decorations/GotCollectible.hpp"
#include "../../graphics/SpriteUtils.hpp"


void Compass::update_item_specific() {

    if (_ready_to_dispose) return;

    update_anim_icon(_x, _y, _physical_width);
    sprite_utils::set_vertical_flip(false, _main_sprite_info, _sub_sprite_info);
    sprite_utils::set_horizontal_flip(false, _main_sprite_info, _sub_sprite_info);

    if (collected) {
        draw_arrow_to_exit();
    } else {

        if (_bought && check_if_can_be_equipped())
            equip();
        else if (!_bought && !_hold_by_main_dude)
            check_if_can_be_pickuped();

        if (_hold_by_main_dude) {
            if (shopping_transaction(this)) {
                collected = true;
                equip();
            }
        }

    }

    update_sprites_position();

}

void Compass::init_sprites() {

    delete_sprites();

    _sub_sprite_info = GameState::instance().sub_oam_manager->initSprite(gfx_saleablePal, gfx_saleablePalLen,
                                                           nullptr, _sprite_size, ObjSize::OBJSIZE_16,
                                                           _spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
    _main_sprite_info = GameState::instance().main_oam_manager->initSprite(gfx_saleablePal, gfx_saleablePalLen,
                                                             nullptr, _sprite_size, ObjSize::OBJSIZE_16,
                                                             _spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);

    update_sprites_position();
    sprite_utils::set_vertical_flip(false, _main_sprite_info, _sub_sprite_info);
    sprite_utils::set_horizontal_flip(false, _main_sprite_info, _sub_sprite_info);

    if (collected)
        //compass is collected and drawn as hud, visibility is set in the function call below
        draw_arrow_to_exit();
    else {
        //compass is an item just like any else so set as visible
        sprite_utils::set_visibility(true, _main_sprite_info, _sub_sprite_info);
        u8 *frame_gfx = sprite_utils::get_frame((u8 *) gfx_saleableTiles, _sprite_size, 2);
        sprite_utils::update_frame(frame_gfx, _sprite_size, _main_sprite_info, _sub_sprite_info);
    }

}

void Compass::draw_arrow_to_exit() {

    MapTile *exit = nullptr;
    GameState::instance().current_level->get_first_tile_of_given_type(MapTileType::EXIT, exit);


    if (exit != nullptr) {

        int tile_x = exit->x * TILE_W;
        int tile_y = exit->y * TILE_H;

        //camera is centered on main dude
        int diff_x = abs((Camera::instance().x + 0.5 * SCREEN_WIDTH) - tile_x);
        int diff_y = abs((Camera::instance().y + 1.5 * SCREEN_HEIGHT) - tile_y);

        _sub_sprite_info->entry->isHidden = false;

        if (diff_y <= SCREEN_HEIGHT * 0.5 && diff_x <= SCREEN_WIDTH * 0.5) {

            _sub_sprite_info->entry->isHidden = true;

        } else {

            u8 *frame_gfx;

            if (diff_x < 6 * TILE_W) {
                //down arrow
                sprite_utils::set_entry_xy(_sub_sprite_info, (SCREEN_WIDTH * 0.5) - 8,
                                           SCREEN_HEIGHT - 2 - _sprite_height);
                frame_gfx = sprite_utils::get_frame((u8 *) gfx_saleableTiles, _sprite_size, 6);
            } else if (diff_y < 3 * TILE_H) {

                if (GameState::instance().main_dude->_x > tile_x) {
                    //left_arrow
                    frame_gfx = sprite_utils::get_frame((u8 *) gfx_saleableTiles, _sprite_size, 5);
                    sprite_utils::set_entry_xy(_sub_sprite_info, 4, SCREEN_HEIGHT * 0.5);

                } else {
                    //right arrow
                    frame_gfx = sprite_utils::get_frame((u8 *) gfx_saleableTiles, _sprite_size, 4);
                    sprite_utils::set_entry_xy(_sub_sprite_info, SCREEN_WIDTH - 4 - _sprite_width, SCREEN_HEIGHT * 0.5);
                }

            } else {

                if (GameState::instance().main_dude->_x > tile_x) {
                    //down-left arrow
                    frame_gfx = sprite_utils::get_frame((u8 *) gfx_saleableTiles, _sprite_size, 8);
                    sprite_utils::set_entry_xy(_sub_sprite_info, 4, SCREEN_HEIGHT - 4 - _sprite_height);

                } else {
                    //down-right arrow
                    sprite_utils::set_entry_xy(_sub_sprite_info, SCREEN_WIDTH - 4 - _sprite_width,
                                               SCREEN_HEIGHT - 4 - _sprite_height);
                    frame_gfx = sprite_utils::get_frame((u8 *) gfx_saleableTiles, _sprite_size, 9);
                }
            }
            _sub_sprite_info->updateFrame(frame_gfx, _sprite_size);
        }
    }

}

void Compass::equip() {
    collected = true;

    auto *g = new GotCollectible(_x - 12, _y - 20, GotCollectible::Type::ITEM);
    GameState::instance().decorations.push_back(g);

    if (!GameState::instance().main_dude->carrying_compass) {
        GameState::instance().main_dude->carrying_compass = true;
        update_sprites_position();
        _x = HUD_ITEMS_ROW_X;
        _y = Hud::instance().items_offset_y;
        Hud::instance().increment_offset_on_grabbed_item();
    } else {
        sprite_utils::set_visibility(false, _main_sprite_info, _sub_sprite_info);
        _ready_to_dispose = true;
    }

}

