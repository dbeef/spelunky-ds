//
// Created by xdbeef on 03.09.18.
//

#ifndef SPELUNKYDS_SPRITE_UTILS_H
#define SPELUNKYDS_SPRITE_UTILS_H

#include "sprite_info.h"
#include "cstdio"

//!> Sources:
//!> C++ Primer, Fifth Edition, page 864, excellent explaining of variadic templates
//!> https://www.reddit.com/r/C_Programming/comments/4sfcpm/are_variadic_functions_computationally_expensive/
namespace sprite_utils {

    //!>variadic template function that recursively sets priority of a set of sprites

    //function to end the recursion and set the last element
    template<typename T>
    void set_priority(ObjPriority priority, T *i) {
//        if(i == nullptr)
//            printf("NULLPTR\n");
        i->entry->priority = priority;
    }

    //this function will be called for all but the last element in the pack
    template<typename T, typename ... Param>
    void set_priority(ObjPriority priority, T *i, Param *... params) {
//        if(i == nullptr)
//            printf("NULLPTR\n");
        i->entry->priority = priority;
        set_priority(priority, params ...);
    }

    //!>variadic template function that recursively sets visibility of a set of sprites

    //function to end the recursion and set the last element
    template<typename T>
    void set_visibility(bool visibility, const T *i) {
//        if(i == nullptr)
//            printf("NULLPTR V\n");
        i->entry->isHidden = !visibility;
    }

    //this function will be called for all but the last element in the pack
    template<typename T, typename ... Param>
    void set_visibility(bool visibility, const T *i, const Param *... params) {
//        if(i == nullptr)
//            printf("NULLPTR V\n");
        i->entry->isHidden = !visibility;
        set_visibility(visibility, params ...);
    }

    //!>variadic template function that recursively sets vertical flip property of a set of sprites

    //function to end the recursion and set the last element
    template<typename T>
    void set_vertical_flip(bool vertical_flip, const T *i) {
//        if(i == nullptr)
//            printf("NULLPTR V_F\n");
        i->entry->vFlip = vertical_flip;
    }

    //this function will be called for all but the last element in the pack
    template<typename T, typename ... Param>
    void set_vertical_flip(bool vertical_flip, const T *i, const Param *... params) {
//        if(i == nullptr)
//            printf("NULLPTR V_F\n");
        i->entry->vFlip = vertical_flip;
        set_vertical_flip(vertical_flip, params ...);
    }

    //!>variadic template function that recursively sets horizontal flip property of a set of sprites

    //function to end the recursion and set the last element
    template<typename T>
    void set_horizontal_flip(bool horizontal_flip, const T *i) {
//        if(i == nullptr)
//            printf("NULLPTR H_F\n");
        i->entry->hFlip = horizontal_flip;
    }

    //this function will be called for all but the last element in the pack
    template<typename T, typename ... Param>
    void set_horizontal_flip(bool horizontal_flip, const T *i, const Param *... params) {
//        if(i == nullptr)
//            printf("NULLPTR H_F\n");
        i->entry->hFlip = horizontal_flip;
        set_horizontal_flip(horizontal_flip, params ...);
    }

    //!>variadic template function that recursively updates gfx frame of a set of sprites

    //function to end the recursion and set the last element
    template<typename T>
    void update_frame(u8 *frameGfx, int size, T *i) {
//        if(i == nullptr)
//            printf("NULLPTR U_F\n");
        i->updateFrame(frameGfx, size);
    }

    //this function will be called for all but the last element in the pack
    template<typename T, typename ... Param>
    void update_frame(u8 *frameGfx, int size, T *i, Param *... params) {
//        if(i == nullptr)
//            printf("NULLPTR U_F\n");
        i->updateFrame(frameGfx, size);
        update_frame(frameGfx, size, params ...);
    }

    //!> sets xy for given sprite entry
    void set_entry_xy(SpriteInfo *i, int x, int y);

    u8 *get_frame(u8 *tileset, int size, int frame_offset);
}

#endif //SPELUNKYDS_SPRITE_UTILS_H
