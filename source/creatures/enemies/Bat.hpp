//
// Created by xdbeef on 02.05.18.
// http://spelunky.wikia.com/wiki/Bat
//

#ifndef SPELUNKYDS_BAT_H
#define SPELUNKYDS_BAT_H

#define MAX_X_SPEED_BAT 4
#define MAX_Y_SPEED_BAT 4

#define BAT_PHYSICAL_HEIGHT 16
#define BAT_PHYSICAL_WIDTH 16

#define BAT_SPRITE_HEIGHT 16
#define BAT_SPRITE_WIDTH 16
#define BAT_SPRITE_SIZE BAT_SPRITE_HEIGHT * BAT_SPRITE_WIDTH

#include "../_BaseCreature.h"
#include "../SpriteState.hpp"
#include "../SpriteInfo.h"

//http://spelunky.wikia.com/wiki/Bat
class Bat : public BaseCreature {

public:

    static constexpr u8 bat_sprite_width = 16;
    static constexpr u8 bat_sprite_height = 16;
    static constexpr u16 bat_physical_width = 16;
    static constexpr u16 bat_physical_height = 16;
    static constexpr SpritesheetType bat_spritesheet_type = SpritesheetType::BAT_JETPACK;

    Bat(int x, int y) : BaseCreature(
            x,
            y,
            bat_sprite_width,
            bat_sprite_height,
            bat_spritesheet_type,
            bat_physical_width,
            bat_physical_height,
            CreatureType::BAT
    ) {
        _pos_update_delta = 30;
        hitpoints = 1;
        init_sprites();
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

    bool can_apply_friction() const override { return false; }

    bool can_apply_gravity() const override { return false; }

    // Other, creature specific

    void set_sprite_hanging();

    void set_sprite_flying_right();

    void set_sprite_flying_left();

    void follow_main_dude();

    void match_animation();
    
    SpriteInfo *mainSpriteInfo {};
    SpriteInfo *subSpriteInfo {};
    u8 *frameGfx{};
    int animFrame{};
    double animFrameTimer{};
    bool hanging{};
    bool hunting{};

};

#endif //SPELUNKYDS_BAT_H
