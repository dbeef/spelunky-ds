//
// Created by xdbeef on 02.06.18.
//

#ifndef SPELUNKYDS_FLAME_ELEMENT_H
#define SPELUNKYDS_FLAME_ELEMENT_H

#include "../_base_creature.h"
#include "../sprite_info.h"

#define flame_sprite_width 16
#define flame_sprite_height 16
#define flame_physical_width 6
#define flame_physical_height 6
#define flame_spritesheet_type SpritesheetType::SPIKES_COLLECTIBLES

class FlameElement : public BaseCreature {

public:

    FlameElement(int x, int y) : BaseCreature(
            x,
            y,
            flame_sprite_width,
            flame_sprite_height,
            flame_spritesheet_type,
            flame_physical_width,
            flame_physical_height
    ) {
        _pos_update_delta = 30;
    //        change_pos_delta_offset = FLAME_CHANGE_POS_DELTA + (rand() % 5);
    init_sprites();
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

    bool can_update_collidable() override { return !finished && !_ready_to_dispose; }

    bool can_apply_friction() override { return true; }

    bool can_apply_gravity() override { return true; }

    // Other, creature specific

    void match_animation();

    SpriteInfo *mainSpriteInfo {};
    SpriteInfo *subSpriteInfo {};
    u8 * frameGfx{};
    int currentFrame{};
    double frameTimer{};
    double pos_inc_timer{};
    double inactive_delay{};
    int pos_inc_delta_offset{};
    bool finished{};
};





#endif //SPELUNKYDS_FLAME_ELEMENT_H
