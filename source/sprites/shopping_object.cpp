//
// Created by xdbeef on 17.06.18.
//


#include "shopping_object.h"
#include "../globals_declarations.h"

void ShoppingObject::init_anim_icon() {
    if (!bought) {
        shopping_icon = new ShoppingIcon();
        shopping_icon->init();
        global::sprites.push_back(shopping_icon);
    }
}

void ShoppingObject::update_anim_icon(int x, int y, int carrier_width) {

    if (!bought) {
        shopping_icon->x = (int)(x + ((0.5 * carrier_width)) - (0.5 *shopping_icon->physical_width));
        shopping_icon->y = y - 12;
    } else
        shopping_icon->ready_to_dispose = true;

}
