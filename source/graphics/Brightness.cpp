//
// Created by dbeef on 5/2/19.
//

#include <nds/ndstypes.h>
#include <nds/arm9/video.h>
#include <cstdlib>

#include "Brightness.hpp"
#include "../time/Timer.h"
#include "../GameState.hpp"

static constexpr uint8 MAXIMUM_BRIGHTNESS = 16;
static constexpr uint8 NORMAL_BRIGHTNESS = 0;
static constexpr uint16 CHANGE_BRIGHTNESS_DELTA = 100;

u8 _brightness_level = NORMAL_BRIGHTNESS;
uint16 _change_brightness_timer = 0;

void brightness::normalize_brightness() {
    if (GameState::instance().just_started_game) {
        //just started the game, so lowering the brightness to the normal level.
        //game starts with the maximum brightness, so the transition between DSiMenu++ would look smoother
        _change_brightness_timer += Timer::getDeltaTime();

        if (_change_brightness_timer > CHANGE_BRIGHTNESS_DELTA) {

            _brightness_level--;

            if (_brightness_level == NORMAL_BRIGHTNESS)
                GameState::instance().just_started_game = false;
            else
                setBrightness(3, _brightness_level);

        }
    } else if (GameState::instance().in_main_menu && GameState::instance().exiting_game) {
        //exiting game, so increasing the brightness to the maximum level so the transition
        //between the game and DSiMenu++ would be smoother
        _change_brightness_timer += Timer::getDeltaTime();

        if (_change_brightness_timer > CHANGE_BRIGHTNESS_DELTA) {

            _brightness_level++;

            if (_brightness_level > MAXIMUM_BRIGHTNESS)
                // TODO: Messy, shouldn't exit here.
                exit(0);

            setBrightness(3, _brightness_level);
        }
    }
}

void brightness::set_maximum_brightness() {
    setBrightness(3, _brightness_level);
    _brightness_level = MAXIMUM_BRIGHTNESS;
}
