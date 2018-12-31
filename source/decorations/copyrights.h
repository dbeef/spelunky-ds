//
// Created by xdbeef on 29.04.18.
//

#ifndef SPELUNKYDS_COPYRIGHTS_H
#define SPELUNKYDS_COPYRIGHTS_H

#include "../creatures/sprite_info.h"
#include "_base_decoration.h"

// Represents copyrights graphic that is rendered in the main menu's lower screen.
class Copyrights : public BaseDecoration {

    // Copyrights graphic is divided into 5 chunks of 32x32 pixels, due to limitations of NDS hardware.
    static constexpr u8 copyrights_graphics = 5;
    static constexpr u8 copyrights_sprite_width = 32;
    static constexpr u8 copyrights_sprite_height = 32;
    static constexpr SpritesheetType copyrights_spritesheet_type = SpritesheetType::SPELUNKY_COPYRIGHTS;

public:

    Copyrights(int x, int y);

    // BaseDecoration overrides

    void introduce_yourself() const override;

    void update_decoration_specific() override;

    // IRenderable overrides

    void init_sprites() override;

    void delete_sprites() override;

    void update_sprites_position() override;

    // Other, decoration specific

    // Copyrights graphic is always drawn on the lower screen.
    SpriteInfo *_subSpriteInfos[copyrights_graphics];

};


#endif //SPELUNKYDS_COPYRIGHTS_H
