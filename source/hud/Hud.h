//
// Created by xdbeef on 24.03.18.
//

#ifndef SPELUNKYDS_HUD_H
#define SPELUNKYDS_HUD_H

#include <nds/jtypes.h>
#include "../level_layout/SpriteInfo.h"
#include "../animations/OAMManager.h"

class Hud {

public:
    void draw();

    void init();

    int dollars_buffer;
    int dollars_timer;
    //it has to be zero, to pass dollars from dollars_buffer to dollars
    int collecting_timer;

    int hearts;
    int bombs;
    int dollars;
    int ropes;

    double time_spent_on_level;
    double total_time_spent;
    int money_on_this_level;
    int level;

    int game_over_timer;

    SpriteInfo *heartSpriteInfo = nullptr;
    SpriteInfo *bombSpriteInfo = nullptr;
    SpriteInfo *dollarSpriteInfo = nullptr;
    SpriteInfo *ropeSpriteInfo = nullptr;
    SpriteInfo *holdingItemSpriteInfo = nullptr;

    void update();
    void collectedMoniez(int value);

    void drawSplashScreenOnLevelDone();
    void drawScoresScreen();

    void hide();

    void show();
};


#endif //SPELUNKYDS_HUD_H
