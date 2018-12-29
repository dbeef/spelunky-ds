//
// Created by xdbeef on 17.06.18.
//


#include "shopping_object.h"
#include "../globals_declarations.hpp"

void ShoppingObject::init_anim_icon() {
    if (!bought) {
        shopping_icon = new ShoppingIcon(0, 0);
        global::decorations_to_add.push_back(shopping_icon);
    }
}

void ShoppingObject::update_anim_icon(int x, int y, int carrier_width) {

    if (global::game_state->robbed_killed_shopkeeper)
        return;

    if (!bought) {
        shopping_icon->_x = (int) (x + ((0.5 * carrier_width)) - 4);
        shopping_icon->_y = y - 12;
    }
}

void ShoppingObject::console_display_name_cost() {
    //if, so the text wouldn't blink because of constant clearing/drawing
    if (!global::hud->holding_item_shopping) {
        global::hud->disable_all_prompts();
        global::hud->holding_item_shopping = true;
        global::hud->holding_item_cost = &cost;
        global::hud->holding_item_name = name;
        global::hud->draw_level_hud();
    }
}

bool ShoppingObject::shopping_transaction(BaseCreature *m) {

    if (!bought) {
        console_display_name_cost();
        if (global::input_handler->l_bumper_down) {
            if (global::hud->money >= cost) {
                bought = true;
                global::hud->money -= cost;
                global::hud->disable_all_prompts();
                global::hud->recently_bough_item = true;
                global::hud->recently_bought_item_name = name;
                global::hud->draw_level_hud();
                return true;
            } else {
                global::hud->disable_all_prompts();
                global::hud->not_enough_money = true;
                global::main_dude->holding_item = false;
                global::hud->draw_level_hud();
                m->bottomCollision = false;
                m->hold_by_main_dude = false;
                return false;
            }
        }
    }
    return false;

}

