//
// Created by xdbeef on 01.04.18.
//

#ifndef SPELUNKYDS_OAMMANAGER_H
#define SPELUNKYDS_OAMMANAGER_H

#define MAX_SPRITE_NUMBER 128

#include "SpritesheetType.hpp"
#include "OamType.hpp"
#include "SpriteInfo.h"
#include "../camera/LayerLevel.hpp"
#include "../preprocessor/Debug.h"

class OAMManager {
public:

    static void init_sprite_hardware();

    explicit OAMManager(OamType);

    static void init();

    static void dispose();

    static inline OAMManager& main(){
        SPELUNKYDS_BREAKING_ASSERT(_main_instance);
        return *_main_instance;
    }

    static inline OAMManager& sub(){
        SPELUNKYDS_BREAKING_ASSERT(_main_instance);
        return *_sub_instance;
    }

    void update();

    SpriteInfo *initSprite(const unsigned short pallette[], int palLen, const unsigned int tiles[], int tilesLen,
                           ObjSize size, SpritesheetType spriteType, bool reuse_palette, bool reuse_tiles, LAYER_LEVEL l);

    void clear_sprite_attributes();

    OamType _oam_type{};
    int offset_multiplier{};
    u16 *sprite_address{};
    u16 *palette_address{};
    u16 *oam_address{};

    u32 current_oam_id_palette = 0;
    u32 current_oam_id_tiles = 0;

    /* Keep track of the available tiles */
    u16 nextAvailableTileIdx = 0;

    OAMTable *oam{};


private:

    static OAMManager* _main_instance;
    static OAMManager* _sub_instance;

};


#endif //SPELUNKYDS_OAMMANAGER_H
