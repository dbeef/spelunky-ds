//
// Created by xdbeef on 16.09.18.
//

#ifndef SPELUNKYDS_SIMPLE_SPRITE_WRAPPER_H
#define SPELUNKYDS_SIMPLE_SPRITE_WRAPPER_H

#include "sprite_info.h"
#include "moving_object.h"
#include "../../build/gfx_bat_snake_jetpack.h"

/**
 * TODO Finish this class.
 * TODO Implement usage of this class - most of the MovingObjects have only 1 sprite per engine so it fits perfectly.
 * 'Simple', because it wraps the simpliest case, where only
 * one sprite per engine is needed, unlike in the case of, let's say, main menu's Spelunky logo,
 * where a few sprites need to be combined to display the whole image.
 * Pros of combining with MovingObject:
 * > (possibly, not implemented yet) writing set_position once and calling everywhere
 * > (possibly, not implemented yet) writing init_sprite once and calling everywhere
 * > (possibly, not implemented yet) writing match_animation once and calling everywhere
 */
class SimpleSpriteWrapper : public MovingObject {

public:

    SpriteInfo *main_sprite_info{};
    SpriteInfo *sub_sprite_info{};
    u8 *frameGfx{};

    void set_position();
};

#endif //SPELUNKYDS_SIMPLE_SPRITE_WRAPPER_H
