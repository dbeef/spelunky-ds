//
// Created by xdbeef on 10.06.18.
//

#ifndef SPELUNKYDS_SMOOCH_H
#define SPELUNKYDS_SMOOCH_H

#include "../entities/SpriteInfo.h"
#include "_BaseDecoration.h"

class Smooch : public BaseDecoration {

    static constexpr u8 smooch_sprite_width = 16;
    static constexpr u8 smooch_sprite_height = 16;
    static constexpr u16 smooch_anim_frame_delta = 70;
    // Heart will bump this many times before disappearing
    static constexpr u16 smooch_cycles_to_hide = 25;
    static constexpr SpritesheetType smooch_spritesheet_type = SpritesheetType::CAVEMAN_DAMSEL;

public:

    Smooch(int x, int y);

    // BaseDecoration overrides

    void introduce_yourself() const override;

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
    u8 _cycle_counter{};
    double _anim_frame_timer{};

private:

    // Updates sprites' graphics according to current animation frame index.
    inline void match_animation();
};

#endif //SPELUNKYDS_SMOOCH_H
