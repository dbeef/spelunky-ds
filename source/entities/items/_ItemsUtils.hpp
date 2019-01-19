//
// Created by xdbeef on 17.06.18.
//

#ifndef SPELUNKYDS_COLLECTIBLES_UTILS_H
#define SPELUNKYDS_COLLECTIBLES_UTILS_H

#include "../creatures/_BaseCreature.h"
#include "../_interfaces/ShoppingObject.h"

namespace collectibles_utils {
    void spawn_random_item(int x, int y);
    ShoppingObject * spawn_item(int x, int y, int r, bool bought);
    void spawn_set_up(int x, int y, BaseCreature *m);
    void spawn_set_up(int x, int y, BaseItem *m);
}

#endif //SPELUNKYDS_COLLECTIBLES_UTILS_H
