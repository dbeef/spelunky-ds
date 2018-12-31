//
// Created by xdbeef on 04.04.18.
//

#ifndef SPELUNKYDS_BOMB_H
#define SPELUNKYDS_BOMB_H

#include <nds/arm9/sprite.h>

#include "../_BaseCreature.h"
#include "../SpriteInfo.h"

#define ARMED_TIME_BLINK_SLOW 2000
#define ARMED_TIME_BLINK_FAST 3500

//http://spelunky.wikia.com/wiki/Bombs
class Bomb : public BaseCreature {
    
    static constexpr u8 bomb_sprite_width = 64;
    static constexpr u8 bomb_sprite_height = 64;
    static constexpr u16 bomb_physical_width = 8;
    static constexpr u16 bomb_physical_height = 8;
    static constexpr SpritesheetType bomb_spritesheet_type = SpritesheetType::BOMB;

public:
    
    Bomb(int x, int y) : BaseCreature(
            x,
            y,
            bomb_sprite_width,
            bomb_sprite_height,
            bomb_spritesheet_type,
            bomb_physical_width,
            bomb_physical_height
    ) {
        init_sprites();
        set_sprite_disarmed();
    }

    // Base creature overrides

    void update_creature_specific() override;

    void introduce_yourself() override { printf("WHIP\n"); };

    void apply_dmg(int dmg_to_apply) override {};

    bool can_update_collidable() const override { return !hold_by_main_dude && explosionFrame == 0; }

    // IRenderable overrides

    void init_sprites() override;

    void delete_sprites() override;

    void update_sprites_position() override;

    // ICollidable overrides

    bool can_apply_friction() const override { return true; }

    bool can_apply_gravity() const override { return true; }

    // Other, creature specific

    void set_sprite_disarmed();

    void set_sprite_armed();

    void explode();

    SpriteInfo *mainSpriteInfo{};
    SpriteInfo *subSpriteInfo{};
    u8 *frameGfx{};
    bool armed{};
    u16 armedTimer{};
    double explosionTimer{};
    u16 explosionFrame{};

};


#endif //SPELUNKYDS_BOMB_H
