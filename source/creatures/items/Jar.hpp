//
// Created by xdbeef on 23.04.18.
//

#ifndef SPELUNKYDS_JAR_H
#define SPELUNKYDS_JAR_H

#include "../_BaseCreature.h"
#include "../SpriteInfo.h"

//http://spelunky.wikia.com/wiki/Pot
class Jar : public BaseCreature {

public:

    static constexpr u8 jar_sprite_width = 16;
    static constexpr u8 jar_sprite_height = 16;
    static constexpr u16 jar_physical_width = 10;
    static constexpr u16 jar_physical_height = 9;
    static constexpr SpritesheetType jar_spritesheet_type = SpritesheetType::JAR;

    Jar(int x, int y) : BaseCreature(
            x,
            y,
            jar_sprite_width,
            jar_sprite_height,
            jar_spritesheet_type,
            jar_physical_width,
            jar_physical_height,
            CreatureType::JAR
    ) {
        init_sprites();
        activated = true;
        hitpoints = 1;
    }

    // Base creature overrides

    void update_creature_specific() override;

    void introduce_yourself() override { printf("WHIP\n"); };

    void apply_dmg(int dmg_to_apply) override;

    bool can_update_collidable() const override { return !hold_by_main_dude; }

    // IRenderable overrides

    void init_sprites() override;

    void delete_sprites() override;

    void update_sprites_position() override;

    // ICollidable overrides

    bool can_apply_friction() const override { return true; }

    bool can_apply_gravity() const override { return true; }

    // Other, creature specific

    void match_animation();

    void set_sprite_attributes();
    
    SpriteInfo *_main_sprite_info {};
    SpriteInfo *_sub_sprite_info {};
    u8 * _frame_gfx{};
    u16 _anim_frame_index{};
    double _anim_frame_timer{};
};



#endif //SPELUNKYDS_JAR_H
