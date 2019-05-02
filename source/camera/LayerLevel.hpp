//
// Created by xdbeef on 24.05.18.
//

#ifndef SPELUNKYDS_LAYER_TYPES_H
#define SPELUNKYDS_LAYER_TYPES_H

// https://devkitpro.org/viewtopic.php?f=38&t=8720
// OBJPRIORITY_0 > BG_PRIORITY_0 >
// OBJPRIORITY_1 > BG_PRIORITY_1 >
// OBJPRIORITY_2 > BG_PRIORITY_2 >
// OBJPRIORITY_3 > BG_PRIORITY_3.
enum class LAYER_LEVEL {
    TOP,
    MIDDLE_TOP,
    MIDDLE_BOT,
    BOTTOM,
};

#endif //SPELUNKYDS_LAYER_TYPES_H
