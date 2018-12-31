//
// Created by xdbeef on 26.04.18.
//

#ifndef SPELUNKYDS_MONIEZ_H
#define SPELUNKYDS_MONIEZ_H

#define MAX_X_SPEED_MONIEZ 4
#define MAX_Y_SPEED_MONIEZ 4

#define VALUE_SINGLE_GOLDBAR 500
#define VALUE_TRIPLE_GOLDBAR 1000
#define VALUE_RUBY_SMALL 1000
#define VALUE_RUBY_BIG 1200

class Moniez {

public:

    Moniez(int value) : _value(value) {
        // do nothing
    }

    int _value{};
    double _collectible_timer{};
    bool _collected{};

};


#endif //SPELUNKYDS_MONIEZ_H
