//
// Created by xdbeef on 06.05.18.
//

#ifndef SPELUNKYDS_CONSOLE_UTILS_H
#define SPELUNKYDS_CONSOLE_UTILS_H

#include <nds/arm9/console.h>
#include "../../build/font.h"
#include "../globals_definitions.hpp"
#include "../camera/layer_level.hpp"

namespace console {

    void init() {
        constexpr int tile_base = 2;
        constexpr int map_base = 8;

        //The default instance utilizes the sub display, approximatly 15KiB of vram C starting
        //at tile base 0 and 2KiB of map at map base 30.
        consoleInit(global::print_console, OBJPRIORITY_0, BgType_Text4bpp, BgSize_T_256x256, map_base, tile_base, true,
                    false);

        ConsoleFont font;

        font.gfx = (u16 *) fontTiles;
        font.pal = (u16 *) fontPal;
        font.numChars = 59;
        font.numColors = 2 /*fontPalLen / 2*/;
        font.bpp = 4;
        font.asciiOffset = 32;
        font.convertSingleColor = true;
        consoleSetFont(global::print_console, &font);
    }

}

#endif //SPELUNKYDS_CONSOLE_UTILS_H
