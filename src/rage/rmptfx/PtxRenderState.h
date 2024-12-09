#pragma once
#include <cstdint>

namespace rage
{
    enum class ePtxCullMode
    {
        NONE = 1,
        CLOCKWISE,
        COUNTERCLOCKWISE,
        COUNT
    };

    struct ptxRenderState
    {
        ePtxCullMode CullMode;
        int32_t BlendSet;
        float DepthBias;
        int32_t LightingMode;
        bool DepthWrite;
        bool DepthTest;
        bool AlphaBlend;
        //might be padding
        int8_t field_13;
    };
};