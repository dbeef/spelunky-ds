//
// Created by xdbeef on 28.04.18.
//

#include "Blood.hpp"
#include "../../GlobalsDeclarations.hpp"
#include "../../../build/gfx_blood_rock_rope_poof.h"
#include "../../collisions/Collisions.hpp"
#include "../../tiles/LevelRenderingUtils.hpp"
#include "../SpriteUtils.hpp"

void Blood::update_creature_specific() {

    //FIXME Add elements to the global entities array
    //update all trail elements
//    for (int a = 0; a < bloodTrail.size(); a++) {
//        bloodTrail.at(a)->draw();
//        bloodTrail.at(a)->update();
//        if (bloodTrail.at(a)->finished) {
//            delete (bloodTrail.at(a));
//            bloodTrail.erase(bloodTrail.begin() + a); //FIXME this erasing is probably wrong
//        }
//    }

    if (_ready_to_dispose) {
        sprite_utils::set_visibility(false, mainSpriteInfo, subSpriteInfo);
        return;
    }

    time_since_last_spawn += *global::timer;

    if (_bottom_collision)
        //dispose blood droplet faster if touching ground
        living_timer += 4 * (*global::timer);
    else
        living_timer += *global::timer;

    if (finished) {
        //if main blood droplet animation is finished,
        //check if all animations in blood trail finished, if so, main blood droplet can be disposed

        bool chain_finished = true;

        for (auto &a : bloodTrail) {
            if (!a->finished) {
                chain_finished = false;
                break;
            }
        }

        if (chain_finished)
            _ready_to_dispose = true;

    } else {
        //animation not finished on main droplet

        animFrameTimer += *global::timer;

        if (animFrameTimer > BLOOD_ANIM_FRAME_DELTA) {
            animFrameTimer = 0;
            currentFrame++;
            match_animation();
        }

        if (!finished && bloodTrail.size() <= 2 && time_since_last_spawn > 120) {
            time_since_last_spawn = 0;
            spawn_blood();
        }

        sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
        sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);
        update_sprites_position();
    }

}

void Blood::init_sprites() {

    delete_sprites();
    
    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_blood_rock_rope_poofPal, gfx_blood_rock_rope_poofPalLen,
                                                        nullptr, _sprite_size, ObjSize::OBJSIZE_8, BLOOD_ROCK_ROPE_POOF,
                                                        true, false, LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_blood_rock_rope_poofPal, gfx_blood_rock_rope_poofPalLen,
                                                          nullptr, _sprite_size, ObjSize::OBJSIZE_8, BLOOD_ROCK_ROPE_POOF,
                                                          true, false, LAYER_LEVEL::MIDDLE_TOP);
    match_animation();
    update_sprites_position();
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);

}

void Blood::spawn_blood() {
    auto *element = new BloodElement(_x, _y);
    element->_x_speed = _x_speed;
    element->_y_speed = _y_speed;
    element->currentFrame = currentFrame;
    element->update();
    bloodTrail.push_back(element);
    global::creatures_to_add.push_back(element);
}

void Blood::match_animation() {

    if (living_timer <= 1200) {
        //while living_timer is less than 1.2 s, repeat the same animation

        if (currentFrame >= 3)
            currentFrame = 0;

        frameGfx = sprite_utils::get_frame((u8 *) gfx_blood_rock_rope_poofTiles, _sprite_size, 11 + currentFrame);

    } else if (living_timer > 1200) {
        //if living_timer greater than 1.2 s, finish animation with the last frames

        if (currentFrame >= 6) {
            finished = true;
            sprite_utils::set_visibility(false, mainSpriteInfo, subSpriteInfo);
        } else
            frameGfx = sprite_utils::get_frame((u8 *) gfx_blood_rock_rope_poofTiles, _sprite_size, currentFrame);

    }

    sprite_utils::update_frame(frameGfx, _sprite_size, mainSpriteInfo, subSpriteInfo);

}

void Blood::update_sprites_position() {
    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);
    sprite_utils::set_entry_xy(mainSpriteInfo, main_x, main_y);
    sprite_utils::set_entry_xy(subSpriteInfo, sub_x, sub_y);
}

void Blood::delete_sprites() {
    delete mainSpriteInfo;
    delete subSpriteInfo;
    mainSpriteInfo = nullptr;
    subSpriteInfo = nullptr;
}
