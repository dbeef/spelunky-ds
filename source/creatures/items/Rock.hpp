//
// Created by xdbeef on 23.04.18.
//

#ifndef SPELUNKYDS_ROCK_H
#define SPELUNKYDS_ROCK_H

#include <nds/arm9/sprite.h>

#include "../_BaseCreature.h"
#include "../SpriteInfo.h"

//http://spelunky.wikia.com/wiki/Rock
class Rock : public BaseCreature {

public:

    static constexpr u8 rock_sprite_width = 8;
    static constexpr u8 rock_sprite_height = 8;
    static constexpr u16 rock_physical_width = 7;
    static constexpr u16 rock_physical_height = 7;
    static constexpr SpritesheetType rock_spritesheet_type = SpritesheetType::BLOOD_ROCK_ROPE_POOF;

    Rock(int x, int y) : BaseCreature(
            x,
            y,
            rock_sprite_width,
            rock_sprite_height,
            rock_spritesheet_type,
            rock_physical_width,
            rock_physical_height
    ) {
        _max_x_speed = 3.5f;
        _friction = ICollidable::default_friction * 12;
        init_sprites();
        activated = true;
    }

    // Base creature overrides

    void update_creature_specific() override;

    void introduce_yourself() override { printf("WHIP\n"); };

    void apply_dmg(int dmg_to_apply) override {};

    bool can_update_collidable() const override { return !hold_by_main_dude; }

    // IRenderable overrides

    void init_sprites() override;

    void delete_sprites() override;

    void update_sprites_position() override;

    // ICollidable overrides

    bool can_apply_friction() const override { return _bottom_collision; }

    bool can_apply_gravity() const override { return true; }

    // Other, creature specific
    
    SpriteInfo *_main_sprite_info{};
    SpriteInfo *_sub_sprite_info{};
    u8 *_frame_gfx{};

};


#endif //SPELUNKYDS_ROCK_H
