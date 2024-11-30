#pragma once
#include <cstdint>

namespace rage
{
    struct Color32
    {
        Color32() : Color(0) {}

        union
        {
            uint32_t Alpha : 8;
            uint32_t Red   : 8;
            uint32_t Green : 8;
            uint32_t Blue  : 8;

            uint32_t Color;
        };
    };
}