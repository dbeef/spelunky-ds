//
// Created by xdbeef on 06.05.18.
//

#ifndef SPELUNKYDS_SOUND_UTILS_H
#define SPELUNKYDS_SOUND_UTILS_H

// For enabling sound effects.
#define WITH_SOUND

namespace sound {
    void load_sounds();

    void start_menu_music();

    void stop_menu_music();

    void start_cave_music();

    void stop_cave_music();
}

#endif //SPELUNKYDS_SOUND_UTILS_H
