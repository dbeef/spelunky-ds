//
// Created by xdbeef on 24.02.18.
//

#include <nds.h>
#include <nds/arm9/sprite.h>
#include <iostream>
#include "MainDude.h"
#include "../Consts.h"
#include "../../build/spelunker.h"

void MainDude::changePos(int keys_held, int keys_up) {

    if (keys_held) {

        if (keys_held & KEY_UP) {
//            if (y >= SCREEN_TOP) y--;
//            state = W_UP;
            if (bottom_collision) {
                y_speed = -MAX_Y_SPEED;
            }
            if ((can_hang_on_tile_left || can_hang_on_tile_right) && hanging_timer > MIN_HANGING_TIME) {
                y_speed = -MAX_Y_SPEED;
                can_hang_on_tile_left = false;
                can_hang_on_tile_right = false;
            }

        }

        if (keys_held & KEY_LEFT) {
            can_hang_on_tile_left = false;

//            if (x >= SCREEN_LEFT) x -= 2;
            if (x_speed > -MAX_X_SPEED && !(can_hang_on_tile_right || can_hang_on_tile_left))
                if (speed_inc_x_timer > 50) {
                    x_speed--;
                    speed_inc_x_timer = 0;
                }
            state = W_LEFT;
        }

        if (keys_held & KEY_RIGHT) {
//            if (x <= SCREEN_RIGHT) x += 2;
            state = W_RIGHT;

            can_hang_on_tile_right = false;

            if (x_speed < MAX_X_SPEED && !(can_hang_on_tile_right || can_hang_on_tile_left)) {
                if (speed_inc_x_timer > 50) {
                    x_speed++;
                    speed_inc_x_timer = 0;
                }
            }
        }

        if (keys_held & KEY_DOWN) {
            can_hang_on_tile_left = false;
            can_hang_on_tile_right = false;
//            if (y <= SCREEN_BOTTOM) y++;
//            state = W_DOWN;
        }

        //
//        if ((keys_up & KEY_LEFT) && !(keys_held & KEY_RIGHT) && bottom_collision) {
//            apply_friction = true;
//            std::cout << "1 " << '\n';
//        } else if ((keys_up & KEY_RIGHT) && !(keys_held & KEY_LEFT) && bottom_collision) {
//            apply_friction = true;
//            std::cout << "2 " << '\n';
//        } else if (!(keys_held & KEY_RIGHT) && !(keys_held & KEY_LEFT) && bottom_collision) {
//            apply_friction = true;
//            std::cout << "3 " << '\n';
//        } else
//            apply_friction = false;

        if (key_pressed_time > 70) {
            key_pressed_time = 0;
            anim_frame++;
        }

        if (anim_frame >= FRAMES_PER_ANIMATION) anim_frame = 0;

    }
}


void MainDude::updateTimers(int timeElapsed) {
    speed_inc_x_timer += timeElapsed;
    speed_inc_y_timer += timeElapsed;
    pos_x_inc_timer += timeElapsed;
    pos_y_inc_timer += timeElapsed;
    key_pressed_time += timeElapsed;
    gravity_timer += timeElapsed;
    friction_timer += timeElapsed;
    hanging_timer += timeElapsed;
    col1_timer += timeElapsed;
/*
    if (gravity_timer > 50) {
        gravity_timer = 0;
        if (y_speed < 0) {
            y_speed++;
            if (y_speed > 0)
                y_speed = 0;
        }
    }*/
}


void MainDude::checkCollisionWitMapTiles(MapTile *mapTiles[32][32]) {

    if (bottom_collision) {
        if (friction_timer > 30) {
            friction_timer = 0;
            if (x_speed > 0) {
                x_speed -= 0.25;
                if (x_speed < 0)
                    x_speed = 0;
            }
            if (x_speed < 0) {
                x_speed += 0.25;
                if (x_speed > 0)
                    x_speed = 0;
            }
        }
    }


    if (x_speed > MAX_X_SPEED)
        x_speed = MAX_X_SPEED;
    if (x_speed < -MAX_X_SPEED)
        x_speed = -MAX_X_SPEED;

    if (y_speed > MAX_Y_SPEED)
        y_speed = MAX_Y_SPEED;
    if (y_speed < -MAX_Y_SPEED)
        y_speed = -MAX_Y_SPEED;


    if (pos_x_inc_timer > 10) {
        x += x_speed;
        pos_x_inc_timer = 0;
    }
    if (pos_y_inc_timer > 10) {
        y += y_speed;
        pos_y_inc_timer = 0;
    }

    left_collision = false;
    right_collision = false;
    bottom_collision = false;
    upper_collision = false;

    checkBottomCollision(mapTiles);
    checkLeftCollision(mapTiles);
    checkRightCollision(mapTiles);
    checkUpperCollision(mapTiles);
    checkCanHangOnTile(mapTiles);

    if (upper_collision || bottom_collision) {
        can_hang_on_tile_left = false;
        can_hang_on_tile_right = false;
    }
}

