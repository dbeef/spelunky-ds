//
// Created by xdbeef on 29.12.18.
//

#ifndef SPELUNKYDS_BASETREASURE_H
#define SPELUNKYDS_BASETREASURE_H


#include "../interfaces/IRenderable.h"
#include "../interfaces/ICollidable.h"

class BaseTreasure : public IRenderable, public ICollidable {

public:

    // Constructor of inheriting class should call init_sprites.
    BaseTreasure(
            int x,
            int y,
            const u16 sprite_width,
            const u16 sprite_height,
            const SpritesheetType spritesheet_type,
            u16 physical_width,
            u16 physical_height
    ) :
            IRenderable(sprite_width, sprite_height, spritesheet_type),
            ICollidable(physical_width, physical_height) {
        set_xy(x, y);
    }

    virtual ~BaseTreasure() = default;

    // Will be called every frame in game loop.
    virtual void update() = 0;

    // Should print its name upper-cased with newline character.
    virtual void introduce_yourself() const = 0;

private:



};


#endif //SPELUNKYDS_BASETREASURE_H
