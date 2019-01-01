//
// Created by xdbeef on 19.05.18.
//

#ifndef SPELUNKYDS_GLOVE_H
#define SPELUNKYDS_GLOVE_H


#include "../SpriteInfo.h"
#include "../Orientation.hpp"
#include "../_BaseCreature.h"
#include "../../interfaces/ShoppingObject.h"

//http://spelunky.wikia.com/wiki/Climbing_Gloves
class Glove : public BaseCreature, public ShoppingObject {

    static constexpr u8 glove_sprite_width = 16;
    static constexpr u8 glove_sprite_height = 16;
    static constexpr u16 glove_physical_width = 16;
    static constexpr u16 glove_physical_height = 15;
    static constexpr u16 glove_cost = 9000;
    static constexpr const char *glove_name = "GLOVE";
    static constexpr SpritesheetType glove_spritesheet_type = SpritesheetType::SALEABLE;

public:

    Glove(int x, int y) : BaseCreature(
            x,
            y,
            glove_sprite_width,
            glove_sprite_height,
            glove_spritesheet_type,
            glove_physical_width,
            glove_physical_height,
            CreatureType::GLOVE
    ), ShoppingObject(glove_cost, glove_name) {
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

    void equip();

    void match_animation();

    double pos_inc_timer{};
    bool collected{};
    SpriteInfo *mainSpriteInfo{};
    SpriteInfo *subSpriteInfo{};
    u8 *frameGfx{};
};


#endif //SPELUNKYDS_GLOVE_H
