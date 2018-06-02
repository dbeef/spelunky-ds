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
    void update_position();
    void set_scroll();
    void instant_focus();
    void apply_map_boundaries();
    void center_camera(int camera_speed);
};

#endif //SPELUNKYDS_CAMERA_H
