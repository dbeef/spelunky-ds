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

void sound::hit() {
#ifdef WITH_SOUND
    mmEffect(SFX_XHIT);
#endif
}

void sound::shotgun() {
#ifdef WITH_SOUND
    mmEffect(SFX_XSHOTGUN);
#endif
}

void sound::pickup() {
#ifdef WITH_SOUND
    mmEffect(SFX_XPICKUP);
#endif
}

void sound::explosion() {
#ifdef WITH_SOUND
    mmEffect(SFX_XEXPLOSION);
#endif
}

void sound::die() {
#ifdef WITH_SOUND
    mmEffect(SFX_XDIE);
#endif
}

void sound::jetpack() {
#ifdef WITH_SOUND
    mmEffect(SFX_XJETPACK);
#endif
}

void sound::whip() {
#ifdef WITH_SOUND
    mmEffect(SFX_XWHIP);
#endif
}

void sound::climb_1() {
#ifdef WITH_SOUND
    mmEffect(SFX_XCLIMB1);
#endif
}

void sound::climb_2() {
#ifdef WITH_SOUND
    mmEffect(SFX_XCLIMB2);
#endif
}

void sound::throwing() {
#ifdef WITH_SOUND
    mmEffect(SFX_XTHROW);
#endif
}

void sound::open_chest() {
#ifdef WITH_SOUND
    mmEffect(SFX_XCHESTOPEN);
#endif
}

void sound::gem() {
#ifdef WITH_SOUND
    mmEffect(SFX_XGEM);
#endif
}

void sound::coin() {
#ifdef WITH_SOUND
    mmEffect(SFX_XCOIN);
#endif
}

void sound::steps() {
#ifdef WITH_SOUND
    mmEffect(SFX_XSTEPS);
#endif
}

void sound::allert() {
#ifdef WITH_SOUND
    mmEffect(SFX_XALERT);
#endif
}

void sound::bat() {
#ifdef WITH_SOUND
    mmEffect(SFX_XBAT);
#endif
}

void sound::skeleton_break() {
#ifdef WITH_SOUND
    mmEffect(SFX_XBREAK);
#endif
}

void sound::jump() {
#ifdef WITH_SOUND
    mmEffect(SFX_XJUMP);
#endif
}

void sound::land() {
#ifdef WITH_SOUND
    mmEffect(SFX_XLAND);
#endif
}

void sound::kiss() {
#ifdef WITH_SOUND
    mmEffect(SFX_XKISS);
#endif
}

void sound::arrow_trap() {
#ifdef WITH_SOUND
    mmEffect(SFX_XARROWTRAP);
#endif
}
