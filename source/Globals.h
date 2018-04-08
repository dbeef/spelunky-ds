//
// Created by xdbeef on 24.02.18.
//

#ifndef SPELUNKYDS_CONSTS_H
#define SPELUNKYDS_CONSTS_H

#include "input/InputHandler.h"
#include "hud/Hud.h"
#include "animations/Camera.h"
#include "level_layout/LevelGenerator.h"

#define FRAMES_PER_ANIMATION 6
#define TICKS_PER_SECOND 30.5554503
#define DMA_CHANNEL 3
#define SPRITESHEET_ROW_WIDTH 6
#define X_SPEED_DELTA_TIME_MS 2
#define X_SPEED_DELTA_VALUE 2
#define FRICTION_DELTA_TIME_MS 30
#define FRICTION_DELTA_SPEED 1
#define Y_SPEED_DELTA_TIME_MS 2
#define GRAVITY_DELTA_SPEED 0.19

extern InputHandler *global_input_handler;
extern Camera *global_camera;
extern LevelGenerator *global_level_generator;
extern OAMManager *global_main_oam_manager;
extern OAMManager *global_sub_oam_manager;
extern Hud *global_hud;

#endif //SPELUNKYDS_CONSTS_H
