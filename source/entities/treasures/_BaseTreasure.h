//
// Created by xdbeef on 31.12.18.
//

#ifndef SPELUNKYDS_BASETREASURE_H
#define SPELUNKYDS_BASETREASURE_H

#include <nds.h>

#include "../_interfaces/IRenderable.h"
#include "../_interfaces/ICollidable.h"

class SpriteInfo;

enum class TreasureType {
    RUBY_BIG,
    RUBY_SMALL,
    TRIPLE_GOLDBAR,
    SINGLE_GOLDBAR
};

enum class RubyColor {
    RED,
    GREEN,
    BLUE
};

class BaseTreasure : public IRenderable, public ICollidable {

public:

    // Constructor of inheriting class should call init_sprites.
    BaseTreasure(
            int x,
            int y,
            u16 sprite_width,
            u16 sprite_height,
            SpritesheetType spritesheet_type,
            u16 physical_width,
            u16 physical_height,
            TreasureType treasure_type
    );

    void update();

    // IRenderable overrides

    // Not overriding init_sprites, that should be done in inheriting class.

    void delete_sprites() override;

    void update_sprites_position() override;

    // ICollidable overrides

    bool can_apply_friction() const override { return true; }

    bool can_apply_gravity() const override { return true; }

    virtual ~BaseTreasure() = default;

    u16 get_dollars_value() const;

    double _collectible_timer;
    const TreasureType  _treasure_type;
    SpriteInfo *_main_sprite_info{};
    SpriteInfo *_sub_sprite_info{};
};


#endif //SPELUNKYDS_BASETREASURE_H
