//
// Created by xdbeef on 19.05.18.
//

#ifndef SPELUNKYDS_MITT_H
#define SPELUNKYDS_MITT_H

#include "../../memory/SpriteInfo.h"
#include "../_common/Orientation.hpp"
#include "../creatures/_BaseCreature.h"
#include "../_interfaces/ShoppingObject.h"

//http://spelunky.wikia.com/wiki/Pitcher%27s_Mitt
class Mitt : public BaseCreature, public ShoppingObject  {

    static constexpr u8 mitt_sprite_width = 16;
    static constexpr u8 mitt_sprite_height = 16;
    static constexpr u16 mitt_physical_width = 14;
    static constexpr u16 mitt_physical_height = 14;
    static constexpr u16 mitt_cost = 4000;
    static constexpr const char *mitt_name = "MITT";
    static constexpr SpritesheetType mitt_spritesheet_type = SpritesheetType::SALEABLE;

public:

    Mitt(int x, int y) : BaseCreature(
            x,
            y,
            mitt_sprite_width,
            mitt_sprite_height,
            mitt_spritesheet_type,
            mitt_physical_width,
            mitt_physical_height,
            CreatureType::MITT
    ), ShoppingObject(mitt_cost, mitt_name) {
        init_anim_icon();
        update_anim_icon(x, y, _physical_width);
        init_sprites();
    }

    // Base creature overrides

    void update_creature_specific() override;

    void introduce_yourself() override { printf("WHIP\n"); };

    void apply_dmg(int dmg_to_apply) override {};

    bool can_update_collidable() const override { return !collected; }

    // IRenderable overrides

    void init_sprites() override;

    void delete_sprites() override;

    void update_sprites_position() override;

    // ICollidable overrides

    bool can_apply_friction() const override { return true; }

    bool can_apply_gravity() const override { return true; }

    // Other, creature specific
    
    double pos_inc_timer{};
    bool collected{};
    SpriteInfo *mainSpriteInfo{};
    SpriteInfo *subSpriteInfo{};
    u8 *frameGfx{};
    void equip();
    void match_animation();
};


#endif //SPELUNKYDS_MITT_H
