//
// Created by xdbeef on 02.06.18.
//

#ifndef SPELUNKYDS_CAPE_H
#define SPELUNKYDS_CAPE_H

#include "../SpriteInfo.h"
#include "../SpriteState.hpp"
#include "../_BaseCreature.h"
#include "../../interfaces/ShoppingObject.h"

//http://spelunky.wikia.com/wiki/Cape
class Cape : public BaseCreature , public ShoppingObject {
    
public:
    
    static constexpr u16 cape_cost = 13000;
    static constexpr const char *cape_name = "CAPE";
    static constexpr u8 cape_sprite_width = 16;
    static constexpr u8 cape_sprite_height = 16;
    static constexpr u16 cape_physical_width = 14;
    static constexpr u16 cape_physical_height = 15;
    static constexpr SpritesheetType cape_spritesheet_type = SpritesheetType::MONIEZ_GOLDBARS;

    Cape(int x, int y) : BaseCreature(
            x,
            y,
            cape_sprite_width,
            cape_sprite_height,
            cape_spritesheet_type,
            cape_physical_width,
            cape_physical_height
    ), ShoppingObject(cape_cost, cape_name) {
        init_sprites();
        init_anim_icon();
        update_anim_icon(x, y, _physical_width);
    }

    // Base creature overrides

    void update_creature_specific() override;

    void introduce_yourself() override { printf("WHIP\n"); };

    void apply_dmg(int dmg_to_apply) override {};

    bool can_update_collidable() const override { return !_collected; }

    // IRenderable overrides

    void init_sprites() override;

    void delete_sprites() override;

    void update_sprites_position() override;

    // ICollidable overrides

    bool can_apply_friction() const override { return true; }

    bool can_apply_gravity() const override { return true; }

    // Other, creature specific

    void match_animation();

    void equip();

    bool _collected{};
    SpriteInfo *_main_sprite_info{};
    SpriteInfo *_sub_sprite_info{};
    double _anim_frame_timer{};
    u16 _anim_frame_index{};
    u8 *_frame_gfx{};

};




#endif //SPELUNKYDS_CAPE_H
