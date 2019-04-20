//
// Created by xdbeef on 04.04.18.
//

#ifndef SPELUNKYDS_ROPE_H
#define SPELUNKYDS_ROPE_H

#define max_rope_chain_size 8
#define max_rope_chain_size_extended 12
#define rope_sprite_width 8
#define rope_sprite_height 8
#define rope_physical_width 8
#define rope_physical_height 8
#define rope_spritesheet_type SpritesheetType::BLOOD_ROCK_ROPE_POOF
#define rope_x_pickuped_offset_left 2
#define rope_x_pickuped_offset_right 6
#define rope_y_pickuped_offset 8

#include <nds/arm9/sprite.h>
#include <vector>

#include "../../entities/decorations/RopeElement.hpp"
#include "../items/_BaseItem.h"

//http://spelunky.wikia.com/wiki/Rope
class Rope : public BaseItem {

public:

    Rope(int x, int y) : BaseItem(
            x,
            y,
            rope_sprite_width,
            rope_sprite_height,
            rope_spritesheet_type,
            rope_physical_width,
            rope_physical_height,
            rope_x_pickuped_offset_left,
            rope_x_pickuped_offset_right,
            rope_y_pickuped_offset
    ) {
        _gravity = 0;
        _bouncing_factor_y = 0;
        _bouncing_factor_x = 0;
        init_sprites();
    }

    // BaseItem overrides

    void update_item_specific() override;

    void print_classname() override { printf("ROPE\n"); };

    // IRenderable overrides

    void init_sprites() override;

    // Other, creature specific

    void set_sprite_throwing_finished();

    void set_sprite_throwing();

    bool is_there_chain_for_this_tile(int rope_y_tiles);

    void add_rope_if_needed();

    bool _extended_rope{};
    bool _thrown{};
    bool _finished{};
    double _expand_timer{};
    double _throwing_timer{};
    std::vector<RopeElement *> _rope_chain{};

};


#endif //SPELUNKYDS_ROPE_H
