//
// Created by xdbeef on 01.04.18.
//

#ifndef SPELUNKYDS_CAMERA_H
#define SPELUNKYDS_CAMERA_H

#include "../preprocessor/Debug.h"

class Camera {
public:

    static void init();

    static void dispose();

    inline static Camera &instance() {
        SPELUNKYDS_BREAKING_ASSERT(_instance);
        return *_instance;
    }

    void update();

    void write_current_position_to_graphics_engines();

    void apply_map_boundaries();

    void instant_focus();

    void incremental_focus(int camera_speed);

    inline void shake() { shakescreen_duration_timer = 350; }

    bool follow_main_dude{};
    double position_update_timer{};

    int x{};
    int y{};

    double shakescreen_duration_timer{};
    int x_shake_direction{};
    int y_shake_direction{};

private:

    static Camera* _instance;

    void apply_shaking();

};

#endif //SPELUNKYDS_CAMERA_H
