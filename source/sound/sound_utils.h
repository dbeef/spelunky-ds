//
// Created by xdbeef on 06.05.18.
//

#ifndef SPELUNKYDS_SOUND_UTILS_H
#define SPELUNKYDS_SOUND_UTILS_H

#include <maxmod9.h>
#include "../../build/soundbank_bin.h"
#include "../../build/soundbank.h"

namespace sound{

    void load_sounds(){
        mmInitDefaultMem((mm_addr) soundbank_bin);
        mmLoadEffect(SFX_MCAVE);
        mmLoadEffect(SFX_MTITLE);
        mmLoadEffect(SFX_XJUMP);
        mmLoadEffect(SFX_XWHIP);
        mmLoadEffect(SFX_XCOIN);
        mmLoadEffect(SFX_XTHROW);
        mmLoadEffect(SFX_XGEM);
        mmLoadEffect(SFX_XEXPLOSION);
        mmLoadEffect(SFX_XSTEPS);
        mmLoadEffect(SFX_XLAND);
    }

    void start_menu_music(){
        global::menu_music_handler = mmEffect(SFX_MTITLE);
    }

}

#endif //SPELUNKYDS_SOUND_UTILS_H
