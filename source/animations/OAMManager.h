//
// Created by xdbeef on 01.04.18.
//

#ifndef SPELUNKYDS_OAMMANAGER_H
#define SPELUNKYDS_OAMMANAGER_H

#define MAX_SPRITE_NUMBER 128

#include "../level_layout/SpriteInfo.h"

class OAMManager {
public:
    int current_oam_id = 0;
    OAMTable *oam;
    SpriteInfo *sprites[MAX_SPRITE_NUMBER];

    void initOAMTable();

    void updateOAM();

    SpriteInfo *initSprite(const unsigned short pallette[], int palLen, const unsigned int tiles[], int tilesLen);
};


#endif //SPELUNKYDS_OAMMANAGER_H
