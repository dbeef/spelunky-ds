//
// Created by xdbeef on 14.06.18.
//

#ifndef SPELUNKYDS_LAMP_H
#define SPELUNKYDS_LAMP_H

#include "_BaseDecoration.h"
#include "../../memory/SpriteInfo.h"

// http://spelunky.wikia.com/wiki/Lantern
class Lamp : public BaseDecoration {

    static constexpr u8 lamp_sprite_width = 16;
    static constexpr u8 lamp_sprite_height = 16;
    static constexpr u16 lamp_anim_frame_delta = 60;
    static constexpr SpritesheetType lamp_spritesheet_type = SpritesheetType::SHOPKEEPER;

public:

    Lamp(int x, int y);

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
    u8 *_frame_gfx{};
    u8 _anim_frame_index{};
    double _anim_frame_timer{};

private:

    // Updates sprites' graphics according to current animation frame index.
    void match_animation();
};


#endif //SPELUNKYDS_LAMP_H
