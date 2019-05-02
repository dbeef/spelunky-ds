//
// Created by xdbeef on 22.07.18.
//

#include "Sound.hpp"
#include <maxmod9.h>
#include <nds/ndstypes.h>
#include "../../build/soundbank_bin.h"
#include "../../build/soundbank.h"

#ifdef WITH_SOUND
static mm_sfxhand menu_music_handler = 0; //integer identifier for menu music
static mm_sfxhand cave_music_handler = 0; //integer identifier for cave level music
#endif

void sound::start_menu_music() {
#ifdef WITH_SOUND
    // probably should load it only once, on start
    mmLoadEffect(SFX_MTITLE);
    menu_music_handler = mmEffect(SFX_MTITLE);
#endif
}

void sound::stop_menu_music() {
#ifdef WITH_SOUND
    mmEffectCancel(menu_music_handler);
    // probably should not unload, due to graphics fragmentation when loading/unloading every level
    mmUnloadEffect(SFX_MTITLE);
#endif
}

void sound::start_cave_music() {
#ifdef WITH_SOUND
    // probably should load it only once, on start
    mmLoadEffect(SFX_MCAVE);
    cave_music_handler = mmEffect(SFX_MCAVE);
#endif
}

void sound::stop_cave_music() {
#ifdef WITH_SOUND
    mmEffectCancel(cave_music_handler);
    // probably should not unload, due to graphics fragmentation when loading/unloading every level
    mmUnloadEffect(SFX_MCAVE);
#endif
}

void sound::load_sounds() {
#ifdef WITH_SOUND
    mmInitDefaultMem((mm_addr) soundbank_bin);
    mmLoadEffect(SFX_XJUMP);
    mmLoadEffect(SFX_XBREAK);
    mmLoadEffect(SFX_XALERT);
    mmLoadEffect(SFX_XARROWTRAP);
    mmLoadEffect(SFX_XKISS);
    mmLoadEffect(SFX_XJETPACK);
    mmLoadEffect(SFX_XWHIP);
    mmLoadEffect(SFX_XCOIN);
    mmLoadEffect(SFX_XTHROW);
    mmLoadEffect(SFX_XGEM);
    mmLoadEffect(SFX_XEXPLOSION);
    mmLoadEffect(SFX_XSTEPS);
    mmLoadEffect(SFX_XLAND);
    mmLoadEffect(SFX_XBAT);
    mmLoadEffect(SFX_XPICKUP);
    mmLoadEffect(SFX_XCLIMB1);
    mmLoadEffect(SFX_XCLIMB2);
    mmLoadEffect(SFX_XHIT);
    mmLoadEffect(SFX_XHURT);
    mmLoadEffect(SFX_XDIE);
    mmLoadEffect(SFX_XCHESTOPEN);
    mmLoadEffect(SFX_XSHOTGUN);
#endif
}
