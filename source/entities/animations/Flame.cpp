//
// Created by xdbeef on 02.06.18.
//


#include <cstdlib>
#include "../../GameState.hpp"
#include "../../../build/gfx_spike_collectibles_flame.h"
#include "../../collisions/Collisions.hpp"
#include "Flame.hpp"
#include "../../memory/SpriteUtils.hpp"
#include "../../time/Timer.h"

void Flame::update_creature_specific() {

    for (unsigned long a = 0; a < flame_trail.size(); a++) {
        flame_trail.at(a)->update();
        if (flame_trail.at(a)->finished) {
            //FIXME Possible leaks in GameState::instance().sprite_infos? Same with blood trails.
            delete (flame_trail.at(a));
            flame_trail.erase(flame_trail.begin() + a);
        }
    }

    if (_ready_to_dispose)
        return;

    update_sprites_position();
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_visibility(true, mainSpriteInfo, subSpriteInfo);

    time_since_last_spawn += Timer::getDeltaTime();

    if (_bottom_collision)
        living_timer += 4 * Timer::getDeltaTime();
    else
        living_timer += Timer::getDeltaTime();

    if (finished) {

        //main flame animation finished, check if other spawned flames finished

        bool chain_finished = true;

        for (auto &a : flame_trail) {
            if (!a->finished) {
                chain_finished = false;
                break;
            }
        }

        if (chain_finished) {
            _ready_to_dispose = true;
            sprite_utils::set_visibility(false, mainSpriteInfo, subSpriteInfo);
        }


    } else {
        //main flame animation not finished, continue and spawn other flames if possible

        animFrameTimer += Timer::getDeltaTime();

        if (animFrameTimer > FLAME_ANIM_FRAME_DELTA) {
            animFrameTimer = 0;
            match_animation();
        }

        if (!finished && flame_trail.size() <= 2 && time_since_last_spawn > 120) {
            time_since_last_spawn = 0;
            spawn_flame();
        }

    }
}

void Flame::init_sprites() {

    delete_sprites();
    
    subSpriteInfo = GameState::instance().sub_oam_manager->initSprite(gfx_spike_collectibles_flamePal,
                                                        gfx_spike_collectibles_flamePalLen, nullptr, _sprite_size,
                                                        ObjSize::OBJSIZE_8, SpritesheetType::SPIKES_COLLECTIBLES, true, false, LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = GameState::instance().main_oam_manager->initSprite(gfx_spike_collectibles_flamePal,
                                                          gfx_spike_collectibles_flamePalLen, nullptr,
                                                          _sprite_size, ObjSize::OBJSIZE_8, SpritesheetType::SPIKES_COLLECTIBLES, true, false,
                                                          LAYER_LEVEL::MIDDLE_TOP);
    match_animation();
    update_sprites_position();
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_visibility(true, mainSpriteInfo, subSpriteInfo);
}


void Flame::spawn_flame() {
    auto *element = new FlameElement(_x, _y);
    element->_x_speed = _x_speed;
    element->_y_speed = _y_speed;
    element->currentFrame = currentFrame;
    element->update();
    element->pos_inc_delta_offset = change_pos_delta_offset;
    flame_trail.push_back(element);
}

void Flame::update_sprites_position() {
    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);
    sprite_utils::set_entry_xy(mainSpriteInfo, main_x, main_y);
    sprite_utils::set_entry_xy(subSpriteInfo, sub_x, sub_y);
}

void Flame::match_animation() {

    if (living_timer <= 1200)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spike_collectibles_flameTiles, _sprite_size, 39);
    else if (living_timer > 1200) {

        currentFrame++;

        if (currentFrame >= 5) {
            finished = true;
            sprite_utils::set_visibility(false, mainSpriteInfo, subSpriteInfo);
        } else
            frameGfx = sprite_utils::get_frame((u8 *) gfx_spike_collectibles_flameTiles, _sprite_size,
                                               currentFrame + 34);

    }

    sprite_utils::update_frame(frameGfx, _sprite_size, mainSpriteInfo, subSpriteInfo);
}

void Flame::delete_sprites() {
    delete mainSpriteInfo;
    delete subSpriteInfo;
    mainSpriteInfo = nullptr;
    subSpriteInfo = nullptr;
}
