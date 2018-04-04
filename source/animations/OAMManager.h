//
// Created by xdbeef on 01.04.18.
//

#ifndef SPELUNKYDS_OAMMANAGER_H
#define SPELUNKYDS_OAMMANAGER_H

#define MAX_SPRITE_NUMBER 128

#include "../level_layout/SpriteInfo.h"


class OAMManager {
public:
    int offset_multiplier;
    u16 *sprite_address;
    u16 *palette_address;
    u16 *oam_address;

    int current_oam_id = 0;

    /* Keep track of the available tiles */
    int nextAvailableTileIdx = 0;

    OAMTable *oam;

    void initOAMTable(u16* spriteAddress, u16* paletteAddress,  u16* oam_address, int offset_multiplier);

    void updateOAM();

    SpriteInfo *initSprite(const unsigned short pallette[], int palLen, const unsigned int tiles[], int tilesLen, int size);
};


#endif //SPELUNKYDS_OAMMANAGER_H
