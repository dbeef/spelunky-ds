//
// Created by xdbeef on 24.02.18.
//

#ifndef SPELUNKYDS_MAINDUDE_H
#define SPELUNKYDS_MAINDUDE_H
#define MAX_X_SPEED 1.5
#define MAX_Y_SPEED 2.3
#define MIN_HANGING_TIME 100

#include <nds.h>
#include "SpriteState.h"
#include "../level_layout/MapTile.h"

struct MainDude {

public:

    int col1_timer;



    int key_pressed_time;

    int gravity_timer;
    int speed_inc_y_timer;
    int speed_inc_x_timer;

    int pos_x_inc_timer;
    int pos_y_inc_timer;

    int hanging_timer;

    int friction_timer = 0;

    bool can_hang_on_tile_left = false;
    bool can_hang_on_tile_right = false;
    bool apply_friction = false;
    bool bottom_collision = false;
    bool upper_collision = false;
    bool left_collision = false;
    bool right_collision = false;

    SpriteState state;
    double x_speed;
    double y_speed;
    int x;
    int y;
    int anim_frame;
    u16 *sprite_gfx_mem;
    u8 *frame_gfx;

    void checkCollisionWitMapTiles(MapTile *mapTiles[32][32]);

    void animateMan(int camera_x, int camera_y);

    void changePos(int keys_held, int keys_up);

    void initMan();

    void updateTimers(int timeElapsed);

    void checkUpperCollision(MapTile *mapTiles[32][32]);

    void checkBottomCollision(MapTile *mapTiles[32][32]);

    void checkLeftCollision(MapTile *mapTiles[32][32]);

    void checkRightCollision(MapTile *mapTiles[32][32]);

    void checkCanHangOnTile(MapTile *mapTiles[32][32]);
};


#endif //SPELUNKYDS_MAINDUDE_H
