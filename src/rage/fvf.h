#pragma once
#include "../Utils.h"

namespace rage
{
    class grcFvf
    {
        uint32_t mFvf;
        uint8_t mFvfSize;
        uint8_t mFlags;
        bool mDynamicOrder;
        uint8_t mChannelCount;
        uint64_t mFvfChannelSizes;
    };
    ASSERT_SIZE(grcFvf, 0x10);
}