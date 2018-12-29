//
// Created by xdbeef on 17.06.18.
//

#ifndef SPELUNKYDS_SHOPPING_ICON_H
#define SPELUNKYDS_SHOPPING_ICON_H

#include "_base_decoration.h"

class ShoppingIcon : public BaseDecoration {

    static constexpr u8 shopping_icon_sprite_width = 16;
    static constexpr u8 shopping_icon_sprite_height = 16;
    static constexpr u16 shopping_icon_anim_frame_delta = 25;
    static constexpr u16 shopping_icon_trigger_delta = 300; //will trigger animation every 300 ms
    static constexpr SpritesheetType shopping_icon_spritesheet_type = SpritesheetType::MONIEZ_GOLDBARS;

public:

    ShoppingIcon(int x, int y);

    void introduce_yourself() const override { printf("LAMP\n"); };

    void update() override;

    void init_sprites() override;

    void delete_sprites() override;

    void update_sprites_position() override;

    // Hides sprites and sets as ready to dispose.
    void set_ready_to_dispose();

    SpriteInfo *_main_sprite_info{};
    SpriteInfo *_sub_sprite_info{};
    u8 _anim_frame_index{};
    double _anim_frame_timer{};
    double _trigger_timer{};

private:
    // Updates sprites' graphics according to current animation frame index.
    void match_animation();
};




#endif //SPELUNKYDS_SHOPPING_ICON_H