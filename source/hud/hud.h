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

    bool introduce_shop{};
    int introduce_shop_timer{};
    const char *shop_name{};

    bool recently_bough_item{};
    int recently_bough_item_timer{};
    const char *recently_bought_item_name{};

    bool not_enough_money{};
    int not_enough_money_timer{};

    bool holding_item_shopping{};
    const char *holding_item_name;
    int *holding_item_cost;

    void disable_all_prompts();

    void draw();

    void init();

    int dollars_buffer{};
    int dollars_timer{};
    //it has to be zero, to pass dollars from dollars_buffer to dollars
    int collecting_timer{};

    int hearts{};
    int bombs{};
    int dollars{};
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

    //changes every time a new item is added to hud
    int items_offset_y{};

    void update();
    void collectedMoniez(int value);

    void draw_on_level_done();
    void draw_scores();

    void hide();

    void show();

    void initSprites();

    void set_position();

    void next_item();

    void draw_item_shop(char *item_name, int i);

    void debug_oam();
};


#endif //SPELUNKYDS_HUD_H
