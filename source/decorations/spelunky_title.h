//
// Created by xdbeef on 29.04.18.
//

#ifndef SPELUNKYDS_SPELUNKY_TITLE_H
#define SPELUNKYDS_SPELUNKY_TITLE_H

#include "_base_decoration.h"

//Represents spelunky_title graphic that is rendered in the main menu's upper screen.
class SpelunkyTitle : public BaseDecoration {

    //Spelunky_title graphic is divided into 3 chunks of 64x64 pixels, due to limitations of NDS hardware.
    static constexpr u8 spelunky_title_graphics = 3;
    static constexpr u8 spelunky_title_sprite_width = 64;
    static constexpr u8 spelunky_title_sprite_height = 64;
    static constexpr SpritesheetType spelunky_title_spritesheet_type = SpritesheetType::SPELUNKY_TITLE;

public:

    SpelunkyTitle(int x, int y);

    void introduce_yourself() const override { printf("SPELUNKY TITLE\n"); };

    void update_decoration_specific() override {};

    void init_sprites() override;

    void delete_sprites() override;

    void update_sprites_position() override;

    //Spelunky_title graphic is always drawn on the upper screen.
    SpriteInfo *_mainSpriteInfos[spelunky_title_graphics];

};

#endif //SPELUNKYDS_SPELUNKY_TITLE_H
