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
        shopping_icon->x = (int) (x + ((0.5 * carrier_width)) - (0.5 * shopping_icon->physical_width));
        shopping_icon->y = y - 12;
    } else
        shopping_icon->ready_to_dispose = true;

}

void ShoppingObject::console_display_name_cost() {
    //if, so the text wouldn't blink because of constant clearing/drawing
    if (!global::hud->holding_item_shopping) {
        global::hud->disable_all_prompts();
        global::hud->holding_item_shopping = true;
        global::hud->holding_item_cost = &cost;
        global::hud->holding_item_name = name;
        global::hud->draw();
    }
}

bool ShoppingObject::shopping_transaction(MovingObject *m) {

    if (!bought) {
        console_display_name_cost();
        if (global::input_handler->l_bumper_down) {
            if (global::hud->money >= cost) {
                bought = true;
                global::hud->money -= cost;
                global::hud->disable_all_prompts();
                global::hud->recently_bough_item = true;
                global::hud->recently_bought_item_name = name;
                global::hud->draw();
                return true;
            } else {
                global::hud->disable_all_prompts();
                global::hud->not_enough_money = true;
                global::main_dude->holding_item = false;
                global::hud->draw();
                m->bottomCollision = false;
                m->hold_by_main_dude = false;
                return false;
            }
        }
    }
    return false;

}

