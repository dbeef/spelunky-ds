//
// Created by xdbeef on 24.03.18.
//

#ifndef SPELUNKYDS_HUD_H
#define SPELUNKYDS_HUD_H

#include "../memory/oam_manager.h"
#define HUD_ITEMS_ROW_X 236

class Hud {

public:

    Hud();

    bool thief{};
    int thief_timer{};

    bool introduce_shop{};
    int introduce_shop_timer{};
    const char *shop_name{};

    bool recently_bough_item{};
    int recently_bough_item_timer{};
    const char *recently_bought_item_name{};

    bool not_enough_money{};
    int not_enough_money_timer{};

    bool holding_item_shopping{};
    const char *holding_item_name{};
    int *holding_item_cost{};

    void disable_all_prompts();

    void draw();

    void init();

    int dollars_buffer{};
    int dollars_timer{};
    //it has to be zero, to pass money from dollars_buffer to money
    int collecting_timer{};

    int hearts{};
    int bombs{};
    int money{};
    int ropes{};

    double time_spent_on_level{};
    double total_time_spent{};
    int money_on_this_level{};
    int level{};

    int game_over_timer{};

    SpriteInfo *heartSpriteInfo {};
    SpriteInfo *bombSpriteInfo {};
    SpriteInfo *dollarSpriteInfo {};
    SpriteInfo *ropeSpriteInfo {};
    SpriteInfo *holdingItemSpriteInfo {};

    //changes every time_utils a new item is added to hud
    int items_offset_y{};

    void update();
    void add_moniez_on_collected_loot(int value);

    void draw_on_level_done();
    void draw_scores();

    void hide_hud_sprites();

    void show_hud_sprites();

    void init_sprites();

    void set_hud_sprites_attributes();

    void increment_offset_on_grabbed_item();

    void debug_oam();

    void draw_collected_loot();

    void draw_killed_npcs();


    void hide_hud();
};


#endif //SPELUNKYDS_HUD_H
