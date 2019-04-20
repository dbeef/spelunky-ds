//
// Created by xdbeef on 04.04.18.
//

#ifndef SPELUNKYDS_ROPE_ELEMENT_H
#define SPELUNKYDS_ROPE_ELEMENT_H

#include <nds/arm9/sprite.h>
#include <vector>

#include "../items/Rope.hpp"
#include "../../camera/Camera.hpp"
#include "../../memory/SpriteInfo.h"
#include "_BaseDecoration.h"

// Represents a single 16x16 element in rope chain.
class RopeElement : public BaseDecoration {
    
public:

    RopeElement(int x, int y) : BaseDecoration(
            x,
            y,
            rope_sprite_width,
            rope_sprite_height,
            rope_spritesheet_type
    ) {
        init_sprites();
    }

    // BaseDecoration overrides

    void print_classname() const override;

    void update_decoration_specific() override;

    // IRenderable overrides

    void init_sprites() override;

    void delete_sprites() override;

    void update_sprites_position() override;

    // Other, creature specific

    void update_visibility();

    double _active_timer{};
    bool _active{};

    // To make creating rope chain smoother, instead of 16x16 rope chunks,
    // there are 8x8 chunks, two per RopeElement, created separately in delay.
    SpriteInfo *_main_sprite_info{};
    SpriteInfo *_sub_sprite_info{};
    SpriteInfo *_main_sprite_info_2{};
    SpriteInfo *_sub_sprite_info_2{};

};


#endif //SPELUNKYDS_ROPE_ELEMENT_H
