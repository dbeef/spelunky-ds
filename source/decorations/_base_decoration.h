//
// Created by xdbeef on 28.12.18.
//

#ifndef SPELUNKYDS_BASE_DECORATION_H
#define SPELUNKYDS_BASE_DECORATION_H

#include <cstdio>

#include "../memory/oam_manager.hpp"
#include "../camera/camera.hpp"
#include "../creatures/spritesheet_type.hpp"
#include "../creatures/sprite_state.hpp"
#include "../creatures/sprite_type.hpp"

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
class BaseDecoration {
public:

    static constexpr u16 calc_sprite_size(const u16 sprite_width, const u16 sprite_height) {
        return sprite_width * sprite_height;
    }

    // Constructor of inheriting class should call init_sprites.
    BaseDecoration(int x, int y, const u16 sprite_width, const u16 sprite_height,
                   const SpritesheetType spritesheet_type) :
            _x(x),
            _y(y),
            _sprite_width(sprite_width),
            _sprite_height(sprite_height),
            _sprite_size(calc_sprite_size(sprite_width, sprite_height)),
            _spritesheet_type(spritesheet_type) {
        //do nothing
    }

    virtual ~BaseDecoration() = default;

    // Will be called every frame in game loop.
    virtual void update() = 0;

    // Should print its name upper-cased with newline character.
    virtual void introduce_yourself() const = 0;

    // Should obtain sprites via OAM manager.
    // Will be called on object initialization and every OAM clean.
    // Should call update_sprites_position.
    virtual void init_sprites() = 0;

    // Should call delete on its SpriteInfo pointers and nullptr them.
    virtual void delete_sprites() = 0;

    // Should set sprites' position to _x/_y values, with applied viewport.
    // Will be called every frame, since both viewport and _x/_y can change.
    virtual void update_sprites_position() = 0;

    int _x;
    int _y;
    bool _ready_to_dispose{};
    const u16 _sprite_width;
    const u16 _sprite_height;
    const u16 _sprite_size;
    const SpritesheetType _spritesheet_type;

protected:

    //Corrects passed x/y positions to current viewport for both upper and lower screen.
    void get_x_y_viewported(int *out_main_x, int *out_main_y, int *out_sub_x, int *out_sub_y) const;

};

#endif //SPELUNKYDS_BASE_DECORATION_H
