//
// Created by dbeef on 5/2/19.
//

#ifndef SPELUNKYDS_BRIGHTNESS_H
#define SPELUNKYDS_BRIGHTNESS_H

namespace brightness {

    //!> this should be called after Vblank (otherwise - crash!)
    void set_maximum_brightness();

    //!> this should be called after Vblank (otherwise - crash!)
    void normalize_brightness();

}

#endif //SPELUNKYDS_BRIGHTNESS_H
