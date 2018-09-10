//
// Created by xdbeef on 02.09.18.
//

#include "globals_declarations.h"

void global::init_globals(){

    input_handler = new InputHandler();
    camera = new Camera();
    main_dude = new MainDude();
    level_generator = new LevelGenerator();
    main_oam_manager = new OAMManager();
    sub_oam_manager = new OAMManager();
    hud = new Hud();
    game_state = new GameState();
//    global::sprites.reserve(256);
//    global::sprite_infos.reserve(256);
}
