//
// Created by xdbeef on 15.05.18.
//

#ifndef SPELUNKYDS_SPIKES_H
#define SPELUNKYDS_SPIKES_H

#include "../_base_creature.h"
#include "../sprite_info.h"

//http://spelunky.wikia.com/wiki/Spikes
class Spikes : public BaseCreature {
    
    static constexpr u8 spikes_sprite_width = 16;
    static constexpr u8 spikes_sprite_height = 16;
    static constexpr u8 spikes_physical_width = 16;
    static constexpr u8 spikes_physical_height = 13;
    static constexpr SpritesheetType spikes_spritesheet_type = SpritesheetType::SPIKES_COLLECTIBLES;

public:

    Spikes(int x, int y) : BaseCreature(
            x,
            y,
            spikes_sprite_width,
            spikes_sprite_height,
            spikes_spritesheet_type,
            spikes_physical_width,
            spikes_physical_height
    ) {
        init_sprites();
    }
    
    // Base creature overrides

    void update_creature_specific() override;

    void introduce_yourself() override { printf("WHIP\n"); };

    void apply_dmg(int dmg_to_apply) override {};

    void onCollisionWithMainCharacter() override {};

    // IRenderable overrides

    void init_sprites() override;

    void delete_sprites() override;

    void update_sprites_position() override;

    // ICollidable overrides

    // Spikes are completely static.
    bool can_apply_friction() override { return false; }
    bool can_update_collidable() override { return false; }
    bool can_apply_gravity() override { return false; }

    // Other, creature specific

    bool _blood{};
    SpriteInfo *_main_sprite_info{};
    SpriteInfo *_sub_sprite_info{};
    u8 *_frame_gfx{};

};


#endif //SPELUNKYDS_SPIKES_H
