#pragma once
#include <cstdint>

namespace rage
{
    struct ptxRenderState
    {
        ptxRenderState()
        {
            CullMode = 2;
            BlendSet = 0;
            DepthBias = 0.0;
            DepthWrite = 0;
            LightingMode = 1;
            AlphaBlend = 1;
            DepthTest = 1;
            field_13 = 0;
        }

        int32_t CullMode;
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