//
// Created by xdbeef on 17.05.18.
//

#ifndef SPELUNKYDS_SHOTGUN_H
#define SPELUNKYDS_SHOTGUN_H

#include "../_base_creature.h"
#include "../../interfaces/shopping_object.h"
#include "../../decorations/Blast.hpp"

//http://spelunky.wikia.com/wiki/Shotgun
class Shotgun: public BaseCreature, public ShoppingObject {

public:

    static constexpr u16 shotgun_cost = 13500;
    static constexpr const char *shotgun_name = "SHOTGUN";
    static constexpr u8 shotgun_sprite_width = 16;
    static constexpr u8 shotgun_sprite_height = 16;
    static constexpr u16 shotgun_physical_width = 13;
    static constexpr u16 shotgun_physical_height = 7;
    static constexpr SpritesheetType shotgun_spritesheet_type = SpritesheetType::SPIKES_COLLECTIBLES;

    Shotgun(int x, int y);

    // Base creature overrides

    void update_creature_specific() override;

    void introduce_yourself() override { printf("WHIP\n"); };

    void apply_dmg(int dmg_to_apply) override {};

    // IRenderable overrides

    void init_sprites() override;

    void delete_sprites() override;

    void update_sprites_position() override;

    // ICollidable overrides

    bool can_update_collidable() override { return !hold_by_main_dude || hold_by_anyone; }

    bool can_apply_friction() override { return true; }

    bool can_apply_gravity() override { return true; }

    // Other, creature specific

    void spawn_bullets();

    void equip();

    void match_animation();

    void handle_shooting();
    
    bool hold_by_anyone{};
    double pos_inc_timer{};
    SpriteInfo *mainSpriteInfo {};
    SpriteInfo *subSpriteInfo {};
    Blast *blast{};
    double cooldown{};
    bool firing{};
    u8 * frameGfx{};


};

#endif //SPELUNKYDS_SHOTGUN_H
