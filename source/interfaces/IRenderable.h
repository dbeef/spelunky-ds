//
// Created by xdbeef on 29.12.18.
//

#ifndef SPELUNKYDS_IRENDERABLE_H
#define SPELUNKYDS_IRENDERABLE_H

#include <nds.h>
#include "../creatures/spritesheet_type.hpp"
#include "IBase.h"

// Renderable provides fields neccesary to render graphic on screen, which are:
// * xy position
// * sprite width/height
// * sprite size (width x height)
// * spritesheet type (palette)
class IRenderable : public virtual IBase {

public:

    static constexpr u16 calc_sprite_size(const u16 sprite_width, const u16 sprite_height) {
        return sprite_width * sprite_height;
    }

    // Constructor of inheriting class should call init_sprites.
    IRenderable(const u16 sprite_width, const u16 sprite_height, const SpritesheetType spritesheet_type) :
            _sprite_width(sprite_width),
            _sprite_height(sprite_height),
            _sprite_size(calc_sprite_size(sprite_width, sprite_height)),
            _spritesheet_type(spritesheet_type) {
        //do nothing
    }

    virtual ~IRenderable() = default;

    const u16 _sprite_width;
    const u16 _sprite_height;
    const u16 _sprite_size;
    const SpritesheetType _spritesheet_type;

    //Corrects passed x/y positions to current viewport for both upper and lower screen.
    void get_x_y_viewported(int *out_main_x, int *out_main_y, int *out_sub_x, int *out_sub_y) const;

};


#endif //SPELUNKYDS_IRENDERABLE_H