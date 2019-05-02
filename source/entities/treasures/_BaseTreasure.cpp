//
// Created by xdbeef on 31.12.18.
//


#include "_BaseTreasure.h"


#include "../../collisions/Collisions.hpp"
#include "../../../build/soundbank.h"

#include "../../GameState.hpp"
#include "../../time/Timer.h"
#include "../../graphics/SpriteUtils.hpp"
#include "../../sound/Sound.hpp"

void BaseTreasure::update_sprites_position() {
    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);
    sprite_utils::set_entry_xy(_main_sprite_info, static_cast<u16>(main_x), static_cast<u16>(main_y));
    sprite_utils::set_entry_xy(_sub_sprite_info, static_cast<u16>(sub_x), static_cast<u16>(sub_y));
}

void BaseTreasure::delete_sprites() {
    delete _main_sprite_info;
    delete _sub_sprite_info;
    _main_sprite_info = nullptr;
    _sub_sprite_info = nullptr;
}

void BaseTreasure::update() {

    if (_ready_to_dispose) return;

    update_collidable();

    update_sprites_position();

    // TODO Call checking this not every frame, but:
    // when collisions with map checked on treasure OR
    // when collisions with map checked on main dude

    if (_collectible_timer >= 500 &&
        Collisions::checkCollisionWithMainDudeWidthBoundary(_x, _y, _physical_width, _physical_height, 8)) {

        if (_spritesheet_type == SpritesheetType::MONIEZ_RUBY)
            sound::gem();
        else if (_spritesheet_type == SpritesheetType::MONIEZ_GOLDBARS)
            sound::coin();

        Hud::instance().add_moniez_on_collected_loot(get_dollars_value());
        sprite_utils::set_visibility(false, _main_sprite_info, _sub_sprite_info);
        GameState::instance().collected_treasures.push_back(_treasure_type);
        _ready_to_dispose = true;
    } else if (_collectible_timer < 500) {
        _collectible_timer += Timer::getDeltaTime();
    }

}

u16 BaseTreasure::get_dollars_value() const {
    switch (_treasure_type) {
        case TreasureType::RUBY_BIG:
            return 1200;
        case TreasureType::RUBY_SMALL:
            return 1000;
        case TreasureType::TRIPLE_GOLDBAR:
            return 1000;
        case TreasureType::SINGLE_GOLDBAR:
            return 500;
        default:
            return 0;
    }
}

BaseTreasure::BaseTreasure(int x, int y, const u16 sprite_width, const u16 sprite_height,
                           const SpritesheetType spritesheet_type, u16 physical_width, u16 physical_height,
                           TreasureType treasure_type) :
        IRenderable(sprite_width, sprite_height, spritesheet_type),
        ICollidable(physical_width, physical_height),
        _collectible_timer(500),
        _treasure_type(treasure_type) {
    set_xy(x, y);
}
