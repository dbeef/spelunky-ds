//
// Created by xdbeef on 23.04.18.
//

#ifndef SPELUNKYDS_ARROW_H
#define SPELUNKYDS_ARROW_H

#define MAX_X_SPEED_ARROW 5
#define MAX_Y_SPEED_ARROW 4

#define ARROW_PHYSICAL_HEIGHT 8
#define ARROW_PHYSICAL_WIDTH 8

#define ARROW_SPRITE_HEIGHT 8
#define ARROW_SPRITE_WIDTH 8
#define ARROW_SPRITE_SIZE ARROW_SPRITE_WIDTH * ARROW_SPRITE_HEIGHT

#include <nds/arm9/sprite.h>

#include "../_base_creature.h"
#include "../sprite_info.h"

//http://spelunky.wikia.com/wiki/Arrow
class Arrow : public BaseCreature {
public:

    static constexpr u8 arrow_sprite_width = 16;
    static constexpr u8 arrow_sprite_height = 16;
    static constexpr u16 arrow_physical_width = 10;
    static constexpr u16 arrow_physical_height = 9;
    static constexpr SpritesheetType arrow_spritesheet_type = SpritesheetType::ARROW;

    Arrow(int x, int y) : BaseCreature(
            x,
            y,
            arrow_sprite_width,
            arrow_sprite_height,
            arrow_spritesheet_type,
            arrow_physical_width,
            arrow_physical_height
    ) {
        init_sprites();
        thrown = true;
        activated = true;
        angle = 90;
        _max_x_speed = 5.0f;
        sprite_type = SpriteType::S_ARROW;
        _friction = 0.085f;
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

    bool can_update_collidable() override { return !hold_by_main_dude; }

    bool can_apply_friction() override { return true; }

    bool can_apply_gravity() override { return true; }

    // Other, creature specific

    void update_frame(int frame_num);

    SpriteInfo *mainSpriteInfo{};
    SpriteInfo *subSpriteInfo{};
    u8 *frameGfx{};
    double angle{};
    bool thrown{};
    double armed_timer{};

};


#endif //SPELUNKYDS_ARROW_H
