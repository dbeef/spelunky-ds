//
// Created by xdbeef on 16.09.18.
//

#include <maxmod9.h>
#include <cstdlib>

#include "RubyBig.h"
#include "../items/Jar.hpp"
#include "../_BaseCreature.h"
#include "../../interfaces/Moniez.hpp"
#include "../../GlobalsDeclarations.hpp"
#include "../../../build/gfx_goldbars.h"
#include "../../../build/gfx_rubies.h"
#include "../items/Rock.hpp"
#include "../../../build/soundbank.h"
#include "../CreatureType.hpp"
#include "../../collisions/Collisions.hpp"
#include "../../tiles/LevelRenderingUtils.hpp"
#include "../SpriteUtils.hpp"

void RubyBig::update_creature_specific() {

    if (_ready_to_dispose) return;

    update_sprites_position();
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);

    if (_collectible_timer >= 500 &&
        Collisions::checkCollisionWithMainDudeWidthBoundary(_x, _y, _physical_width, _physical_height, 8)) {
        mmEffect(SFX_XGEM);
        global::hud->add_moniez_on_collected_loot(get_value(_creature_type));
        sprite_utils::set_visibility(false, mainSpriteInfo, subSpriteInfo);
        global::collected_loot.push_back(_creature_type);
        _ready_to_dispose = true;
    } else if (_collectible_timer < 500) {
        _collectible_timer += *global::timer;
    }

}

void RubyBig::init_sprites() {

    delete_sprites();

    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_rubiesPal, gfx_rubiesPalLen,
                                                        nullptr, _sprite_size, 8,
                                                        _spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_rubiesPal, gfx_rubiesPalLen, nullptr,
                                                          _sprite_size, 8, _spritesheet_type, true,
                                                          false, LAYER_LEVEL::MIDDLE_TOP);

    int offset;

    switch (_ruby_color) {
        case RubyColor::RED:
            offset = 0;
            break;
        case RubyColor::GREEN:
            offset = 1;
            break;
        case RubyColor::BLUE:
            offset = 2;
            break;
        default:
            offset = 0;
            break;
    }

    frameGfx = sprite_utils::get_frame((u8 *) gfx_rubiesTiles, _sprite_size, offset);

    sprite_utils::update_frame(frameGfx, _sprite_size, mainSpriteInfo, subSpriteInfo);

    update_sprites_position();
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_priority(OBJPRIORITY_0, mainSpriteInfo, subSpriteInfo);
}

void RubyBig::update_sprites_position() {
    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);
    sprite_utils::set_entry_xy(mainSpriteInfo, main_x, main_y);
    sprite_utils::set_entry_xy(subSpriteInfo, sub_x, sub_y);
}

void RubyBig::delete_sprites() {
    delete mainSpriteInfo;
    delete subSpriteInfo;
    mainSpriteInfo = nullptr;
    subSpriteInfo = nullptr;
}
