//
// Created by dbeef on 5/3/19.
//

#ifndef SPELUNKYDS_LEVELRENDERER_H
#define SPELUNKYDS_LEVELRENDERER_H

#include <nds/ndstypes.h>
#include "../preprocessor/Debug.h"

class LevelRenderer {

public:

    static void init();

    static void dispose();

    static inline LevelRenderer& instance(){
        SPELUNKYDS_BREAKING_ASSERT(_instance);
        return *_instance;
    }

    void render();

    void init_background_hardware();

    void copy_background_tiles();

    void copy_background_palette();

    int _main_background_id{};

    int _sub_background_id{};

private:

    static LevelRenderer* _instance;

    // needed for sectorizing map
    u16 temp_map[4096]{};

    // cave background with tiles rendered on it
    u16 current_map[4096]{};

    void write_tiles_to_map();

    void copy_current_map_to_engines();

    void write_cave_background_to_map();

    void sectorize_map();
};


#endif //SPELUNKYDS_LEVELRENDERER_H
