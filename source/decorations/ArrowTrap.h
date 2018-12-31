//
// Created by xdbeef on 23.06.18.
//

#ifndef SPELUNKYDS_ARROW_TRAP_H
#define SPELUNKYDS_ARROW_TRAP_H

#include "../creatures/_BaseCreature.h"
#include "../creatures/SpriteInfo.h"
#include "_BaseDecoration.h"

// http://spelunky.wikia.com/wiki/Arrow_Trap
// Arrow trap's graphical representation does not base on a sprite,
// it's created in places where tile map has a tile of arrow trap.
// That way sprites are saved (which are limited on NDS), and arrow traps
// behave like parts of map anyways.
class ArrowTrap : public BaseDecoration {

    static constexpr u16 arrow_trap_physical_width = 16;
    static constexpr u16 arrow_trap_physical_height = 16;

public:

    ArrowTrap(int x, int y, SpriteState sprite_state);

    // BaseDecoration overrides

    void introduce_yourself() const override;

    void update_decoration_specific() override;

    // IRenderable overrides

    void init_sprites() override;

    void delete_sprites() override;

    void update_sprites_position() override;

    // Other, creature specific

private:

    void spawn_arrow() const;

    // Check if there's any moving object in 6 tiles starting from the trap at the same height,
    // if so, then spawn arrow and set as activated.
    bool check_if_can_be_triggered(BaseCreature *pObject);

    // true, if already thrown arrow
    bool _activated{};
    SpriteInfo *_main_sprite_info{};
    SpriteInfo *_sub_sprite_info{};
    u8 *_frame_gfx{};
    SpriteState _sprite_state;

};


#endif //SPELUNKYDS_ARROW_TRAP_H
