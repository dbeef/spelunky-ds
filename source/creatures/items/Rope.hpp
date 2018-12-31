//
// Created by xdbeef on 04.04.18.
//

#ifndef SPELUNKYDS_ROPE_H
#define SPELUNKYDS_ROPE_H

#define max_rope_chain_size 8
#define max_rope_chain_size_extended 12
#define rope_sprite_width 8
#define rope_sprite_height 8
#define rope_physical_width 8
#define rope_physical_height 8
#define rope_spritesheet_type SpritesheetType::BLOOD_ROCK_ROPE_POOF

#include <nds/arm9/sprite.h>
#include <vector>
#include "RopeElement.hpp"

//http://spelunky.wikia.com/wiki/Rope
class Rope : public BaseCreature {

public:

    Rope(int x, int y) : BaseCreature(
            x,
            y,
            rope_sprite_width,
            rope_sprite_height,
            rope_spritesheet_type,
            rope_physical_width,
            rope_physical_height
    ) {
        _bouncing_factor_y = 0;
        _bouncing_factor_x = 0;
        init_sprites();
    }

    // Base creature overrides

    void update_creature_specific() override;

    void introduce_yourself() override { printf("WHIP\n"); };

    void apply_dmg(int dmg_to_apply) override {};

    bool can_update_collidable() const override { return !hold_by_main_dude && !_finished; }

    // IRenderable overrides

    void init_sprites() override;

    void delete_sprites() override;

    void update_sprites_position() override;

    // ICollidable overrides

    bool can_apply_friction() const override { return false; }

    bool can_apply_gravity() const override { return false; }

    // Other, creature specific

    void set_sprite_throwing_finished();

    void set_sprite_throwing();

    bool isThereChainForThisTile(int rope_y_tiles);

    void add_rope_if_needed();

    SpriteInfo *_main_sprite_info{};
    SpriteInfo *_sub_sprite_info{};
    u8 *_frame_gfx{};
    bool _extended_rope{};
    bool _thrown{};
    bool _finished{};
    double _expand_timer{};
    double _throwing_timer{};
    std::vector<RopeElement *> _rope_chain{};

};


#endif //SPELUNKYDS_ROPE_H
