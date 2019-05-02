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
    if (!GameState::instance().hud->holding_item_shopping) {
        GameState::instance().hud->disable_all_prompts();
        GameState::instance().hud->holding_item_shopping = true;
        GameState::instance().hud->holding_item_cost = &_cost;
        GameState::instance().hud->holding_item_name = _name;
        GameState::instance().hud->draw_level_hud();
    }
}

bool ShoppingObject::shopping_transaction(BaseItem *m) {

    if (!_bought) {
        console_display_name_cost();
        if (GameState::instance().input_handler->l_bumper_down) {
            if (GameState::instance().hud->money >= _cost) {
                _bought = true;
                GameState::instance().hud->money -= _cost;
                GameState::instance().hud->disable_all_prompts();
                GameState::instance().hud->recently_bough_item = true;
                GameState::instance().hud->recently_bought_item_name = _name;
                GameState::instance().hud->draw_level_hud();
                return true;
            } else {
                GameState::instance().hud->disable_all_prompts();
                GameState::instance().hud->not_enough_money = true;
                GameState::instance().main_dude->holding_item = false;
                GameState::instance().hud->draw_level_hud();
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
        if (GameState::instance().input_handler->l_bumper_down) {
            if (GameState::instance().hud->money >= _cost) {
                _bought = true;
                GameState::instance().hud->money -= _cost;
                GameState::instance().hud->disable_all_prompts();
                GameState::instance().hud->recently_bough_item = true;
                GameState::instance().hud->recently_bought_item_name = _name;
                GameState::instance().hud->draw_level_hud();
                return true;
            } else {
                GameState::instance().hud->disable_all_prompts();
                GameState::instance().hud->not_enough_money = true;
                GameState::instance().main_dude->holding_item = false;
                GameState::instance().hud->draw_level_hud();
                m->_bottom_collision = false;
                m->hold_by_main_dude = false;
                return false;
            }
        }
    }
    return false;

}

