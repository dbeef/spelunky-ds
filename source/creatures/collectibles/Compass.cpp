//
// Created by xdbeef on 19.05.18.
//


#include <cstdlib>
#include "Compass.hpp"
#include "../../GlobalsDeclarations.hpp"
#include "../../collisions/Collisions.hpp"
#include "../../../build/gfx_saleable.h"
#include "../../decorations/GotCollectible.hpp"
#include "../SpriteUtils.hpp"

#define COMPASS_POS_INC_DELTA 15

void Compass::update_creature_specific() {

    if (_ready_to_dispose) return;

    update_anim_icon(_x, _y, _physical_width);
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);

    if (collected) {
        draw_arrow_to_exit();
    } else {

        if (_bought && check_if_can_be_equipped())
            equip();
        else if (!_bought && !hold_by_main_dude)
            check_if_can_be_pickuped();

        if (hold_by_main_dude) {
            set_pickuped_position(4, -4);
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
    
    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_saleablePal, gfx_saleablePalLen,
                                                        nullptr, _sprite_size, ObjSize::OBJSIZE_16,
                                                        _spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_saleablePal, gfx_saleablePalLen,
                                                          nullptr, _sprite_size, ObjSize::OBJSIZE_16,
                                                          _spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);

    update_sprites_position();
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);

    if (collected)
        //compass is collected and drawn as hud, visibility is set in the function call below
        draw_arrow_to_exit();
    else {
        //compass is an item just like any else so set as visible
        sprite_utils::set_visibility(true, mainSpriteInfo, subSpriteInfo);
        frameGfx = sprite_utils::get_frame((u8 *) gfx_saleableTiles, _sprite_size, 2);
        sprite_utils::update_frame(frameGfx, _sprite_size, mainSpriteInfo, subSpriteInfo);
    }

}

void Compass::update_sprites_position() {

    if (collected) {
        //draw as hud
        sprite_utils::set_entry_xy(mainSpriteInfo, _x, _y);
        sprite_utils::set_priority(OBJPRIORITY_0, mainSpriteInfo, subSpriteInfo);
    } else {

        int main_x, main_y, sub_x, sub_y;
        get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);
        sprite_utils::set_entry_xy(mainSpriteInfo, main_x, main_y);
        sprite_utils::set_entry_xy(subSpriteInfo, sub_x, sub_y);
    }

}

void Compass::draw_arrow_to_exit() {

    MapTile *exit = nullptr;
    global::current_level->get_first_tile_of_given_type(MapTileType::EXIT, exit);


    if (exit != nullptr) {

        int tile_x = exit->x * TILE_W;
        int tile_y = exit->y * TILE_H;

        //camera is centered on main dude
        int diff_x = abs((global::camera->x + 0.5 * SCREEN_WIDTH) - tile_x);
        int diff_y = abs((global::camera->y + 1.5 * SCREEN_HEIGHT) - tile_y);

        subSpriteInfo->entry->isHidden = false;

        if (diff_y <= SCREEN_HEIGHT * 0.5 && diff_x <= SCREEN_WIDTH * 0.5) {

            subSpriteInfo->entry->isHidden = true;

        } else if (diff_x < 6 * TILE_W) {
            //down arrow
            sprite_utils::set_entry_xy(subSpriteInfo, (SCREEN_WIDTH * 0.5) - 8, SCREEN_HEIGHT - 2 - _sprite_height);
            frameGfx = sprite_utils::get_frame((u8 *) gfx_saleableTiles, _sprite_size, 6);
        } else if (diff_y < 3 * TILE_H) {

            if (global::main_dude->_x > tile_x) {
                //left_arrow
                frameGfx = sprite_utils::get_frame((u8 *) gfx_saleableTiles, _sprite_size, 5);
                sprite_utils::set_entry_xy(subSpriteInfo, 4, SCREEN_HEIGHT * 0.5);

            } else {
                //right arrow
                frameGfx = sprite_utils::get_frame((u8 *) gfx_saleableTiles, _sprite_size, 4);
                sprite_utils::set_entry_xy(subSpriteInfo, SCREEN_WIDTH - 4 - _sprite_width, SCREEN_HEIGHT * 0.5);
            }

        } else {

            if (global::main_dude->_x > tile_x) {
                //down-left arrow
                frameGfx = sprite_utils::get_frame((u8 *) gfx_saleableTiles, _sprite_size, 8);
                sprite_utils::set_entry_xy(subSpriteInfo, 4, SCREEN_HEIGHT - 4 - _sprite_height);

            } else {
                //down-right arrow
                sprite_utils::set_entry_xy(subSpriteInfo, SCREEN_WIDTH - 4 - _sprite_width,
                                           SCREEN_HEIGHT - 4 - _sprite_height);
                frameGfx = sprite_utils::get_frame((u8 *) gfx_saleableTiles, _sprite_size, 9);
            }

        }
        
        subSpriteInfo->updateFrame(frameGfx, _sprite_size);
    }

}

void Compass::equip() {
    collected = true;

    auto *g = new GotCollectible(_x - 12, _y - 20, GotCollectible::Type::ITEM);
    global::decorations_to_add.push_back(g);

    if (!global::main_dude->carrying_compass) {
        global::main_dude->carrying_compass = true;
        update_sprites_position();
        _x = HUD_ITEMS_ROW_X;
        _y = global::hud->items_offset_y;
        global::hud->increment_offset_on_grabbed_item();
    } else {
        sprite_utils::set_visibility(false, mainSpriteInfo, subSpriteInfo);
        _ready_to_dispose = true;
    }

}

void Compass::delete_sprites() {
    delete mainSpriteInfo;
    delete subSpriteInfo;
    mainSpriteInfo = nullptr;
    subSpriteInfo = nullptr;
}