void MainDude::initMan() {
    sprite_gfx_mem_main = oamAllocateGfx(&oamMain, SpriteSize_16x16, SpriteColorFormat_256Color);
    sprite_gfx_mem_sub = oamAllocateGfx(&oamSub, SpriteSize_16x16, SpriteColorFormat_256Color);
    frame_gfx = (u8 *) spelunkerTiles;
    dmaCopy(spelunkerPal, SPRITE_PALETTE, 512);
    dmaCopy(spelunkerPal, SPRITE_PALETTE_SUB, 512);
}


void MainDude::animateMan(int camera_x, int camera_y) {

    if (can_hang_on_tile_right) {
        int frame = (2 * 6) + 1;
        u8 *offset = frame_gfx + frame * 16 * 16;
        dmaCopy(offset, sprite_gfx_mem_main, 16 * 16);
        dmaCopy(offset, sprite_gfx_mem_sub, 16 * 16);


        if (this->y <= 320 + 16) {
            oamSet(&oamMain, 0, x - camera_x, y - camera_y, 0, 0, SpriteSize_16x16, SpriteColorFormat_256Color,
                   sprite_gfx_mem_main, -1, false, false, false, false, false);
        }
        else
            oamSet(&oamMain, 0, -16, -16, 0, 0, SpriteSize_16x16, SpriteColorFormat_256Color,
                   0, -1, false, false, false, false, false);

        if(this->y >= 320) {
            oamSet(&oamSub, 0, x - camera_x, y - camera_y - 192, 0, 0, SpriteSize_16x16, SpriteColorFormat_256Color,
                   sprite_gfx_mem_sub, -1, false, false, false, false, false);
        }
        else
            oamSet(&oamSub, 0, -16, -16, 0, 0, SpriteSize_16x16, SpriteColorFormat_256Color,
                   0, -1, false, false, false, false, false);

        return;
    }
    if (can_hang_on_tile_left) {
        int frame = (2 * 6);
        u8 *offset = frame_gfx + frame * 16 * 16;
        dmaCopy(offset, sprite_gfx_mem_main, 16 * 16);
        dmaCopy(offset, sprite_gfx_mem_sub, 16 * 16);
        if (this->y <= 320) {
            oamSet(&oamMain, 0, x - camera_x, y - camera_y, 0, 0, SpriteSize_16x16, SpriteColorFormat_256Color,
                   sprite_gfx_mem_main, -1, false, false, false, false, false);
        }
        else
            oamSet(&oamMain, 0,-16, -16, 0, 0, SpriteSize_16x16, SpriteColorFormat_256Color,
                   0, -1, false, false, false, false, false);


        if(this->y >= 320) {
            oamSet(&oamSub, 0, x - camera_x, y - camera_y - 192, 0, 0, SpriteSize_16x16, SpriteColorFormat_256Color,
                   sprite_gfx_mem_sub, -1, false, false, false, false, false);
        }
        else
            oamSet(&oamSub, 0,-16, -16, 0, 0, SpriteSize_16x16, SpriteColorFormat_256Color,
                   0, -1, false, false, false, false, false);

        return;
    }

    int frame = anim_frame + state * FRAMES_PER_ANIMATION;
    u8 *offset = frame_gfx + frame * 16 * 16;
    dmaCopy(offset, sprite_gfx_mem_main, 16 * 16);
    dmaCopy(offset, sprite_gfx_mem_sub, 16 * 16);
    if (this->y <= 320) {
        oamSet(&oamMain, 0, x - camera_x, y - camera_y, 0, 0, SpriteSize_16x16, SpriteColorFormat_256Color,
               sprite_gfx_mem_main, -1, false, false, false, false, false);
    }
    else
        oamSet(&oamMain, 0, -16, -16,  0, 0, SpriteSize_16x16, SpriteColorFormat_256Color,
               0, -1, false, false, false, false, false);

    if(this->y >= 320) {
        oamSet(&oamSub, 0, x - camera_x, y - camera_y - 192, 0, 0, SpriteSize_16x16, SpriteColorFormat_256Color,
               sprite_gfx_mem_sub, -1, false, false, false, false, false);
    }
    else
        oamSet(&oamSub, 0, -16, -16, 0, 0, SpriteSize_16x16, SpriteColorFormat_256Color,
               0, -1, false, false, false, false, false);

}

void MainDude::checkUpperCollision(MapTile *mapTiles[32][32]) {

    for (int x = 0; x < 32; x++) {
        for (int y = 0; y < 32; y++) {

            if (mapTiles[x][y] == 0)
                continue;

            if (!upper_collision) {
                bool w1 = (this->y < (y * 16) + 16 && (this->y > (y * 16)));
                bool w2 = (this->x > (x * 16) - 16 && (this->x < (x * 16) + 16));
                upper_collision = w1 && w2;

                if (upper_collision) {
                    std::cout << "333" << " " << x << " " << y << " " << this->x << " " << this->y << '\n';
                    y_speed = 0;
                    this->y = (y * 16) + 16;
                }
            } else
                return;
        }
    }
}

