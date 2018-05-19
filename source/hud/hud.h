//
// Created by xdbeef on 24.03.18.
//

#ifndef SPELUNKYDS_HUD_H
#define SPELUNKYDS_HUD_H

#include "../memory/oam_manager.h"

class Hud {

public:

    Hud();

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
    int items_offset_x{};
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
};


#endif //SPELUNKYDS_HUD_H
