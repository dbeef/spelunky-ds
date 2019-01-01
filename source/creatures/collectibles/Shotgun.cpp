//
// Created by xdbeef on 17.05.18.
//

#include <maxmod9.h>
#include <cstdlib>
#include "../../../build/gfx_spike_collectibles_flame.h"
#include "Chest.hpp"
#include "../../collisions/Collisions.hpp"
#include "../../GlobalsDeclarations.hpp"
#include "../../../build/soundbank.h"
#include "Shotgun.hpp"
#include "Bullet.hpp"
#include "../../decorations/GotCollectible.hpp"
#include "../../tiles/LevelRenderingUtils.hpp"
#include "../SpriteUtils.hpp"

#define SHOTGUN_POS_INC_DELTA 15
#define SHOTGUN_COOLDOWN 750

void Shotgun::update_creature_specific() {

    if (_ready_to_dispose) return;

    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);
    update_anim_icon(_x, _y, _physical_width);
    match_animation();

    check_if_can_be_pickuped();

    if (hold_by_main_dude) {

        if (shopping_transaction(this))
            equip();

        global::main_dude->carrying_shotgun = true;
        sprite_state = global::main_dude->sprite_state;
        sprite_utils::set_priority(OBJPRIORITY_0, mainSpriteInfo, subSpriteInfo);

        _y = global::main_dude->_y + 7;
        if (sprite_state == SpriteState::W_LEFT)
            _x = global::main_dude->_x - 4;
        else
            _x = global::main_dude->_x + 7;

    } else {
        sprite_utils::set_priority(OBJPRIORITY_1, mainSpriteInfo, subSpriteInfo);
        global::main_dude->carrying_shotgun = false;
    }

    update_sprites_position();
    handle_shooting();
    kill_mobs_if_thrown(1);
}

void Shotgun::init_sprites() {

    delete_sprites();
    
    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_spike_collectibles_flamePal,
                                                        gfx_spike_collectibles_flamePalLen,
                                                        nullptr, _sprite_size, ObjSize::OBJSIZE_16,
                                                        _spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_spike_collectibles_flamePal,
                                                          gfx_spike_collectibles_flamePalLen,
                                                          nullptr, _sprite_size, ObjSize::OBJSIZE_16,
                                                          _spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);
    match_animation();
    update_sprites_position();
}

void Shotgun::update_sprites_position() {
    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);
    sprite_utils::set_entry_xy(mainSpriteInfo, main_x, main_y);
    sprite_utils::set_entry_xy(subSpriteInfo, sub_x, sub_y);
}

void Shotgun::spawn_bullets() {

    for (int a = 0; a < 3; a++) {
//        printf("\nNEW %i", a);
        //TODO Static pool?
        auto *b = new Bullet(_x, _y - 2);

        if (sprite_state == SpriteState::W_LEFT) {
            b->_x_speed = -4.3 - ((rand() % 20) / 10.0);
            b->_x -= 5;
        } else {
            b->_x_speed = 4.3 + ((rand() % 20) / 10.0);
            b->_x += 5 + _physical_width;
        }

        global::creatures_to_add.push_back(b);

        if (a == 0)
            b->_y_speed = 1;
        if (a == 1)
            b->_y_speed = 0;
        if (a == 2)
            b->_y_speed = -1;
    }
}

void Shotgun::equip() {
    auto *g = new GotCollectible(_x - 12, _y - 20, GotCollectible::Type::ITEM);
    global::decorations_to_add.push_back(g);

}

void Shotgun::match_animation() {

    if (sprite_state == SpriteState::W_LEFT)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spike_collectibles_flameTiles, _sprite_size, 12);
    else
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spike_collectibles_flameTiles, _sprite_size, 11);

    sprite_utils::update_frame(frameGfx, _sprite_size, mainSpriteInfo, subSpriteInfo);
}

void Shotgun::handle_shooting() {
    if (activated && !firing && cooldown > SHOTGUN_COOLDOWN) {

        mmEffect(SFX_XSHOTGUN);
        firing = true;
        cooldown = 0;
        activated = false;
        spawn_bullets();

    } else
        activated = false;

    if (blast->_anim_frame_index >= 9) {
        firing = false;
        blast->_anim_frame_index = 0;
    }

    blast->_firing = firing;

    if (!firing) {
        cooldown += *global::timer;
        if (sprite_state == SpriteState::W_LEFT)
            blast->_x = _x - 10;
        else
            blast->_x = _x + 10;
        blast->_y = _y;
        blast->_sprite_state = sprite_state;
    }
}

void Shotgun::delete_sprites() {
    delete mainSpriteInfo;
    delete subSpriteInfo;
    mainSpriteInfo = nullptr;
    subSpriteInfo = nullptr;
}

Shotgun::Shotgun(int x, int y) : BaseCreature(
        x,
        y,
        shotgun_sprite_width,
        shotgun_sprite_height,
        shotgun_spritesheet_type,
        shotgun_physical_width,
        shotgun_physical_height,
        CreatureType::SHOTGUN
), ShoppingObject(shotgun_cost, shotgun_name) {

    init_anim_icon();
    update_anim_icon(x, y, _physical_width);

    blast = new Blast(0, 0);
    global::decorations_to_add.push_back(blast);
    init_sprites();
}
