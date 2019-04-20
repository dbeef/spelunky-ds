//
// Created by xdbeef on 28.12.18.
//

#ifndef SPELUNKYDS_MENU_ARCH_SIGN_H
#define SPELUNKYDS_MENU_ARCH_SIGN_H

#include "../../memory/SpriteInfo.h"
#include "_BaseDecoration.h"

// Represents arch sign graphic that is rendered in the main menu's upper screen,
// may take form of 'START' or 'SCORES'.
class ArchSign : public BaseDecoration {

    // Arch sign graphic is divided into 2 chunks of 32x32 pixels, due to limitations of NDS hardware.
    static constexpr u8 rock_sign_graphics = 2;
    static constexpr u8 rock_sign_sprite_width = 32;
    static constexpr u8 rock_sign_sprite_height = 32;
    static constexpr SpritesheetType rock_sign_spritesheet_type = SpritesheetType::MENU_SIGNS_START_SCORES;

public:

    enum class Type {
        START,
        SCORES
    };

    ArchSign(int x, int y, Type type);

    // BaseDecoration overrides

    void print_classname() const override;

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

#endif //SPELUNKYDS_MENU_ARCH_SIGN_H
