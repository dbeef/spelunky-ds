//
// Created by xdbeef on 06.05.18.
//

#include "oam_utils.h"
#include "../globals_declarations.h"

namespace oam_utils {

    //TODO Clean only the higher addresses of memory, don't clean up the hud and main dude sprites, so the blinking
    // that the cleaning causes would be less visible.
    void clean_unused_oam() {

        global::clean_unused_oam_timer += *global::timer;

        if (global::clean_unused_oam_timer > 2500 && global::main_oam_manager->current_oam_id_tiles >= 108) {

            for(int a =0;a<global::sprites.size();a++){
                if (global::sprites.at(a)->ready_to_dispose) {
                    delete (global::sprites.at(a));
                    global::sprites.erase(global::sprites.begin() + a);
                }
            }

            global::main_oam_manager->clearAllSprites();
            global::sub_oam_manager->clearAllSprites();

            //            std::cout << "            "<< "MEM CLEAN";

            for (int a = 0; a < global::sprites.size(); a++) {
                //fixme - being killed not always means it's ready for disposing!
                if (!global::sprites.at(a)->ready_to_dispose && !global::sprites.at(a)->killed)
                    global::sprites.at(a)->initSprite();
                else {
                }
            }

            global::hud->initSprites();


            global::clean_unused_oam_timer = 0;
        }

    };
}