//
// Created by xdbeef on 19.05.18.
//

#ifndef SPELUNKYDS_GOT_COLLECTIBLE_H
#define SPELUNKYDS_GOT_COLLECTIBLE_H

#include "../creatures/sprite_info.h"
#include "../creatures/sprite_state.hpp"
#include "_base_decoration.h"

//Represents animation that is launched on collecting new item/bombs/ropes.
class GotCollectible : public BaseDecoration {

    static constexpr u8 got_collectible_sprite_width = 32;
    static constexpr u8 got_collectible_sprite_height = 32;
    static constexpr u16 got_collectible_anim_frame_delta = 90;
    static constexpr u16 got_collectible_cycles_to_hide = 10;
    static constexpr SpritesheetType got_collectible_spritesheet_type = SpritesheetType::GOT_COLLECTIBLE;

public:

    enum class Type {
        ITEM,
        BOMB,
        ROPE
    };

    GotCollectible(int x, int y, Type type);

    void introduce_yourself() const override { printf("GOT COLLECTIBLE\n"); };

    void update_decoration_specific() override;

    void init_sprites() override;

    void delete_sprites() override;

    void update_sprites_position() override;

    double _anim_frame_timer{};
    u8 _anim_frame_index{};
    u8 _cycle_counter{};
    SpriteInfo *_main_sprite_info{};
    SpriteInfo *_sub_sprite_info{};
    u8 *_frame_gfx{};
    Type _type{};

private:

    //Updates sprites' graphics according to current animation frame index.
    void match_animation();

};

#endif //SPELUNKYDS_GOT_COLLECTIBLE_H
