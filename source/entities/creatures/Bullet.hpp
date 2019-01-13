//
// Created by xdbeef on 17.05.18.
//

#ifndef SPELUNKYDS_BULLET_H
#define SPELUNKYDS_BULLET_H

#include "../_BaseCreature.h"
#include "../Orientation.hpp"
#include "../../collisions/Collisions.hpp"
#include "../../tiles/LevelRenderingUtils.hpp"
#include "../SpriteUtils.hpp"

class Bullet : public BaseCreature {

public:

    static constexpr u8 bullet_sprite_width = 16;
    static constexpr u8 bullet_sprite_height = 16;
    static constexpr u16 bullet_physical_width = 4;
    static constexpr u16 bullet_physical_height = 4;
    static constexpr SpritesheetType bullet_spritesheet_type = SpritesheetType::SPIKES_COLLECTIBLES;

    Bullet(int x, int y) : BaseCreature(
            x,
            y,
            bullet_sprite_width,
            bullet_sprite_height,
            bullet_spritesheet_type,
            bullet_physical_width,
            bullet_physical_height,
            CreatureType::BULLET
    ) {
        //checking this in case of firing just in front of a wall
        int xx = floor_div(this->_x + 0.5 * _physical_width, TILE_W);
        int yy = floor_div(this->_y + 0.5 * _physical_height, TILE_H);
        update_collisions_with_map(xx, yy);
        if (killed) {
            _ready_to_dispose = true;
            sprite_utils::set_visibility(false, mainSpriteInfo, subSpriteInfo);
        } else {
            _max_x_speed = 5.0f;
            init_sprites();
            _bouncing_factor_x = 0;
            _bouncing_factor_y = 0;
            _pos_update_delta = 15;
        }
    }

    // Base creature overrides

    void update_creature_specific() override;

    void introduce_yourself() override { printf("WHIP\n"); };

    void apply_dmg(int dmg_to_apply) override {};

    bool can_update_collidable() const override { return !killed; }

    // IRenderable overrides

    void init_sprites() override;

    void delete_sprites() override;

    void update_sprites_position() override;

    // ICollidable overrides

    bool can_apply_friction() const override { return false; }

    bool can_apply_gravity() const override { return false; }

    // Other, creature specific

    void match_animation();

    double pos_inc_timer{};
    SpriteInfo *mainSpriteInfo{};
    SpriteInfo *subSpriteInfo{};
    u8 *frameGfx{};
    int animFrame{};
    double animFrameTimer{};
    double lifetime{};
};


#endif //SPELUNKYDS_BULLET_H
