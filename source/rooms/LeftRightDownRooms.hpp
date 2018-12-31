//
// Created by xdbeef on 02.05.18.
//

#ifndef SPELUNKYDS_LEFTRIGHTDOWNROOMS_H
#define SPELUNKYDS_LEFTRIGHTDOWNROOMS_H

static const int left_right_down_rooms[6][10][10] = {
        {
                {3, 3, 3, 3, 3, 3, 3, 3, 3, 3},
                {0, 9, 0, 5, 0, 0, 0, 0, 0, 0},
                {2, 10, 4, 4, 0, 0, 0, 0, 0, 0},
                {3, 9, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 9, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 9, 0, 0,  0, 0, 0, 0, 0, 0},
                {0, 9, 0, 0, 0, 0, 5, 0, 0, 0},
                {2, 4, 4, 0, 0, 0, 4, 4, 4, 2},
                {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                {1, 2, 2, 0, 0, 0, 0, 0, 2, 1},
        },
        {
                {3, 3, 3, 3, 3, 3, 3, 3, 3, 7},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 3},
                {0, 0,  0, 2, 0, 0, 0, 0, 0, 0},
                {2, 2, 2, 1, 4, 4, 0, 0, 0, 0},
                {3, 1, 1, 3, 0, 0, 0, 0, 2, 2},
                {0, 3, 3, 13, 0, 0, 0, 4, 3, 3},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 4, 4, 0, 0, 0, 0},
                {0, 5, 0, 0, 0, 0, 0, 0, 0, 0},
                {2, 2, 0, 0, 0, 0, 0, 0, 2, 2},
        },
        {
                {3, 3, 3, 3, 3, 3, 3, 3, 3, 7},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 3},
                {0, 0,  0, 2, 0, 0, 0, 0, 0, 0},
                {2, 2, 2, 1, 4, 4, 0, 0, 0, 0},
                {3, 1, 1, 3, 0, 0, 0, 0, 2, 2},
                {0, 3, 3, 0,  0, 0, 0, 4, 3, 3},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 4, 4, 0, 0, 0, 0},
                {0, 5, 0, 0, 0, 0, 0, 0, 0, 0},
                {2, 2, 0, 0, 0, 0, 0, 0, 2, 2},
        },
        {       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 2,  5, 2, 0, 0, 2, 4, 4, 0},
                {0, 1, 2, 1, 2, 2, 3, 0, 0, 0},
                {0, 3, 1, 3, 3, 3, 0, 0, 0, 0},
                {0, 0, 6, 0,  0, 0, 0, 0, 0, 0},
                {0, 5, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 2, 0, 2, 4, 0, 0, 0, 0, 0},
                {0, 0, 0, 6, 0, 0, 0, 0, 0, 5},
                {2, 2, 2, 3, 0, 0, 0, 0, 0, 1},
        },
        {       {5, 3, 0, 0, 0, 0, 0, 0, 0, 3},
                {4, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0,  0, 0, 0, 0, 0, 0, 0, 0},
                {0, 4, 2, 2, 2, 5, 2, 2, 4, 0},
                {0, 0, 3, 3, 3, 5, 3, 3, 0, 0},
                {0, 0, 0, 0,  0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 2, 2, 0, 0},
                {0, 2, 4, 4, 0, 0, 3, 3, 2, 0},
                {2, 1, 0, 0, 0, 0, 0, 0, 3, 2},
        },
        {       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {2, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {8, 4,  5, 0, 5, 2, 2, 2, 7, 2},
                {3, 0, 4, 0, 5, 3, 3, 1, 6, 5},
                {0, 0, 0, 0, 0, 0, 0, 3, 1, 1},
                {0, 0, 0, 0,  0, 0, 0, 0, 3, 3},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 2, 4, 0, 0, 0, 0, 0, 0, 0},
                {2, 1, 0, 0, 0, 0, 0, 0, 2, 2},
                {3, 3, 4, 5, 4, 0, 0, 0, 3, 3},
        }
};


