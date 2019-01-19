//
// Created by xdbeef on 28.12.18.
//

#ifndef SPELUNKYDS_BASE_DECORATION_H
#define SPELUNKYDS_BASE_DECORATION_H

#include "../_interfaces/IRenderable.h"

// Base class for decorations, by which I mean entities with graphical representation, that:
//
// * have no speed fields and don't apply to friction nor gravity, however their x/y position
// can be changed by other entities or in update function.
// * can't be killed (has no hitpoints),
// * can't be collided (has no physical width/height),
// * exist as long as its' ready_to_dispose field is set to false, otherwise they're deleted.
// They will be also deleted on an end of scene, i.e exiting main menu or level.
//
// Examples: Spelunky logo in main menu, copyrights info in main menu, lantern in shop, dollar icon over shop items.
class BaseDecoration : public IRenderable {
public:

    // Constructor of inheriting class should call init_sprites.
    BaseDecoration(
            int x,
            int y,
            const u16 sprite_width,
            const u16 sprite_height,
            const SpritesheetType spritesheet_type) :
            IRenderable(sprite_width, sprite_height, spritesheet_type) {
        set_xy(x, y);
    }

    inline void update(){
        update_sprites_position();
        update_decoration_specific();
    }

    virtual ~BaseDecoration() = default;

    // Will be called every frame in game loop.
    virtual void update_decoration_specific() = 0;

    // Should print its name upper-cased with newline character.
    virtual void introduce_yourself() const = 0;

};

#endif //SPELUNKYDS_BASE_DECORATION_H
