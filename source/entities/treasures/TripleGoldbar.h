//
// Created by xdbeef on 16.09.18.
//

#ifndef SPELUNKYDS_TRIPLE_GOLDBAR_H
#define SPELUNKYDS_TRIPLE_GOLDBAR_H

#include "_BaseTreasure.h"

class TripleGoldbar : public BaseTreasure{

public:

    static constexpr u8 triple_goldbar_sprite_width = 16;
    static constexpr u8 triple_goldbar_sprite_height = 16;
    static constexpr u16 triple_goldbar_physical_width = 16;
    static constexpr u16 triple_goldbar_physical_height = 10;
    static constexpr SpritesheetType triple_goldbar_spritesheet_type = SpritesheetType::MONIEZ_GOLDBARS;

    TripleGoldbar(int x, int y) : BaseTreasure(
            x,
            y,
            triple_goldbar_sprite_width,
            triple_goldbar_sprite_height,
            triple_goldbar_spritesheet_type,
            triple_goldbar_physical_width,
            triple_goldbar_physical_height,
            TreasureType::TRIPLE_GOLDBAR
    ) {
        init_sprites();
    }

    // IRenderable overrides (rest of overrrides are in _BaseTreasure)

    void init_sprites() override;

    // Other, creature specific

};

#endif //SPELUNKYDS_TRIPLE_GOLDBAR_H
