//
// Created by dbeef on 5/3/19.
//

#include "Math.hpp"
#include <cstdlib>

namespace math {
    int floor_div(int a, int b) {
        div_t r = div(a, b);
        if (r.rem != 0 && ((a < 0) ^ (b < 0)))
            r.quot--;
        return r.quot;
    }
}