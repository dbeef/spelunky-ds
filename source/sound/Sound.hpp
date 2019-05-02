//
// Created by xdbeef on 06.05.18.
//

#ifndef SPELUNKYDS_SOUND_UTILS_H
#define SPELUNKYDS_SOUND_UTILS_H

// For enabling sound effects.
//#define WITH_SOUND

namespace sound {

    void load_sounds();

    void start_menu_music();

    void stop_menu_music();

    void start_cave_music();

    void stop_cave_music();

    // every other sound effect

    void hit();

    void shotgun();

    void pickup();

    void explosion();

    void die();

    void jetpack();

    void whip();

    void climb_1();

    void climb_2();

    void throwing();

    void open_chest();

    void gem();

    void coin();

    void steps();

    void allert();

    void bat();

    void skeleton_break();

    void jump();

    void land();

    void kiss();

    void arrow_trap();
}

#endif //SPELUNKYDS_SOUND_UTILS_H
