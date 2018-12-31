//
// Created by xdbeef on 18.05.18.
//

#ifndef SPELUNKYDS_SPRING_SHOES_H
#define SPELUNKYDS_SPRING_SHOES_H


#include "../SpriteInfo.h"
#include "../SpriteState.hpp"
#include "../_BaseCreature.h"
#include "../../interfaces/ShoppingObject.h"

//http://spelunky.wikia.com/wiki/Spring_Shoes
class SpringShoes : public BaseCreature, public ShoppingObject {

    static constexpr u8 spring_shoes_sprite_width = 16;
    static constexpr u8 spring_shoes_sprite_height = 16;
    static constexpr u16 spring_shoes_physical_width = 14;
    static constexpr u16 spring_shoes_physical_height = 12;
    static constexpr u16 spring_shoes_cost = 5000;
    static constexpr const char *spring_shoes_name = "SPRING SHOES";
    static constexpr SpritesheetType spring_shoes_spritesheet_type = SpritesheetType::SALEABLE;

public:

    SpringShoes(int x, int y) : BaseCreature(
            x,
            y,
            spring_shoes_sprite_width,
            spring_shoes_sprite_height,
            spring_shoes_spritesheet_type,
            spring_shoes_physical_width,
            spring_shoes_physical_height
    ), ShoppingObject(spring_shoes_cost, spring_shoes_name) {
        init_anim_icon();
        update_anim_icon(x, y, _physical_width);
        init_sprites();
    }

    // Base creature overrides

    void update_creature_specific() override;

    void introduce_yourself() override { printf("WHIP\n"); };

    void apply_dmg(int dmg_to_apply) override {};

    // IRenderable overrides

    void init_sprites() override;

    void delete_sprites() override;

    void update_sprites_position() override;

    // ICollidable overrides

    bool can_update_collidable() override { return !collected; }

    bool can_apply_friction() override { return true; }

    bool can_apply_gravity() override { return true; }

    // Other, creature specific

    void equip();

    double pos_inc_timer{};
    bool collected{};
    SpriteInfo *mainSpriteInfo{};
    SpriteInfo *subSpriteInfo{};
    u8 *frameGfx{};

};


#endif //SPELUNKYDS_SPRING_SHOES_H
