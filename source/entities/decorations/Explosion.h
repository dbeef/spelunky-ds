//
// Created by dbeef on 19.01.19.
//

#ifndef SPELUNKYDS_EXPLOSION_H
#define SPELUNKYDS_EXPLOSION_H

#include "../../memory/SpriteInfo.h"
#include "_BaseDecoration.h"

class Explosion : public BaseDecoration {

    static constexpr u8 explosion_sprite_width = 64;
    static constexpr u8 explosion_sprite_height = 64;
    static constexpr SpritesheetType explosion_spritesheet_type = SpritesheetType::BOMB;

public:

    Explosion(int x, int y);

    // BaseDecoration overrides

    void print_classname() const override;

    void update_decoration_specific() override;

    // IRenderable overrides

    void init_sprites() override;

    void delete_sprites() override;

    void update_sprites_position() override;

    // Other, decoration specific

    SpriteInfo *_main_sprite_info{};
    SpriteInfo *_sub_sprite_info{};

    u8 _current_frame_index{};
    double _explosion_timer{};
};


#endif //SPELUNKYDS_EXPLOSION_H
