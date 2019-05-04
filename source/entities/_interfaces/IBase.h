//
// Created by xdbeef on 29.12.18.
//

#ifndef SPELUNKYDS_IBASE_H
#define SPELUNKYDS_IBASE_H

class SpriteInfo;

// Base delivers basic functionality needed by every creature/item/decoration etc.
class IBase {

public:

    IBase() = default;

    virtual ~IBase() = default;

    inline void set_xy(int const &x, int const &y) {
        _x = x;
        _y = y;
    }

    int _x{};
    int _y{};
    bool _ready_to_dispose{};

    void set_pickuped_position_not_checking(int pickup_offset_x_left, int pickup_offset_x_right, int pickup_offset_y);

    // Should print its name upper-cased with newline character.
    // For debugging purposes.
    // https://stackoverflow.com/questions/3649278/how-can-i-get-the-class-name-from-a-c-object
    // https://stackoverflow.com/questions/32016809/using-typeid-to-get-name-of-derived-class
    virtual void print_classname() const = 0;

};


#endif //SPELUNKYDS_IBASE_H
