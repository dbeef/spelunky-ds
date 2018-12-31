//
// Created by xdbeef on 10.06.18.
//

#ifndef SPELUNKYDS_SMOOCH_H
#define SPELUNKYDS_SMOOCH_H

#include "../creatures/sprite_info.h"
#include "../creatures/sprite_state.hpp"
#include "../creatures/_base_creature.h"
#include "_base_decoration.h"

class Smooch : public BaseDecoration {

    static constexpr u8 smooch_sprite_width = 16;
    static constexpr u8 smooch_sprite_height = 16;
    static constexpr u16 smooch_anim_frame_delta = 70;
    //heart will bump this many times before disappearing
    static constexpr u16 smooch_cycles_to_hide = 25;
    static constexpr SpritesheetType smooch_spritesheet_type = SpritesheetType::CAVEMAN_DAMSEL;

public:

    Smooch(int x, int y);

    void introduce_yourself() const override { printf("SMOOCH\n"); };

    void update_decoration_specific() override;

    void init_sprites() override;

    void delete_sprites() override;

    void update_sprites_position() override;

    SpriteInfo *_main_sprite_info{};
    SpriteInfo *_sub_sprite_info{};
    u8 *_frame_gfx{};
    u8 _anim_frame_index{};
    u8 _cycle_counter{};
    double _anim_frame_timer{};

private:

    //Updates sprites' graphics according to current animation frame index.
    void match_animation();
};

#endif //SPELUNKYDS_SMOOCH_H
