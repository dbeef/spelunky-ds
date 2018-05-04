//
// Created by xdbeef on 04.05.18.
//

#ifndef SPELUNKYDS_SPRITETYPE_H
#define SPELUNKYDS_SPRITETYPE_H

/**
 * SpriteType is focused on particular sprite, what it represents on screen, i.e blue ruby, or a single goldbar.
 * Helpful when it comes to count all killed npcs or collected loot.
 */
enum SpriteType {
    MAIN_DUDE,                  //0
    MENU_SIGNS_TUTORIAL_QUIT,   //1
    MENU_SIGNS_START_SCORES,    //2
    BOMB,                       //3
    ROCK,                       //4
    JAR,                        //5
    MONIEZ_TRIPLE_GOLD_BARS,    //6
    MONIEZ_RUBY_BIG_RED,        //7
    MONIEZ_RUBY_BIG_BLUE,       //8
    MONIEZ_RUBY_BIG_GREEN,      //9
    SNAKE,                      //10
    ROPES,                      //11
    ROPE_ELEMENT,               //12
    PRE_WHIP,                   //13
    WHIP,                       //14
    HUD,                        //15
    BLOOD,                      //16
    SPELUNKY_TITLE,             //17
    SPELUNKY_COPYRIGHTS,        //18
    BAT,                        //19
    SPIDER                      //20
};

#endif //SPELUNKYDS_SPRITETYPE_H
