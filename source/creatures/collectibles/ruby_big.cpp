//
// Created by xdbeef on 16.09.18.
//

#include <maxmod9.h>
#include <cstdlib>

#include "ruby_big.h"
#include "../items/jar.hpp"
#include "../_base_creature.h"
#include "../../interfaces/moniez.hpp"
#include "../../globals_declarations.hpp"
#include "../../../build/gfx_goldbars.h"
#include "../../../build/gfx_rubies.h"
#include "../items/rock.hpp"
#include "../../../build/soundbank.h"
#include "../sprite_type.hpp"
#include "../../collisions/collisions.hpp"
#include "../../tiles/level_rendering_utils.hpp"
#include "../sprite_utils.hpp"

void RubyBig::update_creature_specific() {

    if (_ready_to_dispose) return;

    update_sprites_position();
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);

    if (!_collected && _collectible_timer >= 500 &&
        Collisions::checkCollisionWithMainDudeWidthBoundary(_x, _y, _physical_width, _physical_height, 8)) {
        mmEffect(SFX_XGEM);
        global::hud->add_moniez_on_collected_loot(_value);
        _collected = true;
        sprite_utils::set_visibility(false, mainSpriteInfo, subSpriteInfo);
        global::collected_loot.push_back(sprite_type);
        _ready_to_dispose = true;
    }

    if (_collectible_timer < 500)
        _collectible_timer += *global::timer;

}

void RubyBig::init_sprites() {

    delete_sprites();
    
    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_rubiesPal, gfx_rubiesPalLen,
                                                        nullptr, _sprite_size, 8,
                                                        _spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_rubiesPal, gfx_rubiesPalLen, nullptr,
                                                          _sprite_size, 8, _spritesheet_type, true,
                                                          false, LAYER_LEVEL::MIDDLE_TOP);

    int ruby_type;

    if (sprite_type == SpriteType::S_MONIEZ_RUBY_BIG_RED)
        ruby_type = 0;
    else if (sprite_type == SpriteType::S_MONIEZ_RUBY_BIG_GREEN)
        ruby_type = 1;
    else if (sprite_type == SpriteType::S_MONIEZ_RUBY_BIG_BLUE)
        ruby_type = 2;
    else ruby_type = 0; //should not happen

    frameGfx = (u8 *) gfx_rubiesTiles + 8 * 8 * (ruby_type) / 2;

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
