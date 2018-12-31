//
// Created by xdbeef on 16.09.18.
//

#ifndef SPELUNKYDS_RUBY_SMALL_H
#define SPELUNKYDS_RUBY_SMALL_H

#include <cstdlib>

#include "../_BaseCreature.h"
#include "../../interfaces/Moniez.hpp"
#include "../SpriteInfo.h"

class RubySmall : public BaseCreature, public Moniez {

public:

    static constexpr u8 ruby_small_sprite_width = 8;
    static constexpr u8 ruby_small_sprite_height = 8;
    static constexpr u16 ruby_small_physical_width = 4;
    static constexpr u16 ruby_small_physical_height = 4;
    static constexpr SpritesheetType ruby_small_spritesheet_type = SpritesheetType::MONIEZ_RUBY;

    RubySmall(int x, int y) : BaseCreature(
            x,
            y,
            ruby_small_sprite_width,
            ruby_small_sprite_height,
            ruby_small_spritesheet_type,
            ruby_small_physical_width,
            ruby_small_physical_height,
            CreatureType::RUBY_SMALL
    ) {
        _ruby_color = static_cast<RubyColor>(rand() % 3);
        init_sprites();
    }

    // Base creature overrides

    void update_creature_specific() override;

    void introduce_yourself() override { printf("WHIP\n"); };

    void apply_dmg(int dmg_to_apply) override {};

    bool can_update_collidable() const override { return true; }

    // IRenderable overrides

    void init_sprites() override;

    void delete_sprites() override;

    void update_sprites_position() override;

    // ICollidable overrides

    bool can_apply_friction() const override { return true; }

    bool can_apply_gravity() const override { return true; }

    // Other, creature specific

    //rename to vectors_update_timer?
    double pos_inc_timer{};
    SpriteInfo *mainSpriteInfo{};
    SpriteInfo *subSpriteInfo{};
    RubyColor _ruby_color;
    u8 *frameGfx{};

};


#endif //SPELUNKYDS_RUBY_SMALL_H
