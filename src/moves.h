#pragma once

#include "status.h"


namespace kwi::moves {

inline uint encode(const uint a, const uint b, const uint c, const uint d) {
    // Ensure that a, b, c, and d are within the 8-bit range
    return ((a & 0xFF) << 24) | ((b & 0xFF) << 16) | ((c & 0xFF) << 8) | (d & 0xFF);
}


inline std::array<std::array<uint, 2>, 2> decode(const uint packed) {
    // Extract a, b, c, and d from the packed uint
    uint a = (packed >> 24) & 0xFF;
    uint b = (packed >> 16) & 0xFF;
    uint c = (packed >> 8) & 0xFF;
    uint d = packed & 0xFF;

    return std::array<std::array<uint, 2>, 2>{
        std::array<uint, 2>{a, b},
        std::array<uint, 2>{c, d}
    };
}
}