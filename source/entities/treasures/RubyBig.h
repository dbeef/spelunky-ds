//
// Created by xdbeef on 16.09.18.
//

#ifndef SPELUNKYDS_RUBY_BIG_H
#define SPELUNKYDS_RUBY_BIG_H

#include "_BaseTreasure.h"

class RubyBig : public BaseTreasure {

public:

    static constexpr u8 ruby_big_sprite_width = 8;
    static constexpr u8 ruby_big_sprite_height = 8;
    static constexpr u16 ruby_big_physical_width = 8;
    static constexpr u16 ruby_big_physical_height = 8;
    static constexpr SpritesheetType ruby_big_spritesheet_type = SpritesheetType::MONIEZ_RUBY;

    RubyBig(int x, int y);

    // IRenderable overrides (rest of overrrides are in _BaseTreasure)

    void init_sprites() override;

    // Other, creature specific

    RubyColor _ruby_color;
};


#endif //SPELUNKYDS_RUBY_BIG_H
