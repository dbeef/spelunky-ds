//
// Created by xdbeef on 16.05.18.
//

#ifndef SPELUNKYDS_CRATE_H
#define SPELUNKYDS_CRATE_H


#include "../_BaseCreature.h"
#include "../../interfaces/ShoppingObject.h"

//http://spelunky.wikia.com/wiki/Crate
class Crate: public BaseCreature {

public:

    static constexpr u8 crate_sprite_width = 16;
    static constexpr u8 crate_sprite_height = 16;
    static constexpr u16 crate_physical_width = 12;
    static constexpr u16 crate_physical_height = 10;
    static constexpr SpritesheetType crate_spritesheet_type = SpritesheetType::SPIKES_COLLECTIBLES;

    Crate(int x, int y) : BaseCreature(
            x,
            y,
            crate_sprite_width,
            crate_sprite_height,
            crate_spritesheet_type,
            crate_physical_width,
            crate_physical_height
    ) {
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

    void drop_loot();

    void play_collectible_animation();
    
    double pos_inc_timer{};
    SpriteInfo *mainSpriteInfo {};
    SpriteInfo *subSpriteInfo {};
    u8 * frameGfx{};
    bool dropped_loot{};
    int animFrame{};
    double animFrameTimer{};
    
};





#endif //SPELUNKYDS_CRATE_H
