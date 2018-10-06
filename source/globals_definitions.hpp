//
// Created by xdbeef on 06.05.18.
//

#ifndef SPELUNKYDS_GLOBALS_DEFINITIONS_H
#define SPELUNKYDS_GLOBALS_DEFINITIONS_H

#include <mm_types.h>
#include <nds/arm9/console.h>
#include "sprites/spritesheet_type.hpp"
#include "sprites/sprite_type.hpp"
#include "sprites/sprite_info.h"
#include "hud/hud.hpp"
#include "tiles/level.hpp"
#include "sprites/main_dude/main_dude.hpp"
#include "sprites/main_dude/game_state.hpp"

namespace global {

    InputHandler *input_handler = nullptr;
    Camera *camera = nullptr;
    MainDude *main_dude = nullptr;
    Level *current_level = nullptr;
    OAMManager *main_oam_manager = nullptr;
    OAMManager *sub_oam_manager = nullptr;
    Hud *hud = nullptr;
    GameState *game_state = nullptr;
    PrintConsole *print_console = nullptr; //initialised in console_utils.h call from main

    //https://stackoverflow.com/questions/8036474/when-vectors-are-allocated-do-they-use-memory-on-the-heap-or-the-stack
    std::vector<MovingObject *> sprites;
    std::vector<MovingObject *> sprites_to_add;
    std::vector<SpriteInfo *> sprite_infos;
    std::vector<SpriteType> killed_npcs;
    std::vector<SpriteType> collected_loot;

    int bg_main_address{};
    int bg_sub_address{};
    double *timer{};
    double clean_unused_oam_timer{};
    mm_sfxhand menu_music_handler{};
    mm_sfxhand cave_music_handler{};

    u16 current_map[4096];
    u16 *temp_map = new u16[4096];
}
#endif //SPELUNKYDS_GLOBALS_DEFINITIONS_H