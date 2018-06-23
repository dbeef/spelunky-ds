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
    S_MAIN_DUDE,                  //0
    S_MENU_SIGNS_TUTORIAL_QUIT,   //1
    S_MENU_SIGNS_START_SCORES,    //2
    S_BOMB,                       //3
    S_ROCK,                       //4
    S_JAR,                        //5
    S_MONIEZ_TRIPLE_GOLDBARS,     //6
    S_MONIEZ_ONE_GOLDBAR,         //6
    S_MONIEZ_RUBY_BIG_RED,        //7
    S_MONIEZ_RUBY_BIG_BLUE,       //8
    S_MONIEZ_RUBY_BIG_GREEN,      //9
    S_MONIEZ_RUBY_SMALL_RED,      //7
    S_MONIEZ_RUBY_SMALL_BLUE,     //8
    S_MONIEZ_RUBY_SMALL_GREEN,    //9
    S_SNAKE,                      //10
    S_ROPES,                      //11
    S_ROPE_ELEMENT,               //12
    S_PRE_WHIP,                   //13
    S_WHIP,                       //14
    S_HUD,                        //15
    S_BLOOD,                      //16
    S_SPELUNKY_TITLE,             //17
    S_SPELUNKY_COPYRIGHTS,        //18
    S_BAT,                        //19
    S_SPIDER,                     //20
    S_SPIKES,                     //21
    S_SHOTGUN,                     //21
    S_ARROW
};

#endif //SPELUNKYDS_SPRITETYPE_H
