//
// Created by xdbeef on 02.09.18.
//

#include "globals_declarations.hpp"

//https://softwareengineering.stackexchange.com/questions/245236/is-heap-fragmentation-a-problem-in-bare-metal-arm-with-g
void global::init_globals() {

    input_handler = new InputHandler();
    camera = new Camera();
    main_dude = new MainDude();
    current_level = new Level();
    current_level->init_map_tiles();
    main_oam_manager = new OAMManager();
    sub_oam_manager = new OAMManager();
    hud = new Hud();
    game_state = new GameState();

    //some of the moving objects aren't rendered as sprites, like the ArrowTrap, which is render as a map tile,
    //so the the list is bigger, though the maximum number of sprites that can be rendered on NDS is 128
    global::sprites.reserve(160);
    //some of the sprites in this list may just wait to be disposed, that's why this size is slightly bigger that 128
    global::sprite_infos.reserve(160);
    //I assume there won't be a situation when we'll add more than 64 objects in a single frame
    global::sprites_to_add.reserve(64);
}
