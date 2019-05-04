//
// Created by dbeef on 5/4/19.
//

#include "Dead.hpp"
#include "../MainDude.hpp"
#include "../../../hud/Hud.hpp"
#include "../../../sound/Sound.hpp"

void Dead::handle_input() {

}

void Dead::update() {

}

void Dead::enter() {
    MainDude::instance().time_since_last_damage = 0;
    Hud::instance().hearts = 0;
    Hud::instance().draw_level_hud();
    MainDude::instance()._bouncing_factor_y = ICollidable::default_bouncing_factor_y;
    MainDude::instance()._y_speed = -MAIN_DUDE_JUMP_SPEED * 0.25;
    Hud::instance().clear_console();
    sound::stop_cave_music();
    sound::die();
}
