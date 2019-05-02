//
// Created by dbeef on 5/2/19.
//

#ifndef SPELUNKYDS_BRIGHTNESS_H
#define SPELUNKYDS_BRIGHTNESS_H


class Brightness {

public:

    //!> this should be called after Vblank (otherwise - crash!)
    static void set_maximum_brightness();

    //!> this should be called after Vblank (otherwise - crash!)
    static void normalize_brightness();

private:

    static uint8 _brightness_level;

    static uint16 change_brightness_timer;
};


#endif //SPELUNKYDS_BRIGHTNESS_H
