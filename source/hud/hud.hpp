//
// Created by xdbeef on 24.03.18.
//

#ifndef SPELUNKYDS_HUD_H
#define SPELUNKYDS_HUD_H

#include "../memory/oam_manager.hpp"
#define HUD_ITEMS_ROW_X 236

class Hud {

public:

    Hud();

    bool thief{};
    double thief_timer{};

    bool introduce_shop{};
    double introduce_shop_timer{};
    const char *shop_name{};

    bool recently_bough_item{};
    double recently_bough_item_timer{};
    const char *recently_bought_item_name{};

    bool not_enough_money{};
    double not_enough_money_timer{};

    bool holding_item_shopping{};
    const char *holding_item_name{};
    int *holding_item_cost{};

    void disable_all_prompts();

    void draw_level_hud();

    void init();

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

    void delete_sprites();
    void update();
    void add_moniez_on_collected_loot(int value);

    void draw_on_level_done();
    void draw_scores();

    void init_sprites();

    void set_hud_sprites_attributes();

    void increment_offset_on_grabbed_item();

    void debug_oam();

    void draw_collected_loot();

    void draw_killed_npcs();

};


#endif //SPELUNKYDS_HUD_H
