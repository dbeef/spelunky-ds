//
// Created by xdbeef on 10.03.18.
//

#include "MapTile.hpp"

/**
 * Set tile properties, like:
 * -> byte values that make graphical representation of this tile when rendered on the map
 * -> can it be collided with main dude and AI's
 * -> can it be destroyed (by a bomb for example)
 * based on the given MapTileType.
 */
void MapTile::match_tile(MapTileType type) {

    //most of the tiles are destroyable and collidable, if happens to be otherwise, set it in a specific switch-case
    destroyable = true;
    collidable = true;

    switch (type) {

        case MapTileType::CAVE_ROCK:
            values[0] = 16;
            values[1] = 17;
            values[2] = 18;
            values[3] = 19;
            mapTileType = MapTileType::CAVE_ROCK;
            break;

        case MapTileType::CAVE_REGULAR:
            values[0] = 20;
            values[1] = 21;
            values[2] = 22;
            values[3] = 23;
            mapTileType = MapTileType::CAVE_REGULAR;
            break;

        case MapTileType::STONE_BLOCK:
            values[0] = 24;
            values[1] = 25;
            values[2] = 26;
            values[3] = 27;
            mapTileType = MapTileType::STONE_BLOCK;
            break;

        case MapTileType::CAVE_DOWN_ORIENTED:
            values[0] = 28;
            values[1] = 29;
            values[2] = 30;
            values[3] = 31;
            mapTileType = MapTileType::CAVE_DOWN_ORIENTED;
            break;

        case MapTileType::CAVE_SOME_GOLD:
            values[0] = 32;
            values[1] = 33;
            values[2] = 34;
            values[3] = 35;
            mapTileType = MapTileType::CAVE_SOME_GOLD;
            break;

        case MapTileType::CAVE_MUCH_GOLD:
            values[0] = 36;
            values[1] = 37;
            values[2] = 38;
            values[3] = 39;
            mapTileType = MapTileType::CAVE_MUCH_GOLD;
            break;

        case MapTileType::CAVE_UP_ORIENTED:
            values[0] = 40;
            values[1] = 41;
            values[2] = 42;
            values[3] = 43;
            mapTileType = MapTileType::CAVE_UP_ORIENTED;
            break;

        case MapTileType::CAVE_UP_DOWN_ORIENTED:
            values[0] = 44;
            values[1] = 45;
            values[2] = 46;
            values[3] = 47;
            mapTileType = MapTileType::CAVE_UP_DOWN_ORIENTED;
            break;

        case MapTileType::LADDER:
            values[0] = 48;
            values[1] = 49;
            values[2] = 50;
            values[3] = 51;
            collidable = false;
            destroyable = false;
            mapTileType = MapTileType::LADDER;
            break;

        case MapTileType::LADDER_DECK:
            values[0] = 52;
            values[1] = 53;
            values[2] = 54;
            values[3] = 55;
            collidable = false;
            destroyable = false;
            mapTileType = MapTileType::LADDER_DECK;
            break;

        case MapTileType::ARROW_TRAP_LEFT:

            values[0] = 56;
            values[1] = 57;
            values[2] = 58;
            values[3] = 59;
            mapTileType = MapTileType::ARROW_TRAP_LEFT;
            break;

        case MapTileType::ARROW_TRAP_RIGHT:
            values[0] = 60;
            values[1] = 61;
            values[2] = 62;
            values[3] = 63;
            mapTileType = MapTileType::ARROW_TRAP_RIGHT;
            break;
        case MapTileType::ENTRANCE:

            values[0] = 64;
            values[1] = 65;
            values[2] = 66;
            values[3] = 67;
            collidable = false;
            destroyable = false;
            mapTileType = MapTileType::ENTRANCE;
            break;

        case MapTileType::EXIT:
            values[0] = 68;
            values[1] = 69;
            values[2] = 70;
            values[3] = 71;
            collidable = false;
            destroyable = false;
            mapTileType = MapTileType::EXIT;
            break;

        case MapTileType::CONSOLE_LEFT_BAR_TOP_ROUNDED:
            values[0] = 72;
            values[1] = 73;
            values[2] = 74;
            values[3] = 75;
            collidable = false;
            destroyable = false;
            mapTileType = MapTileType::CONSOLE_LEFT_BAR_TOP_ROUNDED;
            break;
        case MapTileType::CONSOLE_RIGHT_BAR_TOP_ROUNDED:
            values[0] = 76;
            values[1] = 77;
            values[2] = 78;
            values[3] = 79;
            collidable = false;
            destroyable = false;
            mapTileType = MapTileType::CONSOLE_RIGHT_BAR_TOP_ROUNDED;
            break;

        case MapTileType::CONSOLE_LEFT_BAR_BOT_ROUNDED:

            values[0] = 80;
            values[1] = 81;
            values[2] = 82;
            values[3] = 83;
            collidable = false;
            destroyable = false;
            mapTileType = MapTileType::CONSOLE_LEFT_BAR_BOT_ROUNDED;
            break;

        case MapTileType::CONSOLE_RIGHT_BAR_BOT_ROUNDED:

            values[0] = 84;
            values[1] = 85;
            values[2] = 86;
            values[3] = 87;
            collidable = false;
            destroyable = false;
            mapTileType = MapTileType::CONSOLE_RIGHT_BAR_BOT_ROUNDED;
            break;

        case MapTileType::CONSOLE_TOP_BAR:
            values[0] = 88;
            values[1] = 89;
            values[2] = 90;
            values[3] = 91;
            collidable = false;
            destroyable = false;
            mapTileType = MapTileType::CONSOLE_TOP_BAR;
            break;

        case MapTileType::CONSOLE_BOTTOM_BAR:

            values[0] = 92;
            values[1] = 93;
            values[2] = 94;
            values[3] = 95;
            collidable = false;
            destroyable = false;
            mapTileType = MapTileType::CONSOLE_BOTTOM_BAR;
            break;

        case MapTileType::CONSOLE_LEFT_BAR:
            values[0] = 96;
            values[1] = 97;
            values[2] = 98;
            values[3] = 99;
            collidable = false;
            destroyable = false;
            mapTileType = MapTileType::CONSOLE_LEFT_BAR;
            break;

        case MapTileType::CONSOLE_RIGHT_BAR:
            values[0] = 100;
            values[1] = 101;
            values[2] = 102;
            values[3] = 103;
            collidable = false;
            destroyable = false;
            mapTileType = MapTileType::CONSOLE_RIGHT_BAR;
            break;

        case MapTileType::CONSOLE_BLACK_BACKGROUND:
            values[0] = 104;
            values[1] = 105;
            values[2] = 106;
            values[3] = 107;
            collidable = false;
            destroyable = false;
            mapTileType = MapTileType::CONSOLE_BLACK_BACKGROUND;
            break;

        case MapTileType::CAVE_SMOOTH:
            values[0] = 108;
            values[1] = 109;
            values[2] = 110;
            values[3] = 111;
            collidable = true;
            destroyable = true;
            mapTileType = MapTileType::CAVE_SMOOTH;
            break;

        case MapTileType::SCORES_STAR_DOOR:
            values[0] = 112;
            values[1] = 113;
            values[2] = 114;
            values[3] = 115;
            collidable = false;
            destroyable = false;
            mapTileType = MapTileType::SCORES_STAR_DOOR;
            break;

        case MapTileType::SCORES_SUN_DOOR:
            values[0] = 116;
            values[1] = 117;
            values[2] = 118;
            values[3] = 119;
            collidable = false;
            destroyable = false;
            mapTileType = MapTileType::SCORES_SUN_DOOR;
            break;

        case MapTileType::SCORES_MOON_DOOR:
            values[0] = 120;
            values[1] = 121;
            values[2] = 122;
            values[3] = 123;
            collidable = false;
            destroyable = false;
            mapTileType = MapTileType::SCORES_MOON_DOOR;
            break;

        case MapTileType::SCORES_CHANGING_DOOR:
            values[0] = 124;
            values[1] = 125;
            values[2] = 126;
            values[3] = 127;
            collidable = false;
            destroyable = false;
            mapTileType = MapTileType::SCORES_CHANGING_DOOR;
            break;

        case MapTileType::SHOP_SIGN_RARE:
            values[0] = 128;
            values[1] = 129;
            values[2] = 130;
            values[3] = 131;
            collidable = true;
            destroyable = true;
            mapTileType = MapTileType::SHOP_SIGN_RARE;
            break;

        case MapTileType::SHOP_SIGN_WEAPON:
            values[0] = 132;
            values[1] = 133;
            values[2] = 134;
            values[3] = 135;
            collidable = true;
            destroyable = true;
            mapTileType = MapTileType::SHOP_SIGN_WEAPON;
            break;

        case MapTileType::SHOP_SIGN_BOMBS:
            values[0] = 136;
            values[1] = 137;
            values[2] = 138;
            values[3] = 139;
            collidable = true;
            destroyable = true;
            mapTileType = MapTileType::SHOP_SIGN_BOMBS;
            break;

        case MapTileType::SHOP_SIGN_CLOTHING:
            values[0] = 140;
            values[1] = 141;
            values[2] = 142;
            values[3] = 143;
            collidable = true;
            destroyable = true;
            mapTileType = MapTileType::SHOP_SIGN_CLOTHING;
            break;

        case MapTileType::SHOP_SIGN_CRAPS:
            values[0] = 144;
            values[1] = 145;
            values[2] = 146;
            values[3] = 147;
            collidable = true;
            destroyable = true;
            mapTileType = MapTileType::SHOP_SIGN_CRAPS;
            break;

        case MapTileType::SHOP_SIGN_GENERAL:
            values[0] = 148;
            values[1] = 149;
            values[2] = 150;
            values[3] = 151;
            collidable = true;
            destroyable = true;
            mapTileType = MapTileType::SHOP_SIGN_GENERAL;
            break;

        case MapTileType::SHOP_SIGN_KISSING:
            values[0] = 152;
            values[1] = 153;
            values[2] = 154;
            values[3] = 155;
            collidable = true;
            destroyable = true;
            mapTileType = MapTileType::SHOP_SIGN_KISSING;
            break;

        case MapTileType::NA:
            values[0] = 156;
            values[1] = 157;
            values[2] = 158;
            values[3] = 159;
            collidable = false;
            destroyable = false;
            mapTileType = MapTileType::NA;
            break;

        case MapTileType::SHOP_MUGSHOT_1:
            values[0] = 160;
            values[1] = 161;
            values[2] = 162;
            values[3] = 163;
            collidable = false;
            destroyable = false;
            mapTileType = MapTileType::SHOP_MUGSHOT_1;
            break;

        case MapTileType::SHOP_MUGSHOT_2:
            values[0] = 164;
            values[1] = 165;
            values[2] = 166;
            values[3] = 167;
            collidable = false;
            destroyable = false;
            mapTileType = MapTileType::SHOP_MUGSHOT_2;
            break;

        case MapTileType::SHOP_MUGSHOT_3:
            values[0] = 168;
            values[1] = 169;
            values[2] = 170;
            values[3] = 171;
            collidable = false;
            destroyable = false;
            mapTileType = MapTileType::SHOP_MUGSHOT_3;
            break;

        case MapTileType::SHOP_MUGSHOT_4:
            values[0] = 172;
            values[1] = 173;
            values[2] = 174;
            values[3] = 175;
            collidable = false;
            destroyable = false;
            mapTileType = MapTileType::SHOP_MUGSHOT_4;
            break;

        default:
            break;
    }


}
