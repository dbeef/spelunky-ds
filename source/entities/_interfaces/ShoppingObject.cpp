//
// Created by xdbeef on 17.06.18.
//


#include "ShoppingObject.h"
#include "../../GameState.hpp"

void ShoppingObject::init_anim_icon() {
    if (!_bought) {
        _shopping_icon = new ShoppingIcon(0, 0);
        GameState::instance().decorations.push_back(_shopping_icon);
    }
}

void ShoppingObject::update_anim_icon(int x, int y, int carrier_width) {

    if (GameState::instance().robbed_killed_shopkeeper || !_shopping_icon)
        return;

    if (!_bought && _shopping_icon) {
        _shopping_icon->_x = (int) (x + ((0.5 * carrier_width)) - 4);
        _shopping_icon->_y = y - 12;
    }
}

void ShoppingObject::console_display_name_cost() {
    //if, so the text wouldn't blink because of constant clearing/drawing
    if (!Hud::instance().holding_item_shopping) {
        Hud::instance().disable_all_prompts();
        Hud::instance().holding_item_shopping = true;
        Hud::instance().holding_item_cost = &_cost;
        Hud::instance().holding_item_name = _name;
        Hud::instance().draw_level_hud();
    }
}

bool ShoppingObject::shopping_transaction(BaseItem *m) {

    if (!_bought) {
        console_display_name_cost();
        if (InputHandler::instance().l_bumper_down) {
            if (Hud::instance().money >= _cost) {
                _bought = true;
                Hud::instance().money -= _cost;
                Hud::instance().disable_all_prompts();
                Hud::instance().recently_bough_item = true;
                Hud::instance().recently_bought_item_name = _name;
                Hud::instance().draw_level_hud();
                return true;
            } else {
                Hud::instance().disable_all_prompts();
                Hud::instance().not_enough_money = true;
                GameState::instance().main_dude->holding_item = false;
                Hud::instance().draw_level_hud();
                m->_bottom_collision = false;
                m->_hold_by_main_dude = false;
                return false;
            }
        }
    }
    return false;

}
bool ShoppingObject::shopping_transaction(BaseCreature *m) {

    if (!_bought) {
        console_display_name_cost();
        if (InputHandler::instance().l_bumper_down) {
            if (Hud::instance().money >= _cost) {
                _bought = true;
                Hud::instance().money -= _cost;
                Hud::instance().disable_all_prompts();
                Hud::instance().recently_bough_item = true;
                Hud::instance().recently_bought_item_name = _name;
                Hud::instance().draw_level_hud();
                return true;
            } else {
                Hud::instance().disable_all_prompts();
                Hud::instance().not_enough_money = true;
                GameState::instance().main_dude->holding_item = false;
                Hud::instance().draw_level_hud();
                m->_bottom_collision = false;
                m->hold_by_main_dude = false;
                return false;
            }
        }
    }
    return false;

}

