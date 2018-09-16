//
// Created by xdbeef on 06.05.18.
//

#include "oam_utils.hpp"
#include "../globals_declarations.hpp"

namespace oam_utils {

    //TODO Clean only the higher addresses of memory, don't clean up the hud and main dude sprites, so the blinking
    // that the cleaning causes would be less visible.
    bool clean_unused_oam() {

        global::clean_unused_oam_timer += *global::timer;

        if (/*global::clean_unused_oam_timer > 250 && */global::main_oam_manager->current_oam_id_tiles >= 120) {

            for (int a = 0; a < global::sprites.size(); a++) {
                if (global::sprites.at(a)->ready_to_dispose) {
                    delete (global::sprites.at(a));
                    global::sprites.erase(global::sprites.begin() + a);
                }
            }

            global::main_oam_manager->clearAllSprites();
            global::sub_oam_manager->clearAllSprites();

            for (int a = 0; a < global::sprites.size(); a++) {
                //fixme - being killed not always means it's ready for disposing!
                if (!global::sprites.at(a)->ready_to_dispose /*&& !global::sprites.at(a)->killed*/) {
                    global::sprites.at(a)->initSprite();
                }
            }

            global::hud->init_sprites();

//            global::clean_unused_oam_timer = 0;
            return true;
        }

        return false;
    };


    void delete_all_sprites() {
        //possible memory leak?
        global::main_oam_manager->clearAllSprites();
        global::sub_oam_manager->clearAllSprites();

        for (int a = 0; a < global::sprites.size(); a++) {
            delete (global::sprites.at(a));
            global::sprites.erase(global::sprites.begin() + a);
        }

        global::sprites.clear();
    }
}