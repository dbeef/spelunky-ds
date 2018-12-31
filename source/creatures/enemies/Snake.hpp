//
// Created by xdbeef on 23.04.18.
// http://spelunky.wikia.com/wiki/Snake
//

#ifndef SPELUNKYDS_SNAKE_H
#define SPELUNKYDS_SNAKE_H

#include "../_BaseCreature.h"
#include "../SpriteState.hpp"
#include "../SpriteInfo.h"

//http://spelunky.wikia.com/wiki/Snake
class Snake : public BaseCreature {
    
public:

    static constexpr u8 snake_sprite_width = 16;
    static constexpr u8 snake_sprite_height = 16;
    static constexpr u16 snake_physical_width = 16;
    static constexpr u16 snake_physical_height = 16;
    static constexpr SpritesheetType snake_spritesheet_type = SpritesheetType::SNAKE;

    Snake(int x, int y) : BaseCreature(
            x,
            y,
            snake_sprite_width,
            snake_sprite_height,
            snake_spritesheet_type,
            snake_physical_width,
            snake_physical_height
    ) {
        sprite_type = SpriteType::S_SNAKE;
        init_sprites();
        hitpoints = 1;
        randomizeMovement();
        _pos_update_delta = 35;

    }

    // Base creature overrides

    void update_creature_specific() override;

    void introduce_yourself() override { printf("WHIP\n"); };

    void apply_dmg(int dmg_to_apply) override;

    // IRenderable overrides

    void init_sprites() override;

    void delete_sprites() override;

    void update_sprites_position() override;

    // ICollidable overrides

    bool can_update_collidable() override { return true; }

    bool can_apply_friction() override { return true; }

    bool can_apply_gravity() override { return true; }
    
    // Other, creature specific
    
    void randomizeMovement();

    void set_sprite_left();

    void set_sprite_right();

    void match_animation();

    double pos_inc_timer{};
    SpriteInfo *mainSpriteInfo{};
    SpriteInfo *subSpriteInfo{};
    u8 *frameGfx{};
    int sameDirectionInRow{};
    //Snake goes for random amount of time_utils on random direction, then waits random time_utils and the cycle goes again
    SpriteState spriteState{};
    double waitTimer{};
    double goTimer{};
    int animFrame{};
    double animFrameTimer{};
    
};


#endif //SPELUNKYDS_SNAKE_H
