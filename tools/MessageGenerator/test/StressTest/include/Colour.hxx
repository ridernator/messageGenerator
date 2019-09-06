#pragma once

#include <cstdint>

typedef enum class Colour : uint8_t {
    /**
     * Stop colour
     */
    RED = 0,
    /**
     * Middle colour
     */
    AMBER = 1,
    /**
     * Go colour
     */
    GREEN = 2
} Colour;
