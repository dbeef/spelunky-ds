//
// Created by xdbeef on 04.04.18.
//

#ifndef SPELUNKYDS_BOMB_H
#define SPELUNKYDS_BOMB_H

#include <nds/arm9/sprite.h>

#include "../items/_BaseItem.h"
#include "../../memory/SpriteInfo.h"

#define ARMED_TIME_BLINK_SLOW 2000
#define ARMED_TIME_BLINK_FAST 3500

//http://spelunky.wikia.com/wiki/Bombs
class Bomb : public BaseItem {

    static constexpr u8 bomb_sprite_width = 8;
    static constexpr u8 bomb_sprite_height = 8;
    static constexpr u16 bomb_physical_width = 7;
    static constexpr u16 bomb_physical_height = 7;
    static constexpr SpritesheetType bomb_spritesheet_type = SpritesheetType::BOMB;

public:

    Bomb(int x, int y) : BaseItem(
            x,
            y,
            bomb_sprite_width,
            bomb_sprite_height,
            bomb_spritesheet_type,
            bomb_physical_width,
            bomb_physical_height,
            1, 10, 6
    ) {
        init_sprites();
        set_sprite_disarmed();
    }

    // Base item overrides

    void update_item_specific() override;

    void print_classname() override { printf("BOMB\n"); };

    // IRenderable overrides

    void init_sprites() override;

    // Other, creature specific

private:

    void set_sprite_disarmed();

    void set_sprite_armed();

    void explode();

    bool armed{};
    u16 armedTimer{};

};


#endif //SPELUNKYDS_BOMB_H
