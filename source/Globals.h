//
// Created by xdbeef on 24.02.18.
//

#ifndef SPELUNKYDS_CONSTS_H
#define SPELUNKYDS_CONSTS_H

#include <mm_types.h>
#include "input/InputHandler.h"
#include "hud/Hud.h"
#include "animations/Camera.h"
#include "level_layout/LevelGenerator.h"
#include "animations/MainDude.h"
#include "console/TextManager.h"
#include "sprites/SpriteType.h"

#define FRAMES_PER_ANIMATION 6
#define TICKS_PER_SECOND 30.5554503
#define DMA_CHANNEL 3
#define SPRITESHEET_ROW_WIDTH 6
#define X_SPEED_DELTA_TIME_MS 2
#define X_SPEED_DELTA_VALUE 2
#define FRICTION_DELTA_TIME_MS 30
#define FRICTION_DELTA_SPEED 1
#define Y_SPEED_DELTA_TIME_MS 2
#define GRAVITY_DELTA_SPEED 0.22

namespace global {
    extern TextManager *textManager;
    extern MainDude *main_dude;
    extern InputHandler *input_handler;
    extern Camera *camera;
    extern LevelGenerator *level_generator;
    extern OAMManager *main_oam_manager;
    extern OAMManager *sub_oam_manager;
    extern Hud *hud;
    extern int bg_main_address;
    extern int bg_sub_address;
    extern std::vector<MovingObject *> sprites;
    extern std::vector<SpriteInfo *> spriteInfos;
    extern bool bombed;
    extern double *timer;
    extern bool splash_screen;
    extern bool levels_transition_screen;
    extern bool scores_screen;
    extern u16 current_map[4096];
    extern u16 base_map[4096];
    extern bool in_main_menu;
    extern mm_sfxhand menu_music_handler;
    extern mm_sfxhand cave_music_handler;
    extern std::vector<SpritesheetType> killedNpcs;
    extern std::vector<SpriteType> collectedLoot;
}

#endif //SPELUNKYDS_CONSTS_H
