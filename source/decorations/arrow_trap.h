//
// Created by xdbeef on 23.06.18.
//

#ifndef SPELUNKYDS_ARROW_TRAP_H
#define SPELUNKYDS_ARROW_TRAP_H

#include "_base_decoration.h"
#include "../creatures/_base_creature.h"
#include "../creatures/sprite_info.h"

//http://spelunky.wikia.com/wiki/Arrow_Trap
// Arrow trap's graphical representation does not base on a sprite,
// it's created in places where tile map has a tile of arrow trap.
// That way sprites are saved (which are limited on NDS), and arrow traps
// behave like parts of map anyways.
class ArrowTrap : public BaseDecoration {

    static constexpr u16 arrow_trap_physical_width = 16;
    static constexpr u16 arrow_trap_physical_height = 16;

public:

    ArrowTrap(int x, int y, SpriteState sprite_state);

    void introduce_yourself() const override { printf("LAMP\n"); };

    void update_decoration_specific() override;

    // Since arrow trap has no sprites.
    void init_sprites() override {};
    void delete_sprites() override {};
    void update_sprites_position() override {};

    // Other, creature specific

    void spawn_arrow();

    bool check_if_can_be_triggered(BaseCreature *pObject);

    bool _activated{};
    SpriteInfo *_main_sprite_info{};
    SpriteInfo *_sub_sprite_info{};
    u8 *_frame_gfx{};
    SpriteState _sprite_state;

};


#endif //SPELUNKYDS_ARROW_TRAP_H
