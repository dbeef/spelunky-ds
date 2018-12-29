//
// Created by xdbeef on 29.12.18.
//

#ifndef SPELUNKYDS_IMOVABLE_H
#define SPELUNKYDS_IMOVABLE_H

#include "IBase.h"

// Movable provides fields and functions neccessary to:
// * update xy position depending on xy speed
// * update xy speed depending on friction/gravity
class IMovable : public virtual IBase {

public:

    explicit IMovable(double x_speed = 0, double y_speed = 0) :
            _x_speed(x_speed),
            _y_speed(y_speed) {
        //do nothing
    }

    virtual ~IMovable() = default;

    double _x_speed;
    double _y_speed;

};


#endif //SPELUNKYDS_IMOVABLE_H
