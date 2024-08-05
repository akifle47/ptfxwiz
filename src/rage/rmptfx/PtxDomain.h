#pragma once
#include "PtxKeyFrame.h"

namespace rage
{
    class ptxDomain : datBase
    {
    public:
        float field_4;
        uint32_t mType;
        int32_t field_C;
        rmPtfxKeyframe mKeyFrames[4];
        float field_B0;
        float field_B4;
        float field_B8;
        int8_t field_BC[4];
        float field_C0;
        float field_C4;
        float field_C8;
        int8_t field_CC[4];
        float field_D0;
        float field_D4;
        float field_D8;
        int8_t field_DC[4];
        float field_E0;
        float field_E4;
        float field_E8;
        int8_t field_EC[4];
        float field_F0;
        float field_F4;
        float field_F8;
        int8_t field_FC[4];
        float field_100;
        float field_104;
        float field_108;
        float field_10C;
        float field_110;
        float field_114;
        float field_118;
        void* field_11C;
        int32_t field_120;
        int8_t field_124;
        int8_t field_125;
        int8_t field_126[10];
    };
    ASSERT_SIZE(ptxDomain, 0x130);
}