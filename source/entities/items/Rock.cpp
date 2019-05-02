//
// Created by xdbeef on 04.04.18.
//

#include "Rock.hpp"
#include "../../GameState.hpp"
#include "../main_dude/MainDude.hpp"
#include "../../../build/gfx_blood_rock_rope_poof.h"
#include "../../collisions/Collisions.hpp"
#include "../../tiles/LevelRenderingUtils.hpp"
#include "../../graphics/SpriteUtils.hpp"


void Rock::update_item_specific() {

    if (_ready_to_dispose) return;

    kill_creatures_if_have_speed(1);
    update_sprites_position();
}

void Rock::init_sprites() {

    delete_sprites();

    _sub_sprite_info = OAMManager::sub().initSprite(gfx_blood_rock_rope_poofPal, gfx_blood_rock_rope_poofPalLen,
                                                        nullptr, _sprite_size, ObjSize::OBJSIZE_8, SpritesheetType::BLOOD_ROCK_ROPE_POOF, true, false,
                                                        LAYER_LEVEL::MIDDLE_TOP);
    _main_sprite_info = OAMManager::main().initSprite(gfx_blood_rock_rope_poofPal, gfx_blood_rock_rope_poofPalLen,
                                                          nullptr, _sprite_size, ObjSize::OBJSIZE_8, SpritesheetType::BLOOD_ROCK_ROPE_POOF, true,
                                                          false, LAYER_LEVEL::MIDDLE_TOP);

    u8 *frame_gfx = sprite_utils::get_frame((u8 *) gfx_blood_rock_rope_poofTiles, _sprite_size, 7);
    sprite_utils::update_frame(frame_gfx, _sprite_size, _main_sprite_info, _sub_sprite_info);

    update_sprites_position();

    sprite_utils::set_priority(OBJPRIORITY_0, _main_sprite_info, _sub_sprite_info);
    sprite_utils::set_horizontal_flip(false, _main_sprite_info, _sub_sprite_info);
    sprite_utils::set_vertical_flip(false, _main_sprite_info, _sub_sprite_info);
    sprite_utils::set_visibility(true, _main_sprite_info, _sub_sprite_info);
}
