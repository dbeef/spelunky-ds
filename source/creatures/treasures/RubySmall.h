//
// Created by xdbeef on 16.09.18.
//

#ifndef SPELUNKYDS_RUBY_SMALL_H
#define SPELUNKYDS_RUBY_SMALL_H

#include "_BaseTreasure.h"

class RubySmall : public BaseTreasure {

public:

    static constexpr u8 ruby_small_sprite_width = 8;
    static constexpr u8 ruby_small_sprite_height = 8;
    static constexpr u16 ruby_small_physical_width = 4;
    static constexpr u16 ruby_small_physical_height = 4;
    static constexpr SpritesheetType ruby_small_spritesheet_type = SpritesheetType::MONIEZ_RUBY;

    RubySmall(int x, int y);

    // IRenderable overrides (rest of overrrides are in _BaseTreasure)

    void init_sprites() override;

    // Other, creature specific

    RubyColor _ruby_color;
};


#endif //SPELUNKYDS_RUBY_SMALL_H
