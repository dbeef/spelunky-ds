//
// Created by xdbeef on 01.04.18.
//

#ifndef SPELUNKYDS_CAMERA_H
#define SPELUNKYDS_CAMERA_H

class Camera {
public:
    bool follow_main_dude{};
    int position_update_timer{};
    int x{};
    int y{};

    void update();

    void write_current_position_to_graphics_engines();

    void apply_map_boundaries();

    void instant_focus();

    void incremental_focus(int camera_speed);

};

#endif //SPELUNKYDS_CAMERA_H
