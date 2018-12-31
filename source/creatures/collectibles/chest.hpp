//
// Created by xdbeef on 16.05.18.
//

#ifndef SPELUNKYDS_CHEST_H
#define SPELUNKYDS_CHEST_H

#include "../_base_creature.h"
#include "../sprite_info.h"

//http://spelunky.wikia.com/wiki/Chest
class Chest : public BaseCreature {

public:

    static constexpr u8 chest_sprite_width = 16;
    static constexpr u8 chest_sprite_height = 16;
    static constexpr u16 chest_physical_width = 12;
    static constexpr u16 chest_physical_height = 8;
    static constexpr SpritesheetType chest_spritesheet_type = SpritesheetType::SPIKES_COLLECTIBLES;

    Chest(int x, int y) : BaseCreature(
            x,
            y,
            chest_sprite_width,
            chest_sprite_height,
            chest_spritesheet_type,
            chest_physical_width,
            chest_physical_height
    ) {
        _friction = 0.1f;
        _bouncing_factor_y = ICollidable::default_bouncing_factor_y * 0.5f;
        _max_x_speed = 1.25f;
        _pos_update_delta = 15;
        init_sprites();
    }

    // Base creature overrides

    void update_creature_specific() override;

    void introduce_yourself() override { printf("WHIP\n"); };

    void apply_dmg(int dmg_to_apply) override {};

    // IRenderable overrides

    void init_sprites() override;

    void delete_sprites() override;

    void update_sprites_position() override;

    // ICollidable overrides

    bool can_update_collidable() override { return true; }

    bool can_apply_friction() override { return _bottom_collision; }

    bool can_apply_gravity() override { return true; }

    // Other, creature specific

    void spawn_treasure();

    void match_animation();

    // Other, creature specific

    SpriteInfo *mainSpriteInfo{};
    SpriteInfo *subSpriteInfo{};
    u8 *frameGfx{};
};

#endif //SPELUNKYDS_CHEST_H
