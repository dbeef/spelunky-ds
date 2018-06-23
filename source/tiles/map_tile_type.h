//
// Created by xdbeef on 27.04.18.
//

//https://stackoverflow.com/questions/23288934/c-how-to-have-same-enum-members-name-in-different-enum-names-without-getting-e

#ifndef SPELUNKYDS_MAPTILETYPES_H
#define SPELUNKYDS_MAPTILETYPES_H

enum MapTileType {
    NOTHING,                        //0  NON_COLLIDABLE
    CAVE_REGULAR,                   //1 
    CAVE_UP_ORIENTED,               //2
    CAVE_DOWN_ORIENTED,             //3
    CAVE_UP_DOWN_ORIENTED,          //4
    STONE_BLOCK,                    //5
    CAVE_SOME_GOLD,                 //6
    CAVE_MUCH_GOLD,                 //7
    CAVE_ROCK,                      //8
    LADDER,                         //9  NON_COLLIDABLE
    LADDER_DECK,                    //10 NON_COLLIDABLE
    ENTRANCE,                       //11 NON_COLLIDABLE
    ARROW_TRAP_LEFT,                //12 
    ARROW_TRAP_RIGHT,               //13
    EXIT,                           //14 NON_COLLIDABLE
    CONSOLE_LEFT_BAR,               //15
    CONSOLE_LEFT_BAR_TOP_ROUNDED,   //16
    CONSOLE_LEFT_BAR_BOT_ROUNDED,   //17
    CONSOLE_RIGHT_BAR,              //18
    CONSOLE_RIGHT_BAR_TOP_ROUNDED,  //19
    CONSOLE_RIGHT_BAR_BOT_ROUNDED,  //20
    CONSOLE_TOP_BAR,                //21
    CONSOLE_BOTTOM_BAR,             //22
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
