//
// Created by xdbeef on 24.02.18.
//

#ifndef SPELUNKYDS_CONSTS_H
#define SPELUNKYDS_CONSTS_H

#include <mm_types.h>
#include <nds/arm9/console.h>
#include "input/input_handler.h"
#include "hud/hud.h"
#include "camera/camera.h"
#include "sprites/main_dude/main_dude.h"
#include "sprites/sprite_type.hpp"
#include "tiles/level_generator.h"
#include "sprites/sprite_info.h"
#include "sprites/main_dude/game_state.h"

#define GRAVITY_DELTA_SPEED 0.22 //FIXME isn't it mob-specific?
#define DEFAULT_DMA_CHANNEL 3

//https://stackoverflow.com/questions/21505021/extern-and-global-same-variable-in-same-file
//https://stackoverflow.com/questions/37323227/benefits-of-using-extern-declaration-in-same-file-as-definition
namespace global {

    void init_globals();

    extern MainDude *main_dude;
    extern InputHandler *input_handler;
    extern Camera *camera;
    extern LevelGenerator *level_generator;
    extern OAMManager *main_oam_manager;
    extern OAMManager *sub_oam_manager;
    extern Hud *hud;
    extern GameState *game_state;
    extern PrintConsole *print_console; //in-game console

    extern std::vector<MovingObject *> sprites; //technically list of logical structures called MovingObject FIXME naming
    extern std::vector<SpriteInfo *> sprite_infos; //technically list of wrappers around OAM entries FIXME naming
    extern std::vector<SpriteType> killed_npcs; //list of NPCs killed in current level //FIXME should be SpriteType
    extern std::vector<SpriteType> collected_loot; //list of loot collected in current level

    extern int bg_main_address; //technically, it's an id returned by oam init FIXME naming
    extern int bg_sub_address; //technically, it's an id returned by oam init FIXME naming
    extern double *timer; //global timer, updated in game loop
    extern double clean_unused_oam_timer; //every arbitrary amount of time OAM is checked for unused sprites and cleaned off
    extern u16 current_map[4096]; //cave background with tiles rendered on it
    extern u16 base_map[4096]; //cave background only
    extern mm_sfxhand menu_music_handler; //integer identifier for menu music
    extern mm_sfxhand cave_music_handler; //integer identifier for cave level music
}

#endif //SPELUNKYDS_CONSTS_H
