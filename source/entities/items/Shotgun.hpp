//
// Created by xdbeef on 17.05.18.
//

#ifndef SPELUNKYDS_SHOTGUN_H
#define SPELUNKYDS_SHOTGUN_H

#include "../items/_BaseItem.h"
#include "../../interfaces/ShoppingObject.h"
#include "../../decorations/Blast.hpp"

//http://spelunky.wikia.com/wiki/Shotgun
class Shotgun: public BaseItem, public ShoppingObject {

public:

    static constexpr u16 shotgun_cost = 13500;
    static constexpr const char *shotgun_name = "SHOTGUN";
    static constexpr u8 shotgun_sprite_width = 16;
    static constexpr u8 shotgun_sprite_height = 16;
    static constexpr u16 shotgun_physical_width = 13;
    static constexpr u16 shotgun_physical_height = 7;
    static constexpr u16 shotgun_cooldown = 750;
    static constexpr u16 shotgun_offset_x_left = 4;
    static constexpr u16 shotgun_offset_x_right = 7;
    static constexpr u16 shotgun_offset_y = 7;
    static constexpr SpritesheetType shotgun_spritesheet_type = SpritesheetType::SPIKES_COLLECTIBLES;

    Shotgun(int x, int y);

    // Base item overrides

    void update_item_specific() override;

    // IRenderable overrides

    void init_sprites() override;

    // Other, creature specific

    void spawn_bullets();

    void equip();

    void match_animation();

    void handle_shooting();

    bool _hold_by_anyone;
    Blast *blast{};
    double cooldown{};
    bool firing{};
    Orientation  _orientation;

};

#endif //SPELUNKYDS_SHOTGUN_H
