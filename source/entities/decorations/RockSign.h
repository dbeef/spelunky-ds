//
// Created by xdbeef on 28.12.18.
//

#ifndef SPELUNKYDS_MENU_ROCK_SIGN_H
#define SPELUNKYDS_MENU_ROCK_SIGN_H

#include "_BaseDecoration.h"
#include "../../memory/SpriteInfo.h"

// Represents rock sign graphic that is rendered in the main menu's upper screen,
// may take form of 'TUTORIAL' or 'QUIT'.
class RockSign : public BaseDecoration {

    // Rock sign graphic is divided into 2 chunks of 16x16 pixels, due to limitations of NDS hardware.
    static constexpr u8 rock_sign_graphics = 2;
    static constexpr u8 rock_sign_sprite_width = 16;
    static constexpr u8 rock_sign_sprite_height = 16;
    static constexpr SpritesheetType rock_sign_spritesheet_type = SpritesheetType::MENU_SIGNS_TUTORIAL_QUIT;

public:

    enum class Type {
        TUTORIAL,
        QUIT
    };

    RockSign(int x, int y, Type type);

    // BaseDecoration overrides

    void introduce_yourself() const override;

    void update_decoration_specific() override;

    // IRenderable overrides

    void init_sprites() override;

    void delete_sprites() override;

    void update_sprites_position() override;

    // Other, decoration specific

    // Rock_sign graphic is always drawn on the upper screen.
    SpriteInfo *_mainSpriteInfos[rock_sign_graphics];
    Type _type;
};

#endif //SPELUNKYDS_MENU_ROCK_SIGN_H