//1 snake
//2 bat
//3 spider
//9 arrow trap left oriented (12 tile type)
//10 arrow trap right oriented (13 tile type)

static const int left_right_down_npcs[6][10][10] = {
        {
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
                {0, 0, 0, 0, 0, 2, 0, 2, 3, 0,},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
                {0, 0, 0, 3, 0, 0, 0, 0, 0, 0,},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
                {0, 0, 0, 0,  0, 0, 0, 0, 0, 0,},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
        },
        {
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
                {0, 0, 0, 0, 0, 2, 0, 3, 0, 0,},
                {5, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
                {0, 0, 0, 10, 0, 0, 0, 0, 0, 0,},
                {0, 0, 0, 3, 0, 0, 0, 0, 0, 0,},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
                {0, 0, 0, 0, 0, 3, 0, 0, 0, 0,},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
        },
        {
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
                {2, 3, 0, 0, 0, 0, 0, 0, 0, 0,},
                {5, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
                {0, 0, 0, 0,  0, 0, 0, 0, 0, 0,},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
        },
        {       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 1, 5, 0, 0, 0, 4, 1, 6, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 2, 3, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 2,  3, 2, 0, 0, 0, 0},
                {0, 0, 0, 1, 1, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {1, 1, 4, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        },
        {       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 1, 5, 4, 13, 4, 1, 6, 1, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 2, 3, 2, 3, 2, 3, 0, 0},
                {0, 0, 0, 0, 0, 0, 4, 1, 0, 0},
                {0, 1, 1, 1, 0, 0, 0, 0, 4, 0},
                {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                {0, 0, 2, 3, 0, 0, 2, 3, 0, 0},
        },
        {       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 13, 1, 5, 1, 1, 1},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 3, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 3, 2, 3, 0, 0, 0},
                {0, 0, 0, 0,  0, 0, 0, 2, 0, 0},
                {0, 4, 4, 0, 0, 0, 0, 0, 2, 3},
                {1, 0, 0, 0, 0, 0, 0, 0, 1, 1},
                {0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        }
};

//1 if loot can be placed
static const int left_right_down_loot[6][10][10] = {
        {
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
                {1, 0, 1, 0, 0, 0, 0, 0, 0, 0,},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
                {0, 0, 0, 0, 0, 0, 0, 1, 1, 1,},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
                {0, 1, 1, 0, 0, 0, 0, 0, 1, 0,},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
        },
        {
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
                {1, 1, 1, 0, 0, 0, 0, 0, 0, 0,},
                {0, 0, 0, 0, 0, 0, 0, 0, 1, 1,},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
                {0, 0, 0, 0, 1, 1, 0, 0, 0, 0,},
                {0, 1, 0, 0, 0, 0, 0, 0, 0, 0,},
                {1, 0, 0, 0, 0, 0, 0, 0, 1, 1,},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
        },
        {
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
                {1, 1, 1, 0, 1, 1, 0, 0, 0, 0,},
                {0, 0, 0, 0, 0, 0, 0, 0, 1, 1,},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
                {0, 0, 0, 0, 1, 1, 0, 0, 0, 0,},
                {0, 1, 0, 0, 0, 0, 0, 0, 0, 0,},
                {1, 0, 0, 0, 0, 0, 0, 0, 1, 1,},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
        },
        {       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 1, 1, 0, 0, 0, 1, 1, 1, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 1, 1, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {1, 1, 1, 0, 0, 1, 1, 1, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        },
        {       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 1, 1, 1, 1, 1, 1, 1, 1, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 1, 1, 0, 0},
                {0, 1, 1, 1, 0, 0, 0, 0, 1, 0},
                {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        },
        {       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 1, 1, 0, 1, 1, 1, 1, 1, 1},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 1, 1, 0, 0, 0, 0, 0, 0, 0},
                {1, 0, 0, 0, 0, 0, 0, 0, 1, 1},
                {0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        }

};


#endif //SPELUNKYDS_LEFTRIGHTDOWNROOMS_H