void MainDude::checkBottomCollision(MapTile *mapTiles[32][32]) {

    for (int x = 0; x < 32; x++) {
        for (int y = 0; y < 32; y++) {

            if (mapTiles[x][y] == 0)
                continue;

            if (!bottom_collision) {
                //punkt (x*16)(y*16) to lewy górny róg tile-a
                //oś y rośnie do dołu
                //oś x rośnie w prawo
                //punkt 0,0 to lewy górny punkt mapy
                bool w1 = (this->x > (x * 16) - 16 * 0.75 && this->x < (x * 16) + 16 * 0.75);
                bool w2 = (this->y + 16 <= (y * 16) + 16) && (this->y + 16 >= (y * 16));

                bottom_collision = w1 && w2;

                if (bottom_collision) {
                    //x i y sprajta to lewy górny róg
                    y_speed = 0;
                    this->y = y * 16 - 16;
//                    std::cout << " " << "123123123" << '\n';
                } else {
                    if (speed_inc_y_timer > 5 && !(can_hang_on_tile_left || can_hang_on_tile_right)) {
                        y_speed += 0.20;
                        speed_inc_y_timer = 0;
                    }
                }
            } else
                continue;
        }
    }
}

void MainDude::checkLeftCollision(MapTile *mapTiles[32][32]) {

    for (int x = 0; x < 32; x++) {
        for (int y = 0; y < 32; y++) {

            if (mapTiles[x][y] == 0)
                continue;

            if (!left_collision) {
                bool w1 = (this->y > (y * 16) - 16 && (this->y < (y * 16) + 16));
                bool w2 = (this->x < (x * 16) - 12 && (this->x > (x * 16) - 16));
                left_collision = w1 && w2;

                if (left_collision) {
                    std::cout << "222" << " " << x << " " << y << " " << this->x << " " << this->y << '\n';
                    x_speed = 0;
                    this->x = (x * 16) - 16;
                }
            } else continue;
        }
    }
}

void MainDude::checkRightCollision(MapTile *mapTiles[32][32]) {

    for (int x = 0; x < 32; x++) {
        for (int y = 0; y < 32; y++) {

            if (mapTiles[x][y] == 0)
                continue;


            if (!right_collision) {
                bool w1 = (this->y > (y * 16) - 16 && (this->y < (y * 16) + 16));
                bool w2 = (this->x < (x * 16) + 16 && (this->x > (x * 16) + 12));
                right_collision = w1 && w2;

                if (right_collision) {
                    std::cout << "111" << " " << x << " " << y << " " << this->x << " " << this->y << '\n';
                    x_speed = 0;
                    this->x = (x * 16) + 16;
                }
            } else
                continue;
        }
    }
}

void MainDude::checkCanHangOnTile(MapTile *mapTiles[32][32]) {

    for (int x = 0; x < 32; x++) {
        for (int y = 0; y < 32; y++) {

            if (mapTiles[x][y] == 0)
                continue;

            if (!can_hang_on_tile_left && !can_hang_on_tile_right) {

                if (x == 0 || y == 0 || x == 31 || y == 31)
                    continue;

                if (left_collision || right_collision) {

                    bool upper_tile_exists = mapTiles[x][y - 1] != 0;
                    bool lower_tile_exists = mapTiles[x][y + 1] != 0;

                    if ((upper_tile_exists && lower_tile_exists) || (upper_tile_exists))
                        continue;

                    bool y_bound = (this->y > (y * 16) - 2) && (this->y < (y * 16) + 8);
                    bool x_bound = false;
                    if (right_collision && state == W_LEFT) {

                        bool upper_right_tile_exists = mapTiles[x + 1][y - 1] != 0;
                        if (upper_right_tile_exists)
                            continue;

                        x_bound = (this->x <= (x * 16) + 16 && (this->x >= (x * 16) + 12));
                    } else if (left_collision && state == W_RIGHT) {
                        bool upper_left_tile_exists = mapTiles[x - 1][y - 1] != 0;
                        if (upper_left_tile_exists)
                            continue;

                        x_bound = (this->x <= (x * 16) - 12 && (this->x >= (x * 16) - 16));
                    }

                    if (y_bound && x_bound) {
                        hanging_timer = 0;
                        this->y = (y * 16);
                        y_speed = 0;

                        std::cout << " 99 99 0" << '\n';

                        if (right_collision)
                            can_hang_on_tile_right = true;
                        if (left_collision)
                            can_hang_on_tile_left = true;
                    }
                }
            }
        }
    }
}



