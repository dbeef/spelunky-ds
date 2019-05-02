//
// Created by xdbeef on 02.06.18.
//

#include "../../../build/gfx_spike_collectibles_flame.h"
#include "../../GameState.hpp"
#include "../../collisions/Collisions.hpp"
#include "FlameElement.hpp"
#include "Flame.hpp"

#include "../../time/Timer.h"
#include "../../graphics/SpriteUtils.hpp"

//TODO remove 'finished' flag - duplicates _ready_to_dispose - same for blood element
//TODO Give bombs a bigger rendering priority than other things - but no bigger than hud
void FlameElement::update_creature_specific() {

    if (_ready_to_dispose || finished)
        return;

    update_sprites_position();

    inactive_delay += Timer::getDeltaTime();
    frameTimer += Timer::getDeltaTime();

    if (frameTimer > FLAME_ANIM_FRAME_DELTA) {

        frameTimer = 0;
        currentFrame++;

        if (currentFrame >= 5) {
            finished = true;
            _ready_to_dispose = true;
            sprite_utils::set_visibility(false, mainSpriteInfo, subSpriteInfo);
        } else
            match_animation();
    }
}

void FlameElement::init_sprites() {

    delete_sprites();
    
    subSpriteInfo = GameState::instance().sub_oam_manager->initSprite(gfx_spike_collectibles_flamePal,
                                                        gfx_spike_collectibles_flamePalLen, nullptr,
                                                        _sprite_size, ObjSize::OBJSIZE_8, SpritesheetType::SPIKES_COLLECTIBLES, true,
                                                        false, LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = GameState::instance().main_oam_manager->initSprite(gfx_spike_collectibles_flamePal,
                                                          gfx_spike_collectibles_flamePalLen,
                                                          nullptr, _sprite_size, ObjSize::OBJSIZE_8, SpritesheetType::SPIKES_COLLECTIBLES,
                                                          true, false, LAYER_LEVEL::MIDDLE_TOP);
    match_animation();
    update_sprites_position();
    sprite_utils::set_visibility(true, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);

}

void FlameElement::update_sprites_position() {
    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);
    sprite_utils::set_entry_xy(mainSpriteInfo, static_cast<u16>(main_x), static_cast<u16>(main_y));
    sprite_utils::set_entry_xy(subSpriteInfo, static_cast<u16>(sub_x), static_cast<u16>(sub_y));
}

void FlameElement::match_animation() {
    frameGfx = sprite_utils::get_frame((u8 *) gfx_spike_collectibles_flameTiles, _sprite_size, 34 + currentFrame);
    sprite_utils::update_frame(frameGfx, _sprite_size, mainSpriteInfo, subSpriteInfo);
}

void FlameElement::delete_sprites() {
    delete mainSpriteInfo;
    delete subSpriteInfo;
    mainSpriteInfo = nullptr;
    subSpriteInfo = nullptr;
}
