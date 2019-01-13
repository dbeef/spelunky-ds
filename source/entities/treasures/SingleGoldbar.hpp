//
// Created by xdbeef on 16.09.18.
//

#ifndef SPELUNKYDS_ONE_GOLDBAR_H
#define SPELUNKYDS_ONE_GOLDBAR_H

#include "_BaseTreasure.h"

class SingleGoldbar : public BaseTreasure {

public:

    static constexpr u8 single_goldbar_sprite_width = 16;
    static constexpr u8 single_goldbar_sprite_height = 16;
    static constexpr u16 single_goldbar_physical_width = 8;
    static constexpr u16 single_goldbar_physical_height = 5;
    static constexpr SpritesheetType single_goldbar_spritesheet_type = SpritesheetType::MONIEZ_GOLDBARS;

    SingleGoldbar(int x, int y);

    // IRenderable overrides (rest of overrrides are in _BaseTreasure)

    void init_sprites() override;

    // Other, creature specific

};

#endif //SPELUNKYDS_ONE_GOLDBAR_H
