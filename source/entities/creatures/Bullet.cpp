//
// Created by xdbeef on 17.05.18.
//

#include "../../collisions/Collisions.hpp"
#include "../../../build/gfx_spike_collectibles_flame.h"
#include "_BaseCreature.h"
#include "Bullet.hpp"
#include "../../tiles/LevelRenderingUtils.hpp"

#include "../../GameState.hpp"
#include "../../time/Timer.h"
#include "../../graphics/SpriteUtils.hpp"

#define BULLET_POSITION_INC_DELTA 15
#define BULLET_ANIM_X_OFFSET 5
#define BULLET_ANIM_Y_OFFSET 5
#define BULLET_ANIM_FRAME_DELTA 50

void Bullet::update_creature_specific() {

    if (_ready_to_dispose)
        return;

    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_visibility(true, mainSpriteInfo, subSpriteInfo);
    update_sprites_position();

    lifetime += Timer::getDeltaTime();

    if (_map_collisions_checked) {
        if (_left_collision || _right_collision || _bottom_collision || _upper_collision) {
            killed = true;
            _y_speed = 0;
        }
        _map_collisions_checked = false;
    }

    if (killed) {

        animFrameTimer += Timer::getDeltaTime();

        if (animFrameTimer > BULLET_ANIM_FRAME_DELTA) {

            animFrame++;
            animFrameTimer = 0;

            match_animation();
        }

        if (animFrame >= 6) {
            _ready_to_dispose = true;
            sprite_utils::set_visibility(false, mainSpriteInfo, subSpriteInfo);
        }

    } else if (kill_creatures_jars_if_have_speed_recoil(4) || kill_main_dude_if_have_speed(4))
        killed = true;

}

void Bullet::init_sprites() {

    delete_sprites();

    subSpriteInfo = OAMManager::sub().initSprite(gfx_spike_collectibles_flamePal,
                                                        gfx_spike_collectibles_flamePalLen,
                                                        nullptr, _sprite_size, ObjSize::OBJSIZE_8,
                                                        _spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = OAMManager::main().initSprite(gfx_spike_collectibles_flamePal,
                                                          gfx_spike_collectibles_flamePalLen,
                                                          nullptr, _sprite_size, ObjSize::OBJSIZE_8,
                                                          _spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);

    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_visibility(true, mainSpriteInfo, subSpriteInfo);
    update_sprites_position();
    match_animation();
}

void Bullet::update_sprites_position() {
    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);
    if (!killed) {
        sprite_utils::set_entry_xy(mainSpriteInfo, main_x, main_y);
        sprite_utils::set_entry_xy(subSpriteInfo, sub_x, sub_y);
    } else {
        sprite_utils::set_entry_xy(mainSpriteInfo, main_x - BULLET_ANIM_X_OFFSET, main_y - BULLET_ANIM_Y_OFFSET);
        sprite_utils::set_entry_xy(subSpriteInfo, sub_x - BULLET_ANIM_X_OFFSET, sub_y - BULLET_ANIM_Y_OFFSET);
    }
}

void Bullet::match_animation() {
    if (killed)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spike_collectibles_flameTiles, _sprite_size,
                                           25 + animFrame);
    else
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spike_collectibles_flameTiles, _sprite_size, 25);

    sprite_utils::update_frame(frameGfx, _sprite_size, mainSpriteInfo, subSpriteInfo);
}

void Bullet::delete_sprites() {
    delete mainSpriteInfo;
    delete subSpriteInfo;
    mainSpriteInfo = nullptr;
    subSpriteInfo = nullptr;
}

void Bullet::print_classname() const {
    printf("BULLET\n");
}

