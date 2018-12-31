//
// Created by xdbeef on 16.09.18.
//

#ifndef SPELUNKYDS_RUBY_SMALL_H
#define SPELUNKYDS_RUBY_SMALL_H

#include <cstdlib>

#include "../_base_creature.h"
#include "../../interfaces/moniez.hpp"
#include "../sprite_info.h"

class RubySmall : public BaseCreature, public Moniez {

public:
    
    static constexpr u8 ruby_small_sprite_width = 8;
    static constexpr u8 ruby_small_sprite_height = 8;
    static constexpr u16 ruby_small_physical_width = 4;
    static constexpr u16 ruby_small_physical_height = 4;
    static constexpr SpritesheetType ruby_small_spritesheet_type = SpritesheetType::MONIEZ_RUBY;

    RubySmall(int x, int y) : BaseCreature(
            x,
            y,
            ruby_small_sprite_width,
            ruby_small_sprite_height,
            ruby_small_spritesheet_type,
            ruby_small_physical_width,
            ruby_small_physical_height
    ), Moniez(VALUE_RUBY_SMALL) {
        activated = true;
        _collected = false;
        _collectible_timer = 500;

        //randomly select ruby color
        int type = rand() % 3;

        switch (type) {
            case 0:
                sprite_type = SpriteType::S_MONIEZ_RUBY_SMALL_RED;
                break;
            case 1:
                sprite_type = SpriteType::S_MONIEZ_RUBY_SMALL_GREEN;
                break;
            case 2:
                sprite_type = SpriteType::S_MONIEZ_RUBY_SMALL_BLUE;
                break;
            default:
                break;
        }

        activated = true;
        _collected = false;
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

    bool can_update_collidable() override { return true; }

    bool can_apply_friction() override { return true; }

    bool can_apply_gravity() override { return true; }

    // Other, creature specific

    //rename to vectors_update_timer?
    double pos_inc_timer{};
    SpriteInfo *mainSpriteInfo{};
    SpriteInfo *subSpriteInfo{};
    u8 *frameGfx{};

};


#endif //SPELUNKYDS_RUBY_SMALL_H
