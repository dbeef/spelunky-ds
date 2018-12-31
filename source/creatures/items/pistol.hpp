//
// Created by xdbeef on 18.05.18.
//

#ifndef SPELUNKYDS_PISTOL_H
#define SPELUNKYDS_PISTOL_H

#include "../_base_creature.h"
#include "../sprite_state.hpp"
#include "../../interfaces/shopping_object.h"
#include "../../decorations/blast.hpp"
#include "../../globals_declarations.hpp"

//http://spelunky.wikia.com/wiki/Pistol
class Pistol : public BaseCreature, public ShoppingObject {

public:

    static constexpr u16 pistol_cost = 4000;
    static constexpr const char *pistol_name = "PISTOL";
    static constexpr u8 pistol_sprite_width = 16;
    static constexpr u8 pistol_sprite_height = 16;
    static constexpr u16 pistol_physical_width = 9;
    static constexpr u16 pistol_physical_height = 6;
    static constexpr SpritesheetType pistol_spritesheet_type = SpritesheetType::SPIKES_COLLECTIBLES;

    Pistol(int x, int y) : BaseCreature(
            x,
            y,
            pistol_sprite_width,
            pistol_sprite_height,
            pistol_spritesheet_type,
            pistol_physical_width,
            pistol_physical_height
    ), ShoppingObject(pistol_cost, pistol_name) {

        init_anim_icon();
        update_anim_icon(x, y, _physical_width);
        init_sprites();

        _blast = new Blast(0, 0);
        global::decorations_to_add.push_back(_blast);
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

    bool can_update_collidable() override { return !hold_by_main_dude; }

    bool can_apply_friction() override { return true; }

    bool can_apply_gravity() override { return true; }

    // Other, creature specific

    void equip();

    void spawn_bullet();

    void handle_shooting();

    void match_animation();

    SpriteInfo *_main_sprite_info{};
    SpriteInfo *_sub_sprite_info{};
    u8 *_frame_gfx{};

    Blast *_blast{};
    double _cooldown{};
    bool _firing{};

    u8 _anim_frame_index{};
    double _anim_frame_timer{};
};


#endif //SPELUNKYDS_PISTOL_H
