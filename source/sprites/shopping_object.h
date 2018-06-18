//
// Created by xdbeef on 17.06.18.
//

#ifndef SPELUNKYDS_SHOPPING_OBJECT_H
#define SPELUNKYDS_SHOPPING_OBJECT_H


#include "non_interactive/shopping_icon.h"

class ShoppingObject {

public:

    bool shopping_transaction(MovingObject *m);
    void console_display_name_cost();
    void init_anim_icon();
    void update_anim_icon(int x, int y, int carrier_width);

    bool bought{};
    int cost{};
    const char *name{};
    ShoppingIcon *shopping_icon{};

};



#endif //SPELUNKYDS_SHOPPING_OBJECT_H
