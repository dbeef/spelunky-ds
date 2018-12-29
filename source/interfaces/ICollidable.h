//
// Created by xdbeef on 29.12.18.
//

#ifndef SPELUNKYDS_ICOLLIDABLE_H
#define SPELUNKYDS_ICOLLIDABLE_H

#include <nds.h>
#include "IBase.h"

// Collidable provides fields and functions neccessary to:
// * check collisions between two Collidable objects
// * check collisions between map tiles
class ICollidable : public virtual IBase {

public:

   ICollidable(u16 physical_width, u16 physical_height) :
            _physical_width(physical_width),
            _physical_height(physical_height)
             {
        //do nothing
    }

    virtual ~ICollidable() = default;

    u16 _physical_width;
    u16 _physical_height;

};


#endif //SPELUNKYDS_ICOLLIDABLE_H
