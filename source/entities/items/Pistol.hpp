//
// Created by xdbeef on 18.05.18.
//

#ifndef SPELUNKYDS_PISTOL_H
#define SPELUNKYDS_PISTOL_H

#include "../items/_BaseItem.h"
#include "../_common/Orientation.hpp"
#include "../_interfaces/ShoppingObject.h"
#include "../../entities/decorations/Blast.hpp"
#include "../../GameState.hpp"

//http://spelunky.wikia.com/wiki/Pistol
class Pistol : public BaseItem, public ShoppingObject {

public:
    
    static constexpr u16 pistol_cooldown = 750;
    static constexpr int pistol_x_pickuped_offset_left = 4;
    static constexpr int pistol_x_pickuped_offset_right = 10;
    static constexpr int pistol_y_pickuped_offset = 7;
    static constexpr u16 pistol_cost = 4000;
    static constexpr const char *pistol_name = "PISTOL";
    static constexpr u8 pistol_sprite_width = 16;
    static constexpr u8 pistol_sprite_height = 16;
    static constexpr u16 pistol_physical_width = 9;
    static constexpr u16 pistol_physical_height = 6;
    static constexpr SpritesheetType pistol_spritesheet_type = SpritesheetType::SPIKES_COLLECTIBLES;

    Pistol(int x, int y) : BaseItem(
            x,
            y,
            pistol_sprite_width,
            pistol_sprite_height,
            pistol_spritesheet_type,
            pistol_physical_width,
            pistol_physical_height,
            pistol_x_pickuped_offset_left,
            pistol_x_pickuped_offset_right,
            pistol_y_pickuped_offset
    ), ShoppingObject(pistol_cost, pistol_name) {

        init_anim_icon();
        update_anim_icon(x, y, _physical_width);
        init_sprites();

        _blast = new Blast(0, 0);
        GameState::instance().decorations.push_back(_blast);
    }

    // BaseItem overrides

    void update_item_specific() override;

    void print_classname() const override;

    // IRenderable overrides

    void init_sprites() override;

    // Other, creature specific

    void equip();

    void spawn_bullet();

    void handle_shooting();

    void match_animation();

    Blast *_blast{};
    double _cooldown{};
    bool _firing{};
    Orientation _orientation;
    u8 _anim_frame_index{};
    double _anim_frame_timer{};
};


#endif //SPELUNKYDS_PISTOL_H
