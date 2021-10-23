#pragma once

#include <cstdint>

namespace arse {
    enum class Colour : int8_t {
        /**
         * Stop colour
         */
        RED = 0,
        AMBER = 1,
        /**
         * Go colour
         */
        GREEN = 2
    };
}
