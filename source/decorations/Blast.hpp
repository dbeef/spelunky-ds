//
// Created by xdbeef on 23.06.18.
//

#ifndef SPELUNKYDS_BLAST_H
#define SPELUNKYDS_BLAST_H

#include "../entities/SpriteInfo.h"
#include "../entities/Orientation.hpp"
#include "_BaseDecoration.h"

// Represents blast that occurs on shooting pistol/shotgun.
// Pistol & shotgun have their instances of blast.
class Blast : public BaseDecoration {

    static constexpr u8 blast_anim_frame_delta = 30;
    static constexpr u8 blast_sprite_width = 16;
    static constexpr u8 blast_sprite_height = 16;
    static constexpr u8 blast_firing_offset_x = 14;
    static constexpr u8 blast_firing_offset_y = 6;
    static constexpr SpritesheetType blast_spritesheet_type = SpritesheetType::SPIKES_COLLECTIBLES;

public:

    Blast(int x, int y);

    // BaseDecoration overrides

    void introduce_yourself() const override;

    void update_decoration_specific() override;

    // IRenderable overrides

    void init_sprites() override;

    void delete_sprites() override;

    void update_sprites_position() override;

    // Other, decoration specific

    SpriteInfo *_sub_sprite_info{};
    SpriteInfo *_main_sprite_info{};
    u8 *_frame_gfx{};
    u8 _anim_frame_index{};
    double _anim_frame_timer{};
    bool _firing{};
    Orientation _sprite_state{};

private:

    // Updates sprites' graphics according to current animation frame index.
    void match_animation();

};


#endif //SPELUNKYDS_BLAST_H
