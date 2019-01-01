//
// Created by xdbeef on 06.05.18.
//

#ifndef SPELUNKYDS_SOUND_UTILS_H
#define SPELUNKYDS_SOUND_UTILS_H

#include <maxmod9.h>
#include <nds.h>
#include "../../build/soundbank_bin.h"
#include "../../build/soundbank.h"

namespace sound {

    void load_sounds();

    void start_menu_music();

    void stop_menu_music();

    void start_cave_music();

    void stop_cave_music();

}

#endif //SPELUNKYDS_SOUND_UTILS_H
