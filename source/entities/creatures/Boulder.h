//
// Created by dbeef on 2/17/19.
//

#ifndef SPELUNKYDS_BOULDER_H
#define SPELUNKYDS_BOULDER_H



#include "_BaseCreature.h"
#include "../_common/Orientation.hpp"


//http://spelunky.wikia.com/wiki/Boulder
class Boulder : public BaseCreature {

public:

    static constexpr u8 boulder_sprite_width = 32;
    static constexpr u8 boulder_sprite_height = 32;
    static constexpr u32 boulder_physical_width = 32;
    static constexpr u32 boulder_physical_height = 32;
    static constexpr SpritesheetType boulder_spritesheet_type = SpritesheetType::GOT_COLLECTIBLE;

    Boulder(int x, int y) : BaseCreature(
            x,
            y,
            boulder_sprite_width,
            boulder_sprite_height,
            boulder_spritesheet_type,
            boulder_physical_width,
            boulder_physical_height,
            CreatureType::BOULDER
    ) {
        init_sprites();
        hitpoints = 999;
        _pos_update_delta = 35;
    }

    // Base creature overrides

    void update_creature_specific() override;

    void introduce_yourself() override { printf("WHIP\n"); };

    void apply_dmg(int dmg_to_apply) override;

    bool can_update_collidable() const override { return true; }

    // IRenderable overrides

    void init_sprites() override;

    void delete_sprites() override;

    void update_sprites_position() override;

    // ICollidable overrides

    bool can_apply_friction() const override { return true; }

    bool can_apply_gravity() const override { return true; }

    // Other, creature specific

    void match_animation();

    SpriteInfo *mainSpriteInfo{};
    SpriteInfo *subSpriteInfo{};
    u8 *frameGfx{};
    int sameDirectionInRow{};
    //Boulder goes for random amount of time_utils on random direction, then waits random time_utils and the cycle goes again
    Orientation spriteState{};

    int animFrame{};
    double animFrameTimer{};

};

#endif //SPELUNKYDS_BOULDER_H
