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
    CONSOLE_BLACK_BACKGROUND        //23
};

#endif //SPELUNKYDS_MAPTILETYPES_H
