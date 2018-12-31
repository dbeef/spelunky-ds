//
// Created by xdbeef on 26.04.18.
//

#ifndef SPELUNKYDS_MONIEZ_H
#define SPELUNKYDS_MONIEZ_H

#include <nds.h>
#include "../creatures/CreatureType.hpp"

class Moniez {

public:

    Moniez() : _collectible_timer(500) {
        //do nothing
    }

    enum class RubyColor {
        RED,
        GREEN,
        BLUE
    };

    u16 get_value(CreatureType creature_type) {
        switch (creature_type) {
            case CreatureType::RUBY_BIG:
                return 1200;
            case CreatureType::RUBY_SMALL:
                return 1000;
            case CreatureType::TRIPLE_GOLDBAR:
                return 1000;
            case CreatureType::SINGLE_GOLDBAR:
                return 500;
            default:
                return 0;
        }
    }

    double _collectible_timer;

};


#endif //SPELUNKYDS_MONIEZ_H
