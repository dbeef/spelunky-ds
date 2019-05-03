//
// Created by xdbeef on 06.05.18.
//

#include <cassert>
#include "OamUtils.hpp"
#include "../GameState.hpp"
#include "../time/Timer.h"

namespace oam_utils {

    void wait_for_DMA_done(){
        while(dmaBusy(DEFAULT_DMA_CHANNEL)) {
            // do nothing
        }
    }

    //TODO Clean only the higher addresses of graphics, don't clean up the hud and main dude entities, so the blinking
    // that the cleaning causes would be less visible.
    bool clean_unused_oam() {

        GameState::instance().clean_unused_oam_timer += Timer::getDeltaTime();

        if (/*GameState::instance().clean_unused_oam_timer > 250 && */OAMManager::main().current_oam_id_tiles >= 118) {

//            for (auto &sprite_info : GameState::instance().sprite_infos) {
//                sprite_info->entry = nullptr; //fixme - shouldn't this be deleted first? i guess no
//                delete sprite_info;
//                sprite_info = nullptr;
//            }

            OAMManager::main().clear_sprite_attributes();
            OAMManager::sub().clear_sprite_attributes();

            Hud::instance().delete_sprites();
            MainDude::instance().delete_sprites();
            Whip::instance().delete_sprites();

            std::vector<BaseCreature *>::iterator iter;
            for (iter = GameState::instance().creatures.begin(); iter != GameState::instance().creatures.end();) {
                iter.operator*()->delete_sprites();
                if (iter.operator*()->_ready_to_dispose) {
                    delete iter.operator*();
                    iter = GameState::instance().creatures.erase(iter);
                } else
                    ++iter;
            }
            
            std::vector<BaseDecoration *>::iterator iter_deco;
            for (iter_deco = GameState::instance().decorations.begin(); iter_deco != GameState::instance().decorations.end();) {
                iter_deco.operator*()->delete_sprites();
                if (iter_deco.operator*()->_ready_to_dispose) {
                    delete iter_deco.operator*();
                    iter_deco = GameState::instance().decorations.erase(iter_deco);
                } else
                    ++iter_deco;
            }
            
            std::vector<BaseTreasure *>::iterator iter_treasure;
            for (iter_treasure = GameState::instance().treasures.begin(); iter_treasure != GameState::instance().treasures.end();) {
                iter_treasure.operator*()->delete_sprites();
                if (iter_treasure.operator*()->_ready_to_dispose) {
                    delete iter_treasure.operator*();
                    iter_treasure = GameState::instance().treasures.erase(iter_treasure);
                } else
                    ++iter_treasure;
            }
            std::vector<BaseItem *>::iterator iter_item;
            for (iter_item = GameState::instance().items.begin(); iter_item != GameState::instance().items.end();) {
                iter_item.operator*()->delete_sprites();
                if (iter_item.operator*()->_ready_to_dispose) {
                    delete iter_item.operator*();
                    iter_item = GameState::instance().items.erase(iter_item);
                } else
                    ++iter_item;
            }

//            std::vector<BaseDecoration *>::iterator iter_decorations;
//            for (iter_decorations = GameState::instance().decorations.begin(); iter_decorations != GameState::instance().decorations.end();) {
//                iter_decorations.operator*()->delete_sprites();
//                ++iter;
//            }


//            for (auto &sprite_info: GameState::instance().sprite_infos) {
//                if (sprite_info != nullptr) {
//                    printf("ASSERTION FALSE");
//                    for(int a =0;a<5*60;a++)
//                        swiWaitForVBlank();
//                }
//            }
//https://stackoverflow.com/questions/2275076/is-stdvector-copying-the-objects-with-a-push-back
            GameState::instance().sprite_infos.clear();

//            for (int a = 0; a < GameState::instance().entities.size(); a++) {
//                if (GameState::instance().entities.at(a)->ready_to_dispose) {
//                    delete (GameState::instance().entities.at(a));
//                    GameState::instance().entities.erase(GameState::instance().entities.begin() + a);
//                }
//            }


            MainDude::instance().init_sprites();
            Whip::instance().init_sprites();

            for (auto &creature : GameState::instance().creatures) {
                if (!creature->_ready_to_dispose) {
                    creature->init_sprites();
                }
            }
            for (auto &decoration : GameState::instance().decorations) {
                if (!decoration->_ready_to_dispose) {
                    decoration->init_sprites();
                }
            }
            for (auto &treasure : GameState::instance().treasures) {
                if (!treasure->_ready_to_dispose) {
                    treasure->init_sprites();
                }
            }
            for (auto &treasure : GameState::instance().items) {
                if (!treasure->_ready_to_dispose) {
                    treasure->init_sprites();
                }
            }

            Hud::instance().init_sprites();

//            GameState::instance().clean_unused_oam_timer = 0;
            return true;
        }

        return false;
    };


    void delete_all_sprites() {

        OAMManager::main().clear_sprite_attributes();
        OAMManager::sub().clear_sprite_attributes();

        MainDude::instance().delete_sprites();
        Whip::instance().delete_sprites();
        Hud::instance().delete_sprites();

        for (auto &sprite : GameState::instance().creatures) {
            sprite->delete_sprites(); //deletes its SpriteInfos and nullptrs them
            delete sprite; //deletes sprite itself
        }

        for (auto &sprite : GameState::instance().decorations) {
            sprite->delete_sprites(); //deletes its SpriteInfos and nullptrs them
            delete sprite; //deletes sprite itself
        }

        for (auto &sprite : GameState::instance().treasures) {
            sprite->delete_sprites(); //deletes its SpriteInfos and nullptrs them
            delete sprite; //deletes sprite itself
        }

        for (auto &sprite : GameState::instance().items) {
            sprite->delete_sprites(); //deletes its SpriteInfos and nullptrs them
            delete sprite; //deletes sprite itself
        }

        GameState::instance().creatures.clear(); //deletes pointers to the entities removed above - they're not nullptrs!
        GameState::instance().decorations.clear(); //deletes pointers to the decorations removed above - they're not nullptrs!
        GameState::instance().treasures.clear(); //deletes pointers to the treasures removed above - they're not nullptrs!
        GameState::instance().items.clear(); //deletes pointers to the items removed above - they're not nullptrs!

        //https://stackoverflow.com/questions/2275076/is-stdvector-copying-the-objects-with-a-push-back
        GameState::instance().sprite_infos.clear(); //deletes pointers to the SpriteInfos deleted above - they're not nullptrs!


    }
}