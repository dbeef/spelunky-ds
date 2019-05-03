//
// Created by xdbeef on 01.04.18.
//

#ifndef SPELUNKYDS_CAMERA_H
#define SPELUNKYDS_CAMERA_H

#include "../preprocessor/Debug.h"

class Camera {
public:

    void update();

    void instant_focus();

    inline void follow_main_dude() { _follow_main_dude = true; };

    inline void detach_from_main_dude() { _follow_main_dude = false; };

    inline void shake() { _shakescreen_duration_timer = 350; }

    // static methods

    static void init();

    static void dispose();

    inline static Camera &instance() {
        SPELUNKYDS_BREAKING_ASSERT(_instance);
        return *_instance;
    }

    int x{};

    int y{};

private:

    static Camera *_instance;

    void apply_shaking();

    void write_current_position_to_graphics_engines();

    void apply_map_boundaries();

    void incremental_focus(int camera_speed);

    bool _follow_main_dude{};

    double _position_update_timer{};

    double _shakescreen_duration_timer{};

    int _x_shake_direction{};

    int _y_shake_direction{};
};

#endif //SPELUNKYDS_CAMERA_H
