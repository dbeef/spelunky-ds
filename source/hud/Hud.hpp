//
// Created by xdbeef on 24.03.18.
//

#ifndef SPELUNKYDS_HUD_H
#define SPELUNKYDS_HUD_H

#include <nds/arm9/console.h>
#include "../preprocessor/Debug.h"

#define HUD_ITEMS_ROW_X 236

class SpriteInfo;

class Hud {

public:

    Hud();

    static void init();

    static void dispose();

    inline static Hud &instance() {
        SPELUNKYDS_BREAKING_ASSERT(_instance);
        return *_instance;
    }

    void init_console();

    void clear_console();

    void init_sprites();

    void delete_sprites();

    void update();

    void disable_all_prompts();

    void draw_level_hud();

    void add_moniez_on_collected_loot(int value);

    void draw_on_level_done();

    void draw_scores();

    void set_hud_sprites_attributes();

    void increment_offset_on_grabbed_item();

    void debug_oam();

    void draw_collected_loot();

    void draw_killed_npcs();

    bool thief{};
    double thief_timer{};
    bool introduce_shop{};
    double introduce_shop_timer{};
    bool recently_bough_item{};
    double recently_bough_item_timer{};
    bool not_enough_money{};
    double not_enough_money_timer{};
    bool holding_item_shopping{};

    const char *shop_name{};
    const char *holding_item_name{};
    const char *recently_bought_item_name{};
    u16 *holding_item_cost{};


    int dollars_buffer{};
    double dollars_timer{};
    //it has to be zero, to pass money from dollars_buffer to money
    double collecting_timer{};

    int hearts{};
    int bombs{};
    int money{};
    int ropes{};

    double time_spent_on_level{};
    double total_time_spent{};
    int money_on_this_level{};
    int level{};

    double game_over_timer{};

    SpriteInfo *heartSpriteInfo {};
    SpriteInfo *bombSpriteInfo {};
    SpriteInfo *dollarSpriteInfo {};
    SpriteInfo *ropeSpriteInfo {};
    SpriteInfo *holdingItemSpriteInfo {};

    //changes every time_utils a new item is added to hud
    int items_offset_y{};
    PrintConsole *print_console{}; //in-game console

    static Hud* _instance;
};


#endif //SPELUNKYDS_HUD_H
