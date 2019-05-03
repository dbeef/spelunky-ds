//
// Created by xdbeef on 27.05.18.
//

#ifndef SPELUNKYDS_GAME_STATE_H
#define SPELUNKYDS_GAME_STATE_H

#include <vector>
#include <mm_types.h>
#include <nds/arm9/console.h>
#include "input/InputHandler.hpp"
#include "hud/Hud.hpp"
#include "camera/Camera.hpp"
#include "entities/singletons/MainDude.hpp"
#include "entities/creatures/CreatureType.hpp"
#include "tiles/Level.hpp"
#include "graphics/SpriteInfo.h"
#include "entities/decorations/_BaseDecoration.h"
#include "entities/treasures/_BaseTreasure.h"
#include "entities/items/_BaseItem.h"
#include "tiles/MapTileType.hpp"
#include "preprocessor/Debug.h"
#include "graphics/OamManager.hpp"

// Some resources worth reading:
// https://softwareengineering.stackexchange.com/questions/245236/is-heap-fragmentation-a-problem-in-bare-metal-arm-with-g
// https://stackoverflow.com/questions/8036474/when-vectors-are-allocated-do-they-use-memory-on-the-heap-or-the-stack
// https://stackoverflow.com/questions/21505021/extern-and-global-same-variable-in-same-file
// https://stackoverflow.com/questions/37323227/benefits-of-using-extern-declaration-in-same-file-as-definition

static constexpr uint8 DEFAULT_DMA_CHANNEL = 3;

class GameState {
public:

    GameState();
    ~GameState();

    static void init();

    static void dispose();

    inline static GameState &instance() {
        SPELUNKYDS_BREAKING_ASSERT(_instance);
        return *_instance;
    }

    bool update();

    void start_new_game();

    void start_main_menu();

    void start_scores();

    void start_level_transition_screen();

    void start_next_level();

    void handle_changing_screens();

    bool just_started_game{};
    bool exiting_game{};
    bool robbed_killed_shopkeeper{};
    bool splash_screen{};
    bool levels_transition_screen{};
    bool scores_screen{};
    bool in_main_menu{};
    bool spawned_smooch{};
    bool smooching{};
    int damsels_rescued_this_level{};
    int smooch_timer{};

    // TODO: Make current level a singleton.
    Level *current_level{};

    u16 *temp_map{}; //cave background only

    std::vector<BaseCreature *> creatures;
    std::vector<BaseDecoration *> decorations;
    std::vector<BaseTreasure *> treasures;
    std::vector<BaseItem *> items;

    std::vector<SpriteInfo *> sprite_infos; //technically list of wrappers around OAM entries FIXME naming
    std::vector<CreatureType> killed_npcs; //list of NPCs killed in current level //FIXME should be SpriteType
    std::vector<TreasureType> collected_treasures; //list of loot collected in current level

    int bg_main_address; //technically, it's an id returned by oam init FIXME naming
    int bg_sub_address; //technically, it's an id returned by oam init FIXME naming
    double clean_unused_oam_timer; //every arbitrary amount of time OAM is checked for unused entities and cleaned off
    u16 current_map[4096]; //cave background with tiles rendered on it

private:
    static GameState *_instance;

    void handle_transition_screen_smooch();

};

#endif //SPELUNKYDS_GAME_STATE_H
