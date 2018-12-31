//
// Created by xdbeef on 06.05.18.
//

#include <cassert>
#include "oam_utils.hpp"
#include "../globals_declarations.hpp"

namespace oam_utils {

    //TODO Clean only the higher addresses of memory, don't clean up the hud and main dude creatures, so the blinking
    // that the cleaning causes would be less visible.
    bool clean_unused_oam() {

        global::clean_unused_oam_timer += *global::timer;

        if (/*global::clean_unused_oam_timer > 250 && */global::main_oam_manager->current_oam_id_tiles >= 118) {

//            for (auto &sprite_info : global::sprite_infos) {
//                sprite_info->entry = nullptr; //fixme - shouldn't this be deleted first? i guess no
//                delete sprite_info;
//                sprite_info = nullptr;
//            }

            global::main_oam_manager->clear_sprite_attributes();
            global::sub_oam_manager->clear_sprite_attributes();

            global::hud->delete_sprites();
            global::main_dude->delete_sprites();
            global::main_dude->whip->delete_sprites();

            std::vector<BaseCreature *>::iterator iter;
            for (iter = global::creatures.begin(); iter != global::creatures.end();) {
                iter.operator*()->delete_sprites();
                if (iter.operator*()->_ready_to_dispose) {
                    delete iter.operator*();
                    iter = global::creatures.erase(iter);
                } else
                    ++iter;
            }
            
            std::vector<BaseDecoration *>::iterator iter_deco;
            for (iter_deco = global::decorations.begin(); iter_deco != global::decorations.end();) {
                iter_deco.operator*()->delete_sprites();
                if (iter_deco.operator*()->_ready_to_dispose) {
                    delete iter_deco.operator*();
                    iter_deco = global::decorations.erase(iter_deco);
                } else
                    ++iter_deco;
            }

//            std::vector<BaseDecoration *>::iterator iter_decorations;
//            for (iter_decorations = global::decorations.begin(); iter_decorations != global::decorations.end();) {
//                iter_decorations.operator*()->delete_sprites();
//                ++iter;
//            }


//            for (auto &sprite_info: global::sprite_infos) {
//                if (sprite_info != nullptr) {
//                    printf("ASSERTION FALSE");
//                    for(int a =0;a<5*60;a++)
//                        swiWaitForVBlank();
//                }
//            }
//https://stackoverflow.com/questions/2275076/is-stdvector-copying-the-objects-with-a-push-back
            global::sprite_infos.clear();

//            for (int a = 0; a < global::creatures.size(); a++) {
//                if (global::creatures.at(a)->ready_to_dispose) {
//                    delete (global::creatures.at(a));
//                    global::creatures.erase(global::creatures.begin() + a);
//                }
//            }


            global::main_dude->init_sprites();
            global::main_dude->whip->init_sprites();

            for (unsigned long a = 0; a < global::creatures.size(); a++) {
                //fixme - being killed not always means it's ready for disposing!
                if (!global::creatures.at(a)->_ready_to_dispose /*&& !global::creatures.at(a)->killed*/) {
                    global::creatures.at(a)->init_sprites();
                }
            }
            for (unsigned long a = 0; a < global::decorations.size(); a++) {
                //fixme - being killed not always means it's ready for disposing!
                if (!global::decorations.at(a)->_ready_to_dispose /*&& !global::creatures.at(a)->killed*/) {
                    global::decorations.at(a)->init_sprites();
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
//        global::main_oam_manager->delete_sprite_infos();
//        global::sub_oam_manager->delete_sprite_infos();

//        for (auto &sprite_info : global::sprite_infos) {
//            if (sprite_info != nullptr) {
//                sprite_info->entry = nullptr; //fixme - shouldn't this be deleted first? i guess no
//                delete sprite_info;
//                sprite_info = nullptr;
//            }
//        }
        global::main_oam_manager->clear_sprite_attributes();
        global::sub_oam_manager->clear_sprite_attributes();

        global::main_dude->delete_sprites();
        global::main_dude->whip->delete_sprites();
        global::hud->delete_sprites();

        for (auto &sprite : global::creatures) {
            sprite->delete_sprites(); //deletes its SpriteInfos and nullptrs them
            delete sprite; //deletes sprite itself
        }

        for (auto &sprite : global::creatures_to_add) {
            sprite->delete_sprites(); //deletes its SpriteInfos and nullptrs them
            delete sprite; //deletes sprite itself
        }

        for (auto &sprite : global::decorations) {
            sprite->delete_sprites(); //deletes its SpriteInfos and nullptrs them
            delete sprite; //deletes sprite itself
        }

        for (auto &sprite : global::decorations_to_add) {
            sprite->delete_sprites(); //deletes its SpriteInfos and nullptrs them
            delete sprite; //deletes sprite itself
        }

        global::creatures.clear(); //deletes pointers to the creatures removed above - they're not nullptrs!

        global::decorations.clear(); //deletes pointers to the decorations removed above - they're not nullptrs!

//Assertion will always be false, pointers are copied when pushed to the vector
//        int c =0;
//        for (auto &sprite_info: global::sprite_infos) {
//            if (sprite_info != nullptr) {
//                printf("ASSERTION FALSE %i", c);
//                for(int a =0;a<5*60;a++)
//                    swiWaitForVBlank();
//            }
//            c++;
//        }
//https://stackoverflow.com/questions/2275076/is-stdvector-copying-the-objects-with-a-push-back
        global::sprite_infos.clear(); //deletes pointers to the SpriteInfos deleted above - they're not nullptrs!


    }
}