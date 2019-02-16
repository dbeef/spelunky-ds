//
// Created by xdbeef on 27.04.18.
//

#ifndef SPELUNKYDS_MAPTILETYPES_H
#define SPELUNKYDS_MAPTILETYPES_H

// TODO Python script for rendering room, using MapTileType.hpp?
//https://stackoverflow.com/questions/23288934/c-how-to-have-same-enum-members-name-in-different-enum-names-without-getting-e

/**
 * Indicates type of a MapTile.
 * Tilemap can be found at gfx/tilemaps/gfx_cavebg.png
 */
enum MapTileType {
    NOTHING,                        //0  NON_COLLIDABLE
    CAVE_ROCK,                      //1 from 8
    CAVE_REGULAR,                   //2 from 1
    STONE_BLOCK,                    //3 from 5
    CAVE_DOWN_ORIENTED,             //4 from 3
    CAVE_SOME_GOLD,                 //5 from 6
    CAVE_MUCH_GOLD,                 //6 from 7
    CAVE_UP_ORIENTED,               //7 from 2
    CAVE_UP_DOWN_ORIENTED,          //8 from 4
    LADDER,                         //9  NON_COLLIDABLE
    LADDER_DECK,                    //10 NON_COLLIDABLE
    ARROW_TRAP_LEFT,                //11 //from 12
    ARROW_TRAP_RIGHT,               //12 //from 13
    ENTRANCE,                       //13 NON_COLLIDABLE //from 11
    EXIT,                           //14 NON_COLLIDABLE
    CONSOLE_LEFT_BAR_TOP_ROUNDED,   //15 //from 16
    CONSOLE_RIGHT_BAR_TOP_ROUNDED,  //16 //from 19
    CONSOLE_LEFT_BAR_BOT_ROUNDED,   //17
    CONSOLE_RIGHT_BAR_BOT_ROUNDED,  //18 //from 20
    CONSOLE_TOP_BAR,                //19 //from 21
    CONSOLE_BOTTOM_BAR,             //20 //from 22 1
    CONSOLE_LEFT_BAR,               //21 //from 15
    CONSOLE_RIGHT_BAR,              //22 //from 18 2
    CONSOLE_BLACK_BACKGROUND,       //23
    CAVE_SMOOTH,                    //24
    SCORES_STAR_DOOR,               //25
    SCORES_SUN_DOOR,                //26
    SCORES_MOON_DOOR,               //27
    SCORES_CHANGING_DOOR,           //28
    SHOP_SIGN_RARE,                 //29
    SHOP_SIGN_WEAPON,               //30
    SHOP_SIGN_BOMBS,                //31
    SHOP_SIGN_CLOTHING,             //32
    SHOP_SIGN_CRAPS,                //33
    SHOP_SIGN_GENERAL,              //34
    SHOP_SIGN_KISSING,              //35
    NA,                             //36
    SHOP_MUGSHOT_1,                 //37
    SHOP_MUGSHOT_2,                 //38
    SHOP_MUGSHOT_3,                 //39
    SHOP_MUGSHOT_4,                 //40
};

 #endif //SPELUNKYDS_MAPTILETYPES_H
