//
// Created by xdbeef on 19.05.18.
//

#ifndef SPELUNKYDS_COMPASS_H
#define SPELUNKYDS_COMPASS_H

#include "../../memory/SpriteInfo.h"
#include "../_common/Orientation.hpp"
#include "../creatures/_BaseCreature.h"
#include "../_interfaces/ShoppingObject.h"

//http://spelunky.wikia.com/wiki/Compass
class Compass : public BaseCreature , public ShoppingObject {

public:

    static constexpr u8 compas_sprite_width = 16;
    static constexpr u8 compas_sprite_height = 16;
    static constexpr u16 compas_physical_width = 12;
    static constexpr u16 compas_physical_height = 12;
    static constexpr u16 compas_cost = 3000;
    static constexpr const char *compas_name = "COMPASS";
    static constexpr SpritesheetType compas_spritesheet_type = SpritesheetType::SALEABLE;

public:

    Compass(int x, int y) : BaseCreature(
            x,
            y,
            compas_sprite_width,
            compas_sprite_height,
            compas_spritesheet_type,
            compas_physical_width,
            compas_physical_height,
            CreatureType::COMPASS
    ), ShoppingObject(compas_cost, compas_name) {
        init_anim_icon();
        update_anim_icon(x, y, _physical_width);
        init_sprites();
    }

    // Base creature overrides

    void update_creature_specific() override;

    void introduce_yourself() override { printf("WHIP\n"); };

    void apply_dmg(int dmg_to_apply) override {};

    bool can_update_collidable() const override { return true; }

    // IRenderable overrides

    void init_sprites() override;

    void delete_sprites() override;

    void update_sprites_position() override;

    // ICollidable overrides

    bool can_apply_friction() const override { return true; }

    bool can_apply_gravity() const override { return true; }

    // Other, creature specific

    void draw_arrow_to_exit();

    void equip();

    double pos_inc_timer{};
    bool collected{};
    SpriteInfo *mainSpriteInfo{};
    SpriteInfo *subSpriteInfo{};
    u8 *frameGfx{};
};


#endif //SPELUNKYDS_COMPASS_H
