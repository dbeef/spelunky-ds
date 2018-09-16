//
// Created by xdbeef on 02.09.18.
//

#include "globals_declarations.hpp"

void global::init_globals() {

//    https://softwareengineering.stackexchange.com/questions/245236/is-heap-fragmentation-a-problem-in-bare-metal-arm-with-g
    input_handler = new InputHandler();
    camera = new Camera();
    main_dude = new MainDude();
    current_level = new Level();
    main_oam_manager = new OAMManager();
    sub_oam_manager = new OAMManager();
    hud = new Hud();
    game_state = new GameState();
    global::sprites.reserve(160);
    global::sprite_infos.reserve(128);
    global::sprites_to_add.reserve(64);
}
