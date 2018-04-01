//
// Created by xdbeef on 01.04.18.
//

#ifndef SPELUNKYDS_CAMERA_H
#define SPELUNKYDS_CAMERA_H


class Camera {
public:
    int x;
    int y;
    void updatePosition(int main_dude_x, int main_dude_y);
    void setScroll(int bg_main_index, int bg_sub_index);
};


#endif //SPELUNKYDS_CAMERA_H
