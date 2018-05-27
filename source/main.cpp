#include <nds.h>
#include <nds/arm9/video.h>
#include <nds/arm9/sprite.h>
#include <nds/arm9/console.h>
#include "../build/gfx_cavebg.h"
#include "globals_declarations.h"
#include "globals_definitions.h"
#include "tiles/splash_screen_type.h"
#include "game_loop.h"
#include "sound/sound_utils.h"
#include "time/time_utils.h"
#include "console/console_utils.h"
#include "camera/layer_level.h"

/*

bool done = false;
FILE *pFile;

//---------------------------------------------------------------------------------
void dirlist(const char *path) {
//---------------------------------------------------------------------------------

    DIR *pdir = opendir(path);

    if (pdir != NULL) {

        while (true) {
            struct dirent *pent = readdir(pdir);
            if (pent == NULL) break;

            if (strcmp(".", pent->d_name) != 0 && strcmp("..", pent->d_name) != 0) {
                if (pent->d_type == DT_DIR) {
                    printf("%s/%s <DIR>\n", (strcmp("/", path) == 0) ? "" : path, pent->d_name);
                    char *dnbuf = (char *) malloc(strlen(pent->d_name) + strlen(path) + 2);
                    sprintf(dnbuf, "%s/%s", (strcmp("/", path) == 0) ? "" : path, pent->d_name);
                    dirlist(dnbuf);
                    free(dnbuf);
                } else {
                    printf("%s/%s\n", (strcmp("/", path) == 0) ? "" : path, pent->d_name);
                }
            }
        }

        closedir(pdir);
    } else {
        printf("OPENDIR() FAILIURE.\n");
    }
}


mm_word stream(mm_word length, mm_addr dest, mm_stream_formats format) {
    size_t samplesize;
    switch (format) {
        case MM_STREAM_8BIT_MONO:
            samplesize = 1;
            break;
        case MM_STREAM_8BIT_STEREO:
            samplesize = 2;
            break;
        case MM_STREAM_16BIT_MONO:
            samplesize = 2;
            break;
        case MM_STREAM_16BIT_STEREO:
            samplesize = 4;
            break;
    }

    int res = fread(dest, samplesize, length, pFile);

    if (res) {
        length = res;
        std::cout << length << '\n';
    } else {
        //End of file
        std::cout << "END OF FILE" << '\n';
        length = 0;
        done = true;
    }

    return length; //Return the number of samples read
}
*/


int main() {

    global::game_state->in_main_menu = true;

    sound::load_sounds();
    sound::start_menu_music();

    time::start();

    videoSetMode(MODE_0_2D);
    videoSetModeSub(MODE_0_2D);

    vramSetBankA(VRAM_A_MAIN_BG_0x06000000);
    //vram c is used by the console (by default):
    vramSetBankC(VRAM_C_SUB_BG_0x06200000);

    vramSetBankE(VRAM_E_MAIN_SPRITE);
    vramSetBankD(VRAM_D_SUB_SPRITE);

    oamInit(&oamMain, SpriteMapping_1D_64, false);
    oamInit(&oamSub, SpriteMapping_1D_64, false);

    //https://devkitpro.org/viewtopic.php?f=38&t=8720
    //OBJPRIORITY_0 -> BG_PRIORITY_0 -> OBJPRIORITY_1 -> BG_PRIORITY_1 -> OBJPRIORITY_2 -> BG_PRIORITY_2 -> OBJPRIORITY_3 -> BG_PRIORITY_3.
    global::bg_main_address = bgInit(BG_PRIORITY_3, BgType_Text8bpp, BgSize_B8_512x512, 22, 4);
    global::bg_sub_address = bgInitSub(BG_PRIORITY_3, BgType_Text8bpp, BgSize_B8_512x512, 18, 4);

    bgSetPriority(global::bg_main_address, BG_PRIORITY_3);
    bgSetPriority(global::bg_sub_address, BG_PRIORITY_3);

    dmaCopy(gfx_cavebgTiles, bgGetGfxPtr(global::bg_main_address), sizeof(gfx_cavebgTiles));
    dmaCopy(gfx_cavebgTiles, bgGetGfxPtr(global::bg_sub_address), sizeof(gfx_cavebgTiles));

    global::level_generator->newLayout(timerElapsed(0));
    global::level_generator->generate_splash_screen(SplashScreenType::MAIN_MENU_UPPER);
    global::level_generator->generate_splash_screen(SplashScreenType::MAIN_MENU_LOWER);
    global::level_generator->render_tiles_on_base_map();

    console::init();

/*
    if (nitroFSInit(NULL)) {

        dirlist("/");

        {
            // now, try reading a file to make sure things are working OK.
            FILE *inf = fopen("file1.txt", "rb");
            if (inf) {
                int len;

                fseek(inf, 0, SEEK_END);
                len = ftell(inf);
                fseek(inf, 0, SEEK_SET);

                printf("\nTHE FOLLOWING %d BYTES MESSAGE\nfrom file1.txt is\nbrought to you by fread:\n", len);
                {
                    char *entireFile = (char *) malloc(len + 1);
                    entireFile[len] = 0;
                    if (fread(entireFile, 1, len, inf) != len)
                        printf("SAVAGE ERROR READING BYTES FROM THE FILE!\n");
                    else
                        printf("%s\n-DONE-\n", entireFile);
                    free(entireFile);
                }

                fclose(inf);
            }
        }

        printf("HERE IS THE DIRLIST ONCE MORE:\n");
        dirlist("/");

    } else {
        printf("NITROFSINIT FAILIURE: TERMINATIONG\n");
    }


    timerStart(0, ClockDivider_1024, 1000, 0);

    mm_stream mystream;
    mystream.buffer_length = 4096 * 4; //16kb
    mystream.callback = stream;
    mystream.timer = MM_TIMER1;
    mystream.manual = false;
    mystream.sampling_rate = 16000;
    mystream.format = MM_STREAM_8BIT_MONO;

    pFile = fopen("xpause.raw", "rb");

    mmStreamOpen(&mystream);

    int timer = 0;

    while (1) {
//        mmStreamUpdate();
        swiWaitForVBlank();
        if (done) {
            mmStreamClose();
            fclose(pFile);
        }
//        timer += (timerElapsed(0) / TICKS_PER_SECOND);
//        if(timer > 100 * 1000) {
//            timer = 0;
//
//        }

    }


*/
    dmaCopy(gfx_cavebgPal, BG_PALETTE, gfx_cavebgPalLen);
    dmaCopy(gfx_cavebgPal, BG_PALETTE_SUB, gfx_cavebgPalLen);

    gameloop::scroll();

    time::stop();

    return 0;
}

