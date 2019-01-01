//
// Created by xdbeef on 03.05.18.
// http://spelunky.wikia.com/wiki/Spider
//

#ifndef SPELUNKYDS_SPIDER_H
#define SPELUNKYDS_SPIDER_H

#include "../_BaseCreature.h"
#include "../Orientation.hpp"
#include "../SpriteInfo.h"

//http://spelunky.wikia.com/wiki/Spider
class Spider : public BaseCreature {

public:

    static constexpr u8 spider_sprite_width = 16;
    static constexpr u8 spider_sprite_height = 16;
    static constexpr u16 spider_physical_width = 16;
    static constexpr u16 spider_physical_height = 8;
    static constexpr SpritesheetType spider_spritesheet_type = SpritesheetType::SKELETON_SPIDER;
    
    Spider(int x, int y) : BaseCreature(
            x,
            y,
            spider_sprite_width,
            spider_sprite_height,
            spider_spritesheet_type,
            spider_physical_width,
            spider_physical_height,
            CreatureType::SPIDER
    ) {
        hitpoints = 1;
        hanging = true;
        init_sprites();
        _pos_update_delta = 30;
    }

    // Base creature overrides

    void update_creature_specific() override;

    void introduce_yourself() override { printf("WHIP\n"); };

    void apply_dmg(int dmg_to_apply) override;

    bool can_update_collidable() const override { return !hold_by_main_dude; }

    // IRenderable overrides

    void init_sprites() override;

    void delete_sprites() override;

    void update_sprites_position() override;

    // ICollidable overrides

    bool can_apply_friction() const override { return true; }

    bool can_apply_gravity() const override { return hunting; }

    // Other, creature specific

    void set_sprite_hanging();

    void set_sprite_flipping();

    void set_sprite_jumping();

    void set_sprite_falling();

    void jump_to_main_dude();

    void match_animation();
    
    double pos_inc_timer{};

    SpriteInfo *mainSpriteInfo {};
    SpriteInfo *subSpriteInfo {};
    u8 *frameGfx{};

    int animFrame{};
    double animFrameTimer{};
    double time_since_last_jump{};
    double jump_delay{};
    double random_speed{};
    double time_since_last_big_jump{};

    bool previously_collided{};
    Orientation previous_collision_side{};

    bool hanging{};
    bool hunting{};
    bool jumping{};

};


#endif //SPELUNKYDS_SPIDER_H
