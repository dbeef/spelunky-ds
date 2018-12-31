//
// Created by xdbeef on 16.09.18.
//

#include "TripleGoldbar.h"
#include <maxmod9.h>
#include <cstdlib>
#include "../items/Jar.hpp"
#include "../_BaseCreature.h"
#include "../../interfaces/Moniez.hpp"
#include "../../GlobalsDeclarations.hpp"
#include "../../../build/gfx_goldbars.h"
#include "../../../build/gfx_rubies.h"
#include "../items/Rock.hpp"
#include "../../../build/soundbank.h"
#include "../SpriteType.hpp"
#include "../../collisions/Collisions.hpp"
#include "../../tiles/LevelRenderingUtils.hpp"
#include "../SpriteUtils.hpp"

void TripleGoldbar::update_creature_specific() {

    if (_ready_to_dispose) return;

    update_sprites_position();
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);

    if (!_collected && _collectible_timer >= 500 &&
        Collisions::checkCollisionWithMainDudeWidthBoundary(_x, _y, _physical_width, _physical_height, 8)) {

        if (_spritesheet_type == SpritesheetType::MONIEZ_RUBY)
            mmEffect(SFX_XGEM);
        else if (_spritesheet_type == SpritesheetType::MONIEZ_GOLDBARS)
            mmEffect(SFX_XCOIN);

        global::hud->add_moniez_on_collected_loot(_value);
        _collected = true;
        sprite_utils::set_visibility(false, mainSpriteInfo, subSpriteInfo);
        global::collected_loot.push_back(sprite_type);
        _ready_to_dispose = true;
    }

    if (_collectible_timer < 500)
        _collectible_timer += *global::timer;

}

void TripleGoldbar::init_sprites() {

    delete_sprites();

    frameGfx = (u8 *) gfx_goldbarsTiles;

    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_goldbarsPal, gfx_goldbarsPalLen,
                                                        nullptr, _sprite_size, 16,
                                                        _spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_goldbarsPal, gfx_goldbarsPalLen,
                                                          nullptr, _sprite_size, 16,
                                                          _spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);

    sprite_utils::update_frame(frameGfx, _sprite_size, mainSpriteInfo, subSpriteInfo);

    update_sprites_position();
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_priority(OBJPRIORITY_0, mainSpriteInfo, subSpriteInfo);

}

void TripleGoldbar::update_sprites_position() {
    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);
    sprite_utils::set_entry_xy(mainSpriteInfo, main_x, main_y);
    sprite_utils::set_entry_xy(subSpriteInfo, sub_x, sub_y);
}

void TripleGoldbar::delete_sprites() {
    delete mainSpriteInfo;
    delete subSpriteInfo;
    mainSpriteInfo = nullptr;
    subSpriteInfo = nullptr;
}
